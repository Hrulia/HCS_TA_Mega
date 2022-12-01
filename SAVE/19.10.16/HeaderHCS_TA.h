#pragma once

#include ".\libraries\OneWire\OneWire.h"
#include "myCycle.h"

#define DEBUG 0


#if DEBUG==1 
#define LOG(message) (Serial.print((message))) //� ������ ������� ������� ����
#define LOG2(message,view) (Serial.print((message),(view)))
#elif  DEBUG==0
#define LOG(message)
#define LOG2(message,view)
#endif

//���������� ���������� ����������
//���������������� �����
//static unsigned long timeSwitchValveTTK = 0; //��, ������������ ������ ����� ������������ ��������������� ����� ��� �� ������ ��������� � ������
static unsigned long timeSwitchValveSYS = 0; //��, ������������ ������ ����� ������������ ��������������� ����� ������� �� ������ ��������� � ������
											 //������
static bool  pumpTTK = false; //���������� ��������� ������ ���
static bool  pumpSYS = false; //���������� ��������� ������ �������
							  //static bool  pump�� = false; //���������� ��������� ������ �������������� ����� ()
							  //��������� �������
static bool failure = false; //������ ������� ��������� ���������� ��������� ����� ���





//���������������� �����
#define MAX_TIME_TESTING_VALVE 5000 //10000��, ������������ ����� �������� �������� ������� ������������ ���������. ����� ���� ����������� �� 3200��. � �������� ������ ��������� �� 4300��.

#define PIN_VALVE_TTK_OPEN 9 //��� ���������� ��������� ����� ������ ������� ��� �����
#define PIN_VALVE_TTK_ClOSE 10 //��� ���������� ��������� ����� ������ ������� ��� �����
#define PIN_VALVE_TTK_SIGNAL_OPEN 11 //��� ����������� ������� ������� �������� ����� ������ ������� ��� �����
#define PIN_VALVE_TTK_SIGNAL_ClOSE 12 //��� ����������� ������� ������� �������� ����� ������ ������� ��� �����

#define PIN_VALVE_SYS_OPEN 5 //��� ���������� ��������� ����� ����������� �������
#define PIN_VALVE_SYS_ClOSE 6 //��� ���������� ��������� ����� ����������� �������
#define PIN_VALVE_SYS_SIGNAL_OPEN 7 //��� ����������� ������� ������� �������� ����� ����������� �������
#define PIN_VALVE_SYS_SIGNAL_ClOSE 8 //��� ����������� ������� ������� �������� ����� ����������� �������


//������� ����������� DS18B20
#define PIN_DS18B20 2 //��� ����������� ��������� ����������� DS18B20 

//���� �������
#define PIN_PUMP_TTK 3 //��� ���������� ���������� ������ ���
#define PIN_PUMP_SYS 4 //��� ���������� ���������� ������ �������
//#define PIN_PUMP_�� 3 //��� ���������� ���������� ������ ������������


//OneWire ds18b20(PIN_DS18B20);

//#define PIN_DS18B20_BOILERS 2 //��� ����������� �������� ����������� ������
//#define PIN_DS18B20_HEATING_SYSTEM 3 //��� ����������� �������� ������� ���������, ��� ��������� ����, ���� �� ����????
//
//#define PIN_CONTROL_TTK 12 //��� ���������� ���������� ��� �����
//#define PIN_CONTROL_EK 11 //��� ���������� ���������� �������������� �����
//#define PIN_CONTROL__AUTO_OR_MANUAL_ON 10 //��� ���������� ����������� ������ ������ (������/��������������)
//#define PIN_ERROR_INDICATOR 13 //��� ���������� ����������� ������. 0
//
//#define PIN_BUTTON_AUTO_OR_MANUAL_ON A2  //��� ���� ������ ������ ������ ����/������
//#define PIN_BUTTON_TTK_ON A0 //��� ���� ��������������� ��������� ������ ��� 
//#define PIN_BUTTON_EK_ON A1  //��� ���� ��������������� ��������� ������ ��
//
//#define PIN_TM1638_STB 4
//#define PIN_TM1638_CLK 6
//#define PIN_TM1638_DIO 5
//
//#define PIN_TM1637_Clk 8
//#define PIN_TM1637_DIO 7
//
////#define PIN_RTC_DS1307 //����������� ������ I2C ���������� ��� NANO: A4-SDA, A5-SCL
//
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