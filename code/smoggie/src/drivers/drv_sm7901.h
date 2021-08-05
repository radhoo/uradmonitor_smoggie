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


class SM7901 {

	uint8_t buf[7];
	uint8_t read_cmd[8] = {0x01, 0x03, 0x0, 0x0, 0x0, 0x01, 0x84, 0x0A};
	uint8_t read_size = 8;
	uint8_t index;
	double noise;
	uint16_t crc16(const uint8_t *buffer, uint8_t length);
public:
	SM7901() {
		index = 0;
    	noise = 0;
	}
	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	uint8_t* getSendCmd();
	uint8_t getSendCmdSize() { return read_size; }
	double getNoise();
};