#include "uradmonitor.h"

void uradmonitor::init(char *buf, uint16_t len, DataSendCallback pSendFunc) {
  m_buf = buf;
  m_len = len;
  m_pSendFunc = pSendFunc;

#if DEV_CLASS == MODEL_A3
  p_sserial = new SoftwareSerial(D7, D0 ); // D8 is connected to GND
#else
    p_sserial = new SoftwareSerial(D7, D8); //RX, TX
#endif  
  
  p_sserial->begin(SENSOR_BAUDRATE);
  pinMode(D0, OUTPUT); // HW4: red led HW5: RGB-BLUE
  pinMode(D5, OUTPUT); // HW4: yellow led HW5: RGB-RED
  pinMode(D6, OUTPUT); // HW4: green led HW5: RGB-GREEN
  pinMode(D3, OUTPUT); // speaker

  digitalWrite(D3, 0);
  
  #if VER_HW < 5        
    digitalWrite(D6, HIGH); // green
    digitalWrite(D5, HIGH); // yellow
    digitalWrite(D0, HIGH); // red
    delay(1000);
    digitalWrite(D6, LOW); // green
    digitalWrite(D5, LOW); // yellow
    digitalWrite(D0, LOW); // red
  #else
  for (int i = 0; i < 2; i++) {
    unsigned int rgbColour[3];
    // Start off with red.
   rgbColour[0] = 255;
   rgbColour[1] = 0;
   rgbColour[2] = 0;  
   // Choose the colours to increment and decrement.
   for (int decColour = 0; decColour < 3; decColour += 1) {
     int incColour = decColour == 2 ? 0 : decColour + 1;
     // cross-fade the two colours.
     for(int i = 0; i < 255; i += 1) {
       rgbColour[decColour] -= 1;
       rgbColour[incColour] += 1;
       analogWrite(D5, rgbColour[0]); //red
       analogWrite(D6, rgbColour[1]); // green
       analogWrite(D0, rgbColour[2]); //blue
       delay(2);
     }
   }
  }
  #endif
  /*// ukraine flag
  analogWrite(D5, 0); //red
  analogWrite(D6, 34); // green
  analogWrite(D0, 72); //blue
  delay(1000);
  analogWrite(D5, 100); //red
  analogWrite(D6, 84); // green
  analogWrite(D0, 0); //blue
  delay(1000);*/

  

#ifdef USE_SHT
  sht21.start();

  #if defined(USE_BMP)
    bmp280.start(0x76);
  #endif          
#endif
}

void uradmonitor::setDeviceID(uint32_t devid) {
  deviceId = devid;
}

uint32_t uradmonitor::getDeviceID() {
  return deviceId;
}


bool uradmonitor::initNetwork() {
  // configure WIFI
  WiFi.hostname(settings.ownssid); //test it?!
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(settings.ownssid, settings.ownkey,1,0);
  
  if (strlen(settings.ssid) > 0) 
    WiFi.begin(settings.ssid, settings.key);
}


bool uradmonitor::isConnected() {
  return (strlen(WiFi.localIP().toString().c_str()) && WiFi.status());
}
char* uradmonitor::getLocalIP() {
  strcpy(ipLAN, WiFi.localIP().toString().c_str());
  return ipLAN;
}
char* uradmonitor::getDNSIP() {
  strcpy(ipDNS, WiFi.dnsIP().toString().c_str());
  return ipDNS;
}

// 0: turn off blue led, enable sensor
// 1: turn on blue led, disable sensor
void uradmonitor::enableSensor(bool state) {
  sensorState = state;
  digitalWrite ( BUILTIN_LED, state ); // uses D4
  #if (VER_HW > 4)
    digitalWrite( D3, !state);
  #endif    
}

void uradmonitor::printIPAddressOfHost(const char* host) {
  IPAddress resolvedIP;
  if (!WiFi.hostByName(host, resolvedIP)) {
    //Serial.println("DNS lookup failed.  Rebooting...");
    Serial.flush();
    ESP.reset();
  }
  //Serial.print(host);
  //Serial.print(" IP: ");
  //Serial.println(resolvedIP);
}

void uradmonitor::loadSettings() {
  
  
  
  EEPROM.begin(1024);
  memset(&settings, 0, sizeof(settings));
  
  EEPROM.get(EEPROM_BASE_ADDR, settings); 

  // default settings?
  if (strcmp(settings.sig, "SIG") != 0) {
    strcpy(settings.sig, "SIG");
    // Apply default values
    settings.sendInt  = DEFAULT_SEND_INTERVAL;               
    settings.warmup  = DEFAULT_WARMUP;
    settings.flag_mute = DEFAULT_MUTE;
    settings.disableap = DEFAULT_DISABLEAP;
    settings.flag_encrypt = DEFAULT_ENCRYPT;
    
    memset(settings.ssid, 0, sizeof(settings.ssid)); 
    strncpy(settings.ssid, DEFAULT_INTERNET_SSID, 64); 
    settings.ssid[63] = 0;
  
    memset(settings.key, 0, sizeof(settings.key)); 
    strncpy(settings.key, DEFAULT_INTERNET_KEY, 64); 
    settings.key[63] = 0;

    memset(settings.ownkey, 0, sizeof(settings.ownkey));
    sprintf(settings.ownssid, "uRADMonitor-%02X",  (uint8_t)(deviceId & 0xFF));   
    settings.ownssid[63] = 0;
    
    memset(settings.ownkey, 0, sizeof(settings.ownkey));
#if DEV_CLASS == SMOGGIE_PM_KIT
    strcpy(settings.ownkey, "uradmonitor"); // default password in open source variant
#else    
    sprintf(settings.ownkey, "%08lX", deviceId);
#endif  
    settings.ownkey[63] = 0;

    strcpy(settings.userid,"1"); settings.userid[63] = 0;
    strcpy(settings.userkey,"get it in dashboard"); settings.userkey[63] = 0;
    
    memset(settings.server, 0, sizeof(settings.server)); 
    strncpy(settings.server, DEFAULT_SERVER, 64); 
    settings.server[63] = 0;

    memset(settings.script, 0, sizeof(settings.script)); 
    strncpy(settings.script, DEFAULT_SCRIPT, 64); 
    settings.script[63] = 0;

    // calibration
    for (uint8_t i = 0 ; i< CAL_VALUES; i++)  settings.cal[i] = calib.getDefault(i);
  } 

  
  
  // apply calibration
  for (uint8_t i = 0 ; i< CAL_VALUES; i++) calib.setCalibrationParam(i, settings.cal[i]);
}

void uradmonitor::saveSettings() {
  EEPROM.put(EEPROM_BASE_ADDR, settings); 
  EEPROM.commit(); 
}

void uradmonitor::defaultSettings() {
  EEPROM.begin(1024);
  memset(&settings, 0, sizeof(settings));
  EEPROM.put(EEPROM_BASE_ADDR, settings); 
  EEPROM.commit(); 
  loadSettings();
}


bool uradmonitor::initSensors() {
  pinMode ( BUILTIN_LED, OUTPUT );
  //pinMode ( D3, OUTPUT );
  enableSensor(0); // turns led on, fan off
  delay(500);
  enableSensor(1); // led off, fan on, sensor on
  return true;
}





void uradmonitor::callback_answer(uint16_t webstatuscode, char *serverAnswer) {
  
  code = webstatuscode;
  // if the server replied OK, we reset the watchdog to prevent auto-reset. This is currently the only point in code where we do this.
  if (webstatuscode == 200) {
    pcktsOk ++;
    //wd.petTheDog();
    char value[64] = {0};
    
    bool save = false;
    if (jsonKeyFind(serverAnswer, "\"success\"", "\"", value, 64)) {
      //eeprom.setDeviceID(hex2int(value));
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"sendint\"", "\"", value, 64)) {
      settings.sendInt = atoi(value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"setmute\"", "\"", value, 64)) {
      settings.flag_mute = atoi(value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"warmup\"", "\"", value, 64)) {
      settings.warmup = atoi(value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"key1\"", "\"", value, 64)) {
      strcpy(settings.ssid, value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"key2\"", "\"", value, 64)) {
      strcpy(settings.key, value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    }  else if (jsonKeyFind(serverAnswer, "\"key3\"", "\"", value, 64)) {
      strcpy(settings.userid, value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"key4\"", "\"", value, 64)) {
      strcpy(settings.userkey, value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"disableap\"", "\"", value, 64)) {
      settings.disableap = atoi(value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"encrypt\"", "\"", value, 64)) {
      settings.flag_encrypt = atoi(value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"server\"", "\"", value, 64)) {
      strcpy(settings.server, value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (jsonKeyFind(serverAnswer, "\"script\"", "\"", value, 64)) {
      strcpy(settings.script, value);
      save = true;
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (strstr(serverAnswer, "\"getsettings\"")) {
      Serial.printf("{\"settings\":{\"key1\":\"%s\",\"key2\":\"%s\",\"key3\":\"%s\",\"key4\":\"%s\","
        "\"server\":\"%s\",\"script\":\"%s\","
        "\"warmup\":%u,\"sendint\":%u,\"encrypt\":%u,\"mute\":%u,\"disableap\":%u,"
        "\"sw\":%u,\"hw\":%u,\"status\":\"%s\""
        "}}\r\n",
        settings.ssid, settings.key, settings.userid, settings.userkey, settings.server, settings.script, settings.warmup, settings.sendInt, settings.flag_encrypt, settings.flag_mute, settings.disableap, VER_SW, VER_HW, isConnected()?"connected":"disconnected");
    } else if (strstr(serverAnswer, "\"getdata\"")) {
      Serial.printf("{\"data\":{\"id\":\"%08lX\","
#if !(defined(USE_BME) || defined(USE_BMP))
  "\"temperature\":%.2f,\"humidity\":%.2f,"
#else      
  "\"temperature\":%.2f,\"humidity\":%.2f,\"pressure\":%lu,"
#endif        
#if DEV_CLASS == SMOGGIE_PM  || DEV_CLASS == MODEL_A3 || DEV_CLASS == SMOGGIE_PM_KIT
        "\"pm1\":%u,\"pm25\":%u,\"pm10\":%u,"
#elif DEV_CLASS == SMOGGIE_CO2
        "\"co2\":%u,"
#elif DEV_CLASS == SMOGGIE_GAS
      "\"%s\":%.1f,"
#elif DEV_CLASS == SMOGGIE_RADON
        "\"radon\":%u,"
#elif DEV_CLASS == SMOGGIE_NOISE        
        "\"noise\":%.1f,"
#endif        
        "\"valid\":%u,\"uptime\":%lu}}\r\n",
      getDeviceID(), 
#if !(defined(USE_BME) || defined(USE_BMP))
      data.sensorTemperature,  data.sensorHumidity,
#else        
      data.sensorTemperature,  data.sensorHumidity, (uint32_t) data.sensorPressure,
#endif        
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3  || DEV_CLASS == SMOGGIE_PM_KIT
        (uint16_t)data.sensorPM1,(uint16_t) data.sensorPM25, (uint16_t)data.sensorPM10, 
#elif DEV_CLASS == SMOGGIE_CO2
      (uint16_t)data.sensorCO2,
#elif DEV_CLASS == SMOGGIE_GAS
      data.getName(), data.getGas(),
#elif DEV_CLASS == SMOGGIE_RADON
      (uint16_t)data.sensorRadon,
#elif DEV_CLASS == MODEL_NOISE
      data.sensorNoise,
#endif   
      data.valid, sec);
    } else if (strstr(serverAnswer, "\"getcal\"")) {
        for (uint8_t i = 0; i<CAL_VALUES;i++)
          Serial.printf("%d,%08lX\r\n", i, (uint32_t)settings.cal[i]);
    } else if (strstr(serverAnswer, "\"default\"")) {
      defaultSettings();
      Serial.printf("OK\r\n");
    } 
#if DEV_CLASS == SMOGGIE_CO2 && (SENSOR == SENSOR_CM1106 || SENSOR == SENSOR_CM1107)
    else if (jsonKeyFind(serverAnswer, "\"calco2\"", "\"", value, 64)) {
      uint16_t conc = atoi(value);
    
      p_sserial->write(sensor.getCalCmd(conc), sensor.getCalCmdSize());
      Serial.printf("OK\r\n%s\r\n", value);
    } else if (strstr(serverAnswer, "\"disableabc\"")) {
      p_sserial->write(sensor.getDisableABCCmd(), sensor.getDisableABCCmdSize());
      Serial.printf("OK\r\n");
    } else if (strstr(serverAnswer, "\"enableabc\"")) {
      p_sserial->write(sensor.getEnableABCCmd(), sensor.getEnableABCCmdSize());
      Serial.printf("OK\r\n");
    }
    
#endif
    
    // check calibration params
    for (uint8_t i = 0; i< CAL_VALUES; i++)
      if (jsonKeyFind(serverAnswer, calNoun[i], "\"", value, 64)) {
        uint32_t param = hex2int(value);
        calib.setCalibrationParam(i, param);
        settings.cal[i] = param;
        save = true;
      }
    if (save) {
      saveSettings();
    }
  }
}

void uradmonitor::addAvg(double *current_avg, uint16_t current_count, double new_value) {
   *current_avg = (new_value + *current_avg * current_count) / (current_count + 1) ;
}

double anaoffset = 0xFFFF;

// careful, this needs to work on all variants: PM, CO2, GAS, seeRadon, BUZZ, etc.
void ledGreen(boolean status) {
    digitalWrite(D6, status); // green
    digitalWrite(D5, LOW); // yellow
    digitalWrite(D0, LOW); // red
}
void ledYellow(boolean status) {
  #if VER_HW < 5              
    digitalWrite(D6, LOW); // green
    digitalWrite(D5, status); // yellow
    digitalWrite(D0, LOW); // red
  #elif VER_HW == 5
    analogWrite(D6, 150 * status); // green
    digitalWrite(D5, status); // red
    digitalWrite(D0, LOW); // blue
  #endif
}
void ledRed(boolean status) {
  #if VER_HW < 5
    digitalWrite(D6, LOW); // green
    digitalWrite(D5, LOW); // yellow
    digitalWrite(D0, status); // red
  #elif VER_HW == 5
     digitalWrite(D6, LOW); // green
    digitalWrite(D5, status); // red
    digitalWrite(D0, LOW); // blue
  #endif
}
        


void uradmonitor::callback_second(uint32_t sec) {
  
  
  #ifdef POLL
      p_sserial->write(sensor.getSendCmd(), sensor.getSendCmdSize());
  #endif  


    
  if (settings.disableap > 0 && sec == settings.disableap) {
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_STA);
  }

    
  if (settings.warmup == 0) {
    // read sensors
    if (sec % READ_INTERVAL == 0) {
      uint8_t timeout = 100;

      // assume data is valid
      data.valid = true;
      
      double t = 0, h = 0, p = 0;
     // do {
        #if defined(USE_PMS5003T)
          t = sensor.getTemperature();
          h = sensor.getHumidity();
          
          if (t <= -40 || t >= 125 || h <= 0 || h == 100) data.valid = false;
          
          addAvg(&data.sensorTemperature, mvgAvgCnt, calib.calibrate(t, CAL_TEMPERATURE, 0));
          addAvg(&data.sensorHumidity, mvgAvgCnt, calib.calibrate(h, CAL_HUMIDITY, 1));
        #elif defined(USE_SHT)
          t = sht21.getTemperature();  // get temp from SHT 
          h = sht21.getHumidity(); // get temp from SHT
        
          if (t <= -40 || t >= 125 || h <= 0 || h == 100) data.valid = false;  // h -6 means error - open circuit on humidity sensor

          //Serial.printf("SHT %f %f %d\n", t, h , data.valid);
          
          addAvg(&data.sensorTemperature, mvgAvgCnt, calib.calibrate(t, CAL_TEMPERATURE, 0));
          addAvg(&data.sensorHumidity, mvgAvgCnt, calib.calibrate(h, CAL_HUMIDITY, 1));

          #if defined(USE_BMP)
            bmp280.readSensors(&t, &p);
            if (p < 30000) data.valid = false;
            addAvg(&data.sensorPressure, mvgAvgCnt, calib.calibrate(t, CAL_PRESSURE, 0));
            //Serial.printf("BMP %f %f\n", t, p);
          #endif
      
          
        #elif defined(USE_BME)
          bme280.start(0x76);
          bme280.readSensors(&t, &p, &h);

          if (t <= -40 || t >= 125 || h <= 0 || h == 100 || p == 0) data.valid = false;
          
          addAvg(&data.sensorTemperature, mvgAvgCnt, calib.calibrate(t, CAL_TEMPERATURE, 0));
          addAvg(&data.sensorPressure, mvgAvgCnt, calib.calibrate(p, CAL_PRESSURE, 1));
          addAvg(&data.sensorHumidity, mvgAvgCnt, calib.calibrate(h, CAL_HUMIDITY, 1));
        #endif
        // timeout --;
      //} while ((timeout > 0) && (t > 100 || h > 100 || h == 0 )); // retry BME if needed. Check for deadlocks on damaged sensor?
     
     
     #if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3
     #ifdef DEBUG
     Serial.printf("# sec:%03d ATM:%03u %03u %03u\n", sec, sensor.getPM1(), sensor.getPM25(), sensor.getPM10());
     #endif
      
      if (sensor.getPM25() == 0 || sensor.getPM25() == 1000) data.valid = false;
      
      // factor is the same for PM1 / PM2.5 / PM10 as all laser sensors use some kind of interpolation in software.
      addAvg(&data.sensorPM1, mvgAvgCnt, calib.calibrate(FACTOR * sensor.getPM1(), CAL_PM1, 1));
      addAvg(&data.sensorPM25, mvgAvgCnt, calib.calibrate(FACTOR * sensor.getPM25(), CAL_PM25, 1));
      addAvg(&data.sensorPM10, mvgAvgCnt, calib.calibrate(FACTOR * sensor.getPM10(), CAL_PM10, 1));
      lastSensorVal = data.sensorPM25;
     #elif DEV_CLASS == SMOGGIE_PM_KIT
      
      if (sensor.getPM25() == 0 || sensor.getPM25() == 1000) data.valid = false;
     
        // factor is the same for PM1 / PM2.5 / PM10 as all laser sensors use some kind of interpolation in software.
      addAvg(&data.sensorPM1, mvgAvgCnt, calib.calibrate(FACTOR * sensor.getPM1(), CAL_PM1, 1));
      addAvg(&data.sensorPM25, mvgAvgCnt, calib.calibrate(FACTOR * sensor.getPM25(), CAL_PM25, 1));
      addAvg(&data.sensorPM10, mvgAvgCnt, calib.calibrate(FACTOR * sensor.getPM10(), CAL_PM10, 1));
      lastSensorVal = data.sensorPM25; 
      addAvg(&data.pm03no, mvgAvgCnt, sensor.getPM03No());
      addAvg(&data.pm05no, mvgAvgCnt, sensor.getPM05No());
      addAvg(&data.pm1no, mvgAvgCnt, sensor.getPM1No());
      addAvg(&data.pm25no, mvgAvgCnt, sensor.getPM25No());
      addAvg(&data.pm5no, mvgAvgCnt, sensor.getPM5No());
      addAvg(&data.pm10no, mvgAvgCnt, sensor.getPM10No());
      

      #ifdef DEBUG
       Serial.printf("# sec:%03d ATM:%03u %03u %03u Counts: %03u %03u %03u %03u %03u %03u\n", 
       sec, sensor.getPM1(), sensor.getPM25(), sensor.getPM10(),
       sensor.getPM03No(), sensor.getPM05No(), sensor.getPM1No(), sensor.getPM25No(), sensor.getPM5No(), sensor.getPM10No());
       #endif
       
    #elif DEV_CLASS == SMOGGIE_CO2

      if (sensor.getCO2() == 0 || sensor.getCO2() == 5000) data.valid = false;
      
      addAvg(&data.sensorCO2, mvgAvgCnt, calib.calibrate(sensor.getCO2(), CAL_CO2, 1));
      lastSensorVal = data.sensorCO2;
    #elif DEV_CLASS == SMOGGIE_GAS
      double gas = sensor.getGas();
      
      data.type = sensor.getGasId();
      sensor.getName(data.name, 10);
      data.sensorVoltage = analogRead(A0) * (3.3 / 1024);
      double ana = sensor.getAnalogueConc(data.sensorVoltage);
      if (ana < anaoffset)
        anaoffset = ana;
      ana -= anaoffset;
      if (gas < 0.001)
        gas = ana;
      if (gas < 0.001)
        gas = 0.001;

      addAvg(&data.sensorGas, mvgAvgCnt, calib.calibrate(gas, CAL_ZE03S1, 1) );
      
      addAvg(&data.sensorVoltage, mvgAvgCnt, data.sensorVoltage);
      
      lastSensorVal = data.sensorGas;
    #elif DEV_CLASS == SMOGGIE_RADON
    
      if (sensor.getRadon() == 0) data.valid = false;
      addAvg(&data.sensorRadon, mvgAvgCnt, sensor.getRadon());
      lastSensorVal = data.sensorRadon;
    #elif DEV_CLASS == MODEL_NOISE      
      if (sensor.getNoise() == 0) data.valid = false;
      
      addAvg(&data.sensorNoise, mvgAvgCnt, calib.calibrate(sensor.getNoise(), CAL_NOISE, 1));
      lastSensorVal = data.sensorNoise;
    #endif  

   
      // moving average increment
      mvgAvgCnt ++;
#if DEV_CLASS == SMOGGIE_GAS // we should implement limits for all gases known
  if (data.type == H2S) {
      if (lastSensorVal < 0.01)  // value is in ppm
        ledGreen(1);
      else if (lastSensorVal < 1.5) 
        ledYellow(1);
      else 
        ledRed(1);
  } /*else if (data.type == ZE12_NH3) {
      if (lastSensorVal < 0.5)  // value is in ppm
        ledGreen(1);
      else if (lastSensorVal < 1) 
        ledYellow(1);
      else 
        ledRed(1);
  } else if (data.type == ZE12_CO) {
      if (lastSensorVal < 0.5)  // value is in ppm
        ledGreen(1);
      else if (lastSensorVal < 2) 
        ledYellow(1);
      else 
        ledRed(1);
  } else if (data.type == ZE12_NO2) {
      if (lastSensorVal < 0.1)  // value is in ppm
        ledGreen(1);
      else if (lastSensorVal < 0.2) 
        ledYellow(1);
      else 
        ledRed(1);
  }else if (data.type == O3) {
      if (lastSensorVal < 0.1)  // value is in ppm
        ledGreen(1);
      else if (lastSensorVal < 0.2) 
        ledYellow(1);
      else 
        ledRed(1);
  }*/
  else
        ledGreen(sec % 2); // green
    
  
#else
    // use leds to show limits
     if (lastSensorVal < LIMIT_GREEN) { 
        ledGreen(1);
      } else if (lastSensorVal < LIMIT_YELLOW) {
        ledYellow(1);
      } else {
        ledRed(1);
      }
#endif
      
      // read data complete
    }
    
    // ready to send
    if (sec % settings.sendInt == 0) {
      // if no connection force reconnect
      if (!isConnected()) initNetwork();
      pcktsTotal++;
      
      if (m_pSendFunc) m_pSendFunc();
      
      mvgAvgCnt = 0; // moving average reset
    }
  } else {
    if (lastSensorVal == 0) {
      // blink while warmup, this goes every second, only if lastVal is null
      ledGreen(sec % 2); // green
    }
    // decrease warmup
    settings.warmup --;
    // reset moving average when ready
    if (settings.warmup == 0)
      mvgAvgCnt = 0; // moving average reset
  }

   
  // on SMOGGIE-PM, at sec 0 in every minute we do warmup to normalize PM sensor readings, after warmup we get actual data until sec 20 when we stop fan. FAN DUTY 33%  
  #if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == SMOGGIE_PM_KIT
    #ifdef INCREASE_LIFESPAN
      
      if ((sec % 60) == 0) {
        enableSensor(1);
        settings.warmup = 10;
      } 
      if ((sec % 60) == 30) {
        enableSensor(0); // turns led on, fan off
      }
    #endif
  #endif
  
}

void uradmonitor::loop() {

  // check second tick
  sec = millis() / 1000;
  if (sec != oldsec) {
    oldsec = sec;
    callback_second(sec);
  }
  
  // device protocol
  int n = 0;
  if (Serial.available()) {
    if (n = Serial.readBytesUntil('\n', m_buf, m_len - 1)) {
      m_buf[n] = 0;
      callback_answer(200, m_buf);
    }
  }
  // send to UART sensor
  if (p_sserial->available()) {    
    uint8_t ch = p_sserial->read();
    sensor.fusedata(ch);
  }
}
