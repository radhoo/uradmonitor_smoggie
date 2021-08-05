/**
 * License:  Licensed under CC BY NC 4.0
 *           This license lets others remix, adapt, and build upon your work non-commercially, and although their new works must also acknowledge you 
 *           and be non-commercial, they don’t have to license their derivative works on the same terms.
 *           You should have received a copy of the license with this work.
 *
 * Project:  SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *           It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *           It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 * Author:   Magnasci SRL , www.magnasci.com 
 * Web:      www.uradmonitor.com
 *
 * Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 * Copyright 2015-2021 Magnasci SRL, www.magnasci.com
**/
 
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
