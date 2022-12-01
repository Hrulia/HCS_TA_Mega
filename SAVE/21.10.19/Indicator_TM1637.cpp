// 
// 
// 

#include "Indicator_TM1637.h"


//#define PIN_TM1637_Clk_COMMON A1
//#define PIN_TM1637_DIO A0
//#define PIN_TM1637_DIO2 A2
//#define PIN_TM1637_DIO3 A3


TM1637Display TM1637(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO1);
TM1637Display TM1637_2(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO2);
TM1637Display TM1637_3(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO3);

TM1637Display TM1637_Time(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO_Time);

static byte indicator_Intensity = 7;//интенсивность свечения
//load data from EEPROM
//uint8_t selectedKey = EEPROM.read(0x00); //0x11;
//byte indicatorLED_KEY_Intensity = EEPROM.read(0x01); //4;//интенсивность свечения



//инициализация индикаторов
int init_TM1637(){
	LOG("\nstart (init_TM1637)\n");
	int result = 0; //коды завершения вызываемых функций

	TM1637.clear();
	TM1637_2.clear();
	TM1637_3.clear();
	TM1637_Time.clear();



	TM1637_Time.setBrightness(indicator_Intensity);
	//int time[] = { 7,1,1,1 };
	//TM1637_Time.showNumberDecEx(time[0], 0b01000000);

	TM1637_Time.setBrightness(indicator_Intensity);

	TM1637.setBrightness(indicator_Intensity);
	TM1637_2.setBrightness(indicator_Intensity);
	TM1637_3.setBrightness(indicator_Intensity);
	LOG("\nstop (init_TM1637)\n");
	return result;
	}


//обновление информации о температуре на индикаторах
int indicator_TM1637_Output_temperature()
{
	unsigned long startTime = millis();	LOG("\nstart (indicator_TM1637_Output_temperature)\n");

	//TM1637.clear(); //внести в процедуру инициализации дисплея

	extern float temperature[];
	// переводим в целое число температуру с выбранного датчика (с учетом одного знака после запятой) и выводим на индикатор 
	////Serial.print("intTemp - "); Serial.println(intTemp, HEX);
	byte Digits[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };  // буфер для цифр LED-индикатора
	boolean flagNegativeTemp;

	/*unsigned*/ int intTemp = 8888;

		intTemp = (int)(temperature[0] * 10);  //
	
		flagNegativeTemp = false;
		if (intTemp < 0) { intTemp = 0 - intTemp; flagNegativeTemp = true; }
		// раскидываем 3x-значное число на цифры
		Digits[0] = (byte)((intTemp / 1000) % 10);
		Digits[1] = (byte)((intTemp / 100) % 10);
		Digits[2] = (byte)((intTemp / 10) % 10);
		Digits[3] = (byte)((intTemp) % 10);

		int t;
		t = (int)(temperature[0]*10);
		//TM1637.showNumberDecEx(t, 0b01000000);
		TM1637.showNumberDecEx(temperature[0]*100, 0b01000000);
		TM1637_2.showNumberDecEx(temperature[1]*100, 0b01000000);
		TM1637_3.showNumberDecEx(temperature[4] * 100, 0b01000000);
		
		TM1637_Time.showNumberDecEx(temperature[2] * 100, 0b01000000);



		//отдельную процедуру для вывода чисел отрицательных и разной разрядности
		//..

		////выводом на дисплей
		//if (flagNegativeTemp) {
		//	if (intTemp > 0 && intTemp <= 99) {

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
		//}

















	//TM1637.showNumberDecEx(1234, 1, true);


	// перезапуск таймера вызова функции отображения температруры.
	extern myCycle cycleDispTempTM1637;
	cycleDispTempTM1637.clear();
	cycleDispTempTM1637.reStart();





	//получаем с ds1307 время и выводим его на индикатор
	//////DateTime now = rtcDS1307.now();
	//////Serial.print("System time: "); Serial.print(now.day()); Serial.print("."); Serial.print(now.month()); Serial.print("."); Serial.print(now.year()); Serial.print("  "); Serial.print(now.hour()); Serial.print(":"); Serial.print(now.minute()); Serial.print(":"); Serial.println(now.second());

	//////int time;
	//////time = (int)now.hour() * 100 + (int)now.minute();
	////////0x40 'эта маска включает двоеточие между знаками. 
	//////TM1637.setBrightness(5);
	//////TM1637.showNumberDecEx(time, (now.second() % 2 ? 0x40 : 0x00));

	//delay(3000);
	//TM1637.
	////TM1637.showNumberDecEx(77.88, 0);

	//TM1637.setSegments({ 1,1,1,1 });

	//TM1637.scroll(1, 2, 3, 4, 3000);

	//TM1637.clear();
	//TM1637.brightness(7);
	//
	//uint8_t DispData[] = { _H,_E,_L,_L,_O,_Y,_empty,_i,_r, _empty };

	LOG("stop (indicator_TM1637_Output_temperature) "); LOG((millis() - startTime)); LOG(" mc\n");
	return 0;
}
