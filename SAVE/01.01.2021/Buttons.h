// Buttons.h

#ifndef _BUTTONS_h
#define _BUTTONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "HeaderHCS_TA.h"
////#include "Indicator_TM1637.h"

//Вызоав процедуры опроса клавиатуры
void scanButtons();





#endif

