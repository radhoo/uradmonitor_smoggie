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
