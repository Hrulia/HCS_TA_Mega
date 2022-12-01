// 
// 
// 

#include "Indicator_TM1637.h"
//#include ".\libraries\TM1637_\TM1637.h"


//создаем объекты индикаторы
TM1637Display TM1637_Time(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO_Time);
TM1637Display TM1637(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO1);
TM1637Display TM1637_2(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO2);
TM1637Display TM1637_3(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO3);
TM1637Display TM1637_4(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO3);
TM1637Display TM1637_5(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO3);
TM1637Display TM1637_6(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO3);

static byte indicator_Intensity = 2;//интенсивность свечения
//load data from EEPROM
//uint8_t selectedKey = EEPROM.read(0x00); //0x11;
//byte indicatorLED_KEY_Intensity = EEPROM.read(0x01); //4;//интенсивность свечения

//инициализация индикаторов
void init_TM1637(){
	LOG("\nstart (init_TM1637)\n");

	TM1637_Time.clear();
	TM1637.clear();
	TM1637_2.clear();
	TM1637_3.clear();
	TM1637_4.clear();
	TM1637_5.clear();
	TM1637_6.clear();

	TM1637_Time.setBrightness(indicator_Intensity);
	TM1637.setBrightness(indicator_Intensity);
	TM1637_2.setBrightness(indicator_Intensity);
	TM1637_3.setBrightness(indicator_Intensity);
	TM1637_4.setBrightness(indicator_Intensity);
	TM1637_5.setBrightness(indicator_Intensity);
	TM1637_6.setBrightness(indicator_Intensity);

	LOG("\nstop (init_TM1637)\n");
	}

//обновление информации о времени на индикаторе TM1637
int indicator_TM1637_Output_time()
{
	LOG("\nstart (indicator_TM1637_Output_time)\n");
	int errCod = 0;
	//static unsigned long t;
	//t= millis() - t;
	//Serial.println(t);
	//t = millis();

	TM1637_Time.setBrightness(7);
	//int time;
	//time = (int)g_systemDateTime.hour() * 100 + (int)g_systemDateTime.minute();
	//TM1637_Time.showNumberDecEx(time, (g_systemDateTime.second() % 2 ? 0x40 : 0x00));	
	TM1637_Time.showNumberDecEx((int)(g_systemDateTime.hour() * 100 + g_systemDateTime.minute()), (g_systemDateTime.second() % 2 ? 0x40 : 0x00),true);//0x40 'эта маска включает двоеточие между знаками. 

	// перезапуск таймера вызова функции обновления значения температуры.
	extern myCycle cycleDispSystemTime;
	cycleDispSystemTime.clear();
	cycleDispSystemTime.reStart();

	LOG("stop (indicator_TM1637_Output_time) "); 
	return errCod;
}

//обновление информации о температуре на индикаторах
int indicator_TM1637_Output_temperature()
{
	LOG("\nstart (indicator_TM1637_Output_temperature)\n");
	int errCod = 0;

	TM1637.setBrightness(indicator_Intensity);
	TM1637_2.setBrightness(indicator_Intensity);
	TM1637_3.setBrightness(indicator_Intensity);
	TM1637_4.setBrightness(indicator_Intensity);
	TM1637_5.setBrightness(indicator_Intensity);
	TM1637_6.setBrightness(indicator_Intensity);


	extern float temperature[];
	
	//////////// переводим в целое число температуру с  датчика (с учетом одного знака после запятой) и выводим на индикатор 
	//////////byte digits[] = { 0x00,0x00,0x00,0x00 };  // буфер для цифр LED-индикатора
	//////////boolean flagNegativeTemp = false;

	///////////*unsigned*/ int intTemp = -999;
	//////////	intTemp = (int)(temperature[1] * 100); 


	//////////////int testTemp = -9990;  
	//////////////intTemp = testTemp;

	//////////if (intTemp < 0) { intTemp = 0 - intTemp; flagNegativeTemp = true; } //abs(intTemp) 
	//////////// раскидываем 3x-значное число на цифры
	//////////digits[0] = TM1637.encodeDigit((byte)(intTemp / 1000));
	//////////intTemp %= 1000;
	//////////digits[1] = TM1637.encodeDigit((byte)(intTemp / 100));
	//////////digits[1] |= 0x80; //для отображения точки на индикаторе за эти мразрядом
	//////////intTemp %= 100;
	//////////digits[2] = TM1637.encodeDigit((byte)(intTemp / 10));
	//////////intTemp %= 10;
	//////////digits[3] = TM1637.encodeDigit((byte)(intTemp % 10));

	//////////Serial.print(digits[0]); Serial.print(digits[1]); Serial.print(digits[2]); Serial.println(digits[3]);

	//////////digits[0]
	//////////TM1637.setSegments(digits);


	////////////выводом на дисплей
	////////////if (flagNegativeTemp) {
	////////////	if (intTemp > 0 && intTemp <= 99) {







	//		indicatorLED_KEY.clearDisplayDigit(0, false);
	//		indicatorLED_KEY.setDisplayDigit(0x2D, 1, false, FONT_DEFAULT);
	//		indicatorLED_KEY.setDisplayDigit(Digits[2], 2, true);
	//		indicatorLED_KEY.setDisplayDigit(Digits[3], 3, false);
	//	}
	//	else if (intTemp>99 && intTemp <= 999) {
	//		indicatorLED_KEY.setDisplayDigit(0x2D, 0, false, FONT_DEFAULT);
	//		indicatorLED_KEY.setDisplayDigit(Digits[1], 1, false);
	//		indicatorLED_KEY.setDisplayDigit(Digits[2], 2, true);
	//		indicatorLED_KEY.setDisplayDigit(Digits[3], 3, false);
	//	}
	//	else if (intTemp >999) {
	//		indicatorLED_KEY.setDisplayDigit(0x2D, 0, false, FONT_DEFAULT);
	//		indicatorLED_KEY.setDisplayDigit(Digits[0], 1, false);
	//		indicatorLED_KEY.setDisplayDigit(Digits[1], 2, false);
	//		indicatorLED_KEY.setDisplayDigit(Digits[2], 3, false);
	//	}
	//}
	//else {
	//	if (intTemp >= 0 && intTemp <= 99) {
	//		indicatorLED_KEY.clearDisplayDigit(0, false);
	//		indicatorLED_KEY.clearDisplayDigit(1, false);
	//		indicatorLED_KEY.setDisplayDigit(Digits[2], 2, true);
	//		indicatorLED_KEY.setDisplayDigit(Digits[3], 3, false);
	//	}
	//	else if (intTemp>99 && intTemp <= 999) {
	//		indicatorLED_KEY.clearDisplayDigit(0, false);
	//		indicatorLED_KEY.setDisplayDigit(Digits[1], 1, false);
	//		indicatorLED_KEY.setDisplayDigit(Digits[2], 2, true);
	//		indicatorLED_KEY.setDisplayDigit(Digits[3], 3, false);
	//	}
	//	else if (intTemp >999) {
	//		indicatorLED_KEY.setDisplayDigit(Digits[0], 0, false);
	//		indicatorLED_KEY.setDisplayDigit(Digits[1], 1, false);
	//		indicatorLED_KEY.setDisplayDigit(Digits[2], 2, true);
	//		indicatorLED_KEY.setDisplayDigit(Digits[3], 3, false);
	//	}
	//}*/


	//Расположены индикаторы
	/*
	3
	2
	1
	*/
	//TM1637.showNumberDecEx(-777, 0x40);
		TM1637.showNumberDecEx((temperature[0] * 100), 0x40);//подача системы
		TM1637_2.showNumberDecEx(temperature[10]*100, 0b01000000);//верх ТА
		TM1637_3.showNumberDecEx(temperature[3] * 100, 0b01000000); //подача ТТК

		Serial.print("Podacha TTK"); Serial.println(temperature[3]);
		Serial.print("Obratka TTK"); Serial.println(temperature[4]);
		Serial.print("Verh TA"); Serial.println(temperature[10]);
		Serial.print("Niz TA"); Serial.println(temperature[11]);
		Serial.print("Podacha SYS"); Serial.println(temperature[0]);
		Serial.print("Obratka SYS"); Serial.println(temperature[1]);
		Serial.print("Pomeschenie"); Serial.println(temperature[12]);
		Serial.print("\n");


	// перезапуск таймера вызова функции отображения температруры.
	extern myCycle cycleDispTempTM1637;
	cycleDispTempTM1637.clear();
	cycleDispTempTM1637.reStart();
	
	LOG("stop (indicator_TM1637_Output_temperature) ");
	return errCod;
}
