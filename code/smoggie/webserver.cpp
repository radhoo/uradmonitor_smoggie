#include "webserver.h"

void Webserver::loop() {
  server.handleClient();
}

void Webserver::init(uradmonitor *urad, char *buf, uint16_t len) {
  m_urad = urad;
  m_buf = buf;
  m_len = len;
  // webserver
  server.on("/", std::bind(&Webserver::handleRoot, this));
  server.on("/j", std::bind(&Webserver::handleJSON , this));
  server.on("/c", std::bind(&Webserver::handleConfig , this));
  server.on("/s", std::bind(&Webserver::handleSave , this));
  server.onNotFound (std::bind(&Webserver::handleNotFound , this));
  server.begin();
}


// Build an HTML page to display on the web-server root address`
void Webserver::handleRoot() {
  snprintf(m_buf, m_len,
    "<html><head><meta http-equiv=\"refresh\" content=\"10\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>uRADMonitor</title></head>"
#if DEV_CLASS == MODEL_A3    
    "<body><font face='courier'><br><br><center><h3><u>uRADMonitor <a href=\"https://www.uradmonitor.com/?open=%08lX\">MODEL A3 %08lX</a> - HW:%d SW:%d</u></h3><h4>%s - %s</h4>"
#elif DEV_CLASS == SMOGGIE_RADON
  "<body><font face='courier'><br><br><center><h3><u>uRADMonitor <a href=\"https://www.uradmonitor.com/?open=%08lX\">seeRADON %08lX</a> - HW:%d SW:%d</u></h3><h4>%s - %s</h4>"
#else    
    "<body><font face='courier'><br><br><center><h3><u>uRADMonitor <a href=\"https://www.uradmonitor.com/?open=%08lX\">SMOGGIE %08lX</a> - HW:%d SW:%d</u></h3><h4>%s - %s</h4>"
#endif    
#if !(defined(USE_BME) || defined(USE_BMP))
    "<table cellpadding='10px' style='margin-left: auto;margin-right: auto;overflow-x:auto;display:block;'><tr><td><b>Temperature:</b>%sC<br><b>Humidity:</b>%sRH</td>"
#else    
    "<table cellpadding='10px' style='margin-left: auto;margin-right: auto;overflow-x:auto;display:block;'><tr><td><b>Temperature:</b>%sC<br><b>Pressure:</b>%sPa<br><b>Humidity:</b>%sRH</td>"
#endif    
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3 || DEV_CLASS == SMOGGIE_PM_KIT
    "<td><b>PM1.0:</b>%uug/m^3<br><b>PM2.5:</b>%uug/m^3<br><b>PM10:</b>%uug/m^3</td>"
#elif DEV_CLASS == SMOGGIE_CO2    
    "<td><b>CO2:</b>%uppm<br><br></td>"
#elif DEV_CLASS == SMOGGIE_GAS    
  "<td><b>%s:</b>%.3fppm<br><br><b>VOL:</b>%.3fV</td>"
#elif DEV_CLASS == SMOGGIE_RADON    
    "<td><b>Radon:</b>%uBq/m^3<br><br></td>"
#elif DEV_CLASS == MODEL_NOISE
    "<td><b>Noise:</b>%.1fdB<br><br></td>"
#endif    
    "<td><b>Time:</b>%lus<br><b>Interval:</b>%us<br><b>Stats:</b>%lu/%lu %u</td>"
    "<td><b>WIFI:</b>%s<br><b>IP:</b>%s<br><b>DNS:</b>%s</td></tr></table>"
    "Warmup: %us | %s | <a href=\"/j\">JSON</a> | <a href=\"/c\">CONFIG</a><br><br>"
    "</body></html>",
      m_urad->getDeviceID(), m_urad->getDeviceID(), VER_HW, VER_SW, SENSOR_NAME, (m_urad->sensorState?"<span style=\"color:green;\">running</span>":"<span style=\"color:red;\">idle</span>"),
      
      String(m_urad->data.getTemperature(),2).c_str(), 
#if !(defined(USE_BME) || defined(USE_BMP))
#else
      String(m_urad->data.getPressure(),2).c_str(), 
#endif      
      String(m_urad->data.getHumidity(),2).c_str(),  
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3 || DEV_CLASS == SMOGGIE_PM_KIT
      m_urad->data.getPM1(), m_urad->data.getPM25(), m_urad->data.getPM10(), 
#elif DEV_CLASS == SMOGGIE_CO2    
      m_urad->data.getCO2(),
#elif DEV_CLASS == SMOGGIE_GAS  
      m_urad->data.getName(), m_urad->data.getGas(), m_urad->data.getVoltage(),
#elif DEV_CLASS == SMOGGIE_RADON    
      m_urad->data.getRadon(),
#elif DEV_CLASS == MODEL_NOISE
      m_urad->data.getNoise(),      
#endif    
    m_urad->sec, m_urad->settings.sendInt, m_urad->pcktsOk, m_urad->pcktsTotal, m_urad->code,
    m_urad->isConnected()?"connected":"disconnected", 
    m_urad->getLocalIP(), 
    m_urad->getDNSIP(),
    m_urad->settings.warmup,
    ((m_urad->data.valid == true)?"valid":"invalid"));
  /*// create chart
  snprintf(m_buf, m_len, "%s<div style=\"width:25%%\"><svg viewBox=\"0 0 600 300\" class=\"chart\"><polyline fill=\"none\" stroke=\"#0074d9\" stroke-width=\"1\" points=\"", m_buf);  
  for (uint8_t i = 0; i < CHART_POINTS; i++)
    snprintf(m_buf, m_len, "%s%u,%u ", m_buf, i * 20, m_urad->chart[i]);
  snprintf(m_buf, m_len, "%s\"/></svg></div><br>*/
  snprintf(m_buf, m_len, "%s<font size=\"-2\"><a href=\"https://www.uradmonitor.com\">uRADMonitor</a>, a Magnasci SRL 2015-2022 project</font>", m_buf);  

  uint16_t cs = strlen(m_buf);
  snprintf(m_buf, m_len,"%s<br><br>%u</body></html>", m_buf, strlen(m_buf));
  
  server.send ( 200, "text/html", m_buf );
}

void Webserver::handleJSON() {
  //data.buildJSON(m_buf, m_len,  deviceId,  DEV_CLASS, sec);
  snprintf ( m_buf, m_len, "{\"data\":{\"id\":\"%08lX\",\"type\":\"%X\","
#if !(defined(USE_BME) || defined(USE_BMP))
  "\"temperature\":%s,\"humidity\":%s,"
#else
  "\"temperature\":%s,\"humidity\":%s,\"pressure\":%s,"
#endif
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3
  "\"pm1\":%u,\"pm25\":%u,\"pm10\":%u,"
#elif DEV_CLASS == SMOGGIE_PM_KIT
 "\"pm1\":%u,\"pm25\":%u,\"pm10\":%u,\"pm03no\":%u,\"pm05no\":%u,\"pm1no\":%u,\"pm25no\":%u,\"pm5no\":%u,\"pm10no\":%u,"
#elif DEV_CLASS == SMOGGIE_CO2   
  "\"co2\":%u,"
#elif DEV_CLASS == SMOGGIE_GAS
  "\"%s\":%.3f,"
#elif DEV_CLASS == SMOGGIE_RADON 
  "\"radon\":%u,"
#elif DEV_CLASS == MODEL_NOISE
  "\"nois\":%.1f,"
#endif
  "\"valid\":%u,\"uptime\":%lu}}",
  m_urad->getDeviceID(), DEV_CLASS, 
#if !(defined(USE_BME) || defined(USE_BMP))
  String(m_urad->data.getTemperature(),2).c_str(),  String(m_urad->data.getHumidity(),2).c_str(), 
#else
  String(m_urad->data.getTemperature(),2).c_str(),  String(m_urad->data.getHumidity(),2).c_str(),   String(m_urad->data.getPressure(),2).c_str(), 
#endif  
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3
    m_urad->data.getPM1(), 
    m_urad->data.getPM25(), 
    m_urad->data.getPM10(), 
#elif DEV_CLASS == SMOGGIE_PM_KIT
    m_urad->data.getPM1(), 
    m_urad->data.getPM25(), 
    m_urad->data.getPM10(), 
    m_urad->data.getPM03No(), 
    m_urad->data.getPM05No(), 
    m_urad->data.getPM1No(), 
    m_urad->data.getPM25No(), 
    m_urad->data.getPM5No(), 
    m_urad->data.getPM10No(), 
#elif DEV_CLASS == SMOGGIE_CO2    
    m_urad->data.getCO2(),
#elif DEV_CLASS == SMOGGIE_GAS  
    m_urad->data.getName(),
    m_urad->data.getGas(),
#elif DEV_CLASS == SMOGGIE_RADON 
    m_urad->data.getRadon(),
#elif DEV_CLASS == MODEL_NOISE
    m_urad->data.getNoise(),    
#endif    
  m_urad->data.valid, m_urad->sec);

  server.send( 200, "text/html", m_buf);
}

char *signal(int rssi) {
  if (rssi > -50) return "very good";
  else if (rssi > -60) return "good";
  else if (rssi > -70) return "average";
  else if (rssi > -80) return "low";
  else  return "weak";
}


void Webserver::handleConfig() {
  snprintf( m_buf, m_len, 
  "<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Config</title></head>\
  <body><font face='courier'><div stlye=\"width:80%;max-width:400px;\">\
  <h3>Configuration</h3><br>\
  <form action=\"/s\" method=\"get\">\
  <b>Connect to: %s</b><br>\
  <div style=\"overflow-y: auto;overflow-x: hidden; max-width:400px; height:100px; \">", m_urad->settings.ssid );
  
  // blocking, this takes a little while
  int numberOfNetworks = WiFi.scanNetworks();
  if (numberOfNetworks == 0)
    snprintf( m_buf, m_len,  "%sCannot connect, no network in range.", m_buf);
  else {
    for(int i =0; i<numberOfNetworks; i++) {
      char tmp[64] = {0};
      strncpy(tmp, WiFi.SSID(i).c_str(), 64);
      if (strcmp(tmp, m_urad->settings.ssid) == 0)
        snprintf( m_buf, m_len,  "%s<input type=\"radio\" id=\"ssid\" name=\"ssid\" value=\"%s\" checked/> %s [%s]<br>",m_buf,  tmp, tmp, signal(WiFi.RSSI(i)));
      else
        snprintf( m_buf, m_len,  "%s<input type=\"radio\" id=\"ssid\" name=\"ssid\" value=\"%s\"/> %s [%s]<br>",m_buf,  tmp, tmp, signal(WiFi.RSSI(i)));
    }
  }
  
  snprintf(m_buf, m_len,"%s\
    </div>\
    Enter key: <input type=\"password\" placeholder=\"KEY?\" id=\"wifikey\" name=\"wifikey\" value=\"\">\
    <br><br><b>Sensor WiFi:</b> %s<br>\
    New key: <input type=\"text\" placeholder=\"type key\" id=\"ok1\" name=\"ok1\" value=\"%s\"><br>\
    Retype : <input type=\"text\" placeholder=\"type again\" id=\"ok2\" name=\"ok2\" value=\"%s\"><br>\
    <br>",
  m_buf,
  m_urad->settings.ownssid,
  m_urad->settings.ownkey, m_urad->settings.ownkey
  );

   snprintf(m_buf, m_len,"%s\
    <br><b>Your User (<a href=\"http://www.uradmonitor.com/dashboard\">?</a>):</b><br>\
    User-id: <input type=\"text\" placeholder=\"type key\" id=\"userid\" name=\"userid\" value=\"%s\"><br>\
    User-key : <input type=\"text\" placeholder=\"type again\" id=\"userkey\" name=\"userkey\" value=\"%s\"><br>\
    <br><br><button>Save</button>\
    <button type=\"submit\" formaction=\"/\">Back</button>\
    </form>\
  </font></body>\
  </html>",
  m_buf,
  m_urad->settings.userid,
  m_urad->settings.userkey
  );
    
  server.send( 200, "text/html", m_buf);
}


void Webserver::handleSave() { 
  char tmp1[64] = {0}, tmp2[64] = {0};
  strcpy(tmp1, server.arg("ssid").c_str());
  strcpy(tmp2, server.arg("wifikey").c_str());
  
  if(!strlen(tmp1) || !strlen(tmp2)) {
    server.send(500, "text/plain", "Wrong Wifi SSID or Key");
    return;
  }

  strcpy(tmp1, server.arg("ok1").c_str());
  strcpy(tmp2, server.arg("ok2").c_str());

  if(strcmp(tmp1, tmp2) !=0 || strlen(tmp1) < 8) {
   server.send(500, "text/plain", "Wifi Keys don't match or wrong.");
    return;
  }
  
  // save regardless of result?!
  strncpy(m_urad->settings.ssid, server.arg("ssid").c_str(), 64);
  strncpy(m_urad->settings.key, server.arg("wifikey").c_str(), 64);
  strncpy(m_urad->settings.ownkey, server.arg("ok1").c_str(), 64);
  strncpy(m_urad->settings.userid, server.arg("userid").c_str(), 64);
  strncpy(m_urad->settings.userkey, server.arg("userkey").c_str(), 64);
     
  // commit to EEPROM
  m_urad->saveSettings();
  // go back to main page
  handleRoot();
  
  // 
  m_urad->initNetwork();
}


void Webserver::handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.send ( 404, "text/plain", message );
  
}
