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


#include "drv_zh03a.h"
 #include <Arduino.h>

uint8_t ZH03A::fusedata(char c) {

	buffer[index] = c;
	if ((index == 0 && c != 0x42) || (index == 1 && c != 0x4d)) {
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();

	}

	index ++;


	if (index == 24) index = 0; // start over

	return index;
}

bool ZH03A::checksum() {
	if (index != 23) return false;

	uint16_t crc = (buffer[22]  << 8) | buffer[23];

	uint16_t sum = 0;
	for (uint8_t i =0; i< 22;i++) sum+= buffer[i];

	return (crc == sum);
}

void ZH03A::parsedata() {
	uint16_t pm1ug_std = (buffer[4] << 8) | buffer[5];
	uint16_t pm25ug_std = (buffer[6] << 8) | buffer[7];
	uint16_t pm10ug_std = (buffer[8] << 8) | buffer[9]; // no STD implemented on ZH03A
	

	pm1ug_atm = (buffer[10] << 8) | buffer[11]; // used
	pm25ug_atm = (buffer[12] << 8) | buffer[13]; // used
	pm10ug_atm = (buffer[14] << 8) | buffer[15]; // used

	//Serial.printf("STD:%u %u %u ATM:%u %u %u\n", pm1ug_std, pm25ug_std, pm10ug_std ,pm1ug_atm, pm25ug_atm, pm10ug_atm );
}

uint16_t ZH03A::getPM1() { return pm1ug_atm; }
uint16_t ZH03A::getPM25(){ return pm25ug_atm; }
uint16_t ZH03A::getPM10(){ return pm10ug_atm; }

