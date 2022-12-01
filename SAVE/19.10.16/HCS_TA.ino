/*
    Name:       HCS_TA.ino
    Created:	19.08.2019 11:42:45
    Author:     Serega_nout\Sergey
*/

#include "MotorizedValve.h"
///#include <Wire.h>
#include "Indicator_TM1637.h"

///#include "DS18B20.h"
#include "HeaderHCS_TA.h"
#include "TestComponents.h"
#include "Temp_DS18B20.h"

///#include "Button.h"
//#include "TTimerList.h"
//#include "myCycle.h"
//#include ".\libraries\OneWire\OneWire.h"


////объявление глобальных переменных
////моторизированные краны
static unsigned long timeSwitchValveTTK = 0; //мс, максимальное полное время переключения моторизованного крана ТТК из одного положения в другое
//static unsigned long timeSwitchValveSYS = 0; //мс, максимальное полное время переключения моторизованного крана Системы из одного положения в другое
////насосы
//static bool  pumpTTK = false; //переменная состояния насоса ТТК
//static bool  pumpSYS = false; //переменная состояния насоса системы
//							  //static bool  pumpЭК = false; //переменная состояния насоса электрического котла ()
////состояние системы
//bool failure = false; //сигнал наличия состояния аварийного перегрева котла ТТК


// Cycles
myCycle cycleGetTemp(MS_500, true);  //5c цикл считывания температуры
myCycle cycleDispTempTM1637(MS_05S, true); //5c цикл обновления показаний температуры на индикаторах TM1637 
myCycle cycleTempControlTTK(MS_01M, true); //60с цикл запуска управления трехходовым краном ТТК (защита от холодной обратки) 


// The setup() function runs once each time the micro-controller starts
void setup()
{
	unsigned long startTime = millis();
	Serial.begin(57600);
	LOG("\nstart (setup)\n");
	//pinMode(13,)
	//TCounterDown myTaimer(1000,;

	//настройка пинов для моторизированных кранов 
	//открыт - значит поток направлен в боковой отвод
	pinMode(PIN_VALVE_TTK_OPEN, OUTPUT);
	digitalWrite(PIN_VALVE_TTK_OPEN, HIGH); //active level - LOW

	pinMode(PIN_VALVE_TTK_ClOSE, OUTPUT);
	digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH); //active level - LOW

	pinMode(PIN_VALVE_TTK_SIGNAL_OPEN, INPUT_PULLUP);
	pinMode(PIN_VALVE_TTK_SIGNAL_ClOSE, INPUT_PULLUP);

	//настраиваем выходы управления насосами (по умолчанию выключено)
	//ТТК
	pinMode(PIN_PUMP_TTK, OUTPUT);
	digitalWrite(PIN_PUMP_TTK, HIGH); //active level - LOW
	//Система
	pinMode(PIN_PUMP_SYS, OUTPUT);
	digitalWrite(PIN_PUMP_SYS, HIGH); //active level - LOW
	////электрокотел
	//pinMode(PIN_PUMP_ЭК, OUTPUT);
	//digitalWrite(PIN_PUMP_ЭК, HIGH); //active level - LOW

	//инициализация индикаторов TM1637
	init_TM1637();

	//Инициализация датчиков температуры
	initTemp(); 

	//Тестируем компоненты системы/ Общий тест работоспособности узлов системы
	LOG("Call TestSystem\n");
	testSystem(); //поменять на initValve

	Serial.print("time fo regTest2:  "); Serial.println((timeSwitchValveTTK));

	//int TestSystem() {
	//	//тестируем работу крана защиты обратки ТТК
	//	testValve (PIN_VALVE_TTK_OPEN, PIN_VALVE_TTK_ClOSE, PIN_VALVE_TTK_SIGNAL_OPEN, PIN_VALVE_TTK_SIGNAL_ClOSE, timeSwitchValveTTK);

	//	return 0;
	//}


	LOG("stop (setup) "); LOG((millis() - startTime)); LOG(" mc\n");
}

// Add the main program code into the continuous loop() function
void loop()
{
	unsigned long startTime = millis();
	LOG("\nstart (loop)\n");
	int result = 0; //коды завершения вызываемых функций

	//запуск проверки истечения циклов
	timersWorks(); ///убрать при чеке само проверяется

	if (cycleGetTemp.check()) {
		////Serial.println("GET TEMP");
		//getTemp();

		result = getTemperaturDS18B20();
		if (result) {

			LOG("Error in getTemperaturDS18B20 "); LOG2((uint16_t)result, BIN); LOG("\n");
			//обработка ошибки
			//...
		}
	}

	if (cycleDispTempTM1637.check()) {
		//getTemp();
		result = indicator_TM1637_Output_temperature();
		if (result) {

			LOG("Error in indicator_TM1637_Output_temperature "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		} 
	}

	if (cycleTempControlTTK.check()) {
		//getTemp();

		////static unsigned long t;
		////
		////Serial.println("\n_____cycleTempControlTTK: ");	Serial.print((millis() - t)); Serial.print(" ms"); Serial.print(cycleTempControlTTK.check());
		////t = millis();

		result = temperatureControlTTK();
		if (result) {

			LOG("Error in temperatureControlTTK "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		}
	}






	pinMode(7, INPUT_PULLUP); //открыть
	pinMode(8, INPUT_PULLUP); //закрыть
	extern float temperature[];

	if (!digitalRead(7)) {
		cycleGetTemp.setActive(false);
		cycleGetTemp.clear();
	

		temperature[4]++;
		delay(500);

	}
	if (!digitalRead(8)) {
		cycleGetTemp.setActive(false);
		cycleGetTemp.clear();

		temperature[4]--;
		delay(500);
	}
	if (!digitalRead(7) && !digitalRead(8)) {
		cycleGetTemp.setActive(true);
	}
	////Serial.println(cycleGetTemp.active());
	////pinMode(3, INPUT_PULLUP); //закрыть
	////Button oSignalClose(3, 15); // класс button позволяет фильтровать дребезг контактов с сигнальных концевиков крана. время фильтрации 15мс*на период повторения. 
	////
	////do {
	////	oSignalClose.filterAvarage();
	////	LOG("do");LOG(oSignalClose.flagPress); LOG("\n");
	////} while (oSignalClose.flagPress);



	LOG("stop (loop) "); LOG((millis() - startTime)); LOG(" mc\n");
	delay(0500);
}


//функции обслуживания таймеров
void timersWorks() {
	cycleGetTemp.check();
	cycleDispTempTM1637.check();
	cycleTempControlTTK.check();
}

void eraseCycles() {
	cycleGetTemp.clear();
	cycleDispTempTM1637.clear();
	cycleTempControlTTK.clear();
}