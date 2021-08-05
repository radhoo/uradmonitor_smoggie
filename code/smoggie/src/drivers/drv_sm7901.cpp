/**
 *
 *  License:  GPL v3
 *  Project:  SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *            It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *            It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 *  Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *  Copyright 2015-2021 Magnasci SRL, www.magnasci.com
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
**/

#include "drv_sm7901.h"
#include <Arduino.h>

uint8_t SM7901::fusedata(char c) {
	buf[index] = c;
 	if (index == 0 && c != 0x1) { // 
		index = 0; return 0;
	}
	if (index == 1 && c != 0x3) { // 
		index = 0; return 0;
	}
	if (index == 2 && c != 0x2) { // data length 2
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();
	} 
	index ++;


	if (index == 7) index = 0; // start over

	return index;
}

uint8_t* SM7901::getSendCmd(){ 
  return read_cmd;
}

// modbus crc-16
uint16_t SM7901::crc16(const uint8_t *buffer, uint8_t length) {
	uint16_t crc = 0xFFFF;
	for (int i=0;i<length;i++) {
		crc = crc ^ *(buffer++);
		for (int j=0;j<8;j++) {
			if (crc & 0x0001)
					crc = (crc >> 1) ^ 0xA001;
			else
					crc = crc >> 1;
		}
	}
	return crc  ;
}


bool SM7901::checksum() {

//Serial.printf("%X %X %X %X %X %X %X %d\n", buf[0],buf[1],buf[2],buf[3],buf[4],buf[5], buf[6], index);
	if (index != 6) return false; // < ..56> 0x0D 0x0A
	
	return crc16(buf, 5) == (buf[6] << 8) | buf[5] ;

  	
}

void SM7901::parsedata() {
	noise = ((buf[3] << 8) | buf[4]) / 10.0;
	//Serial.printf("Noise: %.1f\n", noise);
}

double SM7901::getNoise() { return noise; }

// 1 3 2 2 62 38 CD