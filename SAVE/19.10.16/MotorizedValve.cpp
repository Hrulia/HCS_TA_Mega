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

const int tcrash = 90;	//��������� �����������(90 *�)
const int tdanger = 80;	//������������� �����������(80 *�)
const int tneed = 50;	//��������� ����������� �������(50 * �)

int t_flueGas = 200; //����������� ������� ����� ��� �����  


extern bool failure; //������ ������� ��������� ���������� ��������� �����




extern unsigned long timeSwitchValveTTK;//*******************************************************************




//����������� � ����������� ����������� ����������� ��� (������ �� �������� �������)
int temperatureControlTTK() {
	unsigned long startTime = millis();	LOG("\nstart (temperatureControlTTK)\n");
	extern float temperature[];
	if (failure || (t_flueGas > 60) || ((int)temperature[3] > ((int)temperature[11] + 10))) { //����� �������� ��� ���� � ������ ����� ��������� ����� ��� �� 10 *� ���� ����������� ���� ���� ��
		//������ ������ ���
		digitalWrite(PIN_PUMP_TTK, LOW); //active level - LOW
	}
	else{//��������� ������ ���
		digitalWrite(PIN_PUMP_TTK, HIGH); //active level - LOW
	}
	Serial.print("\npodacha "); Serial.println(temperature[3]);
	Serial.print("obratka "); Serial.println(temperature[4]);
	Serial.print("niz TA "); Serial.println(temperature[11]);
	Serial.print("failure"); Serial.println(failure);
	////Serial.print("signal CLOSE"); Serial.println(digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE));
	////Serial.print("signal OPEN"); Serial.println(digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN));
	////Serial.print("time fo reg:  "); Serial.println((timeSwitchValveTTK));
	////Serial.print("time fo reg/10:  "); Serial.println((timeSwitchValveTTK / 10));


	if ((int)temperature[3] > tdanger) { //������������� �������� �����
		if ((int)temperature[3] > tcrash) { //��������� �������� �����
			//�������� ������ ������
			///tone(PIN_EMERGENCY_SIREN, 3000, 300000);
			failure = true;
		}
		//��������� ��������� ������� ����� ����� ����� ���
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//������ ������ ��� � �������
		digitalWrite(PIN_PUMP_TTK, LOW);
		digitalWrite(PIN_PUMP_SYS, LOW);
		return 0;
	}
	failure = false;
	if (((int)temperature[4] > (tneed + 3)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE))) {// ������� ������� ������������ (������� �����) ���� �� 1/25 ������������� ������� ������������ �����
		
		Serial.print("Peregrev obratki"); Serial.println("");
		
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);//active level - LOW
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		delay((timeSwitchValveTTK) / 25);
		digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
	}
	else {
		Serial.print("else Peregrev obratki"); Serial.println("");
			if (((int)temperature[4] < (tneed -3)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN))) {// ������� �������� ������������ (������� �����) ���� �� 1/25 ������������� ������� ������������ �����
				
				Serial.print("Nedogrev obratki"); Serial.println("");
				
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_TTK_OPEN, LOW);
				delay(timeSwitchValveTTK / 25);
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
				digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		}

	}
	// ���������� ������� ������ ������� ����������� �����������.
	extern myCycle cycleTempControlTTK;
	cycleTempControlTTK.clear();
	cycleTempControlTTK.reStart();
	LOG("stop (temperatureControlTTK) "); LOG((millis() - startTime)); LOG(" mc\n");
};