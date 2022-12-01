// Pins.h

#ifndef _PINS_h
#define _PINS_h


//���������������� �����
#define MAX_TIME_TESTING_VALVE 5000 //10000��, ������������ ����� �������� �������� ������� ������������ ���������. ����� ���� ����������� �� 3200��. � �������� ������ ��������� �� 4300��.

#define PIN_VALVE_TTK_OPEN 33 //��� ���������� ��������� ����� ������ ������� ��� �����
#define PIN_VALVE_TTK_ClOSE 34 //��� ���������� ��������� ����� ������ ������� ��� �����
#define PIN_VALVE_TTK_SIGNAL_OPEN 31 //��� ����������� ������� ������� �������� ����� ������ ������� ��� �����
#define PIN_VALVE_TTK_SIGNAL_ClOSE 32 //��� ����������� ������� ������� �������� ����� ������ ������� ��� �����

#define PIN_VALVE_SYS_OPEN 35 //��� ���������� ��������� ����� ����������� �������
#define PIN_VALVE_SYS_ClOSE 36 //��� ���������� ��������� ����� ����������� �������
#define PIN_VALVE_SYS_SIGNAL_OPEN 37 //��� ����������� ������� ������� �������� ����� ����������� �������
#define PIN_VALVE_SYS_SIGNAL_ClOSE 38 //��� ����������� ������� ������� �������� ����� ����������� �������


//������� ����������� DS18B20
#define PIN_DS18B20 2 //��� ����������� ��������� ����������� DS18B20 

//���� �������
#define PIN_PUMP_TTK 29 //��� ���������� ���������� ������ ���
#define PIN_PUMP_SYS 30 //��� ���������� ���������� ������ �������
//#define PIN_PUMP_�� 3 //��� ���������� ���������� ������ ������������



//���������� �� ��1637
#define PIN_TM1637_Clk_COMMON 39
#define PIN_TM1637_DIO1 45
#define PIN_TM1637_DIO2 44
#define PIN_TM1637_DIO3 43
#define PIN_TM1637_DIO4 42
#define PIN_TM1637_DIO5 41
#define PIN_TM1637_DIO6 40
#define PIN_TM1637_DIO_Time 46


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













#endif

