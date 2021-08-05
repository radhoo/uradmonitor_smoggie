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

#include "drv_diginoise.h"

/*
5.Communication Command Description:
  1>.Module software version number command:
    1.1>.For example:BB AA 00 02 00 67
    1.2>Start Character:BB AA,which indicates the beginning of a frame
    1.3>.Command:00,indicating that this frame is the frame that returns the software version number of the module
    1.4>.Data:02 00,indicating that the module software version number is 2.0
    1.5>.Check Value:67,which is the arithmetic and check value of BB,AA,00,02,00.
    1.6>.This frame is the first packet returned after the module is powered on,and only returned once after the power is turned on.
  2>.Decibel data command:
    1.1>.For example:BB AA 01 7F 02 E7
    1.2>Start character:BB AA,which indicates the beginning of a frame
    1.3>.Command:01,indicating that this frame is returning decibel data
    1.4>.Data:7F 02,which indicates that the detected decibel value is 63.9dB.Low byte comes first,high byte comes after,and each unit is 0.1dB
    1.5>.Check value:E7 is the arithmetic and check value of BB,AA,01,7F,02
    1.6>.This frame is returned to the software version after the module is powered on,and is returned every 500ms.

*/

uint8_t DIGINOISE::fusedata(char c) {
	buf[index] = c;

 	if (index == 0 && c != 0xBB) {
		index = 0; return 0;
	}
	if (index == 1 && c != 0xAA) {
		index = 0; return 0;
	}
	if (index == 2 && c != 0x1) {
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();
	}
	index ++;

	if (index == 6) index = 0; // start over

	return index;
}


bool DIGINOISE::checksum() {
	if (index != 5) return false;

  	uint8_t checksum = 0x0;
  	for (uint8_t i = 0; i < 5; i++)
    	checksum += buf[i];
  	return (checksum == buf[5]) ;
}

void DIGINOISE::parsedata() {
	noise = (buf[4] * 256 + buf[3]) / 10.0; 
}

double DIGINOISE::getNoise() { return noise; }

