/**
 *
 *  License:  GPL v3
 *  Project:  SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *            It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *            It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 *  Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *  Copyright 2015-2021 Magnasci SRL, www.magnasci.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
 
#pragma once

#define VER_SW            22



// system
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <Wire.h>

#define SMOGGIE_PM  0x16
#define SMOGGIE_CO2 0x17
#define SMOGGIE_CH2O 0x18
#define SMOGGIE_O3 0x19
#define SMOGGIE_GAS 0x1A
#define SMOGGIE_RADON 0x1D
#define MODEL_A3 0x82
#define MODEL_NOISE 0x1E

// device config
//#include "src/config/devices-16/16000XXX_PMSA003_generic.h"
//#include "src/config/devices-16/16000XXX_PMS1003_generic.h"
//#include "src/config/devices-16/16000XXX_PMS3003_generic.h"
//#include "src/config/devices-16/16000XXX_PMS9003M_generic.h"
//#include "src/config/devices-16/16000XXX_PMS5003_generic.h"
//#include "src/config/devices-16/16000XXX_PMS6003_generic.h"
//#include "src/config/devices-16/16000XXX_PM2009_generic.h"
//#include "src/config/devices-16/16000XXX_ZH03A_generic.h"
//#include "src/config/devices-16/16000XXX_SDS11_generic.h"
//#include "src/config/devices-16/16000XXX_SDS18_generic.h"
//#include "src/config/devices-16/16000XXX_MIX6071_generic.h"
//#include "src/config/devices-16/16000XXX_MIX6070_generic.h"
// --- CO2
//#include "src/config/devices-17/17000015.h" // SMOGGIE-CO2
//#include "src/config/devices-17/17000XXX_CM1106.h" // SMOGGIE-CO2-CUBIC
//#include "src/config/devices-17/17000XXX_CM1107.h" // SMOGGIE-CO2-CUBIC
//#include "src/config/devices-17/17000XXX_MHZ19B.h" // SMOGGIE-CO2-WINSEN/MIXSEN
//#include "src/config/devices-17/17000XXX_MIX6010.h" // SMOGGIE-CO2-/MIXSEN

//-- GAS
#include "src/config/devices-1A/1A000XXX.h" // SMOGGIE-GAS
//#include "src/config/devices-1D/1D0000XX_generic.h" // RADON
//#include "src/config/devices-82/820001D3.h" // MODEL-A3 PM

//-- NOISE
//#include "src/config/devices-1E/1E000XXX_SM7901.h" // MODEL NOISE
//#include "src/config/devices-1E/1E000XXX_DIGINOISE.h" // MODEL NOISE


#define            DEFAULT_DEV_NUMBER        0x3

//#define DEBUG


#define         READ_INTERVAL     2



#include "aux.h"
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
