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
  
  pinMode(D0, OUTPUT); // red led
  pinMode(D5, OUTPUT); // yellow led
  pinMode(D6, OUTPUT); // green led
  pinMode(D3, OUTPUT); // speaker

  digitalWrite(D0, 1);
  digitalWrite(D5, 1);
  digitalWrite(D6, 1);
  digitalWrite(D3, 0);
  delay(500);
  
  digitalWrite(D0, 0);
  digitalWrite(D5, 0);
  digitalWrite(D6, 0);

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
    sprintf(settings.ownkey, "%08lX", deviceId);
    settings.ownkey[63] = 0;
    
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
      Serial.printf("{\"data\":{\"id\":\"%08lX\"," \
        "\"temperature\":%.2f,\"humidity\":%.2f,\"pressure\":%lu,"
#if DEV_CLASS == SMOGGIE_PM  || DEV_CLASS == MODEL_A3
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
        "\"uptime\":%lu}}\r\n",
        
        getDeviceID(), data.sensorTemperature,  data.sensorHumidity, (uint32_t) data.sensorPressure,
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3  
        (uint16_t)data.sensorPM1,(uint16_t) data.sensorPM25, (uint16_t)data.sensorPM10,  (uint32_t)(millis() / 1000)
#elif DEV_CLASS == SMOGGIE_CO2
      (uint16_t)data.sensorCO2
#elif DEV_CLASS == SMOGGIE_GAS
      data.getName(), data.getGas()
#elif DEV_CLASS == SMOGGIE_RADON
      (uint16_t)data.sensorRadon
#elif DEV_CLASS == MODEL_NOISE
      data.sensorNoise
#endif   
  ,sec             
        );
    } else if (strstr(serverAnswer, "\"getcal\"")) {
        for (uint8_t i = 0; i<CAL_VALUES;i++)
          Serial.printf("%d,%08lX\r\n", i, (uint32_t)settings.cal[i]);
    } else if (strstr(serverAnswer, "\"default\"")) {
      defaultSettings();
      Serial.printf("OK\r\n");
    } 
#if DEV_CLASS == SMOGGIE_CO2 && SENSOR_CLASS == CM11XX
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
      
      double t = 0, h = 0, p = 0;
      do {
        bme280.start(0x76);
        bme280.readSensors(&t, &p, &h);
        timeout --;
      } while ((timeout > 0) && (t > 100 || h > 100 || h == 0 )); // retry BME if needed. Check for deadlocks on damaged sensor?
     
      addAvg(&data.sensorTemperature, mvgAvgCnt, calib.calibrate(t, CAL_TEMPERATURE, 0));
      addAvg(&data.sensorPressure, mvgAvgCnt, calib.calibrate(p, CAL_PRESSURE, 1));
      addAvg(&data.sensorHumidity, mvgAvgCnt, calib.calibrate(h, CAL_HUMIDITY, 1));
  
     #if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3
     #ifdef DEBUG
     Serial.printf("# sec:%03d ATM:%03u %03u %03u\n", sec, sensor.getPM1(), sensor.getPM25(), sensor.getPM10());
     #endif
      // factor is the same for PM1 / PM2.5 / PM10 as all laser sensors use some kind of interpolation in software.
      addAvg(&data.sensorPM1, mvgAvgCnt, calib.calibrate(FACTOR * sensor.getPM1(), CAL_PM1, 1));
      addAvg(&data.sensorPM25, mvgAvgCnt, calib.calibrate(FACTOR * sensor.getPM25(), CAL_PM25, 1));
      addAvg(&data.sensorPM10, mvgAvgCnt, calib.calibrate(FACTOR * sensor.getPM10(), CAL_PM10, 1));
      lastSensorVal = data.sensorPM25;
      
      //chart[CHART_POINTS-1] = data.sensorPM25;
    #elif DEV_CLASS == SMOGGIE_CO2
      addAvg(&data.sensorCO2, mvgAvgCnt, calib.calibrate(sensor.getCO2(), CAL_CO2, 1));
      lastSensorVal = data.sensorCO2;
    #elif DEV_CLASS == SMOGGIE_GAS
      addAvg(&data.sensorGas, mvgAvgCnt, calib.calibrate(sensor.getGas(), CAL_ZE03S1, 1) );
      data.type = sensor.getType();
      sensor.getZE03Name(data.name, 10);
      data.sensorVoltage = analogRead(A0) * (3.3 / 1024);
      double ana = sensor.getAnalogueConc(data.sensorVoltage);
      if (ana < anaoffset)
        anaoffset = ana;
      ana -= anaoffset;
      
      addAvg(&data.sensorVoltage, mvgAvgCnt, data.sensorVoltage);
      
      lastSensorVal = data.sensorGas;
    #elif DEV_CLASS == SMOGGIE_RADON
      addAvg(&data.sensorRadon, mvgAvgCnt, sensor.getRadon());
      lastSensorVal = data.sensorRadon;
    #elif DEV_CLASS == MODEL_NOISE      
      addAvg(&data.sensorNoise, mvgAvgCnt, calib.calibrate(sensor.getNoise(), CAL_NOISE, 1));
      lastSensorVal = data.sensorNoise;
    #endif  

   
      // moving average increment
      mvgAvgCnt ++;
#if DEV_CLASS != SMOGGIE_GAS // we should implement limits for all gases known
    // use leds to show limits
     if (lastSensorVal < LIMIT_GREEN) { 
        digitalWrite(D6, HIGH); // green
        digitalWrite(D5, LOW); // yellow
        digitalWrite(D0, LOW); // red
      } else if (lastSensorVal < LIMIT_YELLOW) {
        digitalWrite(D6, LOW); // green
        digitalWrite(D5, HIGH); // yellow
        digitalWrite(D0, LOW); // red
      } else {
        digitalWrite(D6, LOW); // green
        digitalWrite(D5, LOW); // yellow
        digitalWrite(D0, HIGH); // red
      }
#else
      digitalWrite(D6, sec % 2); // green
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
      digitalWrite(D6, sec % 2); // green
      digitalWrite(D5, 0); // yellow
      digitalWrite(D0, 0); // red
    }
    // decrease warmup
    settings.warmup --;
    // reset moving average when ready
    if (settings.warmup == 0)
      mvgAvgCnt = 0; // moving average reset
  }

   
  // on SMOGGIE-PM, at sec 0 in every minute we do warmup to normalize PM sensor readings, after warmup we get actual data until sec 20 when we stop fan. FAN DUTY 33%  
  #if DEV_CLASS == SMOGGIE_PM
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
