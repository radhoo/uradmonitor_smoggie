#include "data_client.h"
#include <Arduino.h>

void DataClient::init(uradmonitor *urad, char *buf, uint16_t len) {
  m_urad = urad;
  m_buf = buf;
  m_len = len;
}

// HTTP Post to uRADMonitor server
bool DataClient::sendData() {
  
}
