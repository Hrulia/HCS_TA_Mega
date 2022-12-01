#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
////////

//// esp partner
//#define ESP_OFF 0
//#define ESP_ON  1
//byte esp = ESP_OFF;

extern myCycle cycleNetworkSharing;
extern myCycle cycle4s; //���� �������� ������� ������ ����������� �� ESP ����� Serial3


//������������� ����� UART ��� �������� ������
int initRxTxForNetSharing(unsigned long speedUart);

//�������� ����������� ������ ����� uart ���� �3
void checkSerial();