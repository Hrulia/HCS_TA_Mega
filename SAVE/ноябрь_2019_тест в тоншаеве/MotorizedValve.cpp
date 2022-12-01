// 
// 
// 

#include "MotorizedValve.h"

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

const int tcrash = 80;	//аварийная температура ТТК(90 *С)
const int tdanger = 70;	//предаварийная температура  ТТК (80 *С)
const int tneed = 20;	//требуемая температура обратки ТТК(50 * С)

int t_flueGas = 200; //температура дымовых газов ТТК котла ()  

/////*uint8_t*/float g_tRoomSetpoint = 20; //заданное значение температуры помещения (к которому стремимся)
bool g_onSchedule=true; //флаг, устанавливающий необходимость поддержания температуры по суточному графику

float dailySheduleOfTemperature[24] = {
0.826, //0-1
0.826, //1-2
0.826, //2-3
0.826, //3-4
0.913, //4-5
0.956, //5-6
0.956, //6-7
0.956, //7-8
0.913, //8-9
0.913, //9-10
0.913, //10-11
0.913, //11-12
0.913, //12-13
0.913, //13-14
0.913, //14-15
0.913, //15-16
0.956, //16-17
1, //17-18
1, //18-19
1, //19-20
1, //20-21
1, //21-22
0.956, //22-23
0.956 };//23-24
						
int initValve() {

	LOG("Start (initValve)\n");
	int result = 0; //коды завершения вызываемых функций

	//тестируем работу крана защиты обратки ТТК
	LOG("\nCall testValveTTK\n");
	result = testValve(PIN_VALVE_TTK_OPEN, PIN_VALVE_TTK_ClOSE, PIN_VALVE_TTK_SIGNAL_OPEN, PIN_VALVE_TTK_SIGNAL_ClOSE, &g_timeSwitchValveTTK);
	if (result) {
		LOG("Error in... "); LOG(result); LOG("\n");
	}
	LOG("g_timeSwitchValveTTK "); LOG(g_timeSwitchValveTTK); LOG(" ms\n");

	//тестируем работу крана регулировки темпратуры системы
	LOG("\nCall testValveSYS\n");//PIN_VALVE_SYS_OPEN
	//result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &g_timeSwitchValveSYS);
	result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &g_timeSwitchValveSYS);


	if (result) {
		LOG("Error in... "); LOG(result); ; LOG("\n");  //обработка ошибки
	}
	LOG("g_timeSwitchValveSYS "); LOG(g_timeSwitchValveSYS); LOG("ms\n");
		
	LOG("stop (initTemp)\n");
	return result;
}

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
	Button oSignalOpen(signalOpen, 15); // класс button позволяет фильтровать дребезг контактов с сигнальных концевиков крана. время фильтрации 15мс*на период повторения. 
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
	*ptimeSwitch = millis() - startTime;	LOG("full opening time = "); LOG(*ptimeSwitch); LOG("ms, errCod = "); LOG(errCod); LOG("\n");

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
	LOG("full closing time = "); LOG(startTime); LOG("ms, errCod = "); LOG(errCod); LOG("\n");

	//запомним наибольшее время из этих операций
	*ptimeSwitch = (startTime>*ptimeSwitch) ? startTime : *ptimeSwitch;
	///LOG("Itog"); LOG(*ptimeSwitch); LOG("\n");

	LOG("stop (testValve)\n");
	return errCod;
}

//Регулировка и поддержание оптимальной температуры ТТК (защита от холодной обратки)
int temperatureControlTTK() {
	LOG("\nstart (temperatureControlTTK)\n");

	int errCod = 0;
	unsigned long startTime = millis();	
	extern float temperature[];			//[3] - подача ттк,			[11]-низ ТА
	//////if (g_failure || (t_flueGas > 60) || ((int)temperature[3] > ((int)temperature[11] + 10))) { //котел топиться или вода в подаче котла нагрелась более чем на 10 *С выше температуры воды низа ТА
	
		/*пока нет датчика дыма, используем такое условие*/
	if (g_failure || (temperature[3] > (temperature[11] + 1))) { 
		//котел топиться или вода в подаче котла нагрелась более чем на 1 *С выше температуры воды низа ТА
		//запуск насоса ТТК
		digitalWrite(PIN_PUMP_TTK, LOW); //active level - LOW
	}
	else{//остановка насоса ТТК
		digitalWrite(PIN_PUMP_TTK, HIGH); //active level - LOW
	}

	if ((int)temperature[3] > tdanger) { //предаварийный перегрев котла
		if ((int)temperature[3] > tcrash) { //аварийный перегрев котла
			//включаем зуммер сирены
			tone(PIN_EMERGENCY_SIREN, 1000, 300000);
			g_failure = true;
		}
		//полностью закрываем боковой отвод крана котла ТТК
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//запуск насосов ТТК и системы
		digitalWrite(PIN_PUMP_TTK, LOW);
		digitalWrite(PIN_PUMP_SYS, LOW);
		return errCod;
	}
	g_failure = false;
	if (((int)temperature[4] > (tneed + 2)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE))) {// обратка горячая призакрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);//active level - LOW
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		delay((g_timeSwitchValveTTK) / 10);
		digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
	}
	else {
			if (((int)temperature[4] < (tneed -2)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN))) {// обратка холодная приоткрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_TTK_OPEN, LOW);
				delay(g_timeSwitchValveTTK / 10);
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
				digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		}

	}
	// перезапуск таймера вызова функции регулировки температуры.
	extern myCycle cycleTempControlTTK;
	cycleTempControlTTK.clear();
	cycleTempControlTTK.reStart();

	LOG("stop (temperatureControlTTK) "); LOG((millis() - startTime)); LOG(" mc\n");
	return errCod;
};

//Регулировка и поддержание оптимальной температуры системы
int temperatureControlSYS() {
	LOG(F("\nstart (temperatureControlSYS)\n"));
	int errCod = 0;
	unsigned long startTime = millis();	

	extern float temperature[];

	//запуск насоса системы	// 10-t11, верх ТА	// 12-t13, температура в помещении
	if (g_failure ||(temperature[10] > (temperature[12]))) { 
		//Температура верха ТА выше температуры в отслеживаемом помещении
		//запуск насоса системы
		LOG("zapusk nasosa SYS");
		digitalWrite(PIN_PUMP_SYS, LOW); //active level - LOW
	}
	else {
		//остановка насоса системы
		digitalWrite(PIN_PUMP_SYS, HIGH); //active level - LOW
		LOG("stop nasosa SYS");
		return errCod;
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


	Serial.print("*******************celevaia temperatura "); Serial.println(tRoomSetpoint);
			//t[12] - температура в помещении.
		if ((temperature[12] < (tRoomSetpoint-0.6)) && (digitalRead(PIN_VALVE_SYS_SIGNAL_ClOSE))) {
			////Serial.print("Xolodno greiu (temperature[12] < (tRoomSetpoint-0,6)   "); Serial.println((temperature[12] < (tRoomSetpoint - 0,6)));
			// температура в контролируемом помещении ниже заданной. Призакрываем кран(боковой отвод) на 1/25 максимального времени переключения крана системы
			digitalWrite(PIN_VALVE_SYS_ClOSE, LOW);//active level - LOW
			digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
			delay((g_timeSwitchValveSYS) / 10);
			digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
			digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
		}
		else {
			if ((temperature[12] > (tRoomSetpoint+0.6)) && (digitalRead(PIN_VALVE_SYS_SIGNAL_OPEN))) {
				////Serial.print("Jarko. open bokovoi (temperature[12] > (tRoomSetpoint+0,6))    "); Serial.println(temperature[12]); Serial.println(tRoomSetpoint); Serial.println((tRoomSetpoint + 0.6));  Serial.println((temperature[12] > (tRoomSetpoint + 0.6)));
				// температура в контролируемом помещении выше заданной. Приоткрываем кран(боковой отвод) на 1/25 максимального времени переключения крана системы
				digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_SYS_OPEN, LOW);
				delay(g_timeSwitchValveSYS / 10);
				digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
				digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
			}

		}

	// перезапуск таймера вызова функции регулировки температуры в системе.
	extern myCycle cycleTempControlSYS;
	cycleTempControlSYS.clear();
	cycleTempControlSYS.reStart();
	LOG("stop (temperatureControlSYS) "); LOG((millis() - startTime)); LOG(" mc\n");
	return errCod;
}
