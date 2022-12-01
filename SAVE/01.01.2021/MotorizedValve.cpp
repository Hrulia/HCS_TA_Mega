// 
// 
// 

#include "Motorizedvalve.h"


// 0- t1, подача cистемы
// 1- t2, обратка системы
// 2- t3, отвод трехходового крана ТТ
// 3- t4, подача ТТК
// 4- t5, обратка ТТК
// 5- t6, верх ТА подача от ТТК
// 6- t7, низ ТА обратка ТТК
// 7- t8, верх ТА подача в систему
// 8- t9, отвод трехходового крана системы
// 9- t10, низ ТА, обратка системы
// 10-t11, верх ТА
// 11-t12, низ ТА
// 12-t13, температура в помещении
// 13-t14, температура на улице
// 14-t15, подача ЭК 
// 15-t16, обратка ЭК


#define PID_TTK 0 //поддержание температуры ТТК по средствам ПИД регулятора
#define PID_SYS 0 //поддержание температуры SYS по средствам ПИД регулятора

const int tcrashTTK = 80;	//аварийная температура ТТК(90 *С)
const int tdangerTTK = 70;	//предаварийная температура  ТТК (80 *С)
const int tneedTTK = 40;	//требуемая температура обратки ТТК(60 * С)

///int t_flueGas = 200; //температура дымовых газов ТТК котла ()  

/////*uint8_t*/float g_tRoomSetpoint = 20; //заданное значение температуры помещения (к которому стремимся)
bool g_onSchedule=true; //флаг, устанавливающий необходимость поддержания температуры по суточному графику

MAX6675_Thermocouple thermocouple(PIN_THERMOCOUPLE_CLK, PIN_THERMOCOUPLE_CS, PIN_THERMOCOUPLE_DO); // Объект термопара (температура дымовых газов)

//Расписание температуры на сутки
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
//0.956 };//23-24
float dailySheduleOfTemperature[24] = {
	0.869, //0-1 (20/23)
	0.783, //1-2
	0.783, //2-3
	0.783, //3-4
	0.783, //4-5 (18/23)
	0.826, //5-6 (19/23)
	0.913, //6-7 (21/23)
	1.000, //7-8 (23/23)
	0.956, //8-9 (22/23)
	0.956, //9-10
	0.913, //10-11
	0.913, //11-12
	0.956, //12-13
	0.913, //13-14
	0.913, //14-15
	0.913, //15-16
	0.913, //16-17
	1.000, //17-18
	1.000, //18-19
	0.956, //19-20
	1.000, //20-21
	1.000, //21-22
	0.956, //22-23
	0.956 };//23-24

//вызов функции тестирования для крана котла и крана системы
int initValve() {
	LOG("\nStart (initvalve)");
	int result = 0; //коды завершения вызываемых функций

	//настройка пинов для моторизированных кранов (открыт - значит поток направлен в боковой отвод)
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

	//тестируем работу крана защиты обратки ТТК
	LOG("\nCall testValveTTK\n");
	result = testValve(PIN_VALVE_TTK_OPEN, PIN_VALVE_TTK_ClOSE, PIN_VALVE_TTK_SIGNAL_OPEN, PIN_VALVE_TTK_SIGNAL_ClOSE, &g_timeSwitchValveTTK);
	if (result) {
		LOG("Error in... "); LOG(result); LOG("\n");
	}
	Serial.print("g_timeSwitchvalveTTK "); Serial.print(g_timeSwitchValveTTK); Serial.print(" ms\n");

	//тестируем работу крана регулировки темпратуры системы
	LOG("\nCall testValveSYS\n");//PIN_VALVE_SYS_OPEN
	//result = testvalve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &g_timeSwitchValveSYS);
	result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &g_timeSwitchValveSYS);


	if (result) {
		LOG("Error in... "); LOG(result); ; LOG("\n");  //обработка ошибки
	}
	Serial.print("g_timeSwitchvalveSYS "); Serial.print(g_timeSwitchValveSYS); Serial.print("ms\n");
		
	LOG("stop (initTemp)\n");
	return result;
}

//тестирование моторизированных кранов. определение времени переключения
int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long * ptimeSwitch)
{
	/*
	return errors code:
	1 - ошибка теста открытия крана
	2 - ошибка теста закрытия крана
	3 - ошибка обоих тестов
	*/
	LOG("Start (testValve)\n");

	int errCod = 0;
	Button oSignalOpen(signalOpen, 15); // класс button позволяет фильтровать дребезг контактов с сигнальных концевиков крана. время фильтрации 15мс*на период повторения loop. 
	Button oSignalClose(signalClose, 15);

	//сохраняем текущее время
	unsigned long startTime = millis();
	//открываем кран
	digitalWrite(open, LOW);
	digitalWrite(close, HIGH);
	///LOG(digitalRead(open));	LOG(digitalRead(close));	LOG(digitalRead(signalOpen));	LOG(digitalRead(signalClose));
	//ждем появления сигнала открытия низкого уровня
	boolean a;

	///LOG("Flagpress"); LOG(oSignalOpen.flagPress); LOG("\n");
	do {
		oSignalOpen.scanState();
		if ((millis() - startTime) > MAX_TIME_TESTING_VALVE) {
			errCod = 1;
			break;
		}
		//a = oSignalOpen.flagPress; LOG(a);
	} while (!oSignalOpen.flagPress);
	/* первый вариант без класса button
	while (digitalRead(signalOpen)!=LOW) {
	if ((millis() - startTime) > MAX_TIME_TESTING_VALVE) {
	errCod = 1;
	break;
	}
	}
	*/
	//снимем напряжение с крана
	digitalWrite(open, HIGH);
	digitalWrite(close, HIGH);
	//сохраняем время выполнения операции открытия
	*ptimeSwitch = millis() - startTime;	Serial.print("full opening time = "); Serial.print(*ptimeSwitch); Serial.print("ms, errCod = "); Serial.println(errCod);

	//сохраняем текущее время
	startTime = millis();
	//закрываем кран
	digitalWrite(close, LOW);
	digitalWrite(open, HIGH);
	//ждем появления сигнала закрытия, низкого уровня с крана
	//LOG("FlagpressClose"); LOG(oSignalOpen.flagPress); LOG("\n"); oSignalOpen.flagPress = false;
	//LOG("FlagpressClose"); LOG(oSignalOpen.flagPress); LOG("\n");
	do {
		oSignalClose.scanState();
		if ((millis() - startTime) > MAX_TIME_TESTING_VALVE) {
			errCod = errCod + 2;
			break;
		}
	} while (!oSignalClose.flagPress);
	///LOG("oSignalClose.flagPress = "); LOG(oSignalClose.flagPress);
	/* первый вариант без класса button
	while (digitalRead(signalClose)) {
	LOG(digitalRead(signalClose));

	//delay(1);  //????непонятное поведение: чаще всего цикл вообще не выполняется(проскакивает). Повидимому проскакивает какая-то наводка на сигнальном проводе
	if ((millis() - startTime) > MAX_TIME_TESTING_VALVE) {
	errCod = errCod+2;
	break;
	}
	}
	*/
	//снимем напряжение с крана
	digitalWrite(open, HIGH);
	digitalWrite(close, HIGH);
	//сохраняем время выполнения операции закрытия
	startTime = millis() - startTime; //т.к. startTime больше не понадобится, то сохраним в него рассчитанное время закрытия
	Serial.print("full closing time = "); Serial.print(startTime); Serial.print("ms, errCod = "); Serial.print(errCod); Serial.print("\n");

	//запомним наибольшее время из этих операций
	*ptimeSwitch = (startTime>*ptimeSwitch) ? startTime : *ptimeSwitch;
	///LOG("Itog"); LOG(*ptimeSwitch); LOG("\n");

	LOG("stop (testValve)\n");
	return errCod;
}

//Регулировка и поддержание оптимальной температуры ТТК (защита от холодной обратки)
int temperatureControlTTK() {
	const  bool LOG = false; //выводить логи работы процедуры
	if (LOG) { Serial.print("\nstart (temperatureControlTTK)\n"); }

	int errCod = 0;
	unsigned long startTime = millis();	
	extern float temperature[];			
	extern int g_t_flueGas; //температура дымовых газов ТТК котла ()  
	extern myCycle cycleTempControlTTK;

	//читаем температуру термопары (температура дымовых газов ТТК)
	g_t_flueGas = (int)thermocouple.readCelsius();
	if (LOG) { Serial.print("Дым - "); Serial.println(g_t_flueGas); }
	

	/*[3] - подача ттк,	[11]-низ ТА*/ if (LOG) { Serial.print("Подача ТТК "); Serial.print(temperature[3]); Serial.print(" Низ ТА "); Serial.println(temperature[11]); }
	if (g_failure || (g_t_flueGas > 120) || ((int)temperature[3] > ((int)temperature[11] + 3))) { //авария, или котел топиться, или вода в подаче котла нагрелась более чем на 5 *С выше температуры воды низа ТА

	///*если нет датчика дыма, используем такое условие: */
	//if (g_failure || (temperature[3] > (temperature[11] + 1))) { //котел топиться или вода в подаче котла нагрелась более чем на 2 *С выше температуры воды низа ТА

		//запуск насоса ТТК
		digitalWrite(PIN_PUMP_TTK, LOW); //active level - LOW
	}
	else{//остановка насоса ТТК
		digitalWrite(PIN_PUMP_TTK, HIGH); //active level - LOW
	}

	if ((int)temperature[3] > tdangerTTK ) { //предаварийный перегрев котла
		Serial.println("Предаварийный перегрев котла!!");
		if ((int)temperature[3] > tcrashTTK) { //аварийный перегрев котла
			 Serial.println("Аварийный перегрев котла!!");
			//включаем зуммер сирены
			//tone(PIN_EMERGENCY_SIREN, 1, 5000); //если динамик без генератора, то даем ему меандр, если со зумером, то просто даем на выход уровень для включения
			digitalWrite(PIN_EMERGENCY_SIREN, LOW); //активный уровень LOW
			//noTone(PIN_EMERGENCY_SIREN);
			g_failure = true;
		}
		//полностью закрываем боковой отвод крана котла ТТК
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//запуск насосов ТТК и системы
		digitalWrite(PIN_PUMP_TTK, LOW);
		///digitalWrite(PIN_PUMP_SYS, LOW); //В теплоаккумуляторе всегда прохладная вода внизу и этого должно бы хватить для остужения перегретого котла.
		////return errCod;
		goto lblExit;
	}
	g_failure = false; /*noTone(PIN_EMERGENCY_SIREN);*/ digitalWrite(PIN_EMERGENCY_SIREN, HIGH); //Выключаем сирену.активный уровень LOW
	
	/*t[4] -обратка ТТК, t[3] - подача ТТК*/ if (LOG) { Serial.print("Обратка ТТК "); Serial.print(temperature[4]); Serial.print(" Подача ТТК "); Serial.println(temperature[3]);}
	if (PID_TTK == 1) {//поддерживаем температуру обратки котла PID регулятором
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
		if (((int)temperature[4] > (tneedTTK + 2)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE))) {// обратка горячая призакрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
			digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);//active level - LOW
			digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
			delay((g_timeSwitchValveTTK) / 10);
			digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
			digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		}
		else {
			if (((int)temperature[4] < (tneedTTK - 2)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN))) {// обратка холодная приоткрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_TTK_OPEN, LOW);
				delay(g_timeSwitchValveTTK / 10);
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
				digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
			}
		}
	}
	
lblExit:
	// перезапуск таймера вызова функции регулировки температуры.
	//extern myCycle cycleTempControlTTK;
	cycleTempControlTTK.clear();
	cycleTempControlTTK.reStart();

	if (LOG) { Serial.print("stop (temperatureControlTTK) "); Serial.print((millis() - startTime)); Serial.println(" mc"); }
	return errCod;
};

//Регулировка и поддержание оптимальной температуры системы
int temperatureControlSYS() {
	const  bool LOG = true; //выводить логи работы процедуры
	if (LOG) { Serial.println(F("\nstart (temperatureControlSYS)\n")); }
	int errCod = 0;
	unsigned long startTime = millis();	
	extern float temperature[];

	//Проверим время и если сейчас ночь и действует ночной тариф, то включим на небольшое время электрокотел.
	//при этом расход теплоносителя из ТА остановим (этот режим под вопросом? может работу насоса системы лучше не останавливать???)
	if (g_systemDateTime.hour() == 5) { /*греемся ЭК в течение пятого часа утра*/
		if (LOG) {Serial.print(F("Запуск электрокотла. Время ")); Serial.println(g_systemDateTime.hour());	}
		//Запускаем насос системы
		digitalWrite(PIN_PUMP_SYS, LOW); //active level - LOW
		//закрываем кран подачи теплоносителя из ТА в систему.
		digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
		digitalWrite(PIN_VALVE_SYS_OPEN, LOW);//active level - LOW
		//Запускаем насос электрокотла и сам электрокотел
		digitalWrite(PIN_CONTROL_EK, LOW); //active level - LOW
		digitalWrite(PIN_PUMP_EK, LOW); //active level - LOW
		goto lblExit; //выходим, далее за температурой в системе не следим.
	}
	else {
		if (LOG) { Serial.print(F("Выключение электрокотла. Время ")); Serial.println(g_systemDateTime.hour()); }
		//Останавливаем ЭК
		digitalWrite(PIN_CONTROL_EK, HIGH); //active level - LOW
		delay(4000); //что бы остыл котел
		//Останавливаем насос ЭК
		digitalWrite(PIN_PUMP_EK, HIGH); //active level - LOW
		//снимаем питание с крана системы
		digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
		digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
	}
	
	//Управление насосом системы
	// 10-t11, верх ТА	// 12-t13, температура в помещении
	if (g_failure ||(temperature[10] > (temperature[12]))) { 
		//Температура верха ТА выше температуры в отслеживаемом помещении
		//запуск насоса системы
		if (LOG) { Serial.print(F("zapusk nasosa SYS"));}
		digitalWrite(PIN_PUMP_SYS, LOW); //active level - LOW
	}
	else {
		//остановка насоса системы
		digitalWrite(PIN_PUMP_SYS, HIGH); //active level - LOW
		if (LOG) {Serial.print(F("stop nasos SYS"));}
		//return errCod;
		goto lblExit;
	}

	/*int*/float tRoomSetpoint = g_tRoomSetpoint;
	if (g_onSchedule) {
		switch (g_systemDateTime.hour()) {
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

	////Serial.print("Podacha TTK"); Serial.println(temperature[3]);
	////Serial.print("Obratka TTK"); Serial.println(temperature[4]);
	////Serial.print("Verh TA"); Serial.println(temperature[10]);
	////Serial.print("Niz TA"); Serial.println(temperature[11]);
	////Serial.print("Podacha SYS"); Serial.println(temperature[0]);
	////Serial.print("Obratka SYS"); Serial.println(temperature[1]);
	////Serial.print("Pomeschenie"); Serial.println(temperature[12]);
	////Serial.print("\n");


	if (LOG) {Serial.print(F("Начало процедуры регулирования. Celevaia temperatura ")); Serial.println(tRoomSetpoint);}

	//t[12] - температура в помещении.			
	if (PID_SYS == 1) {
		//!!!процедура PID регулирования не отлажена и не работает!!!
		//поддерживаем температуру PID регулятором
		int regulatoryStep = 0;
		//P=1, I=8, D=200
		regulatoryStep = PID_3WayValve(tRoomSetpoint, temperature[12], (60.0), (g_timeSwitchValveSYS / 1000.0), (0.5), (1.0), (8.0), (200.0));
		if (regulatoryStep < 0 && (digitalRead(PIN_VALVE_SYS_SIGNAL_OPEN))) {
			//температура в контролируемом помещении выше заданной
			//Уменьшаем подачу тепла в систему, приоткрываем боковой отвод крана
			digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
			digitalWrite(PIN_VALVE_SYS_OPEN, LOW);//active level - LOW
			delay(-(regulatoryStep * 1000));
			digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
			digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
		}
		else if ((regulatoryStep > 0) && (digitalRead(PIN_VALVE_SYS_SIGNAL_ClOSE))) {
			//температура в контролируемом помещении ниже заданной
			//Даем тепло в систему, т.е. призакрываем боковой отвод крана
			digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
			digitalWrite(PIN_VALVE_SYS_ClOSE, LOW);//active level - LOW
			delay((regulatoryStep * 1000));
			digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
			digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
		}
	}
	else {
		//моя программа регулировки постоянными импульсами  
		//t[12] - температура в помещении.	
		//new:дополнительно контролируем, что бы температура в системе не опускалась ниже 30 градусов, что бы в доме не появлялся холод от окон
		float mTminSysPodacha = 30; //минимальная температура подачи системы
		if (((temperature[12] < tRoomSetpoint - 0.1)|| (temperature[0] < mTminSysPodacha)) && (digitalRead(PIN_VALVE_SYS_SIGNAL_ClOSE))) {
			////Serial.print("Xolodno greiu (temperature[12] < (tRoomSetpoint-0,6)   "); Serial.println((temperature[12] < (tRoomSetpoint - 0,6)));
			// температура в контролируемом помещении ниже заданной. Призакрываем кран(боковой отвод) на 1/10 максимального времени переключения крана системы
			digitalWrite(PIN_VALVE_SYS_ClOSE, LOW);//active level - LOW
			digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
			delay((g_timeSwitchValveSYS) / 10);
			digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
			digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
		}
		else {
			if ((temperature[12] > (tRoomSetpoint + 0.2)) && (digitalRead(PIN_VALVE_SYS_SIGNAL_OPEN))) {
				////Serial.print("Jarko. open bokovoi (temperature[12] > (tRoomSetpoint+0,6))    "); Serial.println(temperature[12]); Serial.println(tRoomSetpoint); Serial.println((tRoomSetpoint + 0.6));  Serial.println((temperature[12] > (tRoomSetpoint + 0.6)));
				// температура в контролируемом помещении выше заданной. Приоткрываем кран(боковой отвод) на 1/25 максимального времени переключения крана системы
				digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_SYS_OPEN, LOW);
				delay(g_timeSwitchValveSYS / 10);
				digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
				digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
			}
		}
	} //end if выбора режима регулирования подачи из ТА в систему

lblExit:
	// перезапуск таймера вызова функции регулировки температуры в системе.
	extern myCycle cycleTempControlSYS;
	cycleTempControlSYS.clear();
	cycleTempControlSYS.reStart();
	if (LOG) {Serial.print(F("stop (temperatureControlSYS) ")); Serial.print(millis() - startTime); Serial.println(F("mc"));}
	return errCod;
}

//PID регулятор
int16_t PID_3WayValve(float set_value, float present_value, float cycleS, float valveS, float dead_zone, float k_P = 1, float k_I = 4, float k_D = 4) {
	//PID regulator
	//float set_value; //уставка, в измеряемых единицах (°С)
	//float present_value; //текущее значение, в измеряемых единицах (°С)
	//float cycleS; //время цикла регулятора, сек
	//float valveS; //время полного переключения привода, сек
	//float k_P; //коэффициент П, %/измеряемые единицы (%/°С)
	//float k_I; //коэффициент И, сек
	//float k_D;//коэффициент Д, сек
	//float dead_zone;//зона нечувствительности, измеряемые единицы (°С)
	
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
	if (LOG) { Serial.print("\nstart (PID_3WayValve) "); Serial.println(g_systemDateTime.timestamp(g_systemDateTime.TIMESTAMP_TIME)); }

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
	//}

}
