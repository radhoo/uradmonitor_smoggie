#pragma once

#include "config.h"
#include "uradmonitor.h"
#include <ESP8266WebServer.h>

class Webserver {
  ESP8266WebServer  server;                             // Define a web server at port 80 for HTTP

  uradmonitor *m_urad;
  char *m_buf;
  uint16_t m_len;

  public:

  void init(uradmonitor *urad, char *buf, uint16_t len);
  void loop();

  // webpages
  void handleRoot();
  void handleJSON();
  void handleConfig();
  void handleSave() ;
  void handleNotFound();
  
};
