//==============================================================================
//    E - R A D I O N I C A . C O M,  H.Kolomana 6/A, Djakovo 31400, Croatia
//==============================================================================
// Project   :  SHT21 Arduino Library (V1.0)
// File      :  SHT21.cpp
// Author    :  e-radionica.com 2017
// Licence   :  Open-source ! 
//==============================================================================
//==============================================================================
// Use with any SHT21 breakout. Check ours: 
// https://e-radionica.com/en/sht21-humidity-and-temperature-sensor.html
// If any questions, 
// just contact techsupport@e-radionica.com
//==============================================================================

#include <Arduino.h>
#include <Wire.h>
#include "drv_SHT21.h"

//==============================================================================
// PUBLIC
//==============================================================================
void SHT21::start(uint8_t a) {
	m_addr = a;
	Wire.begin();		
}
float SHT21::getHumidity(void) {
	uint16_t result; 	// return variable
	
	result = readSensor_hm(TRIGGER_RH_MEASUREMENT_NHM);

	return CalcRH(result);
}

float SHT21::getTemperature(void) {
	uint16_t result; 	// return variable
	
	result = readSensor_hm(TRIGGER_T_MEASUREMENT_NHM);

	return CalcT(result);
}

void SHT21::reset() {
	Wire.beginTransmission(m_addr);
	Wire.write(SOFT_RESET);
	Wire.endTransmission();

	delay(15);	// wait for SHT to reset
}

uint8_t SHT21::getSerialNumber(uint8_t return_sn) {

	uint8_t serialNumber[8];

	// read memory location 1
	Wire.beginTransmission(m_addr);
	Wire.write(0xFA);
	Wire.write(0x0F);
	Wire.endTransmission();

	Wire.requestFrom(m_addr,8);
	while(Wire.available() < 8) {}

	serialNumber[5] = Wire.read();	// read SNB_3
	Wire.read();					// CRC SNB_3 not used
	serialNumber[4] = Wire.read();  // read SNB_2
	Wire.read();					// CRC SNB_2 not used
	serialNumber[3] = Wire.read();	// read SNB_1
	Wire.read();					// CRC SNB_1 not used
	serialNumber[2] = Wire.read();	// read SNB_0
	Wire.read();					// CRC SNB_0 not used

	// read memory location 2
	Wire.beginTransmission(m_addr);
	Wire.write(0xFC);
	Wire.write(0xC9);
	Wire.endTransmission();

	Wire.requestFrom(m_addr,6);
	while(Wire.available() < 6) {}

	serialNumber[1] = Wire.read();	// read SNC_1
	serialNumber[0] = Wire.read();  // read SNC_0
	Wire.read();					// CRC SNC_1/SNC_0 not used
	serialNumber[7] = Wire.read();	// read SNA_1			
	serialNumber[6] = Wire.read();	// read SNA_0
	Wire.read();					// CRC SNA_1/SNA_0 not used

	return serialNumber[return_sn];
}

//==============================================================================
// PRIVATE
//==============================================================================

uint16_t SHT21::readSensor_hm(uint8_t command) {
	uint8_t checksum;
	uint8_t data[2];
	uint16_t result;
	uint8_t n = 0;
	uint8_t d;
	
	if(command == TRIGGER_RH_MEASUREMENT_HM || command == TRIGGER_RH_MEASUREMENT_NHM) d = 30;
	if(command == TRIGGER_T_MEASUREMENT_HM || command == TRIGGER_T_MEASUREMENT_NHM) d = 85;
	
	Wire.beginTransmission(m_addr);
	Wire.write(command);
	Wire.endTransmission();
	delay(d);
	Wire.requestFrom(m_addr,3);
	
	while(Wire.available() < 3) {
		delay(10);
		n++;
		if(n>10) return 0;
	}

	data[0] = Wire.read(); 	// read data (MSB)
	data[1] = Wire.read(); 	// read data (LSB)
	checksum = Wire.read();	// read checksum

	result = (data[0] << 8);
	result += data[1];

	if(CRC_Checksum (data,2,checksum)) {
		reset();
		return 1;
	}

	else return result;
}

float SHT21::CalcRH(uint16_t rh) {

	rh &= ~0x0003;	// clean last two bits

  	return (-6.0 + 125.0/65536 * (float)rh); // return relative humidity
}

float SHT21::CalcT(uint16_t t) {

	t &= ~0x0003;	// clean last two bits
 	
	return (-46.85 + 175.72/65536 * (float)t);
}

uint8_t SHT21::CRC_Checksum(uint8_t data[], uint8_t no_of_bytes, uint8_t checksum) {
	uint8_t crc = 0;	
  	uint8_t byteCtr;

 	 //calculates 8-Bit checksum with given polynomial
  	for (byteCtr = 0; byteCtr < no_of_bytes; ++byteCtr)
 	 { crc ^= (data[byteCtr]);
 	   for (uint8_t bit = 8; bit > 0; --bit)
 	   { if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
 	     else crc = (crc << 1);
 	   }
 	 }
 	 if (crc != checksum) return 1;
 	 else return 0;
}
