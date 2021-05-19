/**
 *	File:       	pm2009.cpp
 *	Version:  		1.0
 *	Date:       	Feb 15, 2020
 *	Description:	CUBIC PM2009 Driver class
 *	Project:		uRADMonitor SMOGGIE, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2020 Magnasci SRL, www.magnasci.com
 *
 */

#include "drv_pm2009.h"
#include <Arduino.h>

uint8_t PM2009::fusedata(char c) {
	
	buffer[index] = c;
	if (index == 0 && c != 0x16) {
		index = 0; return 0;
	}
	if (index == 1 && c != 0x35) {
		index = 0; return 0;
	}
	if (index == 2 && c != 0x0B) {
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();

	}

	index ++;


	if (index == 56) index = 0; // start over

	return index;
}

bool PM2009::checksum() {
	if (index != 55) return false;
	uint8_t checksum = 0xFF;
  	for (uint8_t i = 0; i < 55; i++)
    	checksum -= buffer[i];
  	return ((checksum +1) == buffer[55]) ;
}

void PM2009::parsedata() {
	uint16_t tmp1 = 0, tmp25 = 0, tmp10 = 0;

	tmp1 = (buffer[5] << 8) | buffer[6];
	tmp25 = (buffer[9] << 8) | buffer[10];
	tmp10 = (buffer[13] << 8) | buffer[14];
	if (tmp1 <= tmp25 && tmp25 <= tmp10) {
		pm1ug = tmp1;
		pm25ug = tmp25;
		pm10ug = tmp10;
	}
}

uint8_t* PM2009::getSendCmd(){ 
  return read_cmd;
}

uint16_t PM2009::getPM1() { return pm1ug; }
uint16_t PM2009::getPM25() { return pm25ug; }
uint16_t PM2009::getPM10() { return pm10ug; }

