
#include "ValvesAndPumpsController.h"

/*Принадлежность датчиков температуры*/
// [0- t1, подача cистемы
// 1- t2, обратка системы
// 2- t3, Температура в Большой спальне
// 3- t4, подача ТТК
// 4- t5, обратка ТТК
// 5- t6, верх ТА подача от ТТК
// 6- t7, низ ТА обратка ТТК
// 7- t8, верх ТА подача в систему
// 8- t9, отвод трехходового крана системы
// 9- t10, низ ТА, обратка системы
// 10-t11, верх ТА
// 11-t12, низ ТА
// 12-t13, температура в Зале
// 13-t14, температура на улице
// 14-t15, подача ЭК 
// 15-t16, обратка ЭК


MAX6675_Thermocouple thermocouple(PIN_THERMOCOUPLE_CLK, PIN_THERMOCOUPLE_CS, PIN_THERMOCOUPLE_DO); // Объект термопара (температура дымовых газов)

//extern TTimerList TimerList; //Список счетчиков, срабатывающих по прерыванию от программного счетчика  counter0
THandle hCounterStopActionForValve;   // Handle счетчика, прерывающего активное состояние сигналов управления кранами
int m_openPinSYS = 0, m_closePinSYS = 0; //внутренние переменные для этого модуля .cpp. Используются в процедуре tmrStopActionForSysValve, Устанавливаются в процедуре valveAction(). По идее нужно бы делать отдельный класс и их внутренними.


/********************************/
// Start Расписание температуры на сутки
/********************************/
//float dailySheduleOfTemperature[24] = {
//0.826, //0-1
//0.826, //1-2
//0.826, //2-3
//0.826, //3-4
//0.913, //4-5
//0.956, //5-6
//0.956, //6-7
//0.956, //7-8
//0.913, //8-9
//0.913, //9-10
//0.913, //10-11
//0.913, //11-12
//0.913, //12-13
//0.913, //13-14
//0.913, //14-15
//0.913, //15-16
//0.956, //16-17
//1.0, //17-18
//1.0, //18-19
//1.0, //19-20
//1.0, //20-21
//1.0, //21-22
//0.956, //22-23
//0.956 };//23-24 //

//Вариант для ребенка
float dailySheduleOfTemperature[24] ={
	0.983, //0-1 (22,6/23)
	0.983, //1-2 (22,6/23)
	0.983, //2-3 (22,6/23)
	0.991, //3-4 (22,8/23)
	0.991, //4-5 (22,8/23)
	0.995, //5-6 (22,9/23)
	0.995, //6-7 (22,9/23)
	0.995, //7-8 (22,9/23)
	1.000, //8-9 (23/23)
	1.000, //9-10 (23/23)
	0.995, //10-11 (22,9/23)
	1.000, //11-12 (23/23)
	0.995, //12-13 (22,9/23)
	1.000, //13-14 (23/23)
	1.000, //14-15 (23/23)
	0.995, //15-16 (22,9/23)
	0.995, //16-17 (22,9/23)
	0.995, //17-18 (22,9/23)
	1.000, //18-19 (23/23)
	1.000, //19-20 (23/23)
	1.000, //20-21 (23/23)
	0.995, //21-22 (22,9/23)
	0.991, //22-23 (22,8/23)
	0.983 };//23-24 (22,6/23)
//Рабочий вариант для родителей
//float dailySheduleOfTemperature[24] = {
//	0.869, //0-1 (20/23)
//	0.826, //1-2 (19/23)
//	0.826, //2-3 (19/23)
//	0.826, //3-4 (19/23)
//	0.826, //4-5 (19/23)
//	0.913, //5-6 (21/23)
//	0.956, //6-7 (22/23)
//	0.978, //7-8 (22.5/23)
//	0.956, //8-9 (22/23)
//	0.934, //9-10 (21.5/23)
//	0.913, //10-11 (21/23)
//	0.934, //11-12 (21.5/23)
//	0.956, //12-13 (22/23)
//	0.913, //13-14 (21/23)
//	0.913, //14-15 (21/23)
//	0.934, //15-16 (21.5/23)
//	0.956, //16-17 (22/23)
//	0.978, //17-18 (22.5/23)
//	0.956, //18-19 (22/23)
//	0.978, //19-20 (22.5/23)
//	1.000, //20-21 (23/23)
//	0.978, //21-22 (22.5/23)
//	0.956, //22-23 (22/23)
//	0.934 };//23-24 (21.5/23)

/********************************/
// Финишь Расписание температуры на сутки
/********************************/

//настраиваем порты и устанавливаем начальные положения сигналов
void initPortsValvesAndPumps(){
	DEBUG_PRINTLN_VPC("initPortsValvesAndPumps: start");
	//выход сирены аварийного перегрева котла
	pinMode(PIN_EMERGENCY_SIREN, OUTPUT); digitalWrite(PIN_EMERGENCY_SIREN, HIGH);

	//настраиваем выходы управления насосами ТТК и системным
	pinMode(PIN_PUMP_TTK, OUTPUT); 		pumpAction(PIN_PUMP_TTK, AC_OFF);
	pinMode(PIN_PUMP_SYS, OUTPUT);		pumpAction(PIN_PUMP_SYS, AC_OFF);
//Электрокотел, насос и выход управления вклюбчением котла
	pinMode(PIN_PUMP_EK, OUTPUT);			pumpAction(PIN_PUMP_EK, AC_OFF);
	pinMode(PIN_CONTROL_EK, OUTPUT);	EKAction(PIN_CONTROL_EK, AC_OFF);

	//настройка пинов для моторизированных кранов (открыт - значит поток направлен прямо, боковой отвод закрыт)
	//TTK
#if TTK_VALVE_EXISTS 
	// тут осталось старое управление. Новое смотри для крана системы
	pinMode(PIN_VALVE_TTK_OPEN, OUTPUT);
	digitalWrite(PIN_VALVE_TTK_OPEN, HIGH); //active level - LOW
	pinMode(PIN_VALVE_TTK_ClOSE, OUTPUT);
	digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH); //active level - LOW

	pinMode(PIN_VALVE_TTK_SIGNAL_OPEN, INPUT_PULLUP);
	pinMode(PIN_VALVE_TTK_SIGNAL_ClOSE, INPUT_PULLUP);
#endif
	//SYS
	pinMode(PIN_VALVE_SYS_OPEN, OUTPUT);
	pinMode(PIN_VALVE_SYS_ClOSE, OUTPUT);
	pinMode(PIN_VALVE_SYS_SIGNAL_OPEN, INPUT_PULLUP);
	pinMode(PIN_VALVE_SYS_SIGNAL_ClOSE, INPUT_PULLUP);
	valveAction(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, AC_EMPTY);

}

//вызов функции тестирования для кранов котла и системы
int testValves() {
	DEBUG_PRINTLN_VPC(F("testValves(): Start test SYS valve"));//PIN_VALVE_SYS_OPEN
	if (!TEST_VALVE_ON) {	//выход, если тестирование кранов отключено
		globalParameters.g_timeSwitchValveSYS = MAX_TIME_TESTING_VALVE;
		Serial.print(F("testValves(): Тест крана отменен. Значение параметра MAX_TIME_TESTING_VALVE=false. Время переключения крана установлено равным MAX_TIME_TESTING_VALVE и составляет: ")); Serial.println(MAX_TIME_TESTING_VALVE);
		return 0;
	}
	int result = 0; //коды завершения вызываемых функций

	//тестируем работу крана защиты обратки ТТК
#if TTK_VALVE_EXISTS
	//старая процедура. смотри для крана системы - SYS
	DEBUG_PRINTLN_VPC("\nCall testValveTTK\n");
	result = testValve(PIN_VALVE_TTK_OPEN, PIN_VALVE_TTK_ClOSE, PIN_VALVE_TTK_SIGNAL_OPEN, PIN_VALVE_TTK_SIGNAL_ClOSE, &g_timeSwitchValveTTK);
	if (result) {
		DEBUG_PRINTLN_VPC("Error in... "); DEBUG_PRINTLN_VPC(result); LOG("\n");
	}
	Serial.print("g_timeSwitchvalveTTK "); Serial.print(g_timeSwitchValveTTK); Serial.print(" ms\n");
#endif

	//тестируем работу крана регулировки темпратуры системы			
	DEBUG_PRINTLN_VPC(F("testValves(): Start test SYS valve"));//PIN_VALVE_SYS_OPEN
	result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &globalParameters.g_timeSwitchValveSYS);
	if (result) {
		DEBUG_PRINTLN_VPC("Error in... " + String(result)+"\n");  //обработка ошибки
	}
	Serial.print(String(F("Maximum time for full opening/closing of the system tap (g_timeSwitchValveSYS): ")) +String(globalParameters.g_timeSwitchValveSYS)+"ms\n");
	DEBUG_PRINTLN_VPC("testValves: Stop test all valve");
	return result;
}

//Определение времени переключеникрана в закрыто и открытое положения
int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long * ptimeSwitch)
{
	/*
	return errors code:
	1 - ошибка теста открытия крана			
	2 - ошибка теста закрытия крана
	3 - ошибка обоих тестов
	*/
	DEBUG_PRINTLN_VPC(F("\ntestValve: start"));

	int errCod = 0;
	Button oSignalOpen(signalOpen, 15); // класс button позволяет фильтровать дребезг контактов с сигнальных концевиков крана. время фильтрации 15мс*на период повторения loop. 
	Button oSignalClose(signalClose, 15);

	//сохраняем текущее время
	unsigned long startTime = millis();
	//открываем кран
	DEBUG_PRINTLN_VPC(F("_Start opening valve"));

	valveAction(open, close, signalOpen, signalClose, AC_OPEN);  //открываем полностью - длительность нулевая в аргументах

	//ждем появления сигнала открытия низкого уровня
	DEBUG_PRINTLN_VPC(F("_waiting for the signal to fully open the valve"));
	do {
		//Serial.println(F("_in the Do. Waiting for the signal to fully open the valve"));
		oSignalOpen.scanState();
		if ((millis() - startTime) > MAX_TIME_TESTING_VALVE) {
			errCod = 1;
			break;
		}
	} while (!oSignalOpen.flagPress);

	//снимем напряжение с крана
	valveAction(open, close, signalOpen, signalClose, AC_EMPTY);
	///digitalWrite(open, HIGH); //Serial.println("_35-HIGH");
	///digitalWrite(close, HIGH); 

	//сохраняем время выполнения операции открытия
	*ptimeSwitch = millis() - startTime;	Serial.print("full opening time = "); Serial.print(*ptimeSwitch); Serial.print("ms, errCod = "); Serial.println(errCod);

	//сохраняем текущее время
	startTime = millis();
	//закрываем кран
	DEBUG_PRINTLN_VPC("_Start closing valve");
	valveAction(open, close, signalOpen, signalClose, AC_CLOSE);
	//digitalWrite(open, HIGH); //Serial.println("_35-HIGH");
	//digitalWrite(close, LOW); //Serial.println("_231_36-LOW");

	//ждем появления сигнала закрытия, низкого уровня с крана
	DEBUG_PRINTLN_VPC(F("_waiting for the signal to fully close the valve"));
	do {
		//Serial.println("_in the Do. Waiting for the signal to fully close the valve");
		oSignalClose.scanState();
		if ((millis() - startTime) > MAX_TIME_TESTING_VALVE) {
			errCod = errCod + 2;
			break;
		}
	} while (!oSignalClose.flagPress);
	
	//снимем напряжение с крана
	valveAction(open, close, signalOpen, signalClose, AC_EMPTY);
	///digitalWrite(open, HIGH); //Serial.println("_35-HIGH");
	///digitalWrite(close, HIGH);
	//сохраняем время выполнения операции закрытия
	startTime = millis() - startTime; //т.к. startTime больше не понадобится, то сохраним в него рассчитанное время закрытия
	Serial.print(F("full closing time = ")); Serial.print(startTime); Serial.print("ms, errCod = "); Serial.print(errCod); Serial.print("\n");

	//вернем кран в исходное положение(которое было до начала теста)
	//открываем кран до первоначального положения

	unsigned long delayToOriginalPosition=0;
	if (startTime > *ptimeSwitch) {  // проверка. Иногда бывает, что время закрытия меньше времени открытия и тогда разница будет отрицательна, а, т.к. тип беззнаковый, то на выходе получим огромное число.
		delayToOriginalPosition = startTime - *ptimeSwitch; 
	} 
	Serial.print(F("_Delay when rotating the valve to its original position: ")); Serial.println(startTime - *ptimeSwitch);
	valveAction(open, close, signalOpen, signalClose, AC_OPEN, delayToOriginalPosition);

	///digitalWrite(open, LOW); //Serial.println("_35-LOW");
	///digitalWrite(close, HIGH);

	//снимем напряжение с крана
	valveAction(open, close, signalOpen, signalClose, AC_EMPTY);
	///digitalWrite(open, HIGH); //Serial.println("_35-HIGH");
	///digitalWrite(close, HIGH);

	//запомним наибольшее время из этих операций
	*ptimeSwitch = (startTime>*ptimeSwitch) ? startTime : *ptimeSwitch;

	DEBUG_PRINTLN_VPC("testValve: stop");
	return errCod;
}

//Регулировка и поддержание оптимальной температуры ТТК (защита от холодной обратки)
int temperatureControlTTK() {
	//DEBUG_PRINTLN_VPC(F("\ntemperatureControlTTK: start"));
	int errCod = 0;
	unsigned long startTime = millis();	
	extern myCycle cycleTempControlTTK;

	const  bool LOG = true; //выводить логи работы процедуры
	//if (LOG) { Serial.print("\nstart (temperatureControlTTK)\n"); }



	//читаем температуру термопары (температура дымовых газов ТТК)
	globalParameters.g_t_flueGases = thermocouple.readCelsius();
	///if (LOG) { Serial.print("Дым - "); Serial.println(globalParameters.g_t_flueGases); }
	
	//условие запуска насоса при топящемся котле: подача ттк > 55 или дым>350 
	//условия запуска насоса при не топящемся котле: подача ТТК > (Низ ТА+1,5С)

	/*  [3] - подача ттк,	[11]-низ ТА*/ 
	//if (LOG) { Serial.print("Подача ТТК "); Serial.print(temperature[3]); Serial.print(" Низ ТА "); Serial.println(temperature[11]); }
	
	//контроль запуска насоса ТТК
	switch (systemParameters.TTKPumpMode) {
	case SP_ON:
		//запуск насоса ТТК
		digitalWrite(PIN_PUMP_TTK, LOW); //Serial.println(F("Pump TTK start_29 LOW"));
		break;
	case SP_OFF:
		if (!globalParameters.g_failure) { digitalWrite(PIN_PUMP_TTK, HIGH); /*Serial.println(F("Pump TTK stop_29 HIGH"))*/;} //выключаем насос ТТК
	break;
	case SP_AUTO:
		//Температура выхода из ТТК при активном горении топлива (g_t_flueGases>150)
		int maxTempOutTTK = 0;
		if (temperature[10] > 55.0) {		//Если теплоноситель в верху теплоаккумулятора горячее 55 градусов, то макс температуру на котле даем на два градуса выше 
			maxTempOutTTK = temperature[10] + 2.0;
		}
		else {
			maxTempOutTTK = 55.0;
		}

		if ((globalParameters.g_failure) || (globalParameters.g_t_flueGases > 350) ||(globalParameters.g_t_flueGases<150 && (temperature[3] > (temperature[11] + 1.5))) || (globalParameters.g_t_flueGases>=150 && temperature[3] > maxTempOutTTK) ) {
			//запуск насоса ТТК
			digitalWrite(PIN_PUMP_TTK, LOW); //Serial.println(F("Pump TTK start_29 LOW")); //active level - LOW
		}
		else{//остановка насоса ТТК
			if (!globalParameters.g_failure) 
				{ digitalWrite(PIN_PUMP_TTK, HIGH); /*Serial.println(F("Pump TTK stop_29 HIGH"))*/;} //active level - LOW
		}	
		break;
	}

	//режимы перегрева котла
	if ((int)temperature[3] > systemParameters.tDangerTTK ) { //предаварийный перегрев котла
		Serial.println(F("Предаварийный перегрев котла!!"));
		if ((int)temperature[3] > systemParameters.tCrashTTK) { //аварийный перегрев котла
			 Serial.println("Аварийный перегрев котла!!");
			//включаем зуммер сирены
			//tone(PIN_EMERGENCY_SIREN, 1, 5000); //если динамик без генератора, то даем ему меандр, если со зумером, то просто даем на выход уровень для включения
			digitalWrite(PIN_EMERGENCY_SIREN, LOW); //активный уровень LOW
			//noTone(PIN_EMERGENCY_SIREN);
			globalParameters.g_failure = true;
		}
		//полностью открываем кран котла ТТК
#if TTK_VALVE_EXISTS
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
#endif
		//запуск насосов ТТК и системы
		pumpAction(PIN_PUMP_TTK, AC_ON);
		//digitalWrite(PIN_PUMP_TTK, LOW);
		///digitalWrite(PIN_PUMP_SYS, LOW); //В теплоаккумуляторе всегда прохладная вода внизу и этого должно бы хватить для остужения перегретого котла.
		////return errCod;
		goto lblExit;
	}
	globalParameters.g_failure = false; /*noTone(PIN_EMERGENCY_SIREN);*/ digitalWrite(PIN_EMERGENCY_SIREN, HIGH); //Выключаем сирену.активный уровень LOW

#if TTK_VALVE_EXISTS
	/*t[4] -обратка ТТК, t[3] - подача ТТК*/ //if (LOG) { Serial.print("Обратка ТТК "); Serial.print(temperature[4]); Serial.print(" Подача ТТК "); Serial.println(temperature[3]);}
	if (systemParameters.TTKTempControlMode== SP_MIALG) {//поддерживаем температуру обратки котла PID регулятором
		if (LOG) { Serial.println("Используется PID регулировка крана TTK"); }
		int16_t regulatoryStep = 0; 
		//P=1, I=7, D=50, DedZone = 1.5
		regulatoryStep = PID_3WayValve(tneedTTK, temperature[3], (cycleTempControlTTK.period()/1000.0), (g_timeSwitchValveTTK / 1000.0), (1.5), (3.0), (5.0), (100.0));
		
			Serial.print("regulatoryStep ");
			Serial.println(regulatoryStep);
			Serial.print("PIN_VALVE_TTK_SIGNAL_OPEN ");
			Serial.println(digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN));
			Serial.print("PIN_VALVE_TTK_SIGNAL_ClOSE ");
			Serial.println(digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE));

		if (LOG) { Serial.print("regulatoryStep_ТТК  "); Serial.println(regulatoryStep); }
		if (regulatoryStep < 0 && (digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE))) {
			//температура в подаче котла выше заданной
			//Уменьшаем подачу тепла в ТА, приоткрываем боковой отвод крана
			digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);//active level - LOW
			digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
			delay(-(regulatoryStep ));
			digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
			digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		}
		else if ((regulatoryStep > 0) && (digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN))) {
			//температура в подаче котла ниже заданной
			//призакрываем боковой отвод
			digitalWrite(PIN_VALVE_TTK_OPEN, LOW);//active level - LOW
			digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
			delay((regulatoryStep ));
			digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
			digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		}
	}
	else {//моя программа регулировки постоянными импульсами  
		if (LOG) { Serial.println("Используется простая не PID регулировка крана TTK"); }
		//if (((int)temperature[4] > (tneedTTK + 2)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE))) {// обратка горячая призакрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
		//	digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);//active level - LOW
		//	digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//	delay((g_timeSwitchValveTTK) / 10);
		//	digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
		//	digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//}
		//else {
		//	if (((int)temperature[4] < (tneedTTK - 2)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN))) {// обратка холодная приоткрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
		//		digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);//active level - LOW
		//		digitalWrite(PIN_VALVE_TTK_OPEN, LOW);
		//		delay(g_timeSwitchValveTTK / 10);
		//		digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
		//		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//	}
		//}
	}
#endif

lblExit:
	// перезапуск таймера вызова функции регулировки температуры.
	//extern myCycle cycleTempControlTTK;
	cycleTempControlTTK.clear();
	cycleTempControlTTK.reStart();

	//DEBUG_PRINTLN_VPC(F("temperatureControlTTK: stop"));; Serial.print((millis() - startTime)); Serial.println(" mc");
	return errCod;
};

//Регулировка и поддержание оптимальной температуры системы
int temperatureControlSYS() {
	DEBUG_PRINTLN_VPC(F("\ntemperatureControlSYS: start"));
	//yield()!!!!
	const  bool LOG = true; //выводить логи работы процедуры
	if (LOG) { Serial.println(F("\nstart (temperatureControlSYS)\n")); }
	int errCod = 0;
	unsigned long startTime = millis();
	extern float temperature[];

	//Проверим время и если сейчас ночь и действует ночной тариф, то включим на небольшое время электрокотел.
	//при этом расход теплоносителя из ТА остановим (этот режим под вопросом? может работу насоса системы лучше не останавливать???)
			//процедура включения электрокотла на ночь//////	if (flagEKOn && g_systemDateTime.hour() == 5 && g_systemDateTime.minute()<30) { //греемся ЭК c 5 до 5:30 часов утра
			////////		if (LOG) {Serial.print(F("Запуск электрокотла. Время ")); Serial.println(g_systemDateTime.hour());	}
			////////		//Останавливаем насос системы
			////////		digitalWrite(PIN_PUMP_SYS, HIGH); Serial.println(F("Pump SYS stop 30 LOW"));//active level - LOW
			////////		//закрываем кран подачи теплоносителя из ТА в систему.
			////////		/*digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
			////////		digitalWrite(PIN_VALVE_SYS_OPEN, LOW);//active level - LOW*/
			////////		//Запускаем насос электрокотла и сам электрокотел
			////////		digitalWrite(PIN_CONTROL_EK, LOW); //active level - LOW
			////////		digitalWrite(PIN_PUMP_EK, LOW); //active level - LOW
			////////		goto lblExit; //выходим, далее за температурой в системе не следим.
			////////	}
			////////else {
			////////	if (LOG) { Serial.print(F("Выключение электрокотла. Время ")); Serial.println(g_systemDateTime.hour()); }
			////////	//Останавливаем ЭК
			////////	digitalWrite(PIN_CONTROL_EK, HIGH); //active level - LOW
			////////	delay(4000); //что бы остыл котел
			////////	//Останавливаем насос ЭК
			////////	digitalWrite(PIN_PUMP_EK, HIGH); //active level - LOW
			////////	//снимаем питание с крана системы
			////////	digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
			////////	digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
			////////}

	//Управление насосом системы
	//контроль режима запуска насоса системы
	switch (systemParameters.SystemPumpMode) {
	case SP_ON:
		//запуск насоса системы
		pumpAction(PIN_PUMP_SYS, AC_ON); //Serial.println(F("Pump SYS start, 30 LOW"));
		break;
	case SP_OFF:
		if (!globalParameters.g_failure) { pumpAction(PIN_PUMP_SYS, AC_OFF); } //выключаем насос системы
		break;
	case SP_AUTO:
		// 10-t11, верх ТА	// 12-t13, температура в помещении
		if (globalParameters.g_failure || (temperature[10] > temperature[12])) {
			//Температура верха ТА выше температуры в отслеживаемом помещении
			//запуск насоса системы
			if (LOG) { Serial.print(F("SYStem pump start")); }
			pumpAction(PIN_PUMP_SYS, AC_ON); //Serial.println(F("Pump SYS start 30 LOW"));
		}
		else {
			//остановка насоса системы
			pumpAction(PIN_PUMP_SYS, AC_OFF);
			if (LOG) { Serial.print(F("stop nasos SYS")); }
			//return errCod;
			goto lblExit;
		}
		break;
	}

	//определяем целевую температуру для регулировки
	float tRoomSetpoint = systemParameters.RoomSetPointTemperature;
	if (systemParameters.onSchedule) {
		switch (globalParameters.g_systemDateTime.hour()) {
		case 0:
			tRoomSetpoint *= dailySheduleOfTemperature[0];
			break;
		case 1:
			tRoomSetpoint *= dailySheduleOfTemperature[1];
			break;
		case 2:
			tRoomSetpoint *= dailySheduleOfTemperature[2];
			break;
		case 3:
			tRoomSetpoint *= dailySheduleOfTemperature[3];
			break;
		case 4:
			tRoomSetpoint *= dailySheduleOfTemperature[4];
			break;
		case 5:
			tRoomSetpoint *= dailySheduleOfTemperature[5];
			break;
		case 6:
			tRoomSetpoint *= dailySheduleOfTemperature[6];
			break;
		case 7:
			tRoomSetpoint *= dailySheduleOfTemperature[7];
			break;
		case 8:
			tRoomSetpoint *= dailySheduleOfTemperature[8];
			break;
		case 9:
			tRoomSetpoint *= dailySheduleOfTemperature[9];
			break;
		case 10:
			tRoomSetpoint *= dailySheduleOfTemperature[10];
			break;
		case 11:
			tRoomSetpoint *= dailySheduleOfTemperature[11];
			break;
		case 12:
			tRoomSetpoint *= dailySheduleOfTemperature[12];
			break;
		case 13:
			tRoomSetpoint *= dailySheduleOfTemperature[13];
			break;
		case 14:
			tRoomSetpoint *= dailySheduleOfTemperature[14];
			break;
		case 15:
			tRoomSetpoint *= dailySheduleOfTemperature[15];
			break;
		case 16:
			tRoomSetpoint *= dailySheduleOfTemperature[16];
			break;
		case 17:
			tRoomSetpoint *= dailySheduleOfTemperature[17];
			break;
		case 18:
			tRoomSetpoint *= dailySheduleOfTemperature[18];
			break;
		case 19:
			tRoomSetpoint *= dailySheduleOfTemperature[19];
			break;
		case 20:
			tRoomSetpoint *= dailySheduleOfTemperature[20];
			break;
		case 21:
			tRoomSetpoint *= dailySheduleOfTemperature[21];
			break;
		case 22:
			tRoomSetpoint *= dailySheduleOfTemperature[22];
			break;
		case 23:
			tRoomSetpoint *= dailySheduleOfTemperature[23];
			break;
		}
	}
	globalParameters.RoomSetPointTempatureCurrent = tRoomSetpoint;
	////Serial.print("Podacha TTK"); Serial.println(temperature[3]);
	////Serial.print("Obratka TTK"); Serial.println(temperature[4]);
	////Serial.print("Verh TA"); Serial.println(temperature[10]);
	////Serial.print("Niz TA"); Serial.println(temperature[11]);
	////Serial.print("Podacha SYS"); Serial.println(temperature[0]);
	////Serial.print("Obratka SYS"); Serial.println(temperature[1]);
	////Serial.print("Pomeschenie"); Serial.println(temperature[12]);
	////Serial.print("\n");

	if (LOG) { Serial.print(F("Начало процедуры регулирования. Текущая целевая температура с учетом расписания tRoomSetpoint: ")); Serial.println(tRoomSetpoint); }

	//t[12] - температура в помещении.			
	if (systemParameters.SysTempControlMode == SP_PID) {
		//!!!процедура PID регулирования не отлажена и не работает!!!
		// /PID?T=41.0&C=60 kP=1.0&kI=4.0&kD=4.0
		DEBUG_PRINTLN_VPC(F("\ntemperatureControlSYS: PID Регулятор включен"));
		//поддерживаем температуру PID регулятором
		int regulatoryStep = 0;
		//P=1, I=8, D=200
		//temperature[12]-температра в Зале
		//temperature[2]-температра в большой спальне
		//temperature[0]-температра в подаче системы
		Serial.println("PID_парам: Т, С, tP, tI, tD: " + String(systemParameters.pid_set_value)+ systemParameters.pid_cycleS+ systemParameters.pid_kP+ systemParameters.pid_kI+ systemParameters.pid_kD);
		regulatoryStep = PID_3WayValve(/*tRoomSetpoint*/systemParameters.pid_set_value, temperature[0], (systemParameters.pid_cycleS), (globalParameters.g_timeSwitchValveSYS / 1000.0), (0.2), (systemParameters.pid_kP), (systemParameters.pid_kI), (systemParameters.pid_kD));
		Serial.println("PID_regulatoryStep: " + String(regulatoryStep));
		if (regulatoryStep < 0 && (digitalRead(PIN_VALVE_SYS_SIGNAL_ClOSE))) {
			//температура в контролируемом помещении выше заданной, призакрываем кран
			valveAction(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, AC_CLOSE, -regulatoryStep*1000);
		}
		else if ((regulatoryStep > 0) && (digitalRead(PIN_VALVE_SYS_SIGNAL_ClOSE))) {
			//температура в контролируемом помещении ниже заданной/ Даем тепло в систему, приоткрываем кран
			valveAction(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, AC_OPEN, regulatoryStep * 1000);
		}
	}
	else {
		//моя программа регулировки постоянными импульсами  
		//t[12] - температура в помещении.	
		//new:дополнительно контролируем, что бы температура в системе не опускалась ниже определенного уровня, 
		// что бы в доме не появлялся холод от окон
		//float mTminSysPodacha - минимальная температура подачи системы
		// [13]-t14, температура на улице
		if (temperature[13] < 0) { systemParameters.TminSysPodacha = 36.6; } //36,  35, вариант для зимы)	//33было жарко в доме
		if (temperature[13] < -10) { systemParameters.TminSysPodacha = 37; } //37,  36, вариант для зимы)	//34 жарковато в доме. 35 в большой спальне было жарко ночью
		if (temperature[13] < -20) { systemParameters.TminSysPodacha = 37; } //37,  вариант для зимы) //-24 с окон сифонит. 36 не жарко, хочется теплее. температура в зале за час упала на градус.
		if (temperature[13] < -25) { systemParameters.TminSysPodacha = 38; } //38, вариант для зимы)	//37, 
		if (temperature[13] < -30) { systemParameters.TminSysPodacha = 39; } //39, вариант для зимы)	//38, 
		if (temperature[13] < -35) { systemParameters.TminSysPodacha = 40; }	//40,  
		if ((temperature[13] > 0) && ((temperature[12] < tRoomSetpoint + 1.5))) { systemParameters.TminSysPodacha = 35; } //, если на улице холодно, но в помещении на 1,5 градуса выше заданной, то в батарее держим минимальную темпратуру пониже.
		if ((temperature[13] > 0) && ((temperature[12] > tRoomSetpoint + 1.5))) { systemParameters.TminSysPodacha = 34; }
		if ((temperature[13] > 5) && ((temperature[12] < tRoomSetpoint + 1.5))) { systemParameters.TminSysPodacha = 32; }
		if ((temperature[13] > 5) && ((temperature[12] > tRoomSetpoint + 1.5))) { systemParameters.TminSysPodacha = 29; }
		if (temperature[13] > 10) { systemParameters.TminSysPodacha = 26; }
		if (temperature[13] > 15) { systemParameters.TminSysPodacha = 23; }
		Serial.println(String("Минимальная температура подачи: ") + systemParameters.TminSysPodacha);

		///Serial.println(String("T[0] ") + temperature[0]); Serial.println(String("mTminSysPodacha ") + mTminSysPodacha);

		//Нижняя граница диапазона минимальной температуры подачи
		bool TminSysPodachaLowerBound = (temperature[0] < (systemParameters.TminSysPodacha - 0.2));
		///Serial.println(String("TminSysPodachaLowerBound ") + TminSysPodachaLowerBound);

		//Верхняя граница диапазона минимальной температуры подачи
		bool TminSysPodachaUpperBound = (temperature[0] > (systemParameters.TminSysPodacha + 1.0));
		///Serial.println(String("TminSysPodachaUpperBound ") + TminSysPodachaUpperBound);

		if (((temperature[12] < tRoomSetpoint - 0.1) || TminSysPodachaLowerBound) && (digitalRead(PIN_VALVE_SYS_SIGNAL_OPEN))) {
			///Serial.print(String("Увеличим подачу на "));
			// температура в контролируемом помещении ниже заданной. 
			//приоткрываем трехходовой кран.Если задача поддержать минимальную темературу в системе, то шаги крана сделаем в два раза уже, чем при регулировании для поддержания температуры в помещении
			long duration = 0;
			if (TminSysPodachaLowerBound) {
				duration = ((globalParameters.g_timeSwitchValveSYS) / 11);
				///Serial.println(String("1/11 шаг"));
			}
			else {
				duration = ((globalParameters.g_timeSwitchValveSYS) / 10);
				///Serial.println(String("1/10 шаг"));
			}
			///Serial.print(String("Длительность открытия ")+String(globalParameters.g_timeSwitchValveSYS));
			///Serial.print(String("Расчитанная длительность  ") + String(duration));
			valveAction(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, AC_OPEN, duration);
		}
		else {
			if ((temperature[12] > (tRoomSetpoint /*+ 0.1*/) && (TminSysPodachaUpperBound)) && (digitalRead(PIN_VALVE_SYS_SIGNAL_ClOSE))) {
				///Serial.print(String("Уменьшим подачу на "));
				// температура в контролируемом помещении выше заданной. Приоткрываем кран(боковой отвод) на 1/25 максимального времени переключения крана системы
				long duration = 0;
				if (temperature[0] > (systemParameters.TminSysPodacha + 3.0)) {

					duration = ((globalParameters.g_timeSwitchValveSYS) / 10);
					///Serial.println(String("1/10 шаг (температура подачи выше минимально-допустимой на +3 градуса)"));
				}
				else {
					duration = ((globalParameters.g_timeSwitchValveSYS) / 11);
					///Serial.println(String("1/11 шаг"));
				}
				valveAction(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, AC_CLOSE, duration);
			}
		}
	}
lblExit:
	// перезапуск таймера вызова функции регулировки температуры в системе.
	extern myCycle cycleTempControlSYS;
	cycleTempControlSYS.clear();
	cycleTempControlSYS.reStart();
	if (LOG) { Serial.print(F("stop (temperatureControlSYS) ")); Serial.print(millis() - startTime); Serial.println(F("mc")); }
	return errCod;
}


//PID регулятор
int16_t PID_3WayValve(float set_value, float present_value, float cycleS, float valveS, float dead_zone, float k_P = 1, float k_I = 4, float k_D = 4) {{
	//PID regulator
	float set_value; //уставка, в измеряемых единицах (°С)
	float present_value; //текущее значение, в измеряемых единицах (°С)
	float cycleS; //время цикла регулятора, сек
	float valveS; //время полного переключения привода, сек
	float k_P; //коэффициент П, %/измеряемые единицы (%/°С)
	float k_I; //коэффициент И, сек
	float k_D;//коэффициент Д, сек
	float dead_zone;//зона нечувствительности, измеряемые единицы (°С)
	
		//формула ПИД:
		//  ΔYn = Kp·([En]-[En-1]+Tc/Ti·[En-1]+Td/Tc·([En]-2[En-1]+[En-2])), где
		//  Tc - время квантования, время цикла, цикл регулирования, цикл приращения
		//	ΔYn - приращение на цикле, приращение управляющего воздействия
		//	Yn - управляющее воздействие;
		//  Yn = [Yn-1]+ΔYn
		//	En - ошибка регулирования на текущем шаге;
		//	[En-1] - ошибка регулирования на предыдущем шаге;
		//	Ti - время интегрирования, сек;
		//	Td - время дифференцирования, сек;
		//	En = SV - PV, где
		//	SV - заданное значение контролируемого параметра;
		//	PV - текущее значение контролируемого параметра.
		// для трехходового крана: 
		// (ΔYn·Tпривода)/100%
		// если >0, то открываем
		// если <0, то закрываем

	const  bool LOG = true; //выводить логи работы процедуры
	if (LOG) { Serial.print("\nstart (PID_3WayValve) "); Serial.println(globalParameters.g_systemDateTime.timestamp(globalParameters.g_systemDateTime.TIMESTAMP_TIME)); }

	float e_1; // Текущее рассогласование
	static float e_2; // Рассогласование на - 1 шаге
	static float e_3; // Рассогласование на - 2 шага
	float d_t; // Время воздействия на текущем шагу регулирования
	static float sum_d_t; // Накопленное время воздействия

	e_1 = set_value - present_value; // Текущее рассогласование измеряемых единиц
	if (k_I == 0.0) { k_I = 9999.0; } // Деление на 0, выключение Интегральной составляющей

	// Ограничения
	k_P = constrain(k_P, (-99.0), (99.0)); // Кр -99.0...99.0 %/измеряемых единиц, знак + для нагревателя, знак - для холодильника
	k_I = constrain(k_I, (1.0), (9999.0)); // Тi 1...9999.0 сек
	k_D = constrain(k_D, (0.0), (9999.0)); // Тd 0...9999.0 сек
	cycleS = constrain(cycleS, (1.0), (18000.0)); // Цикл до 5 минут
	valveS = constrain(valveS, (3.0), (250.0)); // Время привода 3...250.0 сек
	dead_zone = constrain(dead_zone, (0.0), (9999.0)); // Зона нечувствительности 0...9999.0 измеряемых единиц

	if (LOG) { 
		Serial.print("set_value ");
		Serial.println(set_value);
		Serial.print("present_value ");
		Serial.println(present_value);


		Serial.print("k_P ");
		Serial.println(k_P);
		Serial.print("k_I ");
		Serial.println(k_I);
		Serial.print("k_D ");
		Serial.println(k_D);
		Serial.print("cycleS ");
		Serial.println(cycleS);
		//Serial.print("valveS ");
		//Serial.println(valveS);
		Serial.print("dead_zone ");
		Serial.println(dead_zone);
		Serial.print("e_1 Дельта ");
		Serial.println(e_1);
		Serial.print("e_2 ");
		Serial.println(e_2);
		Serial.print("e_3 ");
		Serial.println(e_3);
	}

	if (abs(e_1) > dead_zone) {// отклонение больше зоны нечувствительности
		//расчитываем приращение управляющего воздействия на этом цикле
		d_t = k_P * (e_1 - e_2 + cycleS * e_2 / k_I + k_D * (e_1 - 2 * e_2 + e_3) / cycleS) * valveS / 100.0; // Время воздействия на текущем шагу регулирования       
		////e_3 = e_2; // Запись рассогласования -2 шага назад
		////e_2 = e_1; // Запись рассогласования -1 шаг назад
	}
	else {
		d_t = 0.0;
	}
	e_3 = e_2; // Запись рассогласования -2 шага назад
	e_2 = e_1; // Запись рассогласования -1 шаг назад

	if (LOG) {
		Serial.print("d_t ");
		Serial.println(d_t);
	}

	//расчитываем суммарное воздействие на исполнительный механизм с учетом воздействия с предыдущего шага.
	/*sum_d_t = sum_d_t + d_t;*/  //для крана нет необходимости сумировать с величиной воздействия на предыдущем шаге, т.к. под действием предыдущего воздействия кран повернулся и зафиксировался в том положении, т.е. сохранил предыдущее воздействие в своем положении

	//Ограничиваем время максимального воздействия на кран его максимальными измеренными при инициализации значением переключения
	d_t= constrain(d_t, (-valveS), (valveS));
	if (LOG) {
		Serial.print("d_t ");
		Serial.println((long)(d_t*1000));}

//не производим переключения крана, если время воздействиия слишком коротко, а накапливаем результат
	//if ((sum_d_t += d_t) < (valveS / 10)) {
	//	return 0;
	//}
	//else {
	//	return (int)(sum_d_t ); //вернем время в секундах!
	//}
	if (abs(d_t) < (valveS / 10)) {
		return 0;
	}
	else {
		return (d_t * 1000); //вернем время 
		//return -10;
	}
	//
	//if (d_t < (valveS / 10)) {  
	//	if ((sum_d_t += d_t) < (valveS / 10)) {
	//		return 0;
	//	}
	//	else {
	//		float ret = sum_d_t+ d_t;
	//		sum_d_t = 0;
	//		return (int)(sum_d_t * 1000); //вернем время в миллисекундах
	//	}
	//	return (int)(d_t * 1000); //вернем время в миллисекундах
	}
}

// Элементарные функции управления насосами и кранами, электрокотлом и приводом поддувальной дверки
void pumpAction(int controlPin, enumAction action){
	//active level pin LOW
	switch (action) {
	case AC_ON:
		digitalWrite(controlPin, LOW);
		break;
	case AC_OFF:
		digitalWrite(controlPin, HIGH);
		break;
	}
}

void EKAction(int controlPin, enumAction action) {
	pumpAction(controlPin, action);
}

//процедура запускаети начало определенного действия у крана, а вот завершение действия будет происходить уже через прерывание по таймеру.
void valveAction(int openPin, int closePin, int signalOpen, int signalClose, enumAction action, long duration){
	//Serial.println("\n\n Зашли в процедуру valveAction(). актион: "+String(action)+". Длительность: " +String(duration));
	//зададим переменные, которые будут использоваться в процедуре tmrStopActionForSysValve, вызываемой по сработке внутреннего таймера timer0 (генерируется прерывание).
	m_openPinSYS = openPin;
	m_closePinSYS = closePin;
	//active level pin LOW
	switch (action) {
	case AC_EMPTY: //напряжение на кран не подается
		digitalWrite(openPin, HIGH);
		digitalWrite(closePin, HIGH);
		break;
	case AC_OPEN:
		digitalWrite(closePin, HIGH);
		digitalWrite(openPin, LOW);
		//Запускаем таймер по системному таймеру Timer0 на время duration (в секундах)
		if (TimerList.isActive(hCounterStopActionForValve)) { // проверяем, если такой счетчик уже существует и он запущен, то обновим его период срабатывания в соответствии с новой командой.
			//hCounterStopActionForValve = TimerList.Add(duration , tmrStopActionForSysValve);
			TimerList.setNewInterval(hCounterStopActionForValve, duration );
		}
		else {
			hCounterStopActionForValve = TimerList.Add(duration, tmrStopActionForSysValve);
		}
		break;
	case AC_CLOSE:
		digitalWrite(openPin, HIGH);
		digitalWrite(closePin, LOW);
		//Запускаем таймер по системному таймеру Timer0 на время duration (в секундах)
		if (TimerList.isActive(hCounterStopActionForValve)) { // проверяем, если такой счетчик уже существует и он запущен, то обновим его период срабатывания в соответствии с новой командой.
		hCounterStopActionForValve = TimerList.Add(duration, tmrStopActionForSysValve);
		TimerList.setNewInterval(hCounterStopActionForValve, duration );
		}
		else {
			hCounterStopActionForValve = TimerList.Add(duration, tmrStopActionForSysValve);
		}
		break;
	}
}

void tmrStopActionForSysValve(void) {
	///Serial.println("\n\n Зашли в процедуру tmrStopActionForSysValve()");
	///Serial.println(m_openPinSYS);
	///Serial.println(m_closePinSYS);

	//при сработке таймера снимаем напряжение с крана и прекращаем действие
	digitalWrite(m_openPinSYS, HIGH);
	digitalWrite(m_closePinSYS, HIGH);
	//останавливаем и удаляем таймер
	TimerList.Delete(hCounterStopActionForValve);
}

//Процедура управления дверкой поддувала
void doorAirControl() {
	DEBUG_PRINTLN_VPC(F("\ndoorAirControl: start"));

	unsigned long startTime = millis();
	extern float temperature[];
	
	//Управление насосом системы
	//
	switch (systemParameters.SystemPumpMode) {
	case SP_ON:
		//запуск насоса системы
		pumpAction(PIN_PUMP_SYS, AC_ON); //Serial.println(F("Pump SYS start, 30 LOW"));
		break;
	case SP_OFF:
		if (!globalParameters.g_failure) { pumpAction(PIN_PUMP_SYS, AC_OFF); } //выключаем насос системы
		break;
	case SP_AUTO:
		// 10-t11, верх ТА	// 12-t13, температура в помещении
		if (globalParameters.g_failure || (temperature[10] > temperature[12])) {
			//Температура верха ТА выше температуры в отслеживаемом помещении
			//запуск насоса системы
			if (LOG) { Serial.print(F("SYStem pump start")); }
			pumpAction(PIN_PUMP_SYS, AC_ON); //Serial.println(F("Pump SYS start 30 LOW"));
		}
		else {
			//остановка насоса системы
			pumpAction(PIN_PUMP_SYS, AC_OFF);
			if (LOG) { Serial.print(F("stop nasos SYS")); }
			//return errCod;
			goto lblExit;
		}
		break;
	}

	//определяем целевую температуру для регулировки
	float tRoomSetpoint = systemParameters.RoomSetPointTemperature;
	if (systemParameters.onSchedule) {



}