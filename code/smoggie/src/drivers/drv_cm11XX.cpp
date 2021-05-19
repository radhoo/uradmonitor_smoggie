/**
 *	File:			drv_cm1106.cpp
 *	Version:		1.0
 *	Date:			Nov 6, 2019
 *	Description:	Cubic CM1106  co2 sensor driver
 *	Project:		uRADMonitor AIR, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2019 Magnasci SRL, www.magnasci.com
 *
 */

#include "drv_cm11XX.h"


uint8_t CM11XX::fusedata(char c) {
	buf[index] = c;
 
	if (index == 0 && c != 0x16) {
		index = 0; return 0;
	}
	if (index == 1 && c != 0x05) {
		index = 0; return 0;
	}
  if (index == 2 && c != 0x01) {
   index = 0; return 0;
  }
  
	if (checksum()) {
		parsedata();
	}

	index ++;


	if (index == 8) index = 0; // start over

	return index;
}

uint8_t* CM11XX::getSendCmd(){ 
  return read_cmd;
}

bool CM11XX::checksum() {
  
  if (index != 7) return false;
  
  
  uint8_t checksum = 0;
  for (uint8_t i = 0; i < 7; i++)
    checksum = (checksum + buf[i]) % 256;
  checksum = 256 - checksum;
  return (checksum  == buf[7]) ;
}


void CM11XX::parsedata() {
	co2 = buf[3] * 256 + buf[4]; 
}

uint16_t CM11XX::getCO2() { return co2; }

uint8_t* CM11XX::getCalCmd(uint16_t conc) {
	 //Cumulative sum of data = 256-(HEAD+LEN+CMD+DATA)%256
	calco2[3] = (conc >> 8) & 0xFF;
	calco2[4] = conc & 0xFF;
	calco2[5] = 256 - (calco2[0] + calco2[1] + calco2[2] + calco2[3] + calco2[4]) % 256;
	return calco2;
  }
