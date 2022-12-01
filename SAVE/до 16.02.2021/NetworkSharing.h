#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
////////

//// esp partner
//#define ESP_OFF 0
//#define ESP_ON  1
//byte esp = ESP_OFF;

extern myCycle cycleNetworkSharing;
extern myCycle cycle4s; //цикл отпарвки команды своего присутствия на ESP через Serial3


//Инициализация порта UART для сетевого обмена
int initRxTxForNetSharing(unsigned long speedUart);

//проверка поступления данных через uart порт №3
void checkSerial();