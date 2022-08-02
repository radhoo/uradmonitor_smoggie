/* Create a WiFi access point and provide a web server on it. */
// FW006 - current
#include "uradmonitor.h"
#include "webserver.h"
#include "data_client.h"

uradmonitor       urad;
Webserver         web;
DataClient        client;

char buffer[BUFFER_SIZE];

void sendFunc() {
  client.sendData();
}

void setup() {
  Serial.begin(9600);
  
  urad.init(buffer, BUFFER_SIZE, &sendFunc);
  // configure globals
#if DEV_CLASS == SMOGGIE_PM_KIT
  byte mac[6];  
  WiFi.macAddress(mac);
  urad.setDeviceID(((uint32_t)DEV_CLASS << 24) | ((uint32_t)mac[3] << 16) | ((uint32_t)mac[4] << 8) | ((uint32_t)mac[5]));
  #ifdef DEBUG
    Serial.printf("Device ID: %08lX\n", urad.getDeviceID());
  #endif  
#else
  urad.setDeviceID(((uint32_t)DEV_CLASS << 24) | ((uint32_t)DEFAULT_DEV_NUMBER & 0x00FFFFFF));
#endif
  // goes before initnetwork
  urad.loadSettings();
  
  // init sensors
  urad.initSensors();

  urad.initNetwork();  

  
  // init webserver
  web.init(&urad, buffer, BUFFER_SIZE);

  // init client
  client.init(&urad, buffer, BUFFER_SIZE);
 
}



void loop() {
  urad.loop() ;
  web.loop(); 
}
