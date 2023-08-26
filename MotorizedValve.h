// MotorizedValve.h

#ifndef _MOTORIZEDVALVE_h
#define _MOTORIZEDVALVE_h
	#include "arduino.h"



	#include "HeaderHCS_TA.h"
	#include      ".\libraries\MAX6675_Thermocouple\src\MAX6675_Thermocouple.h" 

	//����� ���� ����������������� ����������������� ������ � ������������ ������������� ������� ������������
	int initValve();

	//������������ ���������
	int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long  * valveTimeSwitch);


	//����������� � ����������� ����������� ����������� ��� (������ �� �������� �������)
	int temperatureControlTTK();

	//����������� � ����������� ����������� ����������� �������
	int temperatureControlSYS();

	//��� ��������� ��� ���������������� ������
	int16_t PID_3WayValve(float set_value, float present_value, float cycle, float valve, float dead_zone, float k_P = 1, float k_I = 4, float k_D = 4);


#endif

