// 
// 
// 

#include "MotorizedValve.h"
////#include "Button.h" //Button oSignalClose(10, 15); // ����� button ��������� ����������� ������� ��������� � ���������� ���������� �����. ����� ���������� 15��*�� ������ ����������. 


// 0- t1, ������ c������
// 1- t2, ������� �������
// 2- t3, ����� ������������ ����� ��
// 3- t4, ������ ���
// 4- t5, ������� ���
// 5- t6, ���� �� ������ �� ���
// 6- t7, ��� �� ������� ���
// 7- t8, ���� �� ������ � �������
// 8- t9, ����� ������������ ����� �������
// 9- t10, ��� ��, ������� �������
// 10-t11, ���� ��
// 11-t12, ��� ��
// 12-t13, ����������� � ���������
// 13-t14, ����������� �� �����
// 14-t15, ������ �� 
// 15-t16, ������� ��

const int tcrash = 90;	//��������� �����������(90 *�)
const int tdanger = 80;	//������������� �����������(80 *�)
const int tneed = 50;	//��������� ����������� �������(50 * �)

int t_flueGas = 200; //����������� ������� ����� ��� �����  


extern bool g_failure; //������ ������� ��������� ���������� ��������� �����




extern unsigned long g_timeSwitchValveTTK;//*******************************************************************
										
int initValve() {

	LOG("Start (initValve)\n");
	int result = 0; //���� ���������� ���������� �������

					//��������� ������ ����� ������ ������� ���
	LOG("\nCall testValveTTK\n");
	result = testValve(PIN_VALVE_TTK_OPEN, PIN_VALVE_TTK_ClOSE, PIN_VALVE_TTK_SIGNAL_OPEN, PIN_VALVE_TTK_SIGNAL_ClOSE, &g_timeSwitchValveTTK);
	if (result) {
		LOG("Error in... "); LOG(result); LOG("\n");
	}
	LOG("g_timeSwitchValveTTK "); LOG(g_timeSwitchValveTTK); LOG(" ms\n");

	Serial.print("time fo regTest1:  "); Serial.println((g_timeSwitchValveTTK));

	//��������� ������ ����� ����������� ���������� �������
	LOG("\nCall testValveSYS\n");
	result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &g_timeSwitchValveSYS);


	if (result) {
		LOG("Error in... "); LOG(result); ; LOG("\n");  //��������� ������
	}
	LOG("g_timeSwitchValveSYS "); LOG(g_timeSwitchValveSYS); LOG("ms\n");


	//////��������� ������ 10 �������� ����������� ������� �����
	////LOG("\ntest_DS18B20\n");
	////result = testDS18B20(1);
	////	if (result) {
	////		LOG("Error in... "); LOG(result); ; LOG("\n");  //��������� ������
	////	}

	//////��������� ���������� TM1637
	////	//...


	LOG("\nstop (initTemp)\n");
	return result;
}

int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long * ptimeSwitch)
{
	LOG("Start (testValve)\n");
	/*
	return errors code:
	1 - ������ ����� �������� �����
	2 - ������ ����� �������� �����
	3 - ������ ����� ������
	*/
	LOG("Start (testValve)\n");

	int errCod = 0;
	Button oSignalOpen(signalOpen, 15); // ����� button ��������� ����������� ������� ��������� � ���������� ���������� �����. ����� ���������� 15��*�� ������ ����������. 
	Button oSignalClose(signalClose, 15);

	//��������� ������� �����
	unsigned long startTime = millis();
	//��������� ����
	digitalWrite(open, LOW);
	digitalWrite(close, HIGH);
	///LOG(digitalRead(open));	LOG(digitalRead(close));	LOG(digitalRead(signalOpen));	LOG(digitalRead(signalClose));
	//���� ��������� ������� �������� ������� ������
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
	/* ������ ������� ��� ������ button
	while (digitalRead(signalOpen)!=LOW) {
	if ((millis() - startTime) > MAX_TIME_TESTING_VALVE) {
	errCod = 1;
	break;
	}
	}
	*/
	//������ ���������� � �����
	digitalWrite(open, HIGH);
	digitalWrite(close, HIGH);
	//��������� ����� ���������� �������� ��������
	*ptimeSwitch = millis() - startTime;	LOG("full opening time = "); LOG(*ptimeSwitch); LOG("ms, errCod = "); LOG(errCod); LOG("\n");

	//��������� ������� �����
	startTime = millis();
	//��������� ����
	digitalWrite(close, LOW);
	digitalWrite(open, HIGH);
	//���� ��������� ������� ��������, ������� ������ � �����
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
	/* ������ ������� ��� ������ button
	while (digitalRead(signalClose)) {
	LOG(digitalRead(signalClose));

	//delay(1);  //????���������� ���������: ���� ����� ���� ������ �� �����������(������������). ���������� ������������ �����-�� ������� �� ���������� �������
	if ((millis() - startTime) > MAX_TIME_TESTING_VALVE) {
	errCod = errCod+2;
	break;
	}
	}
	*/
	//������ ���������� � �����
	digitalWrite(open, HIGH);
	digitalWrite(close, HIGH);
	//��������� ����� ���������� �������� ��������
	startTime = millis() - startTime; //�.�. startTime ������ �� �����������, �� �������� � ���� ������������ ����� ��������
	LOG("full closing time = "); LOG(startTime); LOG("ms, errCod = "); LOG(errCod); LOG("\n");

	//�������� ���������� ����� �� ���� ��������
	*ptimeSwitch = (startTime>*ptimeSwitch) ? startTime : *ptimeSwitch;
	///LOG("Itog"); LOG(*ptimeSwitch); LOG("\n");

	LOG("\nstop (testValve)\n");
	return errCod;
}





//����������� � ����������� ����������� ����������� ��� (������ �� �������� �������)
int temperatureControlTTK() {
	LOG("\nstart (temperatureControlTTK)\n");

	int errCod = 0;
	unsigned long startTime = millis();	
	extern float temperature[];
	if (g_failure || (t_flueGas > 60) || ((int)temperature[3] > ((int)temperature[11] + 10))) { //����� �������� ��� ���� � ������ ����� ��������� ����� ��� �� 10 *� ���� ����������� ���� ���� ��
		//������ ������ ���
		digitalWrite(PIN_PUMP_TTK, LOW); //active level - LOW
	}
	else{//��������� ������ ���
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


	if ((int)temperature[3] > tdanger) { //������������� �������� �����
		if ((int)temperature[3] > tcrash) { //��������� �������� �����
			//�������� ������ ������
			///tone(PIN_EMERGENCY_SIREN, 3000, 300000);
			g_failure = true;
		}
		//��������� ��������� ������� ����� ����� ����� ���
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//������ ������ ��� � �������
		digitalWrite(PIN_PUMP_TTK, LOW);
		digitalWrite(PIN_PUMP_SYS, LOW);
		return 0;
	}
	g_failure = false;
	if (((int)temperature[4] > (tneed + 3)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE))) {// ������� ������� ������������ (������� �����) ���� �� 1/25 ������������� ������� ������������ �����
		
		Serial.print("Peregrev obratki"); Serial.println("");
		
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);//active level - LOW
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		delay((g_timeSwitchValveTTK) / 25);
		digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
	}
	else {
		Serial.print("else Peregrev obratki"); Serial.println("");
			if (((int)temperature[4] < (tneed -3)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN))) {// ������� �������� ������������ (������� �����) ���� �� 1/25 ������������� ������� ������������ �����
				
				Serial.print("Nedogrev obratki"); Serial.println("");
				
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_TTK_OPEN, LOW);
				delay(g_timeSwitchValveTTK / 25);
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
				digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		}

	}
	// ���������� ������� ������ ������� ����������� �����������.
	extern myCycle cycleTempControlTTK;
	cycleTempControlTTK.clear();
	cycleTempControlTTK.reStart();
	LOG("stop (temperatureControlTTK) "); LOG((millis() - startTime)); LOG(" mc\n");
	return errCod;
};


//����������� � ����������� ����������� ����������� �������
int temperatureControlSYS() {

	LOG("\nstart (temperatureControlSYS)\n");

	int errCod = 0;
	unsigned long startTime = millis();	
	
	// ���������� ������� ������ ������� ����������� �����������.
	extern myCycle cycleTempControlTTK;
	cycleTempControlTTK.clear();
	cycleTempControlTTK.reStart();
	LOG("stop (temperatureControlSYS) "); LOG((millis() - startTime)); LOG(" mc\n");
	return errCod;
}
