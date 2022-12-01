// MotorizedValve.h

#ifndef _MOTORIZEDVALVE_h
#define _MOTORIZEDVALVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "HeaderHCS_TA.h"

//Общий тест работоспособности моторизированныхх кранов с определением максимального времени переключения
int initValve();

//тестирование мотокрана
int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long  * valveTimeSwitch);


//Регулировка и поддержание оптимальной температуры ТТК (защита от холодной обратки)
int temperatureControlTTK();

//Регулировка и поддержание оптимальной температуры системы
int temperatureControlSYS();




#endif

