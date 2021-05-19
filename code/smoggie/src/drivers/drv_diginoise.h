/**
 *	File:			drv_diginoise.h
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

#pragma once
#include <stdint.h>


/*
	reports BB AA 01 7F 02 E7
 */

class DIGINOISE {

	uint8_t buf[10];
  	
	uint8_t index;
	double noise;
	
public:
	double offset; // analogue offset
	DIGINOISE() {
		index = 0;
		noise = 0;
	}

	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	
  	double getNoise();
};
