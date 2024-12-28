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

#include "myCycle.h"
#include "Pins.h"   //тут собраны все пины.
#include "Button.h" //Button oSignalClose(10, 15); // класс button позволяет фильтровать дребезг контактов с сигнальных концевиков крана. время фильтрации 15мс*на период повторения. 



#define DEBUG 1


#if DEBUG==1
#define LOG(message) (Serial.print((message))) //В режиме отладки выводим логи
#define LOG2(message,view) (Serial.print((message),(view)))
#elif  DEBUG==0
#define LOG(message)
#define LOG2(message,view)
#endif

//моторизированные краны
#define MAX_TIME_TESTING_VALVE 10000 //10000мс, максимальное время ожидания операции полного переключения мотокрана. Новый кран отрабатывал за 3200мс. С поднятой ручной крутилкой за 4300мс.

//Number of sensor DS18b20 connected to system
#define NUMBER_OF_DS18B20 16

//******         объявление глобальных переменных состояния системы       *********************************
//
//текущее время/дата в системе 
	extern  volatile DateTime g_systemDateTime;

//моторизированные краны
	/*static*/extern unsigned long g_timeSwitchValveTTK; //мс, максимальное полное время переключения моторизованного крана ТТК из одного положения в другое
	/*static*/extern unsigned long g_timeSwitchValveSYS; //мс, максимальное полное время переключения моторизованного крана Системы из одного положения в другое
//насосы
	///*static*/ extern bool  g_pumpTTK; //переменная состояния насоса ТТК
	///*static*/ extern bool  g_pumpSYS; //переменная состояния насоса системы
	///*static*/ extern bool  g_pumpЭК = false; //переменная состояния насоса электрического котла ()
//состояние перегрева ТТК котла					
	/*static*/ extern bool g_failure; //сигнал наличия состояния аварийного перегрева котла ТТК

//заданное значение температуры помещения (к которому стремимся при регулировке)
	extern /*uint8_t*/float g_tRoomSetpoint; 