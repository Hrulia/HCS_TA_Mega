#pragma once

/* файлах ino глобальные еременные видны везде.
	в файлах x.cpp глобальные переменные не видны, если они не описаны в файлах x.h
	поэтому глобальные переменные объявляем через extern в файле "HeaderHCS_TA.h", а если
	это экземпляр какого-то класса, то библиотека с классом так же должна присоединяться в модуле "HeaderHCS_TA.h" 
	до объявления переменной. основное определение может быть в любом модуле.
	*/



#include <EEPROM.h>
#include <RTClib.h>
#include <OneWire.h>
#include ".\libraries\EEManager-main\src\EEManager.h"

#include "myCycle.h"
#include "Pins.h"   //тут собраны все пины.
#include "Button.h" //Button oSignalClose(10, 15); // класс button позволяет фильтровать дребезг контактов с сигнальных концевиков крана. время фильтрации 15мс*на период повторения. 
//#include "ValvesAndPumpsController.h"
//#include "Indicator_TM1637.h"


#define DEBUG 0


#if DEBUG==1
#define LOG(message) (Serial.print((message))) //В режиме отладки выводим логи
#define LOG2(message,view) (Serial.print((message),(view)))
#elif  DEBUG==0
#define LOG(message)
#define LOG2(message,view)
#endif

// КОНФИГУРАЦИЯ СИСТЕМЫ
#define TTK_VALVE_EXISTS 0			// Наличие в системе крана ТТК
#define MAIN_UART_SPEED 115200	// Скорость основного серийного порта
#define TO_ESP_UART_SPEED 115200	// Скорость серийного порта (по умолчанию Serial3) для обмена с ESP8266

//моторизированные краны
#define TEST_VALVE_ON false			// Выполнять тестирование кранов, или брать врмя открытия по максимальному таймауту (в случае false)
#define MAX_TIME_TESTING_VALVE 125000 //10000мс для первого шарового крана. Новый кран по паспорту 120с. , максимальное время ожидания операции полного переключения мотокрана. Новый кран отрабатывал за 3200мс. С поднятой ручной крутилкой за 4300мс.

//Number of sensor DS18b20 connected to system
#define NUMBER_OF_DS18B20 16

//Массив значений температур даттчиков
extern float temperature[NUMBER_OF_DS18B20];

	// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ СИСТЕМЫ:
struct GlobalParametersStructure {
	//время
	volatile DateTime g_systemDateTime;			//	текущее время/дата в системе 

	//моторизированные краны
	unsigned long g_timeSwitchValveTTK = 0; //мс, максимальное полное время переключения моторизованного крана ТТК из одного положения в другое
	unsigned long g_timeSwitchValveSYS = 0; //мс, максимальное полное время переключения моторизованного крана Системы из одного положения в другое

	//котел ТТК
	bool g_failure = false; //сигнал наличия состояния аварийного перегрева котла ТТК
	float g_t_flueGases = 0; //температура дымовых газов ТТК котла 

	/*Пока перебивает эту переменную локальный float tRoomSetpoin*/
	float RoomSetPointTempatureCurrent;	// Целевое значение температуры помещения с учетом суточного расписания (именно к этому значению стремимся при регулировке)

} /*globalParameters*/;

// ПАРАМЕТРЫ СИСТЕМЫ
enum sysParam { SP_ERR = -1, SP_EMPTY, SP_ON, SP_OFF, SP_AUTO, SP_OPEN, SP_CLOSE, SP_MIALG, SP_PID }; //Перечисления для параметров работы системы

struct MegaParametersStruct {
	//Метод поддержания температуры
	int SysTempControlMode = SP_MIALG;		//SP_MIALG (6) – мой алгоритм регулирования, SP_PID (7) - PID регулятор
	int TTKTempControlMode = SP_MIALG;

	// режим работы насосов
	int TTKPumpMode = SP_AUTO;						//1 - on, 2 - off, 3 - auto
	int SystemPumpMode = SP_AUTO;					//1 - on, 2 - off, 3 - auto

// Режим управления дверкой поддувала
	int DoorAirMode = SP_AUTO;						//4 - open, 5 - close, 3 - auto

// температурные уставки системы
	int tCrashTTK = 80;										// аварийная температура ТТК(80 °С)
	int tDangerTTK = 70;									// предаварийная температура  ТТК (70 °С)
	int tNeedTTK = 40;										// требуемая температура обратки ТТК(60 °С)
	float TminSysPodacha;									// минимальная температура подачи системы (изменяется в зависимости от температуры наружного воздуха. Чем холоднее, тем холоднее воздух стекает с окон) 

	bool onSchedule = true;								// флаг, поддержаниt температуры системы по суточному графику
	float RoomSetPointTemperature=24.0;		// Целевое значение температуры помещения, задаем системе.
	byte indicator_Intensity = 4;//интенсивность свечения

	//PID регулятор
	float pid_set_value = 41.0; //Целевая температура для ПИд регулятора
	float pid_cycleS = 60; //Цикл регулятора
	float pid_kP = 1.0;
	float pid_kI = 4.0;
	float pid_kD = 4.0;
	
} /*systemParameters*/; //в данном объекте храним параметры системы
