// MotorizedValve.h

#ifndef _MOTORIZEDVALVE_h
#define _MOTORIZEDVALVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "HeaderHCS_TA.h"

//����� ���� ����������������� ����������������� ������ � ������������ ������������� ������� ������������
int initValve();

//������������ ���������
int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long  * valveTimeSwitch);


//����������� � ����������� ����������� ����������� ��� (������ �� �������� �������)
int temperatureControlTTK();

//����������� � ����������� ����������� ����������� �������
int temperatureControlSYS();




#endif

