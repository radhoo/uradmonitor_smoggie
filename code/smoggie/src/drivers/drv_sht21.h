//==============================================================================
//    E - R A D I O N I C A . C O M,  H.Kolomana 6/A, Djakovo 31400, Croatia
//==============================================================================
// Project   :  SHT21 Arduino Library (V1.0)
// File      :  SHT21.h
// Author    :  e-radionica.com 2017
// Licence   :  Open-source ! 
//==============================================================================
//==============================================================================
// Use with any SHT21 breakout. Check ours: 
// https://e-radionica.com/en/sht21-humidity-and-temperature-sensor.html
// If any questions,  
// just contact techsupport@e-radionica.com
//==============================================================================

#ifndef SHT21_H
#define SHT21_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>

//---------- Defines -----------------------------------------------------------
#define SHT21_ADDR 0x40	// I2C device address

const uint16_t POLYNOMIAL = 0x131;  // P(x)=x^8+x^5+x^4+1 = 100110001

//==============================================================================
#define TRIGGER_T_MEASUREMENT_HM 0XE3   // command trig. temp meas. hold master
#define TRIGGER_RH_MEASUREMENT_HM 0XE5  // command trig. hum. meas. hold master
#define TRIGGER_T_MEASUREMENT_NHM 0XF3  // command trig. temp meas. no hold master
#define TRIGGER_RH_MEASUREMENT_NHM 0XF5 // command trig. hum. meas. no hold master
#define USER_REGISTER_W 0XE6		    // command writing user register
#define USER_REGISTER_R 0XE7            // command reading user register
#define SOFT_RESET 0XFE                 // command soft reset
//==============================================================================
// HOLD MASTER - SCL line is blocked (controlled by sensor) during measurement
// NO HOLD MASTER - allows other I2C communication tasks while sensor performing
// measurements.


class SHT21
{
	private:
		uint8_t m_addr;				// sensor I2C address
		
		//==============================================================================
		uint16_t readSensor_hm(uint8_t command);
		//==============================================================================
		// reads SHT21 with hold master operation mode
		// input:	temp/hum command
		// return:	temp/hum raw data (16bit scaled)
		

		//==============================================================================
		float CalcRH(uint16_t rh);
		//==============================================================================
		// calculates the relative humidity
		// input:  rh:	 humidity raw value (16bit scaled)
		// return:		 relative humidity [%RH] (float)

		//==============================================================================
		float CalcT(uint16_t t);
		//==============================================================================
		// calculates the temperature
		// input:  t: 	temperature raw value (16bit scaled)
		// return:		relative temperature [°C] (float)

		//==============================================================================
		uint8_t CRC_Checksum(uint8_t data[], uint8_t no_of_bytes, uint8_t checksum);
		//==============================================================================
		// CRC-8 checksum for error detection
		// input:  data[]       checksum is built based on this data
		//         no_of_bytes  checksum is built for n bytes of data
		//         checksum     expected checksum
		// return:              1 			   = checksum does not match
		//                      0              = checksum matches

		
	public:
		void start(uint8_t addr = SHT21_ADDR);
		//==============================================================================
		float getHumidity(void);
		//==============================================================================
		// calls humidity measurement with hold master mode

		//==============================================================================
		float getTemperature(void);
		//==============================================================================
		// calls temperature measurement with hold master mode

		//==============================================================================
		void reset();
		//==============================================================================
		// performs a soft reset, delays 15ms

		//==============================================================================
		uint8_t getSerialNumber(uint8_t return_sn);
		//==============================================================================
		// returns electronical identification code depending of selected memory
		// location

};

#endif
