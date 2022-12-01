#pragma once

#include ".\libraries\OneWire\OneWire.h"
//#include "Wire.h" //������ � ����������� I2C(TWI)
#include "myCycle.h"
#include "Pins.h"   //��� ������� ��� ����.
#include "Button.h" //Button oSignalClose(10, 15); // ����� button ��������� ����������� ������� ��������� � ���������� ���������� �����. ����� ���������� 15��*�� ������ ����������. 
#include ".\libraries\RTClib\RTClib.h"



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


//���������� ���������� ���������� ��������� �������

//������� �����/���� � ������� 
	extern volatile DateTime g_systemDateTime;

//���������������� �����
	/*static*/extern unsigned long g_timeSwitchValveTTK; //��, ������������ ������ ����� ������������ ��������������� ����� ��� �� ������ ��������� � ������
	/*static*/extern unsigned long g_timeSwitchValveSYS; //��, ������������ ������ ����� ������������ ��������������� ����� ������� �� ������ ��������� � ������
//������
	/*static*/ extern bool  g_pumpTTK; //���������� ��������� ������ ���
	/*static*/ extern bool  g_pumpSYS; //���������� ��������� ������ �������
	///*static*/ extern bool  g_pump�� = false; //���������� ��������� ������ �������������� ����� ()
//��������� ��������� ��� �����					
	/*static*/ extern bool g_failure; //������ ������� ��������� ���������� ��������� ����� ���

//�������� �������� ����������� ��������� (� �������� ��������� ��� �����������)
	extern uint8_t g_tRoomSetpoint; 