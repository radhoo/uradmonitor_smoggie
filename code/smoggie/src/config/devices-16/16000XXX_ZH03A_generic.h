/**
 *	License:	Licensed under CC BY NC 4.0
 *  			This license lets others remix, adapt, and build upon your work non-commercially, and although their new works must also acknowledge you 
 *  			and be non-commercial, they don’t have to license their derivative works on the same terms.
 *  			You should have received a copy of the license with this work.
 *
 *  Project:    SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *  			It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *  			It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 *  Author:  	Magnasci SRL , www.magnasci.com 
 *	Web:		www.uradmonitor.com
 *
 *  Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2021 Magnasci SRL, www.magnasci.com
**/


#pragma once

#include "../sensors.h"


#define 			ENCRYPT_PAYLOAD										// Payload should always be encrypted, E3/E3B format
#define 			DEFAULT_SEND_INTERVAL 		60	 					// 5min for LoraWAN
#define 			DEFAULT_WARMUP		 		10	 					// 110sec for CO2
#define 			VER_HW						4
#define				DEV_CLASS					0x16
#define				DEFAULT_SERVER 				"data.uradmonitor.com"	// The name of the virtual host which you want to  contact to - hostname
#define 			DEFAULT_SCRIPT 				"/api/v1/upload/ex/"	// server resource to call
#define 			DEFAULT_INTERNET_SSID		""			// default WIFI to connect to
#define 			DEFAULT_INTERNET_KEY		""			// default WIFI key
#define 			DEFAULT_MUTE				1
// new in FW22
#define       		DEFAULT_DISABLEAP     		0         // 0 if not used, or number of seconds to disable internal hotspot
#define 			DEFAULT_ENCRYPT				1


#define 			SENSOR 						SENSOR_ZH03A
#define 			SENSOR_BAUDRATE				9600
//#define        		DEFAULT_DEV_NUMBER      	0x192

#define INCREASE_LIFESPAN

//#define POLL

#include "../../data/data_ex.h"

// drivers
#include "../../drivers/drv_bme280.h"
#include "../../drivers/drv_zh03a.h"

#define SENSOR_CLASS ZH03A
#define SENSOR_NAME "WINSEN ZH03A"
#define DATA_CLASS	 Data_EX
#define FACTOR	1.0

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
