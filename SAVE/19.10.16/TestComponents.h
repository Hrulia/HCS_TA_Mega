// TestComponents.h
//������� ������� ������������ ����������� �������



#ifndef _TESTCOMPONENTS_h
#define _TESTCOMPONENTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//����� ���� ����������������� ����������� �������
int testSystem();

//������������ ���������
int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long  * valveTimeSwitch);

//������������ �������� �����������
int testDS18B20(uint8_t pin);



#endif


