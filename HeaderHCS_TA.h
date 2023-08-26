#pragma once

/* ������ ino ���������� ��������� ����� �����.
	� ������ x.cpp ���������� ���������� �� �����, ���� ��� �� ������� � ������ x.h
	������� ���������� ���������� ��������� ����� extern � ����� "HeaderHCS_TA.h", � ����
	��� ��������� ������-�� ������, �� ���������� � ������� ��� �� ������ �������������� � ������ "HeaderHCS_TA.h" 
	�� ���������� ����������. �������� ����������� ����� ���� � ����� ������.
	*/



#include <EEPROM.h>
#include <RTClib.h>
#include <OneWire.h>

#include "myCycle.h"
#include "Pins.h"   //��� ������� ��� ����.
#include "Button.h" //Button oSignalClose(10, 15); // ����� button ��������� ����������� ������� ��������� � ���������� ���������� �����. ����� ���������� 15��*�� ������ ����������. 



#define DEBUG 0


#if DEBUG==1
#define LOG(message) (Serial.print((message))) //� ������ ������� ������� ����
#define LOG2(message,view) (Serial.print((message),(view)))
#elif  DEBUG==0
#define LOG(message)
#define LOG2(message,view)
#endif

//���������������� �����
#define MAX_TIME_TESTING_VALVE 10000 //10000��, ������������ ����� �������� �������� ������� ������������ ���������. ����� ���� ����������� �� 3200��. � �������� ������ ��������� �� 4300��.

//Number of sensor DS18b20 connected to system
#define NUMBER_OF_DS18B20 16

//******         ���������� ���������� ���������� ��������� �������       *********************************
//
//������� �����/���� � ������� 
	extern  volatile DateTime g_systemDateTime;

//���������������� �����
	/*static*/extern unsigned long g_timeSwitchValveTTK; //��, ������������ ������ ����� ������������ ��������������� ����� ��� �� ������ ��������� � ������
	/*static*/extern unsigned long g_timeSwitchValveSYS; //��, ������������ ������ ����� ������������ ��������������� ����� ������� �� ������ ��������� � ������
//������
	///*static*/ extern bool  g_pumpTTK; //���������� ��������� ������ ���
	///*static*/ extern bool  g_pumpSYS; //���������� ��������� ������ �������
	///*static*/ extern bool  g_pump�� = false; //���������� ��������� ������ �������������� ����� ()
//��������� ��������� ��� �����					
	/*static*/ extern bool g_failure; //������ ������� ��������� ���������� ��������� ����� ���

//�������� �������� ����������� ��������� (� �������� ��������� ��� �����������)
	extern /*uint8_t*/float g_tRoomSetpoint; 