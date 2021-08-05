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
