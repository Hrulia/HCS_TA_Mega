// 
// 
// 

#include "Indicator_TM1637.h"
#include ".\libraries\TM1637_\TM1637.h"


//создаем объекты индикаторы
/*TM1637Display *pTM1637_3;*/
TM1637Display TM1637_Time(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO_Time);
TM1637Display TM1637(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO1);	//ТТК подача
TM1637Display TM1637_2(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO2);	//ТА верх
TM1637Display TM1637_3(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO3); // SYS подача
TM1637Display TM1637_4(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO4); // ТТК обратка
TM1637Display TM1637_5(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO5); // ТА низ
TM1637Display TM1637_6(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO6); // SYS обратка
TM1637Display TM1637_7(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO7); // ЭК подача
TM1637Display TM1637_8(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO8); // ЭК обратка
TM1637Display TM1637_9(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO9); // Целевая температура
TM1637Display TM1637_10(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO10); // температура в помещении
TM1637Display TM1637_11(PIN_TM1637_Clk_COMMON, PIN_TM1637_DIO11); // температура на улице
  

static byte indicator_Intensity = 4;//интенсивность свечения
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
	TM1637_7.clear();
	TM1637_8.clear();
	TM1637_9.clear();
	TM1637_10.clear();
	TM1637_11.clear();

	TM1637_Time.setBrightness(7);
	TM1637.setBrightness(indicator_Intensity);
	TM1637_2.setBrightness(indicator_Intensity);
	TM1637_3.setBrightness(indicator_Intensity);
	TM1637_4.setBrightness(indicator_Intensity);
	TM1637_5.setBrightness(indicator_Intensity);
	TM1637_6.setBrightness(indicator_Intensity);
	TM1637_7.setBrightness(indicator_Intensity,true);
	TM1637_8.setBrightness(indicator_Intensity, true);
	TM1637_9.setBrightness(7);
	TM1637_10.setBrightness(1);
	TM1637_11.setBrightness(2,true);

	TM1637.showNumberDecEx(8888,0xF0); //что бы видеть, что индикаторы исправны
	TM1637_2.showNumberDecEx(8888, 0xF0);
	TM1637_3.showNumberDecEx(8888, 0xF0);
	TM1637_4.showNumberDecEx(8888, 0xF0);
	TM1637_5.showNumberDecEx(8888,0xF0);
	TM1637_6.showNumberDecEx(8888,0xF0);
	TM1637_7.showNumberDecEx(8888,0xF0);
	TM1637_8.showNumberDecEx(8888,0xF0);
	TM1637_9.showNumberDecEx(8888,0xF0);
	TM1637_10.showNumberDecEx(8888,0xF0);
	TM1637_11.showNumberDecEx(8888,0xF0);

	LOG("\nstop (init_TM1637)\n");
	}

//обновление информации о времени на индикаторе TM1637
int indicator_TM1637_Output_time()
{
	int errCod = 0;

	//TM1637_Time.setBrightness(7);
	//int time;
	//time = (int)g_systemDateTime.hour() * 100 + (int)g_systemDateTime.minute();
	//TM1637_Time.showNumberDecEx(time, (g_systemDateTime.second() % 2 ? 0x40 : 0x00));	
	TM1637_Time.showNumberDecEx((int)(g_systemDateTime.hour() * 100 + g_systemDateTime.minute()), (g_systemDateTime.second() % 2 ? 0x40 : 0x00),true);//0x40 'эта маска включает двоеточие между знаками. 

	////Serial.print("Системное время "); Serial.println(g_systemDateTime.timestamp());
	// перезапуск таймера вызова функции обновления значения системного времени.
	extern myCycle cycleDispSystemTime;
	cycleDispSystemTime.clear();
	cycleDispSystemTime.reStart();
	return errCod;
}

//обновление информации о температуре на индикаторах
int indicator_TM1637_Output_temperature()
{
	const  bool LOG = false; //выводить логи работы процедуры
	if (LOG) { Serial.print("\nstart (indicator_TM1637_Output_temperature)\n"); }

	int errCod = 0;

	extern float temperature[];
	if (LOG) { 
		Serial.print("SYS подача "); Serial.println(temperature[0]);
		Serial.print("SYS обратка "); Serial.println(temperature[1]);
		Serial.print("ТТК подача "); Serial.println(temperature[3]);
		Serial.print("ТТК обратка "); Serial.println(temperature[4]);
		Serial.print("ТА верх "); Serial.println(temperature[10]);
		Serial.print("ТА низ "); Serial.println(temperature[11]);
		Serial.print("Помещение "); Serial.println(temperature[12]);
		Serial.print("Улица "); Serial.println(temperature[13]);
		Serial.print("ЭК подача "); Serial.println(temperature[14]);
		Serial.print("ЭК обратка "); Serial.println(temperature[15]);
	}

	TM1637.showNumberMsn(temperature[3]);
	TM1637_2.showNumberMsn(temperature[10]);
	TM1637_3.showNumberMsn(temperature[0]);
	TM1637_4.showNumberMsn(temperature[4]);
	TM1637_5.showNumberMsn(temperature[11]);
	TM1637_6.showNumberMsn(temperature[1]);
	//TM1637_7.showNumberMsn(temperature[14]);

	extern float g_t_flueGases;
	TM1637_7.showNumberMsn(g_t_flueGases,1);//временно выведем дымовые газы на чужой индикатор

	TM1637_8.showNumberMsn(temperature[15]);
	TM1637_9.showNumberMsn(g_tRoomSetpoint,1);
	TM1637_10.showNumberMsn(temperature[12]);
	TM1637_11.showNumberMsn(temperature[13]);
	

		/*обработка кнопки*/
		Button buttonReinitTemp(12, 1); // класс button позволяет фильтровать дребезг контактов. время фильтрации 15мс*на период повторения функции. 
		buttonReinitTemp.scanState();
		int i = 0;
		buttonReinitTemp.scanState();
		digitalWrite(13, buttonReinitTemp.flagPress);
		while (buttonReinitTemp.flagPress) {
			//Serial.println("Knopka nazhata"); Serial.print(i);
			digitalWrite(13, HIGH);
				g_tRoomSetpoint+=0.5; //увеличиваем по половине градуса
				if (g_tRoomSetpoint > 26) { g_tRoomSetpoint = 20.0;}
				TM1637_9.showNumberMsn(g_tRoomSetpoint, 1);
			delay(500);
			buttonReinitTemp.scanState();
			//////сохраним в ПЗУ. Загрузка состояния в процедуре setup
			////EEPROM.write(0x00, (uint8_t)g_tRoomSetpoint); //
			//////Отправка информации о на сервер брокера MQTT
			////Serial3.print(F("?sendGTargetTemp=")); Serial3.println(g_tRoomSetpoint, 1);
		}
		if (buttonReinitTemp.flagClick) {
			//после завершения процедуры установки целевой температуры кнопкой, сохраним ее в EEPROM и отправим MQTT брокеру
			Serial.println("____________________Кнопка сделала Клик");
			//сохраним в ПЗУ. Загрузка состояния в процедуре setup
			EEPROM.write(0x00, (uint8_t)g_tRoomSetpoint);
			//Отправка информации о на сервер брокера MQTT
			Serial3.print(F("?sendGTargetTemp=")); Serial3.println(g_tRoomSetpoint, 1);
		}

	// перезапуск таймера вызова функции отображения температруры.
	extern myCycle cycleDispTempTM1637;
	cycleDispTempTM1637.clear();
	cycleDispTempTM1637.reStart();

	if (LOG) { Serial.println("stop (indicator_TM1637_Output_temperature) "); }
	return errCod;
}
