// 
// 
// 

#include "MotorizedValve.h"

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

const int tcrash = 80;	//��������� ����������� ���(90 *�)
const int tdanger = 70;	//������������� �����������  ��� (80 *�)
const int tneed = 30;	//��������� ����������� ������� ���(50 * �)

int t_flueGas = 200; //����������� ������� ����� ��� ����� ()  

////uint8_t g_tRoomSetpoint = 20; //�������� �������� ����������� ��������� (� �������� ���������)
bool g_onSchedule=true; //����, ��������������� ������������� ����������� ����������� �� ��������� �������

uint8_t dailySheduleOfTemperature[24] = {
18, //0-1
18, //1-2
18, //2-3
18, //3-4
18, //4-5
20, //5-6
22, //6-7
22, //7-8
23, //8-9
23, //9-10
23, //10-11
23, //11-12
23, //12-13
23, //13-14
23, //14-15
23, //15-16
23, //16-17
23, //17-18
23, //18-19
24, //19-20
25, //20-21
25, //21-22
25, //22-23
18 };//23-24

////extern unsigned long g_timeSwitchValveTTK;//*******************************************************************
										
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

	//��������� ������ ����� ����������� ���������� �������
	LOG("\nCall testValveSYS\n");//PIN_VALVE_SYS_OPEN
	//result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &g_timeSwitchValveSYS);
	result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &g_timeSwitchValveSYS);


	if (result) {
		LOG("Error in... "); LOG(result); ; LOG("\n");  //��������� ������
	}
	LOG("g_timeSwitchValveSYS "); LOG(g_timeSwitchValveSYS); LOG("ms\n");
		
	LOG("stop (initTemp)\n");
	return result;
}

int testValve(uint8_t open, uint8_t close, uint8_t signalOpen, uint8_t signalClose, unsigned long * ptimeSwitch)
{
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

	LOG("stop (testValve)\n");
	return errCod;
}

//����������� � ����������� ����������� ����������� ��� (������ �� �������� �������)
int temperatureControlTTK() {
	LOG("\nstart (temperatureControlTTK)\n");

	int errCod = 0;
	unsigned long startTime = millis();	
	extern float temperature[];			//[3] - ������ ���,			[11]-��� ��
	//////if (g_failure || (t_flueGas > 60) || ((int)temperature[3] > ((int)temperature[11] + 10))) { //����� �������� ��� ���� � ������ ����� ��������� ����� ��� �� 10 *� ���� ����������� ���� ���� ��
	
		/*���� ��� ������� ����, ���������� ����� �������*/
	if (g_failure || ((int)temperature[3] > ((int)temperature[11] + 10))) { //����� �������� ��� ���� � ������ ����� ��������� ����� ��� �� 10 *� ���� ����������� ���� ���� ��

		//������ ������ ���
		digitalWrite(PIN_PUMP_TTK, LOW); //active level - LOW
	}
	else{//��������� ������ ���
		digitalWrite(PIN_PUMP_TTK, HIGH); //active level - LOW
	}

	if ((int)temperature[3] > tdanger) { //������������� �������� �����
		if ((int)temperature[3] > tcrash) { //��������� �������� �����
			//�������� ������ ������
			tone(PIN_EMERGENCY_SIREN, 1000, 300000);
			g_failure = true;
		}
		//��������� ��������� ������� ����� ����� ����� ���
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//������ ������� ��� � �������
		digitalWrite(PIN_PUMP_TTK, LOW);
		digitalWrite(PIN_PUMP_SYS, LOW);
		return errCod;
	}
	g_failure = false;
	if (((int)temperature[4] > (tneed + 2)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE))) {// ������� ������� ������������ (������� �����) ���� �� 1/25 ������������� ������� ������������ �����
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);//active level - LOW
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		delay((g_timeSwitchValveTTK) / 10);
		digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
	}
	else {
			if (((int)temperature[4] < (tneed -2)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN))) {// ������� �������� ������������ (������� �����) ���� �� 1/25 ������������� ������� ������������ �����
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_TTK_OPEN, LOW);
				delay(g_timeSwitchValveTTK / 10);
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
	LOG(F("\nstart (temperatureControlSYS)\n"));
	int errCod = 0;
	unsigned long startTime = millis();	

	extern float temperature[];

	//������ ������ �������	// 10-t11, ���� ��	// 12-t13, ����������� � ���������
	if (g_failure ||((int)temperature[10] > ((int)temperature[12] + 10))) { //����������� ����� �� ���� ����������� � ������������� ���������
		//������ ������ �������
		LOG("zapusk nasosa SYS");
		digitalWrite(PIN_PUMP_SYS, LOW); //active level - LOW
	}
	else {//��������� ������ �������
		digitalWrite(PIN_PUMP_SYS, HIGH); //active level - LOW
		LOG("stop nasosa SYS");
		return errCod;
	}

	int tRoomSetpoint = g_tRoomSetpoint;
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
	//Serial.println 
		if (((int)temperature[12] < (tRoomSetpoint)) && (digitalRead(PIN_VALVE_SYS_SIGNAL_ClOSE))) {
			// ����������� � �������������� ��������� ���� ��������. ������������ ����(������� �����) �� 1/25 ������������� ������� ������������ ����� �������
			digitalWrite(PIN_VALVE_SYS_ClOSE, LOW);//active level - LOW
			digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
			delay((g_timeSwitchValveSYS) / 10);
			digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
			digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
		}
		else {
			if (((int)temperature[12] > (tRoomSetpoint)) && (digitalRead(PIN_VALVE_SYS_SIGNAL_OPEN))) {
				// ����������� � �������������� ��������� ���� ��������. ������������ ����(������� �����) �� 1/25 ������������� ������� ������������ ����� �������
				digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_SYS_OPEN, LOW);
				delay(g_timeSwitchValveSYS / 10);
				digitalWrite(PIN_VALVE_SYS_ClOSE, HIGH);
				digitalWrite(PIN_VALVE_SYS_OPEN, HIGH);
			}

		}

	// ���������� ������� ������ ������� ����������� ����������� � �������.
	extern myCycle cycleTempControlSYS;
	cycleTempControlSYS.clear();
	cycleTempControlSYS.reStart();
	LOG("stop (temperatureControlSYS) "); LOG((millis() - startTime)); LOG(" mc\n");
	return errCod;
}