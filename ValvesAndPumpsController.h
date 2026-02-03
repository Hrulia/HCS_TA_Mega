
#ifndef _VALVESAND_PUMPS_CONTROLLER_h
#define _VALVESAND_PUMPS_CONTROLLER_h

	///#include "arduino.h"
	#include "HeaderHCS_TA.h"
	#include "./libraries/MAX6675_Thermocouple/src/MAX6675_Thermocouple.h"	//	https://github.com/YuriiSalimov/MAX6675_Thermocouple
	#include "./libraries/Arduino_TimerList-master/TimerList.h"	//  https://github.com/DetSimen/Arduino_TimerList

//включение отладки в модуле /* Оформление отладки как у Алекса Гайвера*/
#define DEBUG_ENABLE_VPC
#ifdef DEBUG_ENABLE_VPC
	#define DEBUG_PRINT_VPC(x) (Serial.print(x))
	#define DEBUG_PRINTLN_VPC(x) (Serial.println(x))
	#define DEBUG_PRINTR_VPC(x,r) (Serial.print(x,r))
#else
	#define DEBUG_PRINT_VPC(x) 
	#define DEBUG_PRINTLN_VPC(x) 
	#define DEBUG_PRINTR_VPC(x,r) 
#endif // DEBUG_ENABLE_VPC

	extern GlobalParametersStructure globalParameters;
	extern MegaParametersStruct systemParameters;


	//Инициализация портов
void initPortsValvesAndPumps();
	

	//Общий тест работоспособности моторизированныхх кранов с определением максимального времени переключения
	int testValves();

	//тестирование мотокрана
	int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long  * valveTimeSwitch);


	//Регулировка и поддержание оптимальной температуры ТТК (защита от холодной обратки)
	int temperatureControlTTK();

	//Регулировка и поддержание оптимальной температуры системы
	int temperatureControlSYS();

	//ПИД регулятор для моторизированных кранов
	int16_t PID_3WayValve(float set_value, float present_value, float cycle, float valve, float dead_zone, float k_P = 1, float k_I = 4, float k_D = 4);

	// Элементарные функции управления насосами и кранами
	enum enumAction { AC_ERROR = -1, AC_EMPTY, AC_ON, AC_OFF, AC_AUTO, AC_OPEN, AC_CLOSE};
	void pumpAction(int controlPin, enumAction action=AC_OFF);
	void EKAction(int controlPin, enumAction action = AC_OFF);
	// Функция задания действия с краном
	void valveAction(int openPin, int closePin, int signalOpen, int signalClose, enumAction action= AC_EMPTY, long duration=0);
	// Функция обратного вызова, вызываемая по прерыванию внутреннего таймера и выполняющая сброс заданного для крана действия (например, когда окончено время импульса открывания крана)
	void tmrStopActionForSysValve(void);


#endif

