#pragma once

#include "config.h"

#define BUFFER_SIZE       4096
#define EEPROM_BASE_ADDR  0
#define CHART_POINTS      0



typedef void (*DataSendCallback)();

class uradmonitor {
  char              *m_buf,
                    ipLAN[15], ipDNS[15];                    // IPv4 for LAN  IP and DNS
  SoftwareSerial    *p_sserial;                              // RX, TX on Smoggie HW002
  //test
  //HardwareSerial    *hs;                              // RX, TX on Smoggie HW002
  //SoftwareSerial    *se2;                              // RX, TX on Smoggie HW002
  DataSendCallback  m_pSendFunc;
  

  BME280            bme280;                                  // BME280 I2C
  Calibration       calib;

  uint16_t          m_len         = 0,
                    mvgAvgCnt     = 0; 
  uint32_t          deviceId,
                    oldsec        = 0;
                
  float             lastSensorVal = 0; 
public:
  SENSOR_CLASS      sensor;
  DATA_CLASS        data;

  //uint16_t          chart[CHART_POINTS] = {0};

  t_settings        settings;                               // eeprom settings structure
  uint16_t          code          = 0;
  uint32_t          pcktsOk       = 0, 
                    pcktsTotal    = 0,
                    sec           = 0;
  bool              sensorState;     
                    
  void        init(char *buf, uint16_t len, DataSendCallback pSendFunc);
  void        setDeviceID(uint32_t devid);
  uint32_t    getDeviceID();
  
  bool        initNetwork();
  bool        isConnected();
  char*       getLocalIP();
  char*       getDNSIP();

  // 0: turn off blue led, enable sensor  1: turn on blue led, disable sensor
  void        enableSensor(bool state);

  void        printIPAddressOfHost(const char* host);
  
  void        loadSettings();
  void        saveSettings();
  void        defaultSettings();

  bool        initSensors();

  void        addAvg(double *current_avg, uint16_t current_count, double new_value);
  void        callback_answer(uint16_t webstatuscode, char *serverAnswer);
  void        callback_second(uint32_t sec);
  void        loop();

};
