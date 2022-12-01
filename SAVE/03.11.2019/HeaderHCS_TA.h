#pragma once

#include ".\libraries\OneWire\OneWire.h"
//#include "Wire.h" //работа с интерфейсом I2C(TWI)
#include "myCycle.h"
#include "Pins.h"   //тут собраны все пины.
#include "Button.h" //Button oSignalClose(10, 15); // класс button позволяет фильтровать дребезг контактов с сигнальных концевиков крана. время фильтрации 15мс*на период повторения. 
#include ".\libraries\RTClib\RTClib.h"



#define DEBUG 0


#if DEBUG==1 
#define LOG(message) (Serial.print((message))) //В режиме отладки выводим логи
#define LOG2(message,view) (Serial.print((message),(view)))
#elif  DEBUG==0
#define LOG(message)
#define LOG2(message,view)
#endif

//моторизированные краны
#define MAX_TIME_TESTING_VALVE 10000 //10000мс, максимальное время ожидания операции полного переключения мотокрана. Новый кран отрабатывал за 3200мс. С поднятой ручной крутилкой за 4300мс.


//объявление глобальных переменных состояния системы

//текущее время/дата в системе 
	extern volatile DateTime g_systemDateTime;

//моторизированные краны
	/*static*/extern unsigned long g_timeSwitchValveTTK; //мс, максимальное полное время переключения моторизованного крана ТТК из одного положения в другое
	/*static*/extern unsigned long g_timeSwitchValveSYS; //мс, максимальное полное время переключения моторизованного крана Системы из одного положения в другое
//насосы
	/*static*/ extern bool  g_pumpTTK; //переменная состояния насоса ТТК
	/*static*/ extern bool  g_pumpSYS; //переменная состояния насоса системы
	///*static*/ extern bool  g_pumpЭК = false; //переменная состояния насоса электрического котла ()
//состояние перегрева ТТК котла					
	/*static*/ extern bool g_failure; //сигнал наличия состояния аварийного перегрева котла ТТК

//заданное значение температуры помещения (к которому стремимся при регулировке)
	extern uint8_t g_tRoomSetpoint; 