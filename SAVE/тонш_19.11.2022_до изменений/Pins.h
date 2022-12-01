// Pins.h

#ifndef _PINS_h
#define _PINS_h

//пин - вход прерывания от модуля RTC
#define PIN_RTC_INTERRUPT 3 //MEGA-2,3,21,20,19,18; 328-2, 3 

//моторизированные краны
////#define MAX_TIME_TESTING_VALVE 5000 //10000мс, максимальное время ожидания операции полного переключения мотокрана. Новый кран отрабатывал за 3200мс. С поднятой ручной крутилкой за 4300мс.

#define PIN_VALVE_TTK_OPEN 33 //пин управления открытием крана защиты обратки ТТК котла
#define PIN_VALVE_TTK_ClOSE 34 //пин управления закрытием крана защиты обратки ТТК котла
#define PIN_VALVE_TTK_SIGNAL_OPEN 31 //пин поступления сигнала полного открытия крана защиты обратки ТТК котла
#define PIN_VALVE_TTK_SIGNAL_ClOSE 32 //пин поступления сигнала полного закрытия крана защиты обратки ТТК котла

#define PIN_VALVE_SYS_OPEN 35 //пин управления открытием крана ругулировки системы
#define PIN_VALVE_SYS_ClOSE 36 //пин управления закрытием крана регулировки системы
#define PIN_VALVE_SYS_SIGNAL_OPEN 37 //пин поступления сигнала полного открытия крана регулировки системы
#define PIN_VALVE_SYS_SIGNAL_ClOSE 38 //пин поступления сигнала полного закрытия крана регулировки системы


#define PIN_EMERGENCY_SIREN 28 //пин выхода аварийного перегрева ТТК котла

//Датчики температуры DS18B20
#define PIN_DS18B20 2 //пин подключения датчииков температуры DS18B20 

//Пины подключения термопары
#define PIN_THERMOCOUPLE_CLK 7 //вывод CLK ( SCK ) модуля на чипе MAX6675
#define PIN_THERMOCOUPLE_CS 6 //вывод CS  ( SS ) модуля на чипе MAX6675
#define PIN_THERMOCOUPLE_DO 5 //вывод DO  ( SO, MISO ) модуля на чипе MAX6675

//реле насосов
#define PIN_PUMP_TTK 29 //пин управления включением насоса ТТК
#define PIN_PUMP_SYS 30 //пин управления включением насоса системы
#define PIN_PUMP_EK 50//пин управления включением насоса электрокотла
#define PIN_CONTROL_EK 53 //пин управления включением электрокотла

//Пин подачи сигнала сброса ESP
#define PIN_RESET_ESP 4


//созжены на магонии
//39
//43
//44
//45
//50 - странно ведет себя. как будто не управляется и всегда LOW/

//Индикаторы на ТМ1637
#define PIN_TM1637_Clk_COMMON 58 //(A4)
#define PIN_TM1637_DIO1 69 //(A15)
#define PIN_TM1637_DIO2 68 //(A14)  
#define PIN_TM1637_DIO3 67 //(A13)
#define PIN_TM1637_DIO4 66 //(A12)
#define PIN_TM1637_DIO5 65 //(A11)
#define PIN_TM1637_DIO6 64 //(A10)
#define PIN_TM1637_DIO7 63 //(A9)
#define PIN_TM1637_DIO8 62 //(A8)
#define PIN_TM1637_DIO9 61  //(A7)
#define PIN_TM1637_DIO10 60 //(A6)
#define PIN_TM1637_DIO11 59 //(A5)
#define PIN_TM1637_DIO_Time 57 //(A3)

//#define PIN_CONTROL__AUTO_OR_MANUAL_ON 10 //пин управления индикатором режима работы (ручной/автоматический)
//#define PIN_ERROR_INDICATOR 13 //пин упарвления индикатором ошибки. 0
//
//#define PIN_BUTTON_AUTO_OR_MANUAL_ON A2  //пин вход выбора режима работы авто/ручной
//#define PIN_BUTTON_TTK_ON A0 //пин вход принудительного включения насоса ТТК 
//#define PIN_BUTTON_EK_ON A1  //пин вход принудительного включения насоса ЭК


////#define PIN_RTC_DS1307 //Стандартные выводы I2C интерфейса для NANO: A4-SDA, A5-SCL
//

#endif