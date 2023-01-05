// 
// 
// 

#include "SystemTime.h"



//sei();   ��������� ����������. �������� � ������� ISR

RTC_DS1307 rtc;
//RTC_DS1302 rtc;

int initSystemTime() {
	LOG("\nstart (initSystemTime)\n");
	int errCod = 0;
	bool setClock = false;  //��������� ������������� ���� �� ������� ���������� ������ ��� ������ ��� ��������, ��� ������������ �������, ������� �� ��������� false
	
	rtc.begin(); /*���� �� ��������� ���� ���� ������ � ������� ��� ���.*/
	//���� RTC �� ��������, �� ��������, ��������� ����� ���������� ������ � ����� RTC �� 1��
	if (!rtc.isrunning()||setClock||0) {
		LOG("RTC is NOT running!\n");
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(__DATE__, __TIME__));	
		//Serial.println(__TIME__);
		//Serial.println(__DATE__);
		rtc.writeSqwPinMode(DS1307_SquareWave1HZ);
	}

	pinMode(PIN_RTC_INTERRUPT, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(PIN_RTC_INTERRUPT), ISR_UpdateSystemTime, FALLING);//FALLING, CHANGE

	LOG("stop (initSystemTime) "); 
	return errCod;
}

void ISR_UpdateSystemTime() {
	sei();  //��������� ������ ���������� � ���� ������
	g_systemDateTime = rtc.now();
	//Serial.println(String(rtc.now().hour())+":"+ String(rtc.now().minute()));

	//digitalWrite(13, !(digitalRead(13)));
}

