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


////���������� ���������� ����������
////���������������� �����
static unsigned long timeSwitchValveTTK = 0; //��, ������������ ������ ����� ������������ ��������������� ����� ��� �� ������ ��������� � ������
//static unsigned long timeSwitchValveSYS = 0; //��, ������������ ������ ����� ������������ ��������������� ����� ������� �� ������ ��������� � ������
////������
//static bool  pumpTTK = false; //���������� ��������� ������ ���
//static bool  pumpSYS = false; //���������� ��������� ������ �������
//							  //static bool  pump�� = false; //���������� ��������� ������ �������������� ����� ()
////��������� �������
//bool failure = false; //������ ������� ��������� ���������� ��������� ����� ���


// Cycles
myCycle cycleGetTemp(MS_500, true);  //5c ���� ���������� �����������
myCycle cycleDispTempTM1637(MS_05S, true); //5c ���� ���������� ��������� ����������� �� ����������� TM1637 
myCycle cycleTempControlTTK(MS_01M, true); //60� ���� ������� ���������� ����������� ������ ��� (������ �� �������� �������) 


// The setup() function runs once each time the micro-controller starts
void setup()
{
	unsigned long startTime = millis();
	Serial.begin(57600);
	LOG("\nstart (setup)\n");
	//pinMode(13,)
	//TCounterDown myTaimer(1000,;

	//��������� ����� ��� ���������������� ������ 
	//������ - ������ ����� ��������� � ������� �����
	pinMode(PIN_VALVE_TTK_OPEN, OUTPUT);
	digitalWrite(PIN_VALVE_TTK_OPEN, HIGH); //active level - LOW

	pinMode(PIN_VALVE_TTK_ClOSE, OUTPUT);
	digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH); //active level - LOW

	pinMode(PIN_VALVE_TTK_SIGNAL_OPEN, INPUT_PULLUP);
	pinMode(PIN_VALVE_TTK_SIGNAL_ClOSE, INPUT_PULLUP);

	//����������� ������ ���������� �������� (�� ��������� ���������)
	//���
	pinMode(PIN_PUMP_TTK, OUTPUT);
	digitalWrite(PIN_PUMP_TTK, HIGH); //active level - LOW
	//�������
	pinMode(PIN_PUMP_SYS, OUTPUT);
	digitalWrite(PIN_PUMP_SYS, HIGH); //active level - LOW
	////������������
	//pinMode(PIN_PUMP_��, OUTPUT);
	//digitalWrite(PIN_PUMP_��, HIGH); //active level - LOW

	//������������� ����������� TM1637
	init_TM1637();

	//������������� �������� �����������
	initTemp(); 

	//��������� ���������� �������/ ����� ���� ����������������� ����� �������
	LOG("Call TestSystem\n");
	testSystem(); //�������� �� initValve

	Serial.print("time fo regTest2:  "); Serial.println((timeSwitchValveTTK));

	//int TestSystem() {
	//	//��������� ������ ����� ������ ������� ���
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
	int result = 0; //���� ���������� ���������� �������

	//������ �������� ��������� ������
	timersWorks(); ///������ ��� ���� ���� �����������

	if (cycleGetTemp.check()) {
		////Serial.println("GET TEMP");
		//getTemp();

		result = getTemperaturDS18B20();
		if (result) {

			LOG("Error in getTemperaturDS18B20 "); LOG2((uint16_t)result, BIN); LOG("\n");
			//��������� ������
			//...
		}
	}

	if (cycleDispTempTM1637.check()) {
		//getTemp();
		result = indicator_TM1637_Output_temperature();
		if (result) {

			LOG("Error in indicator_TM1637_Output_temperature "); LOG(result); LOG("\n");
			//��������� ������
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
			//��������� ������
			//...
		}
	}






	pinMode(7, INPUT_PULLUP); //�������
	pinMode(8, INPUT_PULLUP); //�������
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
	////pinMode(3, INPUT_PULLUP); //�������
	////Button oSignalClose(3, 15); // ����� button ��������� ����������� ������� ��������� � ���������� ���������� �����. ����� ���������� 15��*�� ������ ����������. 
	////
	////do {
	////	oSignalClose.filterAvarage();
	////	LOG("do");LOG(oSignalClose.flagPress); LOG("\n");
	////} while (oSignalClose.flagPress);



	LOG("stop (loop) "); LOG((millis() - startTime)); LOG(" mc\n");
	delay(0500);
}


//������� ������������ ��������
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