#include "NetworkSharing.h"	


#define DEBUG_NSH

//Инициализация порта UART для сетевого обмена
int initRxTxForNetSharing(int speedUart)
{
	Serial3.begin(speedUart);
	#ifdef DEBUG_NSH
		Serial.println(F("Modul NetworkSHaring started"));
	#endif

	////modulRobotdyn = MODUL_ENABLE;
	////started("RobotDyn", true);
	return 0;
}

void checkSerial(){
	#ifdef DEBUG_NSH 
		Serial.println(F("start NetworkSharing.checkSerial"));
	#endif





		// перезапуск таймера вызова функции обработки входящих запросов по сети от клиентов.
		cycleNetworkSharing.clear();
		cycleNetworkSharing.reStart();

	#ifdef DEBUG_NSH 
		Serial.println(F("stop NetworkSharing.checkSerial"));
	#endif
}
	

void checkSerial() {
	while (Serial3.available() > 0) {
		if (sFlag) {
			serialReq = "";
			sFlag = false;
		}
		char c = Serial3.read();
		if (c == 10) {
			sFlag = true;
			parseSerialStr();
		}
		else if (c == 13) {
			// skip
		}
		else {
			if (serialReq.length() < MAX_SERIAL_REQ) {
				serialReq += c;
			}
		} // if
	} // while (Serial3.available() > 0)
} // checkSerial()














	////calculating Control Sum
	//byte calcCS(byte* array1, int n)
	//{
	//	//Serial.println("Calculating CRC");
	//	byte i = 0;
	//	byte tempCS = 0x5E;
	//	while (i < n)
	//	{
	//		tempCS ^= array1[i];
	//		i++;
	//	}
	//	//Serial.print("CRC "); Serial.println((char)tempCS);
	//	return tempCS;
	//}