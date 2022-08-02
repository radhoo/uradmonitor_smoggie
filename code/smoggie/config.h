#pragma once

#define VER_SW            25



// system
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <Wire.h>

#define SMOGGIE_PM_KIT 0x20

// device config
#include "src/config/devices-20/2000XXX_PMS5003_generic.h"


//#define DEBUG


#define         READ_INTERVAL     2



#include "misc.h"
#include "Calibration.h"
#include "limits.h"

// eeprom settings structure
typedef struct {
  char sig[4];
  uint16_t sendInt;   // 2
  uint16_t warmup;    // 2
  uint8_t flag_mute;  // 1
  uint16_t disableap; // 2
  uint8_t flag_encrypt;

  char ssid[64];      // 64
  char key[64];       // 64
  char userid[64];    // 64
  char userkey[64];   // 64
  char ownssid[64];
  char ownkey[64];
  char server[64];
  char script[64];

  uint32_t cal[CAL_VALUES];  // 60B: CAL_VALUES * 4 size, concatenated as AABB for ax+b

} t_settings; // total:
