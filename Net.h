// Net.h

#ifndef _NET_h
#define _NET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "HeaderHCS_TA.h"

//Массив температур от датчиков DS18B20. Количество их =  NUMBER_OF_DS18B20
// [0]- t1, подача cистемы
// [1]- t2, обратка системы
// [2]- t3, отвод трехходового крана ТТК
// [3]- t4, подача ТТК
// [4]- t5, обратка ТТК
// [5]- t6, верх ТА подача от ТТК
// [6]- t7, низ ТА обратка ТТК
// [7- t8, верх ТА подача в систему
// [8]- t9, отвод трехходового крана системы
// [9]- t10, низ ТА, обратка системы
// [10]-t11, верх ТА
// [11]-t12, низ ТА
// [12]-t13, температура в помещении
// [13]-t14, температура на улице
// [14]-t15, подача ЭК 
// [15]-t16, обратка ЭК
extern float temperature[];


	 //byte _mac[6];	// MAC-адрес
	 //byte _ip[4];	// IP-адрес
	 //byte _Dns[4];	// адрес DNS-сервера
	 //byte _gateway[4];	// адрес сетевого шлюза
	 //byte _subnet[4];	// маска подсети
	 //uint16_t _serverPort; //порт для ардуино сервера

	 //Инициализируем модуль сетевой модуль
	int initNet();

	//Обработка запросов от Net клиентов
	int NetRequests();

	//calculate Control Sum
	byte calcCS(byte* array1, int n);

//
//class NetClass
//{
//
// protected:
//	 ///byte _mac[6];		// MAC-адрес
//	 ///byte _ip[4];		// IP-адрес
//	 //byte _Dns[4];	// адрес DNS-сервера
//	 //byte _gateway[4];// адрес сетевого шлюза
//	 //byte _subnet[4];	// маска подсети
//	 uint16_t _serverPort; //порт для ардуино сервера
//
// public:
//	 NetClass();
//
//
//	void init();
//};

//extern NetClass Net;

#endif

