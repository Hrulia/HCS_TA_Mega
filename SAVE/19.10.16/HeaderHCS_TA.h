#pragma once

#include ".\libraries\OneWire\OneWire.h"
#include "myCycle.h"

#define DEBUG 0


#if DEBUG==1 
#define LOG(message) (Serial.print((message))) //В режиме отладки выводим логи
#define LOG2(message,view) (Serial.print((message),(view)))
#elif  DEBUG==0
#define LOG(message)
#define LOG2(message,view)
#endif

//объявление глобальных переменных
//моторизированные краны
//static unsigned long timeSwitchValveTTK = 0; //мс, максимальное полное время переключения моторизованного крана ТТК из одного положения в другое
static unsigned long timeSwitchValveSYS = 0; //мс, максимальное полное время переключения моторизованного крана Системы из одного положения в другое
											 //насосы
static bool  pumpTTK = false; //переменная состояния насоса ТТК
static bool  pumpSYS = false; //переменная состояния насоса системы
							  //static bool  pumpЭК = false; //переменная состояния насоса электрического котла ()
							  //состояние системы
static bool failure = false; //сигнал наличия состояния аварийного перегрева котла ТТК





//моторизированные краны
#define MAX_TIME_TESTING_VALVE 5000 //10000мс, максимальное время ожидания операции полного переключения мотокрана. Новый кран отрабатывал за 3200мс. С поднятой ручной крутилкой за 4300мс.

#define PIN_VALVE_TTK_OPEN 9 //пин управления открытием крана защиты обратки ТТК котла
#define PIN_VALVE_TTK_ClOSE 10 //пин управления закрытием крана защиты обратки ТТК котла
#define PIN_VALVE_TTK_SIGNAL_OPEN 11 //пин поступления сигнала полного открытия крана защиты обратки ТТК котла
#define PIN_VALVE_TTK_SIGNAL_ClOSE 12 //пин поступления сигнала полного закрытия крана защиты обратки ТТК котла

#define PIN_VALVE_SYS_OPEN 5 //пин управления открытием крана ругулировки системы
#define PIN_VALVE_SYS_ClOSE 6 //пин управления закрытием крана регулировки системы
#define PIN_VALVE_SYS_SIGNAL_OPEN 7 //пин поступления сигнала полного открытия крана регулировки системы
#define PIN_VALVE_SYS_SIGNAL_ClOSE 8 //пин поступления сигнала полного закрытия крана регулировки системы


//Датчики температуры DS18B20
#define PIN_DS18B20 2 //пин подключения датчииков температуры DS18B20 

//реле насосов
#define PIN_PUMP_TTK 3 //пин управления включением насоса ТТК
#define PIN_PUMP_SYS 4 //пин управления включением насоса системы
//#define PIN_PUMP_ЭК 3 //пин управления включением насоса электрокотла


//OneWire ds18b20(PIN_DS18B20);

//#define PIN_DS18B20_BOILERS 2 //Пин подключения датчиков температуры котлов
//#define PIN_DS18B20_HEATING_SYSTEM 3 //Пин подключения датчиков системы отопления, или помещений дома, пока не знаю????
//
//#define PIN_CONTROL_TTK 12 //пин управления включением ТТК котла
//#define PIN_CONTROL_EK 11 //пин управления включением электрического котла
//#define PIN_CONTROL__AUTO_OR_MANUAL_ON 10 //пин управления индикатором режима работы (ручной/автоматический)
//#define PIN_ERROR_INDICATOR 13 //пин упарвления индикатором ошибки. 0
//
//#define PIN_BUTTON_AUTO_OR_MANUAL_ON A2  //пин вход выбора режима работы авто/ручной
//#define PIN_BUTTON_TTK_ON A0 //пин вход принудительного включения насоса ТТК 
//#define PIN_BUTTON_EK_ON A1  //пин вход принудительного включения насоса ЭК
//
//#define PIN_TM1638_STB 4
//#define PIN_TM1638_CLK 6
//#define PIN_TM1638_DIO 5
//
//#define PIN_TM1637_Clk 8
//#define PIN_TM1637_DIO 7
//
////#define PIN_RTC_DS1307 //Стандартные выводы I2C интерфейса для NANO: A4-SDA, A5-SCL
//
//
//#define NUMBER_OF_DS18B20 5//Number of sensor connected
//#define RESOLUSHION_DS18B20 0x5f //1F-9bit, 3F-10bit, 5F-11bit, 7F-12bit
//#define DELAY_READ_DS18B20 400 // 9bit-93.75ms; 10-187.5; 11-375; 12-750 
//
///*команды для ds18b20*/
////ROM commands
//#define SKIP_ROM 0xCC			//Пропуск ROM
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