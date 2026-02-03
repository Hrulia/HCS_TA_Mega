// Temp_DS18B20.h
//Опрос датчиков температуры DS18B20


#ifndef _TEMP_DS18B20_h
	#define _TEMP_DS18B20_h

	#include "arduino.h"
	#include "HeaderHCS_TA.h"


	extern GlobalParametersStructure globalParameters;

	//Предварительная инициализация и тестирование всех датчиков температуры
	int initTemp();

	//поиск всех подключенных DS18B20
	void findAllconnectDS18B20(); 

	//Получение температуры со всех DS18B20
	//int getTemperaturDS18B20(OneWire&);//передача аргумента по ссылке
	uint16_t getTemperaturDS18B20();


#endif