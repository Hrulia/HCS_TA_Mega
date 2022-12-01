/*
//почему-то, если сначала открыть кран, а потом закрывать, то цикл ожидания сигнала закрытия крана пропускался???

Тестирование компонентов системы:
1. моторизированных кранов на ТТК и системе
2. тестирование датчиков температуры DS18B20

*/

#include "TestComponents.h"
#include "HeaderHCS_TA.h"
#include "Button.h"

//Button oSignalClose(10, 15); // класс button позволяет фильтровать дребезг контактов с сигнальных концевиков крана. время фильтрации 15мс*на период повторения. 

extern unsigned long timeSwitchValveTTK=0;//*************************************************************************


int testSystem() {

	LOG("Start (testSystem)\n");
	int result = 0; //коды завершения вызываемых функций

	//тестируем работу крана защиты обратки ТТК
	LOG("\nCall testValveTTK\n");
	result=testValve(PIN_VALVE_TTK_OPEN, PIN_VALVE_TTK_ClOSE, PIN_VALVE_TTK_SIGNAL_OPEN, PIN_VALVE_TTK_SIGNAL_ClOSE, &timeSwitchValveTTK);
	if (result) { LOG("Error in... "); LOG(result);LOG("\n");
	}
	LOG("timeSwitchValveTTK "); LOG(timeSwitchValveTTK); LOG(" ms\n");

	Serial.print("time fo regTest1:  "); Serial.println((timeSwitchValveTTK));

	//тестируем работу крана регулировки темпратуры системы
	LOG("\nCall testValveSYS\n");
	result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &timeSwitchValveSYS);


	if (result) { LOG("Error in... "); LOG(result); ; LOG("\n");  //обработка ошибки
	}
	LOG("timeSwitchValveSYS "); LOG(timeSwitchValveSYS); LOG("ms\n");


	//тестируем работу 10 датчиков температуры обвязки котла
	LOG("\ntest_DS18B20\n");
	result = testDS18B20(1);
		if (result) {
			LOG("Error in... "); LOG(result); ; LOG("\n");  //обработка ошибки
		}

	//тестируем индикаторы TM1637
		//...



	return 0;
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
	*ptimeSwitch =(startTime>*ptimeSwitch)? startTime : *ptimeSwitch;
	///LOG("Itog"); LOG(*ptimeSwitch); LOG("\n");
	return errCod;
}

int testDS18B20(uint8_t pin)
{
	LOG("Start (testDS18B20)\n");

	LOG("!!!  NOT IMPLEMENTED  !!!\n");

	int errCod = 0;
	return errCod;
}







