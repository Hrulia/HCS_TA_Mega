#pragma once

#include ".\libraries\OneWire\OneWire.h"
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

//���������� ���������� ���������� ��������� �������
//���������������� �����
	/*static*/extern unsigned long g_timeSwitchValveTTK; //��, ������������ ������ ����� ������������ ��������������� ����� ��� �� ������ ��������� � ������
	/*static*/extern unsigned long g_timeSwitchValveSYS; //��, ������������ ������ ����� ������������ ��������������� ����� ������� �� ������ ��������� � ������
//������
	/*static*/ extern bool  g_pumpTTK; //���������� ��������� ������ ���
	/*static*/ extern bool  g_pumpSYS; //���������� ��������� ������ �������
	///*static*/ extern bool  g_pump�� = false; //���������� ��������� ������ �������������� ����� ()
//��������� ��������� ��� �����					
	/*static*/ extern bool g_failure; //������ ������� ��������� ���������� ��������� ����� ���




//
//#define NUMBER_OF_DS18B20 5//Number of sensor connected
//#define RESOLUSHION_DS18B20 0x5f //1F-9bit, 3F-10bit, 5F-11bit, 7F-12bit
//#define DELAY_READ_DS18B20 400 // 9bit-93.75ms; 10-187.5; 11-375; 12-750 
//
///*������� ��� ds18b20*/
////ROM commands
//#define SKIP_ROM 0xCC			//������� ROM
////Functional commands
//#define CONVERT_TEMPERATURE 0x44			//convert temperature
//#define WRITE_SCRATCHPAD 0x4E	//Write Scratchpad
//#define COPY_SCRATCHPAD 0x48	//Copy Scratchpad
//#define READ_SCRATCHPAD 0xBE	//Read Scratchpad [BEh])
////
/////
////#define EB 1
////#define TTK 2
////