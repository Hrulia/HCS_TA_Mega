/*
    Name:       HCS_TA.ino
    Created:	19.08.2019 11:42:45
    Author:     Serega_nout\Sergey
*/

#include "Buttons.h"
#include "SystemTime.h"
#include "Pins.h"   //тут собраны все пины.

#include "MotorizedValve.h"
#include "HeaderHCS_TA.h"
#include "Temp_DS18B20.h"
#include "Indicator_TM1637.h"


//Определение(выделение памяти) глобальных переменных состояния системы

//текущее время/дата в системе 
	volatile DateTime g_systemDateTime;

//моторизированные краны
	/*static*/ unsigned long g_timeSwitchValveTTK = 0; //мс, максимальное полное время переключения моторизованного крана ТТК из одного положения в другое
	/*static*/ unsigned long g_timeSwitchValveSYS = 0; //мс, максимальное полное время переключения моторизованного крана Системы из одного положения в другое

//насосы
	/*static*/ bool  g_pumpTTK = false; //переменная состояния насоса ТТК
	/*static*/ bool  g_pumpSYS = false; //переменная состояния насоса системы
	///*static*/ extern bool  g_pumpЭК = false; //переменная состояния насоса электрического котла ()

//состояние перегрева ТТК котла					
	/*static*/ bool g_failure = false; //сигнал наличия состояния аварийного перегрева котла ТТК

//заданное значение температуры помещения (к которому стремимся при регулировке)
	/*uint8_t*/float g_tRoomSetpoint=23;

// Cycles
	myCycle cycleDispSystemTime(250, true); //0,25 сек. Вывод информации о системном времени на индикатор ТМ1637
	myCycle cycleGetTemp(15000, true);  //5c цикл считывания температуры (MS_05S)
	myCycle cycleDispTempTM1637(150, true); //5c цикл обновления показаний температуры на индикаторах TM1637 
	myCycle cycleTempControlTTK(MS_30S, true); //60с цикл запуска управления трехходовым краном ТТК (защита от холодной обратки) (MS_01M)
	myCycle cycleTempControlSYS(MS_01M, true); //60с цикл запуска управления трехходовым краном системы 


// The setup() function runs once each time the micro-controller starts
void setup()
{
	unsigned long startTime = millis();
	Serial.begin(115200);
	LOG("\nstart (setup)\n");
	
	pinMode(13, OUTPUT);
	//pinMode(46, OUTPUT);
	//digitalWrite(46, HIGH);

	//настройка пинов для моторизированных кранов 
	//открыт - значит поток направлен в боковой отвод
	//TTK
	pinMode(PIN_VALVE_TTK_OPEN, OUTPUT);
	digitalWrite(PIN_VALVE_TTK_OPEN, HIGH); //active level - LOW
	pinMode(PIN_VALVE_TTK_ClOSE, OUTPUT);
	digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH); //active level - LOW

	pinMode(PIN_VALVE_TTK_SIGNAL_OPEN, INPUT_PULLUP);
	pinMode(PIN_VALVE_TTK_SIGNAL_ClOSE, INPUT_PULLUP);
	//SYS
	pinMode(PIN_VALVE_SYS_OPEN, OUTPUT);
	digitalWrite(PIN_VALVE_SYS_OPEN, HIGH); //active level - LOW
	pinMode(PIN_VALVE_SYS_ClOSE, OUTPUT);
	digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH); //active level - LOW

	pinMode(PIN_VALVE_SYS_SIGNAL_OPEN, INPUT_PULLUP);
	pinMode(PIN_VALVE_SYS_SIGNAL_ClOSE, INPUT_PULLUP);

	//выход сирены аварийного перегрева котла
	pinMode(PIN_EMERGENCY_SIREN, OUTPUT); 

	//настраиваем выходы управления насосами (по умолчанию насосы включены НЗ контактами реле)
	//ТТК
	pinMode(PIN_PUMP_TTK, OUTPUT);
	digitalWrite(PIN_PUMP_TTK, HIGH); //active level - LOW
	//Система
	pinMode(PIN_PUMP_SYS, OUTPUT);
	digitalWrite(PIN_PUMP_SYS, HIGH); //active level - LOW
	////электрокотел
	//pinMode(PIN_PUMP_ЭК, OUTPUT);
	//digitalWrite(PIN_PUMP_ЭК, HIGH); //active level - LOW


	//инициализация RTC
	initSystemTime();

	//инициализация индикаторов TM1637
	init_TM1637(); //void

	//Инициализация датчиков температуры
	initTemp(); 

	//Тестирование моторизированных кранов
	initValve(); 

	//int initValve() {
	//	//тестируем работу крана защиты обратки ТТК
	//	testValve (PIN_VALVE_TTK_OPEN, PIN_VALVE_TTK_ClOSE, PIN_VALVE_TTK_SIGNAL_OPEN, PIN_VALVE_TTK_SIGNAL_ClOSE, g_timeSwitchValveTTK);

	//	return 0;
	//}


	LOG("stop (setup) "); LOG((millis() - startTime)); LOG(" mc\n");
}

// Add the main program code into the continuous loop() function
void loop()
{
	unsigned long startTime = millis();
	//LOG("\nstart (loop)\n");
	int result = 0; //коды завершения вызываемых функций

	//запуск проверки истечения циклов
	timersWorks(); ///убрать при чеке само проверяется

	//обновление информации о системном времени на TM1637
	if (cycleDispSystemTime.check()) {
		result = indicator_TM1637_Output_time();
		if (result) {
			LOG("Error in indicator_TM1637_Output_time "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		}
	}

	//обновление информации с датчиков температуры
	if (cycleGetTemp.check()) {
		result = getTemperaturDS18B20();
		if (result) {
			LOG("Error in getTemperaturDS18B20 "); LOG2((uint16_t)result, BIN); LOG("\n");
			//обработка ошибки
			//...
		}
	}

	//Обновление информации о температуре на индикаторах TM1637
	if (cycleDispTempTM1637.check()) {
		result = indicator_TM1637_Output_temperature();
		if (result) {
			LOG("Error in indicator_TM1637_Output_temperature "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		} 
	}

	//управление краном ТТК (защита от холодной обратки)
	if (cycleTempControlTTK.check()) {
		result = temperatureControlTTK();
		if (result) {
			LOG("Error in temperatureControlTTK "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		}
	}

	//управление краном системы
	if (cycleTempControlSYS.check()) {
		result = temperatureControlSYS();
		if (result) {
			LOG("Error in temperatureControlSYS "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		}
	}

	////	temperature[4]++;
	////	delay(500);

	////}
	////if (!digitalRead(8)) {
	////	cycleGetTemp.setActive(false);
	////	cycleGetTemp.clear();

	////	temperature[4]--;
	////	delay(500);
	////}
	////if (!digitalRead(7) && !digitalRead(8)) {
	////	cycleGetTemp.setActive(true);
	////}

	//LOG("stop (loop) "); LOG((millis() - startTime)); LOG(" mc\n");
}


//функции обслуживания таймеров
void timersWorks() {
	cycleGetTemp.check();
	cycleDispTempTM1637.check();
	cycleTempControlTTK.check();
}

////void eraseCycles() {
////	cycleGetTemp.clear();
////	cycleDispTempTM1637.clear();
////	cycleTempControlTTK.clear();
////}