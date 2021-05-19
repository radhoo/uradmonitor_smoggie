/**
 *	File:       	pms_generic.cpp
 *	Version:  		1.0
 *	Date:       	Jan 20, 2018 - OCT 17, 2020
 *	Description:	Plantower PMS7003 / PMSA003 / PMS5003 / PMS1003 driver class
 *	Project:		uRADMonitor AIR, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2020 Magnasci SRL, www.magnasci.com
 *
 */

#include "drv_pms_generic.h"
 #include <Arduino.h>

uint8_t PMS::fusedata(char c) {
	
	buffer[index] = c;
	if (index == 0 && c != 0x42 && c != 0x44) {  // 0x42 Plantower, 0x44 Mixsen MIX6071
		index = 0; return 0;
	}
	if (index == 1 && c != 0x4d) {
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();

	}

	index ++;


	if (index == 32) index = 0; // start over

	return index;
}

bool PMS::checksum() {
	if (index != 31) return false;

	uint16_t crc = (buffer[30]  << 8) | buffer[31];

	uint16_t sum = 0;
	for (uint8_t i =0; i< 30;i++) sum+= buffer[i];

	return (crc == sum);
}

void PMS::parsedata() {
	pm1ug_std = (buffer[4] << 8) | buffer[5];
	pm25ug_std = (buffer[6] << 8) | buffer[7];
	pm10ug_std = (buffer[8] << 8) | buffer[9];
	
	pm1ug_atm = (buffer[10] << 8) | buffer[11];
	pm25ug_atm = (buffer[12] << 8) | buffer[13];
	pm10ug_atm = (buffer[14] << 8) | buffer[15];
}

// USING ATM ! (since FW15)
uint16_t PMS::getPM1() { return pm1ug_atm; }
uint16_t PMS::getPM25(){ return pm25ug_atm; }
uint16_t PMS::getPM10(){ return pm10ug_atm; }

