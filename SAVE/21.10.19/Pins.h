// Pins.h

#ifndef _PINS_h
#define _PINS_h


//моторизированные краны
#define MAX_TIME_TESTING_VALVE 5000 //10000мс, максимальное время ожидания операции полного переключения мотокрана. Новый кран отрабатывал за 3200мс. С поднятой ручной крутилкой за 4300мс.

#define PIN_VALVE_TTK_OPEN 33 //пин управления открытием крана защиты обратки ТТК котла
#define PIN_VALVE_TTK_ClOSE 34 //пин управления закрытием крана защиты обратки ТТК котла
#define PIN_VALVE_TTK_SIGNAL_OPEN 31 //пин поступления сигнала полного открытия крана защиты обратки ТТК котла
#define PIN_VALVE_TTK_SIGNAL_ClOSE 32 //пин поступления сигнала полного закрытия крана защиты обратки ТТК котла

#define PIN_VALVE_SYS_OPEN 35 //пин управления открытием крана ругулировки системы
#define PIN_VALVE_SYS_ClOSE 36 //пин управления закрытием крана регулировки системы
#define PIN_VALVE_SYS_SIGNAL_OPEN 37 //пин поступления сигнала полного открытия крана регулировки системы
#define PIN_VALVE_SYS_SIGNAL_ClOSE 38 //пин поступления сигнала полного закрытия крана регулировки системы


//Датчики температуры DS18B20
#define PIN_DS18B20 2 //пин подключения датчииков температуры DS18B20 

//реле насосов
#define PIN_PUMP_TTK 29 //пин управления включением насоса ТТК
#define PIN_PUMP_SYS 30 //пин управления включением насоса системы
//#define PIN_PUMP_ЭК 3 //пин управления включением насоса электрокотла



//Индикаторы на ТМ1637
#define PIN_TM1637_Clk_COMMON 39
#define PIN_TM1637_DIO1 45
#define PIN_TM1637_DIO2 44
#define PIN_TM1637_DIO3 43
#define PIN_TM1637_DIO4 42
#define PIN_TM1637_DIO5 41
#define PIN_TM1637_DIO6 40
#define PIN_TM1637_DIO_Time 46


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













#endif

