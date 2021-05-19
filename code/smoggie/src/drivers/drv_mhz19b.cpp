/**
 *	File:			drv_mhz19b.cpp
 *	Version:		1.0
 *	Date:			Jan 20, 2018
 *	Description:	Winsense MHZ19B co2 sensor driver
 *	Project:		uRADMonitor AIR, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2019 Magnasci SRL, www.magnasci.com
 *
 */

#include "drv_mhz19b.h"

uint8_t MHZ19B::fusedata(char c) {
	buf[index] = c;
 //Serial.printf("%02X ", c);
	if (index == 0 && c != 0xFF) {
		index = 0; return 0;
	}
	if (index == 1 && c != 0x86) {
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();
	}

	index ++;


	if (index == 9) index = 0; // start over

	return index;
}

uint8_t* MHZ19B::getSendCmd(){ 
  return read_cmd;
}

bool MHZ19B::checksum() {
	if (index != 8) return false;
  uint8_t checksum = 0xFF;
  for (uint8_t i = 1; i < 8; i++)
    checksum -= buf[i];
  return ((checksum +1) == buf[8]) ;
}

void MHZ19B::parsedata() {
	co2 = buf[2] * 256 + buf[3]; 
}

uint16_t MHZ19B::getCO2() { return co2; }
