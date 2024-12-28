
#include "Temp_DS18B20.h"


//#define NUMBER_OF_DS18B20 16										//Number of sensor connected  (define in the 'HeaderHCS_TA.h')
#define RESOLUSHION_DS18B20 RESOLUSHION_12bit			//RESOLUSHION_9bit = 0x1F (0.5°C), RESOLUSHION_10bit = 0x3F (0.25°C),RESOLUSHION_11bit = 0x5F (0.125°C),RESOLUSHION_12bit = 0x7F (0.0625°C)

#define DELAY_READ_DS18B20 1000										// 9bit-93.75ms; 10-187.5ms; 11-375ms; 12-750ms //время ожидания завершения процесса преобразования температуры датчиками
																									//за время 400мс не успевают конвертировать температуру датчики на паразитом питании.
																									// ЗА 600 не успевает конвертироваться температура с первого датчика (сис. подача)

/**********************    команды для ds18b20     ******************************/
//ROM commands
#define SKIP_ROM 0xCC			//Пропуск ROM (перед групповой командой)
//Functional commands
#define CONVERT_TEMPERATURE 0x44			//convert temperature
#define WRITE_SCRATCHPAD 0x4E	//Write Scratchpad
#define COPY_SCRATCHPAD 0x48	//Copy Scratchpad
#define READ_SCRATCHPAD 0xBE	//Read Scratchpad [BEh])
//

enum { RESOLUSHION_9bit = 0x1F, RESOLUSHION_10bit = 0x3F,RESOLUSHION_11bit = 0x5F,RESOLUSHION_12bit = 0x7F};

//Массив значений температур даттчиков
float temperature[NUMBER_OF_DS18B20];
////unsigned int temperature[NUMBER_OF_DS18B20];

//многомерный массив подключенных датчиков (uint8_t)
/*датчики Тоншаево */
const uint8_t  ds18b20_address[NUMBER_OF_DS18B20][8] = {
	{ 0x28, 0xFF, 0xAD, 0xA7, 0x60, 0x18, 0x02, 0x3B },  // [0]- t1, подача cистемы
	{ 0x28, 0x4D, 0xF7, 0xBF, 0x04, 0x00, 0x00, 0x43 },  // 1- t2, обратка системы
	{ 0x28, 0x97, 0x55, 0xC0, 0x04, 0x00, 0x00, 0xCA },  // 2- t3, отвод трехходового крана ТТК
	{ 0x28, 0x20, 0x0F, 0x40, 0x04, 0x00, 0x00, 0xB9 },  // 3- t4, подача ТТК
	{ 0x28, 0x07, 0xBB, 0x3F, 0x04, 0x00, 0x00, 0xE8 },  // 4- t5, обратка ТТК
	{ 0x28, 0x97, 0x55, 0xC0, 0x04, 0x00, 0x00, 0xCA },  // 5- t6, верх ТА подача от ТТК
	{ 0x28, 0x97, 0x55, 0xC0, 0x04, 0x00, 0x00, 0xCA },  // 6- t7, низ ТА обратка ТТК
	{ 0x28, 0x97, 0x55, 0xC0, 0x04, 0x00, 0x00, 0xCA },  // 7- t8, верх ТА подача в систему
	{ 0x28, 0x97, 0x55, 0xC0, 0x04, 0x00, 0x00, 0xCA },  // 8- t9, отвод трехходового крана системы
	{ 0x28, 0x97, 0x55, 0xC0, 0x04, 0x00, 0x00, 0xCA },  // 9- t10, низ ТА, обратка системы
	{ 0x28, 0xFF, 0x75, 0x61, 0x68, 0x18, 0x01, 0xAC },  // 10-t11, верх ТА
	{ 0x28, 0xFF, 0xF6, 0xDD, 0x67, 0x18, 0x01, 0xF5 },  // 11-t12, низ ТА
	{ 0x28, 0xFF, 0x04, 0x60, 0x68, 0x18, 0x01, 0x83 },  // 12-t13, температура в помещении
	{ 0x28, 0xD9, 0xEA, 0x16, 0xA8, 0x01, 0x3C, 0x10 },  // 13-t14, температура на улице
	{ 0x28, 0xAA, 0x18, 0x40, 0x04, 0x00, 0x00, 0xB1 },  // 14-t15, подача ЭК 
	{ 0x28, 0x97, 0x55, 0xC0, 0x04, 0x00, 0x00, 0xCA },  // 15-t16, температура в спальне //обратка ЭК 0x28, 0x97, 0x55, 0xC0, 0x04, 0x00, 0x00, 0xCA 
};																										 // 0x28, 0xAA, 0x18, 0x40, 0x04, 0x00, 0x00, 0xB1 - датчик в кочегарке

// датчики НН
//const uint8_t  ds18b20_address[NUMBER_OF_DS18B20][8] = {
//	{ 0x28, 0xB8, 0x45, 0xA3, 0x09, 0x00, 0x00, 0x2C },  // 0- t1, подача cистемы
//	{ 0x28, 0xFD, 0x10, 0xC0, 0x04, 0x00, 0x00, 0xFD },  // 1- t2, обратка системы
//	{ 0x28, 0xFF, 0xA8, 0xA5, 0x60, 0x18, 0x02, 0xEE },  // 2- t3, отвод трехходового крана ТТК
//	{ 0x28, 0xFF, 0x19, 0xE1, 0x67, 0x18, 0x01, 0xA1 },  // 3- t4, подача ТТК
//	{ 0x28, 0xFF, 0xf9, 0xA9, 0x60, 0x18, 0x02, 0xA4 },  // 4- t5, обратка ТТК
//	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 5- t6, верх ТА подача от ТТК
//	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 6- t7, низ ТА обратка ТТК
//	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 7- t8, верх ТА подача в систему
//	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 8- t9, отвод трехходового крана системы
//	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 9- t10, низ ТА, обратка системы
//	{ 0x28, 0xFD, 0x10, 0xC0, 0x04, 0x00, 0x00, 0xFD },  // 10-t11, верх ТА
//	{ 0x28, 0xFF, 0xF6, 0xDD, 0x67, 0x18, 0x01, 0xF5 },  // 11-t12, низ ТА
//	{ 0x28, 0xFF, 0x04, 0x60, 0x68, 0x18, 0x01, 0x83 },  // 12-t13, температура в помещении
//	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 13-t14, температура на улице
//	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 14-t15, подача ЭК 
//	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 15-t16, обратка ЭК
//};


// объект для работы со всеми датчиками температуры
OneWire ds18b20(PIN_DS18B20);  
//таймер ожидания процесса преобразования температуры
myCycle TimerConvTemp(DELAY_READ_DS18B20, false); 



//Предварительная инициализация и тестирование всех датчиков температуры (при Debag задается разрядность каждому датчику
int initTemp()
{
	LOG("\nstart (initTemp)\n");
	int result = 0; //коды завершения вызываемых функций
	
	//в режиме отладки определяем адреса всех подключенных датчиков
	if (DEBUG == 1) {
		findAllconnectDS18B20(); //тут же устанавливается разрешение на все найденные датчики
	}
	LOG("\nstop (initTemp)\n");
	return 0;
}


uint16_t getTemperaturDS18B20()
//int getTemperaturDS18B20(OneWire& ds18b20)
{
	/*
	return error codes:
	соответствующий бит равный 1 в переменной errCod сообщает о нарушении при считывании температуры датчика с этим же номером.
	Причем, температуру этого датчика установим заведомо не реальной, равной -99,00 °С
	*/	
	const  bool LOG = false; //выводить логи работы процедуры
	const int NumRepeatRequests = 7; //максимальное количество повторных запросов на считывание температуры с датчика

	if (LOG) { Serial.print("\nstart (getTemperaturDS18B20) "); Serial.println(g_systemDateTime.timestamp(g_systemDateTime.TIMESTAMP_TIME)); }

	//if (LOG) {
	//	Serial.print("0 "); Serial.println(temperature[0]);
	//	Serial.print("1 "); Serial.println(temperature[1]);
	//	Serial.print("2 "); Serial.println(temperature[2]);
	//	Serial.print("3 "); Serial.println(temperature[3]);
	//	Serial.print("4 "); Serial.println(temperature[4]);
	//	Serial.print("5 "); Serial.println(temperature[5]);
	//	Serial.print("6 "); Serial.println(temperature[6]);
	//	Serial.print("7 "); Serial.println(temperature[7]);
	//	Serial.print("8 "); Serial.println(temperature[8]);
	//	Serial.print("9 "); Serial.println(temperature[9]);
	//	Serial.print("10 "); Serial.println(temperature[10]);
	//	Serial.print("11 "); Serial.println(temperature[11]);
	//	Serial.print("12 "); Serial.println(temperature[12]);
	//	Serial.print("13 "); Serial.println(temperature[13]);
	//	Serial.print("14 "); Serial.println(temperature[14]);
	//	Serial.print("15 "); Serial.println(temperature[15]);
	//}
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
	unsigned long startTime = millis();	

		uint16_t errCod = 0;

		byte data[9]; //bufer for scratchpad
		int i;
		int currentNumberOfDS18B20;

		//процедура увеличения счетчика таймера времени конвертации температуры
		TimerConvTemp.check();
		if (!TimerConvTemp.active()) {  //если не запущен процесс преобразования температуры
			if (LOG) {Serial.println("коменда на конвертацию температуры всем DS18B20");}
			//даем команду на преобразование температуры всем датчикам сразу
			if (ds18b20.reset() != 1) {
				errCod = 0xFF; LOG("Error reset 1-Wire bus (May be the sensor not connected?)\n"); 
			}
			ds18b20.write(SKIP_ROM);//обращение ко всем датчикам
			ds18b20.write(CONVERT_TEMPERATURE);// начать преобразование (без паразитного питания) */
			//запускаем таймер ожидания преобразования температуры
			TimerConvTemp.setActive(true);
			}
		else {
			if (TimerConvTemp.go()) { //процесс конвертации температуры завершен, можно считывать
				//остановим и сбросим таймер конвертации
				TimerConvTemp.setActive(false);
				TimerConvTemp.clear();
				////// перезапуск таймера вызова функции чтения температруры.
				////extern myCycle cycleGetTemp;
				////cycleGetTemp.clear();
				////cycleGetTemp.reStart();
				
				//осуществляем проход по всем датчикам и считывание температуры
				currentNumberOfDS18B20 = 0;
				for (auto &dsAddr : ds18b20_address)  //&dsAddr имеет тип 'arr[8]'
				{
					int repeatedRequest = 0;  //считаем количество повторных запросов на считывание температуры

					if (LOG) {
						Serial.print("\nSensor temperature "); Serial.print(currentNumberOfDS18B20);
						Serial.print("\nAddress DS18B20: "); Serial.print(dsAddr[0], HEX); Serial.print(" "); Serial.print(dsAddr[1], HEX); Serial.print(" "); Serial.print(dsAddr[2], HEX); Serial.print(" "); Serial.print(dsAddr[3], HEX); Serial.print(" "); Serial.print(dsAddr[4], HEX); Serial.print(" "); Serial.print(dsAddr[5], HEX); Serial.print(" "); Serial.print(dsAddr[6], HEX); Serial.print(" "); Serial.print(dsAddr[7], HEX); Serial.print("\n");
					}
lblStartReadTemper:
					ds18b20.reset();
					ds18b20.select(dsAddr);
					
					if (LOG) { Serial.print("Read screatchpad: "); }
					ds18b20.write(READ_SCRATCHPAD);

					for (i = 0; i < 9; i++) {// we need 9 bytes
						data[i] = ds18b20.read();	if (LOG) { Serial.print(data[i], HEX); Serial.print(" "); }
					}
					if (ds18b20.crc8(data, 8) != data[8]){
						if (LOG) {
							("\nAnswer with incorrect CRC! Calculation CRC: "); Serial.println(ds18b20.crc8(data, 7), HEX);
							Serial.print("Repeat request temperature "); Serial.println(repeatedRequest);
						}
						if (repeatedRequest < NumRepeatRequests) { repeatedRequest++; goto lblStartReadTemper; }
							
						errCod |= (1<<currentNumberOfDS18B20); //установим битик соответствующий текущему номеру датчика( сдвиг эквивалентен возведению двойки в степень)
						//установим этому датчику нереальную температуру равную -99,00 *С 
						data[1] = 0xF9; data[0] = 0xD0;
						/*currentNumberOfDS18B20++; continue;*/
					}
					else { if (LOG) { Serial.println("\nAnswer CRC is correct"); } }

					//рассчитываем температуру в зависимости от заданного для датчиков разрешения
					int16_t raw = (data[1] << 8) | data[0];
					byte cfg = (data[4] & 0x60); //ыделяем 5 и 6 биты регистра конфигурации, где хранится разрешение датчика
					// at lower resolution, the low bits are undefined, so let's zero them //обнуляем не значащие младшие биты
					if (cfg == 0x00) raw = raw & ~7;		// 9 bit resolution, 93.75 ms
					else if (cfg == 0x20) raw = raw & ~3;	// 10 bit res, 187.5 ms
					else if (cfg == 0x40) raw = raw & ~1;	// 11 bit res, 375 ms
															// default is 12 bit resolution, 750 ms conversion time

					//Save temperature value in the temperature array
				//	/*test*/Serial.print("\nТ сис. до "); Serial.print(temperature[0],4); Serial.println("°C");
					temperature[currentNumberOfDS18B20] =  (float)raw / 16.0;// LOG("Temperature: "); LOG(temperature[currentNumberOfDS18B20]); LOG(" *C: \n");
				//	/*test*/Serial.print("Т сис. после обновления "); Serial.print(temperature[0],4); Serial.println("°C");

					/*//массив для тестирования
					temperature[0] = 10;
					temperature[1] = 11;
					temperature[2] = 12;
					temperature[3] = 13;
					temperature[4] = 14;
					temperature[5] = 15;
					temperature[6] = 16;
					temperature[7] = 17;
					temperature[8] = 18;
					temperature[9] = 19;
					temperature[10] = 20;
					temperature[11] = 21;
					temperature[12] = 22;
					temperature[13] = 23;
					temperature[14] = 24;
					temperature[15] = 25;
					//*/

					currentNumberOfDS18B20++;
					} //for
				// перезапуск таймера вызова функции чтения температруры.
				extern myCycle cycleGetTemp;
				cycleGetTemp.clear();
				cycleGetTemp.reStart();
			}
		}
		if (LOG) { Serial.print("stop (getTemperaturDS18B20) "); Serial.print((millis() - startTime)); Serial.println(" ms"); }
		return errCod;
}


///*
//поиск адресов всех подключенных датчиков. 
//А так же установка разрешения для найденного датчика  
///* возможно тут же получение температуры первый раз */
void findAllconnectDS18B20()
{
	LOG("\nstart (findAllconnectDS18B20)\n");
	// для смены разрешения
	//  ds.reset(); // сброс шины
	//  ds.select(addr2); //выставить адрес
	//  ds.write(0x4E); // разрешение записать конфиг
	//  ds.write(0x7F); // Th контроль температуры макс 128грд
	//  ds.write(0xFF); //Tl контроль температуры мин -128грд
	//  ds.write(0x60); // 0x60 12-бит разрешение, 0x00 -9бит разрешение

	uint8_t addr[8];
	uint8_t data[12];
	uint8_t totalSensor = 0;
	//в цикле ищем адреса всех подключенных датчиков
	while (ds18b20.search(addr))
	{
		//выводим адрес датчика
		for (uint8_t i = 0; i < 8; i++) {
			LOG2(addr[i], HEX); LOG("  ");
		}
		if (OneWire::crc8(addr, 7) != addr[7]) { LOG("   CRC_FAULT "); }
		else { LOG("   CRC_OK "); }
		//установим необходимое разрешение найденного датчика
		ds18b20.reset();
		//device selection
		ds18b20.select(addr);
		ds18b20.write(WRITE_SCRATCHPAD); //Write Scratchpad
		ds18b20.write(0xFF);
		ds18b20.write(0xFF);
		ds18b20.write(RESOLUSHION_DS18B20);   
		ds18b20.write(COPY_SCRATCHPAD); //Запишем блокнот в ПЗУ

		//read scratchpad for check
		ds18b20.reset();
		ds18b20.select(addr);
		ds18b20.write(READ_SCRATCHPAD);
		for (int i = 0; i < 4; i++) { ds18b20.read(); }
		if (ds18b20.read() == RESOLUSHION_DS18B20) { LOG("Resolution_set_OK ("); LOG2(RESOLUSHION_DS18B20, HEX); LOG(")\n"); }
		else { LOG("Resolution_NOT_set!\n"); }

		totalSensor++;
	}
	LOG("Total sensors - "); LOG(totalSensor); LOG("\n");
	LOG("stop (findAllconnectDS18B20)\n");
}

