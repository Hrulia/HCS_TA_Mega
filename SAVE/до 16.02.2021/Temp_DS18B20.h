// Temp_DS18B20.h
//Опрос датчиков температуры DS18B20


#ifndef _TEMP_DS18B20_h
#define _TEMP_DS18B20_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

////#include ".\libraries\OneWire\OneWire.h"
#include "HeaderHCS_TA.h"

#define NUMBER_OF_DS18B20 16//Number of sensor connected

//Предварительная инициализация и тестирование всех датчиков температуры
int initTemp();

//поиск всех подключенных DS18B20
void findAllconnectDS18B20(); 

//Получение температуры со всех DS18B20
//int getTemperaturDS18B20(OneWire&);//передача аргумента по ссылке
int getTemperaturDS18B20();


#endif