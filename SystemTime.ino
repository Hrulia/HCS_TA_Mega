// 
// 
// 
//#include <RTClib.h>

//sei();   разрешает прерывания. например в функции ISR

RTC_DS1307 rtc;
//RTC_DS1302 rtc;

////текущее время/дата в системе 
//volatile DateTime g_systemDateTime;

int initSystemTime() {
	Serial.print("\nstart (initSystemTime)\n");
	int errCod = 0;
	bool setClock = false;  //указывает устанавливать часы по времени компиляции скетча при каждой его загрузке, или перезагрузке ардуино, поэтому по умолчанию false

	rtc.begin(); /*надо бы проверять есть часы вообще в системе или нет.*/
							 //если RTC не запущены, то запустим, установим время компиляции скетча и выход RTC на 1Гц
	if (!rtc.isrunning() || setClock || 0) {
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

//обновление переменной системного времени по прерыванию от RTC
void ISR_UpdateSystemTime() {
	sei();  //разрешает другие прерывания в этот момент
	globalParameters.g_systemDateTime = rtc.now();

	//вывод в порт времени системы
	//Serial.println(String(rtc.now().hour())+":"+ String(rtc.now().minute()));

	//digitalWrite(13, !(digitalRead(13)));
}

//Функция обновления системного времени от значения от какого-то внешнего источника
void UpdateSystemTime(const char* NewTimeStr) {
	rtc.adjust(DateTime("", NewTimeStr)); //nullptr????
	globalParameters.g_systemDateTime = DateTime("", NewTimeStr);
}
