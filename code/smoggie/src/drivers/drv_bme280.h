/**
 *    License:    Licensed under CC BY NC 4.0
 *                This license lets others remix, adapt, and build upon your work non-commercially, and although their new works must also acknowledge you 
 *                and be non-commercial, they don’t have to license their derivative works on the same terms.
 *                You should have received a copy of the license with this work.
 *
 *  Project:    SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *                It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *                It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 *  Author:       Magnasci SRL , www.magnasci.com 
 *    Web:        www.uradmonitor.com
 *
 *  Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *    Copyright 2015-2021 Magnasci SRL, www.magnasci.com
**/


#pragma  once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Connecting SDO to GND results in slave
// address 1110110 (0x76); connection it to VDDIO results in slave address 1110111 (0x77), which
// is the same as BMP280’s I²C address
#define BME280_ADDR (0x76<<1) //0x77 default I2C address
#define BME280_ID	0x60


class BME280 {
    enum
    {
      BME280_REGISTER_T1              = 0x88,
      BME280_REGISTER_T2              = 0x8A,
      BME280_REGISTER_T3              = 0x8C,

      BME280_REGISTER_P1              = 0x8E,
      BME280_REGISTER_P2              = 0x90,
      BME280_REGISTER_P3              = 0x92,
      BME280_REGISTER_P4              = 0x94,
      BME280_REGISTER_P5              = 0x96,
      BME280_REGISTER_P6              = 0x98,
      BME280_REGISTER_P7              = 0x9A,
      BME280_REGISTER_P8              = 0x9C,
      BME280_REGISTER_P9              = 0x9E,

      BME280_REGISTER_H1              = 0xA1,
      BME280_REGISTER_H2              = 0xE1,
      BME280_REGISTER_H3              = 0xE3,
      BME280_REGISTER_H4              = 0xE4,
      BME280_REGISTER_H5              = 0xE5,
      BME280_REGISTER_H6              = 0xE7,

      BME280_REGISTER_CHIPID             = 0xD0,
      BME280_REGISTER_VERSION            = 0xD1,
      BME280_REGISTER_SOFTRESET          = 0xE0,

      BME280_REGISTER_CAL26              = 0xE1,  // R calibration stored in 0xE1-0xF0
	  BME280_CRC_DATA_ADDR				 = 0xE8,

      BME280_REGISTER_CONTROLHUMID       = 0xF2,
      BME280_REGISTER_STATUS             = 0XF3,
      BME280_REGISTER_CONTROL            = 0xF4,
      BME280_REGISTER_CONFIG             = 0xF5,
      BME280_REGISTER_PRESSUREDATA       = 0xF7,
      BME280_REGISTER_TEMPDATA           = 0xFA,
      BME280_REGISTER_HUMIDDATA          = 0xFD,
    };

public:

	bool  	start( uint8_t addr = BME280_ADDR);
	void 	readSensors(double *temperature, double *pressure, double *humidity);
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

	uint8_t  reg_H1;
	int16_t  reg_H2;
	uint8_t  reg_H3;
	int16_t  reg_H4;
	int16_t  reg_H5;
	int8_t   reg_H6;

	int32_t t_fine;
	int32_t adc_TPH;
	int32_t var1, var2;
	uint32_t p;

	void getcalibration();
};
