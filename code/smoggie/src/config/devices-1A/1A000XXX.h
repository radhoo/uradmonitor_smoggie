
/**
 *  License:    GPL v3
 *  
 *  Project:    SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *  It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *  It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 *  Copyright 2019 by Magnasci SRL, www.magnasci.com
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
 */

#pragma once

#include "../sensors.h"


#define 			ENCRYPT_PAYLOAD										// Payload should always be encrypted, E3/E3B format
#define 			DEFAULT_SEND_INTERVAL 		60	 					// 5min for LoraWAN
#define 			DEFAULT_WARMUP		 		10	 					// 90sec for gas
#define 			VER_HW						4
#define				DEV_CLASS					SMOGGIE_GAS
#define				DEFAULT_SERVER 				"data.uradmonitor.com"	// The name of the virtual host which you want to  contact to - hostname
#define 			DEFAULT_SCRIPT 				"/api/v1/upload/ex/"	// server resource to call
#define 			DEFAULT_INTERNET_SSID		""			// default WIFI to connect to
#define 			DEFAULT_INTERNET_KEY		""			// default WIFI key
#define 			DEFAULT_MUTE				1
// new in FW22
#define       		DEFAULT_DISABLEAP     		0         // 0 if not used, or number of seconds to disable internal hotspot
#define 			DEFAULT_ENCRYPT				1


#define 			SENSOR 						SENSOR_ZE03
#define 			SENSOR_BAUDRATE				9600


#include "../../data/data_ex.h"
// drivers
#include "../../drivers/drv_bme280.h"
#include "../../drivers/drv_ze03.h"

#define SENSOR_CLASS ZE03
#define SENSOR_NAME "MIXSEN MIX2820"
//#define SENSOR_NAME "WINSEN ZE03"
#define DATA_CLASS	 Data_EX

#define				CAL_DEF_TEMPERATURE			0x00648258		// -6
#define				CAL_DEF_PRESSURE			0x00640000		// 1*x + 0
#define				CAL_DEF_HUMIDITY			0x006407D0		// +20
#define				CAL_DEF_VOC					0x00640000		// 1*x + 0
#define				CAL_DEF_CO2					0x00640000		// 1*x + 0
#define				CAL_DEF_CH2O				0x00640000		// 1*x + 0
#define				CAL_DEF_PM25				0x00640000		// 1*x + 0
#define				CAL_DEF_NOISE				0x00640000		// 1*x + 0
#define				CAL_DEF_PM1					0x00640000		// 1*x + 0
#define				CAL_DEF_PM10				0x00640000		// 1*x + 0
#define				CAL_DEF_O3					0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S1				0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S2				0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S3				0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S4				0x00640000		// 1*x + 0

#define				MUL_NH3  					0.001
#define				MUL_H2S  					0.01
#define				MUL_CO   					0.001
#define				MUL_C6H6 					0.001
#define				MUL_O3   					0.001
#define				MUL_SO2  					0.001
#define				MUL_NO2  					0.001
#define				MUL_HCL  					0.001
#define				MUL_CL2  					0.001
#define				MUL_HCN  					0.001
