// Indicator_TM1637.h
#pragma once

#ifndef _INDICATOR_TM1637_h
#define _INDICATOR_TM1637_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "HeaderHCS_TA.h"
#include "TM1637Display.h"


//������������� �����������
int init_TM1637();


//����� ����������� �� ���������� TM1637
int indicator_TM1637_Output_temperature();

#endif

