/**
 *	File:       	pms7003.h
 *	Version:  		1.0
 *	Date:       	Jan 20, 2018
 *	Description:	Plantower PMS7003 / PMSA003 driver class
 *	Project:		uRADMonitor AIR, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2018 Magnasci SRL, www.magnasci.com
 *
 */

#pragma once
#include <stdint.h>

#define CO2_SENSOR SENSOR_CO2_WINSEN
#define CO2_SENSOR_NAME "MH-Z19B"

class MHZ19B {

	uint8_t buf[10];
  uint8_t read_cmd[9] = {0xFF,0x01,0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79}; // FF0186000000000079
  uint8_t read_size = 9;
	uint8_t index;
	uint16_t co2;
public:
	MHZ19B() {
		index = 0;
    co2 = 0;
	}
	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	uint8_t* getSendCmd();
  uint8_t getSendCmdSize() { return read_size; }
  uint16_t getCO2();
};
