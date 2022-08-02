#include "data_client.h"
#include <Arduino.h>

void DataClient::init(uradmonitor *urad, char *buf, uint16_t len) {
  m_urad = urad;
  m_buf = buf;
  m_len = len;
}

// HTTP Post to uRADMonitor server
bool DataClient::sendData() {
  //Serial.println("sending");
  // prepare data
  m_urad->data.packData(m_urad->settings.flag_encrypt, m_buf, m_len, m_urad->getDeviceID(), m_urad->sec, VER_HW, VER_SW);
  //Serial.printf("Prep send [%s][did:%08lX]\n", m_buf, m_urad->getDeviceID());
  // send it via EX protocol
  if ( client.connect(m_urad->settings.server, 80)) {
    // build HTTP packet
    client.print("POST "); client.print(m_urad->settings.script); client.print("/");       
    client.print(m_buf); client.println(" HTTP/1.1");
    client.print("Host: "); client.println(m_urad->settings.server);
    client.println("Accept: */*");
    client.println("Content-Length: 0");
    client.printf("X-User-id: %s\r\n", m_urad->settings.userid);
    client.printf("X-User-hash: %s\r\n", m_urad->settings.userkey);
    client.printf("X-Device-id: %08lX\r\n", m_urad->getDeviceID());
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    
    // read answer
    int n = 0;
    char value[20] = {0};
    uint16_t code = 0;
    while (n = client.readBytesUntil('\n', m_buf, m_len - 1))  {
      m_buf[n] = 0;
      if (find(m_buf, "HTTP/", " ", " ", value, 20))
        code = atoi(value);
       //Serial.printf("(%s)[%s]\n", value, m_buf);
      if (strchr(m_buf, '{') && strchr(m_buf, '}'))
        m_urad->callback_answer(code, m_buf);          
    }
    
    client.stop();  
    return true;    
  } else {
    //Serial.println("can't connect");
    return false;
  }
}
