/*
//������-��, ���� ������� ������� ����, � ����� ���������, �� ���� �������� ������� �������� ����� �����������???

������������ ����������� �������:
1. ���������������� ������ �� ��� � �������
2. ������������ �������� ����������� DS18B20

*/

#include "TestComponents.h"
#include "HeaderHCS_TA.h"
#include "Button.h"

//Button oSignalClose(10, 15); // ����� button ��������� ����������� ������� ��������� � ���������� ���������� �����. ����� ���������� 15��*�� ������ ����������. 

extern unsigned long timeSwitchValveTTK=0;//*************************************************************************


int testSystem() {

	LOG("Start (testSystem)\n");
	int result = 0; //���� ���������� ���������� �������

	//��������� ������ ����� ������ ������� ���
	LOG("\nCall testValveTTK\n");
	result=testValve(PIN_VALVE_TTK_OPEN, PIN_VALVE_TTK_ClOSE, PIN_VALVE_TTK_SIGNAL_OPEN, PIN_VALVE_TTK_SIGNAL_ClOSE, &timeSwitchValveTTK);
	if (result) { LOG("Error in... "); LOG(result);LOG("\n");
	}
	LOG("timeSwitchValveTTK "); LOG(timeSwitchValveTTK); LOG(" ms\n");

	Serial.print("time fo regTest1:  "); Serial.println((timeSwitchValveTTK));

	//��������� ������ ����� ����������� ���������� �������
	LOG("\nCall testValveSYS\n");
	result = testValve(PIN_VALVE_SYS_OPEN, PIN_VALVE_SYS_ClOSE, PIN_VALVE_SYS_SIGNAL_OPEN, PIN_VALVE_SYS_SIGNAL_ClOSE, &timeSwitchValveSYS);


	if (result) { LOG("Error in... "); LOG(result); ; LOG("\n");  //��������� ������
	}
	LOG("timeSwitchValveSYS "); LOG(timeSwitchValveSYS); LOG("ms\n");


	//��������� ������ 10 �������� ����������� ������� �����
	LOG("\ntest_DS18B20\n");
	result = testDS18B20(1);
		if (result) {
			LOG("Error in... "); LOG(result); ; LOG("\n");  //��������� ������
		}

	//��������� ���������� TM1637
		//...



	return 0;
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







