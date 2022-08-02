/*
** BMP280 Sensor Library for AVR Microcontrollers
** Version:       0.1.0
** Date:          August, 2022
** License:       LGPL v3
** Copyright:     (C) 2009 - 2015 Radu Motisan, radu.motisan@gmail.com
*  Ported to Arduino (C)2022 Radu Motisan, radu.motisan@magnasci.com
** Description:	The BMP280 from Bosch is a temperature and pressure sensor with a I2C interface
**
** www.pocketmagic.net
**
** This file is a part of "Portable Environmental Monitor" open source project presented on
** https://hackaday.io/project/4977-portable-environmental-monitor
**
** This project is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 3 of the License,
** or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma  once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Connecting SDO to GND results in slave
// address 1110110 (0x76); connection it to VDDIO results in slave address 1110111 (0x77), which
// is the same as BMP280’s I²C address
#define BMP280_ADDR (0x76<<1) //0x77 default I2C address
#define BMP280_ID	0x58


class BMP280 {
    enum
    {
      BMP280_REGISTER_T1              = 0x88,
      BMP280_REGISTER_T2              = 0x8A,
      BMP280_REGISTER_T3              = 0x8C,

      BMP280_REGISTER_P1              = 0x8E,
      BMP280_REGISTER_P2              = 0x90,
      BMP280_REGISTER_P3              = 0x92,
      BMP280_REGISTER_P4              = 0x94,
      BMP280_REGISTER_P5              = 0x96,
      BMP280_REGISTER_P6              = 0x98,
      BMP280_REGISTER_P7              = 0x9A,
      BMP280_REGISTER_P8              = 0x9C,
      BMP280_REGISTER_P9              = 0x9E,

      BMP280_REGISTER_H1              = 0xA1,
      BMP280_REGISTER_H2              = 0xE1,
      BMP280_REGISTER_H3              = 0xE3,
      BMP280_REGISTER_H4              = 0xE4,
      BMP280_REGISTER_H5              = 0xE5,
      BMP280_REGISTER_H6              = 0xE7,

      BMP280_REGISTER_CHIPID             = 0xD0,
      BMP280_REGISTER_VERSION            = 0xD1,
      BMP280_REGISTER_SOFTRESET          = 0xE0,

      BMP280_REGISTER_CAL26              = 0xE1,  // R calibration stored in 0xE1-0xF0
	  BMP280_CRC_DATA_ADDR				 = 0xE8,

      BMP280_REGISTER_CONTROLHUMID       = 0xF2,
      BMP280_REGISTER_STATUS             = 0XF3,
      BMP280_REGISTER_CONTROL            = 0xF4,
      BMP280_REGISTER_CONFIG             = 0xF5,
      BMP280_REGISTER_PRESSUREDATA       = 0xF7,
      BMP280_REGISTER_TEMPDATA           = 0xFA,
      BMP280_REGISTER_HUMIDDATA          = 0xFD,
    };

public:

	bool  	start( uint8_t addr = BMP280_ADDR);
	void 	readSensors(double *temperature, double *pressure);
	//bool 	checkCRC();
private:
	uint8_t m_id;				// sensor returned ID, must match 0x60
	uint8_t m_addr;				// sensor I2C address
	uint8_t buffer[26];//26

	uint16_t reg_T1;
	int16_t  reg_T2;
	int16_t  reg_T3;

	uint16_t reg_P1;
	int16_t  reg_P2;
	int16_t  reg_P3;
	int16_t  reg_P4;
	int16_t  reg_P5;
	int16_t  reg_P6;
	int16_t  reg_P7;
	int16_t  reg_P8;
	int16_t  reg_P9;


	int32_t t_fine;
	int32_t adc_TP;
	int32_t var1, var2;
	uint32_t p;

	void getcalibration();
};
