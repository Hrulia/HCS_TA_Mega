// MotorizedValve.h

#ifndef _MOTORIZEDVALVE_h
#define _MOTORIZEDVALVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "HeaderHCS_TA.h"

//Регулировка и поддержание оптимальной температуры ТТК (защита от холодной обратки)
int temperatureControlTTK();





#endif

