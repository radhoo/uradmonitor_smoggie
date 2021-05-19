/**
 *	File:			drv_sm7901.h
 *	Version:		1.0
 *	Date:			Oct 10, 2020
 *	Description:	SM7901 UART Noise sensor
 *	Project:		uRADMonitor SMOGGIE, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2020 Magnasci SRL, www.magnasci.com
 *
 */

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