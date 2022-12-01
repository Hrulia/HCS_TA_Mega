// 
// 
// 

#include "MotorizedValve.h"
////#include "Button.h" //Button oSignalClose(10, 15); // класс button позволяет фильтровать дребезг контактов с сигнальных концевиков крана. время фильтрации 15мс*на период повторения. 


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

const int tcrash = 90;	//аварийная температура(90 *С)
const int tdanger = 80;	//предаварийная температура(80 *С)
const int tneed = 50;	//требуемая температура обратки(50 * С)

int t_flueGas = 200; //температура дымовых газов ТТК котла  


extern bool g_failure; //сигнал наличия состояния аварийного перегрева котла




extern unsigned long g_timeSwitchValveTTK;//*******************************************************************
										
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

	Serial.print("time fo regTest1:  "); Serial.println((g_timeSwitchValveTTK));

	//тестируем работу крана регулировки темпратуры системы
	LOG("\nCall testValveSYS\n");
	result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &g_timeSwitchValveSYS);


	if (result) {
		LOG("Error in... "); LOG(result); ; LOG("\n");  //обработка ошибки
	}
	LOG("g_timeSwitchValveSYS "); LOG(g_timeSwitchValveSYS); LOG("ms\n");


	//////тестируем работу 10 датчиков температуры обвязки котла
	////LOG("\ntest_DS18B20\n");
	////result = testDS18B20(1);
	////	if (result) {
	////		LOG("Error in... "); LOG(result); ; LOG("\n");  //обработка ошибки
	////	}

	//////тестируем индикаторы TM1637
	////	//...


	LOG("\nstop (initTemp)\n");
	return result;
}

int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long * ptimeSwitch)
{
	LOG("Start (testValve)\n");
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

	LOG("\nstop (testValve)\n");
	return errCod;
}





//Регулировка и поддержание оптимальной температуры ТТК (защита от холодной обратки)
int temperatureControlTTK() {
	LOG("\nstart (temperatureControlTTK)\n");

	int errCod = 0;
	unsigned long startTime = millis();	
	extern float temperature[];
	if (g_failure || (t_flueGas > 60) || ((int)temperature[3] > ((int)temperature[11] + 10))) { //котел топиться или вода в подаче котла нагрелась более чем на 10 *С выше температуры воды низа ТА
		//запуск насоса ТТК
		digitalWrite(PIN_PUMP_TTK, LOW); //active level - LOW
	}
	else{//остановка насоса ТТК
		digitalWrite(PIN_PUMP_TTK, HIGH); //active level - LOW
	}
	Serial.print("\npodacha "); Serial.println(temperature[3]);
	Serial.print("obratka "); Serial.println(temperature[4]);
	Serial.print("niz TA "); Serial.println(temperature[11]);
	Serial.print("g_failure"); Serial.println(g_failure);
	////Serial.print("signal CLOSE"); Serial.println(digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE));
	////Serial.print("signal OPEN"); Serial.println(digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN));
	////Serial.print("time fo reg:  "); Serial.println((g_timeSwitchValveTTK));
	////Serial.print("time fo reg/10:  "); Serial.println((g_timeSwitchValveTTK / 10));


	if ((int)temperature[3] > tdanger) { //предаварийный перегрев котла
		if ((int)temperature[3] > tcrash) { //аварийный перегрев котла
			//включаем зуммер сирены
			///tone(PIN_EMERGENCY_SIREN, 3000, 300000);
			g_failure = true;
		}
		//полностью закрываем боковой отвод крана котла ТТК
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//запуск насоса ТТК и системы
		digitalWrite(PIN_PUMP_TTK, LOW);
		digitalWrite(PIN_PUMP_SYS, LOW);
		return 0;
	}
	g_failure = false;
	if (((int)temperature[4] > (tneed + 3)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE))) {// обратка горячая призакрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
		
		Serial.print("Peregrev obratki"); Serial.println("");
		
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);//active level - LOW
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		delay((g_timeSwitchValveTTK) / 25);
		digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
	}
	else {
		Serial.print("else Peregrev obratki"); Serial.println("");
			if (((int)temperature[4] < (tneed -3)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN))) {// обратка холодная приоткрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
				
				Serial.print("Nedogrev obratki"); Serial.println("");
				
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_TTK_OPEN, LOW);
				delay(g_timeSwitchValveTTK / 25);
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

	LOG("\nstart (temperatureControlSYS)\n");

	int errCod = 0;
	unsigned long startTime = millis();	
	
	// перезапуск таймера вызова функции регулировки температуры.
	extern myCycle cycleTempControlTTK;
	cycleTempControlTTK.clear();
	cycleTempControlTTK.reStart();
	LOG("stop (temperatureControlSYS) "); LOG((millis() - startTime)); LOG(" mc\n");
	return errCod;
}
