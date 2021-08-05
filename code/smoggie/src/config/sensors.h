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

// smoggie - PM , 11 variants
#define SENSOR_PMSA003   0x1601	//"PLANTOWER PMSA003"
#define SENSOR_PM2009    0x1602 //"CUBIC PM2009" / Plantower PMS5003 clone 
#define SENSOR_PMS5003   0x1603 //"PLANTOWER PMS5003"
#define SENSOR_ZH03A	 0x1604 //"WINSEN ZH03A" / Plantower PMS3003 clone , STD/ATM equal
#define SENSOR_PMS9003M	 0x1605 //"PLANTOWER PMS9003M" , STD/ATM equal
#define SENSOR_PMS1003	 0x1606 //"PLANTOWER PMS1003"
#define SENSOR_PMS3003	 0x1607 //"PLANTOWER PMS3003"
#define SENSOR_SDS11	 0x1608 //"Nova Fitness SDS11"
#define SENSOR_SDS18	 0x1609 //"Nova Fitness SDS18"
#define SENSOR_MIX6070   0x160A //"MIXSEN 6070" / Plantower PMS7003 clone , STD big, ATM small
#define SENSOR_MIX6071   0x160B //"MIXSEN 6071" / Plantower PMS7003 clone
#define SENSOR_PMS6003   0x160C //"PLANTOWER PMS6003"
#define SENSOR_PM3006    0x160D //"CUBIC PM3006"

// smoggie co2
#define SENSOR_MHZ19B    0x1701 //"WINSEN MHZ19B"
#define SENSOR_CM1106    0x1702 //"CUBIC CM1106"
#define SENSOR_CM1107    0x1703 //"CUBIC CM1107"
#define SENSOR_MIX6010   0x1704 //"MIXSEN MIX6010"

// smoggie ch2o / o3
#define SENSOR_ZE08CH2O  0x1801 //"WINSEN ZE08-CH2O"
#define SENSOR_ZE25O3    0x1901 //"WINSEN ZE25-O3"

// smoggie gas
#define SENSOR_ZE03      0x1A01 //"WINSEN ZE03"
#define SENSOR_MIX2801   0x1A02 //"MIXSEN MIX2801"

// smoggie radon
#define SENSOR_RADONSS   0x1D01 //"EUROGAS RADON SS"
#define SENSOR_RADONTERA 0x1D02 //"TESLA TERA TSRS"

// smoggie noise
#define	SENSOR_SM7901    0x1E01 //"SM7901"
#define	SENSOR_DIGINOISE 0x1E02 //"DIGI NOISE" https://www.aliexpress.com/item/4001061787078.html