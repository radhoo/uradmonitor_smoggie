/**
 *  File:     drv_cm1106.h
 *  Version:    1.0
 *  Date:     Nov 6, 2019
 *  Description:  Cubic CM1106  co2 sensor driver
 *  Project:    uRADMonitor AIR, part of Global environmental monitoring network
 *  License:    Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *  Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *  Copyright 2015-2019 Magnasci SRL, www.magnasci.com
 *
 */

#pragma once
#include <stdint.h>


class CM11XX {

	uint8_t buf[10];
  uint8_t read_cmd[4] = 	{0x11, 0x01, 0x01, 0xED}; // 110101ED
  uint8_t abc_off[10] = 	{0x11, 0x07, 0x10, 0x64, 0x02, 0x07, 0x01, 0x90, 0x64, 0x76},
  			abc_on[10]=		{0x11, 0x07, 0x10, 0x64, 0x00, 0x07, 0x01, 0x90, 0x64, 0x78},	// 7 days
  			calco2[6] = 	{0x11, 0x03, 0x03, 0x02, 0x58, 0x8F};							// default 600ppm set
		
  uint8_t read_size = 4;
	uint8_t index;
	uint16_t co2;
public:
	CM11XX() {
		index = 0;
    co2 = 0;
	}

 
	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	uint8_t* getSendCmd();
  	uint8_t getSendCmdSize() { return read_size; }
  	uint16_t getCO2();
  
  	uint8_t* getCalCmd(uint16_t conc);
  	uint8_t getCalCmdSize() { return 6; }

  	uint8_t* getDisableABCCmd() { return  abc_off;}
  	uint8_t getDisableABCCmdSize() { return  10;}
	

	uint8_t* getEnableABCCmd() { return  abc_on;}
  	uint8_t getEnableABCCmdSize() { return  10;}
	
};
