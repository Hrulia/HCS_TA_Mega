// 
// 
// 

#include "SystemTime.h"



//sei();   разрешает прерывани€. например в функции ISR

RTC_DS1307 rtc;
//RTC_DS1302 rtc;

int initSystemTime() {
	LOG("\nstart (initSystemTime)\n");
	int errCod = 0;
	bool setClock = false;  //указывает устанавливать часы по времени компил€ции скетча при каждой его загрузке, или перезагрузке ардуино, поэтому по умолчанию false
	
	rtc.begin(); /*надо бы провер€ть есть часы вообще в системе или нет.*/
	//если RTC не запущены, то запустим, установим врем€ компил€ции скетча и выход RTC на 1√ц
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
	sei();  //разрешает другие прерывани€ в этот момент
	g_systemDateTime = rtc.now();
	//Serial.println(String(rtc.now().hour())+":"+ String(rtc.now().minute()));

	//digitalWrite(13, !(digitalRead(13)));
}

