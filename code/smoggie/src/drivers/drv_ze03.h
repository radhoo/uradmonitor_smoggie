/**
 *	File:			drv_ze03.h
 *	Version:		1.0
 *	Date:			Feb 1, 2020
 *	Description:	Winsense ZE03 sensor driver
 *	Project:		uRADMonitor SMOGGIE, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2020 Magnasci SRL, www.magnasci.com
 *
 */

#pragma once
#include <stdint.h>

#define GAS_SENSOR SENSOR_GAS_WINSEN
#define GAS_SENSOR_NAME "ZE03"

enum {
	NH3  = 0x02,// 2
	H2S  = 0x03,// 3
	CO   = 0x04,// 4
	C6H6 = 0x1B,// benzene
	O3   = 0x2A,//42
	SO2  = 0x2B,//43
	NO2  = 0x2C,//44
	HCL  = 0x2E, //46
	CL2  = 0x31,//49
	HCN  = 0xF0
};


class ZE03 {

	uint8_t buf[10];
  	
	uint8_t index;
	double gas;
  uint8_t type;
public:
	ZE03() {
		index = 0;
    gas = 0;
	}
	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	
  double getGas();
  uint8_t getType();
  
  double getMultiplier();
  void getZE03Name(char *out, uint8_t len);

  uint16_t getMax();
  double getAnalogueConc(double vol);
};
