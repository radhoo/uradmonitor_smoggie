/**
 *	File:			zh03a.cpp
 *	Version:		1.0
 *	Date:			Mar 25, 2020
 *	Description:	Winsen ZH03A driver class
 *	Project:		uRADMonitor A3, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2020 Magnasci SRL, www.magnasci.com
 *
 */

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

