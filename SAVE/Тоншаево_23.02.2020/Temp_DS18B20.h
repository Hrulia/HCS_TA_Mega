// Temp_DS18B20.h
//����� �������� ����������� DS18B20


#ifndef _TEMP_DS18B20_h
#define _TEMP_DS18B20_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

////#include ".\libraries\OneWire\OneWire.h"
#include "HeaderHCS_TA.h"

//��������������� ������������� � ������������ ���� �������� �����������
int initTemp();

//����� ���� ������������ DS18B20
void findAllconnectDS18B20(); 

//��������� ����������� �� ���� DS18B20
//int getTemperaturDS18B20(OneWire&);//�������� ��������� �� ������
int getTemperaturDS18B20();


#endif