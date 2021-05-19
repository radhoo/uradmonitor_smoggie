/**
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 *	Project:	uRADMonitor model A/A2/A3/INDUSTRIAL, part of Global environmental monitoring network
 *	Web:		www.uradmonitor.com
 *	License:	Proprietary, all rights reserved.
 *
 *	This file contains proprietary information which may be legally privileged. Any unauthorized use or dissemination is prohibited.
 *	It is for the intended recipient only. If you are not the intended recipient you must not use, disclose, distribute, copy or print this file.
 *
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2020 Magnasci SRL, www.magnasci.com
**/

#pragma once
#include <stdint.h>
#include "aux.h"



#define        CAL_VALUES          15

extern char calNoun[CAL_VALUES][10];

// linear ax+b compensations with small real numbers (-327 .. 327) with two decimals
#define				CAL_TEMPERATURE				0x00			// temperature calibration index -02
#define				CAL_PRESSURE				  0x01			// pressure calibration index -03
#define				CAL_HUMIDITY				  0x02			// humidity calibration index -04
#define				CAL_VOC						    0x03			// voc calibration index -06
#define				CAL_CO2						    0x04			// -07
#define				CAL_CH2O					    0x05			// -08
#define				CAL_PM25					    0x06			// -09 pm calibration index
#define				CAL_NOISE					    0x07			// -11 noise
#define				CAL_PM1						    0x08			// -09 pm calibration index
#define				CAL_PM10					    0x09			// -09 pm calibration index
#define				CAL_O3						    0x0A			//
#define				CAL_ZE03S1					  0x0B			// by def this is ZE03-O3
#define				CAL_ZE03S2					  0x0C			// by def this is ZE03-NO2
#define				CAL_ZE03S3					  0x0D			// by def this is ZE03-SO2
#define				CAL_ZE03S4					  0x0E			// by def this is ZE03-CO
// default

#define				CAL_DEF_NOCHANGE			0x00640000		// 1*x + 0

// --- values:
#ifndef CAL_DEF_TEMPERATURE
  #define				CAL_DEF_TEMPERATURE		0x006483E8		// -10
#endif
#ifndef CAL_DEF_PRESSURE  
  #define				CAL_DEF_PRESSURE			CAL_DEF_NOCHANGE		
#endif
#ifndef CAL_DEF_HUMIDITY  
  #define				CAL_DEF_HUMIDITY			0x00640960		// +24
#endif
#ifndef CAL_DEF_VOC  
  #define				CAL_DEF_VOC					  CAL_DEF_NOCHANGE		
#endif
#ifndef CAL_DEF_CO2
  #define				CAL_DEF_CO2					  CAL_DEF_NOCHANGE		
#endif  
#ifndef CAL_DEF_CH2O
  #define				CAL_DEF_CH2O				  CAL_DEF_NOCHANGE		
#endif  
#ifndef CAL_DEF_PM25
  #define				CAL_DEF_PM25				  CAL_DEF_NOCHANGE		
#endif
#ifndef  CAL_DEF_NOISE
  #define				CAL_DEF_NOISE				  CAL_DEF_NOCHANGE		
#endif
#ifndef CAL_DEF_PM1  
  #define				CAL_DEF_PM1					  CAL_DEF_NOCHANGE		
#endif
#ifndef CAL_DEF_PM10  
  #define				CAL_DEF_PM10				  CAL_DEF_NOCHANGE		
#endif  
#ifndef CAL_DEF_O3
  #define				CAL_DEF_O3					  CAL_DEF_NOCHANGE		
#endif
#ifndef   CAL_DEF_ZE03S1
  #define				CAL_DEF_ZE03S1				CAL_DEF_NOCHANGE		
  #define				CAL_DEF_ZE03S2				CAL_DEF_NOCHANGE		
  #define				CAL_DEF_ZE03S3				CAL_DEF_NOCHANGE		
  #define				CAL_DEF_ZE03S4				CAL_DEF_NOCHANGE		
#endif

class Calibration {
	uint32_t	cal[CAL_VALUES];

public:

	Calibration() {}

	uint32_t 	getDefault(uint8_t i);
	void 		calibrate(double *value, uint8_t parameterIndex, bool forcedPositive);
	void 		calibrate(uint32_t *value, uint8_t parameterIndex, bool forcedPositive);
  double calibrate(double value, uint8_t parameterIndex, bool forcedPositive) ;
  bool 		setCalibrationParam(uint8_t parameterIndex, uint32_t calibrationParam);

};
