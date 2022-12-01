//Restrictions for the free version of ThingSpeak:
//	1. Number of messages - 3 million / year(~8 200 / day)
//	2. Message update interval limit - Every 15 seconds
//	3. Number of channels - 4 (8 filds)
//	4. Private channel sharing - Limited to 3 shares


////#define DEBUG_NSH2



//син.кор.сер.
////// esp partner
////#define MEGA_OFF 0
////#define MEGA_ON  1
////byte mega = MEGA_OFF;

//myCycle cycleNetworkSharing2(MS_01S, true);			//Переданных на третийт порт даанных. (эмулируем работу esp)

//////Инициализация порта UART №2 для сетевого обмена
////int initRxTxForNetSharing2(unsigned long speedUart)
////{
////	Serial2.begin(speedUart);
////#ifdef DEBUG_NSH2
////	Serial.println(F("Modul Serial2 started"));
////#endif
////	return 0;
////}

////bool sFlag2 = true;
////unsigned long megaTimer = millis();
////
////#define MAX_SERIAL_REQ2  32
////String serialReq2 = "";
////
//////проверяем поступили-ли данные в порт Serial2
////void checkSerial2() {
////#ifdef DEBUG_NSH2 
////	Serial.println(F("ESP_start NetworkSharing.checkSerial"));
////#endif
////	Serial.println("esp_while (Serial2.available() > 0");
////	while (Serial2.available() > 0) {
////		//Serial.println(F("Serial availeble"));
////		if (sFlag2) {
////			serialReq2 = "";
////			sFlag2 = false;
////		}
////		char c2 = Serial2.read();
////		//Serial.println(F("10-char"));
////		if (c2 == 10) {
////			sFlag2 = true;
////			parseSerialStr2();
////		}
////		else if (c2 == 13) {
////			//Serial.println(F("13-char"));
////			// skip
////		}
////		else {
////			if (serialReq2.length() < MAX_SERIAL_REQ) {
////
////				serialReq2 += c2;
////				//Serial.println("serialReq: "+String(serialReq));
////			}
////		}
////	} // while (Serial3.available() > 0)
////	Serial.println("esp_exit Serial2.available() > 0");
////		// перезапуск таймера вызова функции обработки входящих запросов по сети.
////	cycleNetworkSharing2.clear();
////	cycleNetworkSharing2.reStart();
////} // checkSerial()
////
////	//Разбор строки, поступившей в Serial3
////void parseSerialStr2() {
////	if (serialReq2[0] == '?') {
////		parseSerialCmd2();
////	}
////	else {
////#ifdef DEBUG_NSH2 
////		Serial.print("[");
////		Serial.print(serialReq);
////		Serial.println("]");
////#endif
////	}
////}
////
//////Разбор строки, поступившей в Serial3
////void parseSerialCmd2() {
////	String command2, parameter2;
////	if (serialReq2.indexOf(F("?")) >= 0) {
////		int pBegin2 = serialReq2.indexOf(F("?")) + 1;
////		if (serialReq2.indexOf(F("=")) >= 0) {
////			int pParam2 = serialReq2.indexOf(F("="));
////			command2 = serialReq2.substring(pBegin2, pParam2);
////			parameter2 = serialReq2.substring(pParam2 + 1);
////		}
////		else {
////			command2 = serialReq2.substring(pBegin2);
////			parameter2 = "";
////		}
////#ifdef DEBUG_NSH2 
////		Serial.print(F("command2/parameter2: "));
////		Serial.print(command2);
////		Serial.print(F("/"));
////		Serial.println(parameter2);
////#endif
////		//Разбор поступивших команд
////		if (command2 == F("mega")) {															//?mega
////			if (parameter2 == F("1")) {
////				mega = MEGA_ON;
////				megaTimer = millis();
////#ifdef DEBUG_NSH2 
////				Serial.println(F("Mega - working!"));
////#endif
////			}
////		}else if (command2 == F("puttemp")) {												//?puttemp
////			//int iparam2 = parameter2.toFloat();
////				// приём float чисел через сериал
////				// десятичный разделитель - . (точка)
////				// разделитель - ; (семиколон)
////				// пример посылки: 5.326;-3.589
////
////			//parameter2       // содержит последовательность значений температур через разделитель
////			int n = 0; //String buf_1="";
////			while (parameter2.length() > 0) {
////				byte dividerIndex = parameter2.indexOf(';');   // ищем индекс разделителя
////				String buf_1 = parameter2.substring(0, dividerIndex);    // создаём строку с первым числом
////				temperature[n] = buf_1.toFloat();
////				parameter2 = parameter2.substring(dividerIndex + 1);   // остаток строки
////				n++;
////				//Serial.println();
////			}
////			Serial.println(temperature[0]);  // проверка
////
////		}
////	} // if (request.indexOf(F("?")) >= 0)
////} // parseSerialCmd()


////void checkMegaAndESP2() {
////	//checkSerial();
////	//Serial.println(F("?mega=1"));
////	Serial2.println(F("?mega=1"));
////	if (millis() - megaTimer > 8000) {
////		mega = MEGA_OFF; //больше восьми секунд модуль ESP не отвечал. Необходимо его сбросить
////		megaTimer = millis();
////		//reset ESP
////		//....
////	}
////}	// cheсkMegaAndESP() 



////#define NUMBER_OF_DS18B20 16//Number of sensor DS18B20 
////
////
////myCycle cycleRequestTemperature(MS_30S, true);//
////myCycle cycleSendingDataToTheSiteIoT(MS_05M, true);//цикл отправки данных о температуре на сайт IoT
////extern myCycle cycleNetworkSharing2;
////
////float temperatures[(NUMBER_OF_DS18B20+1)]; //Массив температур датчиков DS18B20 + датчик температуры дыма


//void setup() {
//	Serial.begin(115000);
// Serial.setTimeout(50);    // таймаут шоб не ждать (по умолч. аж секунда)
//}
////
////void loopESP() {
////	//проверяем поступление команды по порту uart от MEGи
////	if (cycleNetworkSharing2.check()) {
////		checkSerial2();  
////	}
////
////	//Запрос данных о температуре из модуля MEGA
////	if (cycleRequestTemperature.check()) {
////		RequestTemperature();
////	}
////
////	//отправки данных о температуре на сайт IoT (не чаще раза в 15 секунд)
////	if (cycleSendingDataToTheSiteIoT.check()) {
////		//SendingDataToTheSiteIoT();  
////	}
////
////}

//////Получаем информацию о температуре с модуля MEGA
////void RequestTemperature() {
////	//запрос к меге на передачу всех значений температуры 
////	Serial2.println("?gettemp=A");
////	Serial.println("ESP-?gettemp=A");
////
////	//Serial.setTimeout(250); //удали в esp
////	//получаем данные из uart
////	checkSerial2();
////
////	// перезапуск таймера вызова функции.
////	cycleRequestTemperature.clear();
////	cycleRequestTemperature.reStart();
////}


//void SendingDataToTheSiteIoT() {
//
//
//
//	// перезапуск таймера вызова функции обработки входящих запросов по сети.
//	cyclecycleNetworkSharing2.clear();
//	cyclecycleNetworkSharing2.reStart();
//}