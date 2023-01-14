#include "NetworkSharing.h"	


//#define DEBUG_NSH

//син.кор.сер.
// esp partner
#define ESP_OFF 0
#define ESP_ON  1
byte esp = ESP_OFF;

extern float temperature[];
extern int BoilerPumpMode;			//1 - on, 2 - off, 3 - auto
extern int SystemPumpMode;			//1 - on, 2 - off, 3 - auto
extern int SysTempControlMode;	//1 – мой алгоритм регулирования, 2 - PID регулятор
extern int DoorAirMode;					//1 - open, 2 - close, 3 - auto


//Инициализация порта UART №3 для сетевого обмена
int initRxTxForNetSharing(unsigned long speedUart)
{
	Serial3.begin(speedUart);
	#ifdef DEBUG_NSH
		Serial.println(F("Modul NetworkSharing started at Serial3"));
	#endif
	return 0;
}

bool sFlag = true;
unsigned long espTimer = millis();

#define MAX_SERIAL_REQ  50 //32, 50, 100
String serialReq = "";  

//проверяем поступили-ли данные в порт Serial3
void checkSerial() {
	#ifdef DEBUG_NSH 
		//Serial.println(F("start NetworkSharing.checkSerial"));
	#endif

	while (Serial3.available() > 0) {
		//Serial.println(F("Serial availeble"));
		if (sFlag) {
			serialReq = "";
			sFlag = false;
		}
		char c = Serial3.read();
		//Serial.println(F("10-char"));
		if (c == 10) {
			sFlag = true;
			parseSerialStr();
			}
		else if (c == 13) {
			//Serial.println(F("13-char"));
			// skip
			}
		else {
			if (serialReq.length() < MAX_SERIAL_REQ) {
		
				serialReq += c;
				//Serial.println("serialReq: "+String(serialReq));
			}
		}
	} // while (Serial3.available() > 0)
	
} // checkSerial()

//Разбор строки, поступившей в Serial3, поиск символа начала команды
	void parseSerialStr() {
		if (serialReq[0] == '?') {
			parseSerialCmd();
		}
		else {
			#ifdef DEBUG_NSH 
				Serial.print("[");
				Serial.print(serialReq);
				Serial.println("]");
			#endif
		}
	}

//Разбор строки команды, поступившей в Serial3, обработка команд
	void parseSerialCmd() {
		String command, parameter;
		if (serialReq.indexOf(F("?")) >= 0) {
			int pBegin = serialReq.indexOf(F("?")) + 1;
			if (serialReq.indexOf(F("=")) >= 0) {
				int pParam = serialReq.indexOf(F("="));
				command = serialReq.substring(pBegin, pParam);
				parameter = serialReq.substring(pParam + 1);
			}
			else {
				command = serialReq.substring(pBegin);
				parameter = "";
			}
#ifdef DEBUG_NSH 
			Serial.print(F("com/param: "));
			Serial.print(command);
			Serial.print(F("/"));
			Serial.println(parameter);
#endif
			//Разбор поступивших команд////
//******
// ?esp
			if (command == F("esp")) {																//?esp
				if (parameter == F("1")) {
					esp = ESP_ON;
					espTimer = millis();
#ifdef DEBUG_NSH 
					Serial.println(F("ESP - working!"));
#endif
				}
			}

//******
// ?reqesttemp
			else if (command == F("reqesttemp")) {												// ?reqesttemp
				if (parameter == F("A")) {
					/* для тестирования */
//temperature[0] = 0.11;
//temperature[1] = 1.11;
//temperature[2] = 2.11;
//temperature[3] = 3.11;
//temperature[4] = 4.11;
//temperature[5] = 5.11;
//temperature[6] = 6.11;
//temperature[7] = 7.11;
//temperature[8] = 8.11;
//temperature[9] = 9.11;
//temperature[10] = 10.11;
//temperature[11] = 11.11;
//temperature[12] = 12.11;
//temperature[13] = 13.11;
//temperature[14] = 14.11;
//temperature[15] = 15.11;
//g_t_flueGases = 597;
//g_RoomSetpointCurrent=23.3;

					Serial3.print(F("?sendtemp0=")); Serial3.println(temperature[0]);	delay(10);
					Serial3.print(F("?sendtemp1=")); Serial3.println(temperature[1]); delay(10);
					Serial3.print(F("?sendtemp2=")); Serial3.println(temperature[2]); delay(10);
					Serial3.print(F("?sendtemp3=")); Serial3.println(temperature[3]); delay(10);
					Serial3.print(F("?sendtemp4=")); Serial3.println(temperature[4]); delay(10);
					Serial3.print(F("?sendtemp5=")); Serial3.println(temperature[5]); delay(10);
					Serial3.print(F("?sendtemp6=")); Serial3.println(temperature[6]); delay(10);
					Serial3.print(F("?sendtemp7=")); Serial3.println(temperature[7]); delay(10);
					Serial3.print(F("?sendtemp8=")); Serial3.println(temperature[8]); delay(10);
					Serial3.print(F("?sendtemp9=")); Serial3.println(temperature[9]); delay(10);
					Serial3.print(F("?sendtemp10=")); Serial3.println(temperature[10]); delay(10);
					Serial3.print(F("?sendtemp11=")); Serial3.println(temperature[11]); delay(10);
					Serial3.print(F("?sendtemp12=")); Serial3.println(temperature[12]); delay(10);
					Serial3.print(F("?sendtemp13=")); Serial3.println(temperature[13]); delay(10);
					Serial3.print(F("?sendtemp14=")); Serial3.println(temperature[14]); delay(10);
					Serial3.print(F("?sendtemp15=")); Serial3.println(temperature[15]); delay(10);
					Serial3.print(F("?sendtemp16=")); Serial3.println(g_t_flueGases); delay(10);
					Serial3.print(F("?sendtemp17=")); Serial3.println(g_RoomSetpointCurrent); delay(10);
					//Serial3.print(temperature[0]); Serial3.print(";");
				}
				else {
					int iparam = parameter.toInt();
					if (iparam < NUMBER_OF_DS18B20) {
						Serial3.print(F("?sendtemp")); Serial3.print(iparam); Serial3.print(F("=")); Serial3.println(temperature[iparam], 2);
					}
					else {
#ifdef DEBUG_NSH 
						Serial.println(F("Wrong number of the requested temperature sensor!"));
#endif
					}
				}
			}

			//******
			// ?esplog 
						/*Этот case пора удалять, все служебные данные полученные через uart выводятся в [] скобках. */
			else if (command == F("esplog")) {														// ?esplog
				Serial.println("espLog: " + parameter);
			}

			//******
			// ?sendrssi
			else if (command == F("sendrssi")) {
				// получили от ESP значение RSSI сигнала
				Serial.println("espRSSI: " + parameter + "dBm");
			}

//******			
// ?SetGTargetTemp=newT //получение целевой температуры
			else if (command == F("SetGTargetTemp")) {
				///Serial.println("Command ESP: ?SetGTargetTemp/param:" + parameter);
				//переводим строковый параметр в тип float и присваиваем переменной g_tRoomSetpoint
				g_tRoomSetpoint = parameter.toFloat();
				///Serial.print("Now g_tRoomSetpoint="); Serial.println(g_tRoomSetpoint);
				//обновим информацию на индикаторе TM1637
				extern TM1637Display TM1637_9;
				TM1637_9.showNumberMsn(g_tRoomSetpoint, 1);
				//сохраним в EEPROM. Загрузка состояния в процедуре setup
				EEPROM.write(0x00, (uint8_t)g_tRoomSetpoint);
				//Отправка информации о на сервер брокера MQTT
				Serial3.print(F("?sendGTargetTemp=")); Serial3.println(g_tRoomSetpoint, 1);
			}

//******			
// ?getSystemParameters //Запрос от ESP системных параметров
			else if (command == F("getSystemParameters")) {
				///Serial.println("Request from ESP: ?getSystemParameters/param:" + parameter);
				// X – работы насоса котла;
				// Y – режим работы насоса системы;
				// Z – алгоритм регулирования температуры системы;
				// K – режим работы привода заслонки поддувала
				//reserve
				//reserve
				Serial3.print(F("?sendSystemParameters=")); Serial3.println(String(BoilerPumpMode) + SystemPumpMode + SysTempControlMode + DoorAirMode + "0" + "0");
			}

			//******			
			// ?setBoilerPumpMode //Команда установки режима работы насоса ТТК
			else if (command == F("setBoilerPumpMode")) {
				///Serial.println("Request from ESP: ?setBoilerPumpMode/param:" + parameter);
				//[0] - BoilerPumpMode				//1 - on, 2 - off, 3 - auto
				if (parameter.toInt() > 0 && parameter.toInt() < 4) {
					BoilerPumpMode = parameter.toInt();
				}
				else {
					///Serial.println(F("Error in command from ESP ?setBoilerPumpMode, param:") + parameter);
				}
				//Sending updated system parameters to ESP
				Serial3.print(F("?sendSystemParameters=")); Serial3.println(String(BoilerPumpMode) + SystemPumpMode + SysTempControlMode + DoorAirMode + "0" + "0");
			}

	//******			
	// ?setSystemPumpMode //Команда установки режима работы насоса системы
			else if (command == F("setSystemPumpMode")) {
				///Serial.println("Request from ESP: ?setSystemPumpMode/param:" + parameter);
				//[1] - SystemPumpMode				//1 - on, 2 - off, 3 - auto
				if (parameter.toInt() > 0 && parameter.toInt() < 4) {
					SystemPumpMode = parameter.toInt();
				}
				else {
					///Serial.println("Error in command from ESP ?setSystemPumpMode, param:" + parameter);
				}
				//Sending updated system parameters to ESP
				Serial3.print(F("?sendSystemParameters=")); Serial3.println(String(BoilerPumpMode) + SystemPumpMode + SysTempControlMode + DoorAirMode + "0" + "0");
			}
		}
	}

//проверка работоспособного состояния модуля сетевых функций ESP8266
void checkMegaAndESP() {
	//checkSerial();
		//Serial.println(F("?mega=1"));
		//Отправка модулю esp информации о своем присутствии 
		Serial3.println(F("?mega=1"));

		//для теста. потом удали.
		Serial3.println(F("?reqestrssi"));

		////if (millis() - espTimer > 300000) {
		////	//Попадаем сюда, если модуль esp более хх секунд не присылал информации о своем присутствии.
		////	esp = ESP_OFF; 
		////	espTimer = millis();
		////	//делаем reset ESP
		////	//DEBUGLN("Module MEGA reseting");
		////	digitalWrite(PIN_RESET_ESP, LOW);
		////	delay(200);
		////	digitalWrite(PIN_RESET_ESP, HIGH);

		////}
}	// cheсkMegaAndESP() 




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