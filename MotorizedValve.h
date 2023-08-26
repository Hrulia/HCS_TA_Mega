// MotorizedValve.h

#ifndef _MOTORIZEDVALVE_h
#define _MOTORIZEDVALVE_h
	#include "arduino.h"



	#include "HeaderHCS_TA.h"
	#include      ".\libraries\MAX6675_Thermocouple\src\MAX6675_Thermocouple.h" 

	//Общий тест работоспособности моторизированныхх кранов с определением максимального времени переключения
	int initValve();

	//тестирование мотокрана
	int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long  * valveTimeSwitch);


	//Регулировка и поддержание оптимальной температуры ТТК (защита от холодной обратки)
	int temperatureControlTTK();

	//Регулировка и поддержание оптимальной температуры системы
	int temperatureControlSYS();

	//ПИД регулятор для моторизированных кранов
	int16_t PID_3WayValve(float set_value, float present_value, float cycle, float valve, float dead_zone, float k_P = 1, float k_I = 4, float k_D = 4);


#endif

