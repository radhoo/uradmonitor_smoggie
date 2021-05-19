/**
 *	File:			drv_diginoise.cpp
 *	Version:		1.0
 *	Date:			Sept 30, 2020
 *	Description:	TTL/UART Noise sensor
 *	Project:		uRADMonitor INDUSTRIAL, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2020 Magnasci SRL, www.magnasci.com
 *
 */

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

