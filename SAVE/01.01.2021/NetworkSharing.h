#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


extern myCycle cycleNetworkSharing;

//Инициализация порта UART для сетевого обмена
int initRxTxForNetSharing(int speedUart);

//проверка поступления данных через uart порт
void checkSerial();