#pragma once

#include "uradmonitor.h"
#include <ESP8266HTTPClient.h>


class DataClient {
  WiFiClient        client;
  uradmonitor       *m_urad;
  char              *m_buf;
  uint16_t          m_len;

  public:
  void init(uradmonitor *urad, char *buf, uint16_t len);  
  bool sendData();
};
