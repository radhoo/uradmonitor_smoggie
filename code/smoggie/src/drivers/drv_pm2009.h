/**
 *	File:       	pm2009.h
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

#pragma once
#include <stdint.h>

class PM2009 {

	uint8_t buffer[56]; //56
	uint8_t index;
	// pm in ug/m^3
	uint16_t pm1ug, pm25ug, pm10ug;

	
  	uint8_t read_cmd[5] = {0x11, 0x02, 0x0B, 0x07, 0xDB};
  	uint8_t read_size = 5;
public:
	PM2009() {
		// instant value
		pm1ug = 0;
		pm25ug = 0;
		pm10ug = 0;
		// compensated values
		index = 0;
	}
	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	uint8_t* getSendCmd();
	uint8_t getSendCmdSize() { return read_size; }

	uint16_t getPM1();
	uint16_t getPM25();
	uint16_t getPM10();

	
};