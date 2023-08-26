/*
    Name:       HCS_TA.ino
    Created:	19.08.2019 11:42:45
    Author:     Serega_nout\Sergey
*/

//#include <EEPROM.h>
///#include <Ethernet.h>
///#include "Net.h"
//#include <RTClib.h>
//
//#include <Adafruit_SPIDevice.h>
//#include <Adafruit_I2CRegister.h>
//#include <Adafruit_BusIO_Register.h>
//#include <Adafruit_I2CDevice.h>


#include "Pins.h"   //тут собраны все пины.

#include "MotorizedValve.h"
#include "HeaderHCS_TA.h"
#include "Temp_DS18B20.h"
#include "Indicator_TM1637.h"


//текущее время/дата в системе 
volatile DateTime g_systemDateTime;

//моторизированные краны
	/*static*/ unsigned long g_timeSwitchValveTTK = 0; //мс, максимальное полное время переключения моторизованного крана ТТК из одного положения в другое
	/*static*/ unsigned long g_timeSwitchValveSYS = 0; //мс, максимальное полное время переключения моторизованного крана Системы из одного положения в другое

//насосы
	///*static*/ bool  g_pumpTTK = false; //переменная состояния насоса ТТК
	///*static*/ bool  g_pumpSYS = false; //переменная состояния насоса системы
	///*static*/ extern bool  g_pumpЭК = false; //переменная состояния насоса электрического котла ()

//котел ТТК
	/*static*/ bool g_failure = false; //сигнал наличия состояния аварийного перегрева котла ТТК
	float g_t_flueGases = 0; //температура дымовых газов ТТК котла 

//целевое значение температуры помещения 
	/*uint8_t*/float g_tRoomSetpoint =  (float)EEPROM.read(0x00);
//Целевое значение температуры помещения с учетом суточного расписания (именно к этому значению стремимся при регулировке)
	float g_RoomSetpointCurrent;

//суточное расписание для целевой температуры (множители)

// Cycles
	myCycle cycleDispSystemTime(MS_250, true);		//0,25 сек. Вывод информации о системном времени на индикатор ТМ1637
	myCycle cycleGetTemp(MS_13S, true);						//15c, 5c цикл считывания температуры (MS_05S)
	myCycle cycleDispTempTM1637(MS_03S, true);		//2c, 150мc цикл обновления показаний температуры на индикаторах TM1637, такая частось, чтобы точки на индикаторе часов "тикали", мигали плавно каждую секунду 
	myCycle cycleTempControlTTK(MS_10S, true);		//8.5c, при 6 сек. показания термопары 0гр. Цикл запуска управления трехходовым краном ТТК (защита от холодной обратки)
	myCycle cycleTempControlSYS(MS_03M, true);		//была 60 cделал 180. 60с цикл запуска управления трехходовым краном системы 

	myCycle cyclecheckMegaAndESP(MS_01M, true);			//1мин цикл отправки в esp через serial3 команды своего присутствия: ?mega=1 и проверки поступления команды присутствия ESP


	// Serialprint
	void StreamPrint_progmem(Print &out, PGM_P format, ...) {
		// program memory version of printf - copy of format string and result share a buffer so as to avoid too much memory use
		char formatString[128], *ptr;
		strncpy_P(formatString, format, sizeof(formatString)); // copy in from program mem
																													 // null terminate - leave last char since we might need it in worst case for result's \0
		formatString[sizeof(formatString) - 2] = '\0';
		ptr = &formatString[strlen(formatString) + 1]; // our result buffer...
		va_list args;
		va_start(args, format);
		vsnprintf(ptr, sizeof(formatString) - 1 - strlen(formatString), formatString, args);
		va_end(args);
		formatString[sizeof(formatString) - 1] = '\0';
		out.print(ptr);
	}

	//Использование этих дефайнов приводит к снижению потребления ОЗУ. (но это не точно), взято из проекта ams)
#define Serialprint(format, ...) StreamPrint_progmem(Serial,PSTR(format),##__VA_ARGS__)
#define Streamprint(stream,format, ...) StreamPrint_progmem(stream,PSTR(format),##__VA_ARGS__)


// The setup() function runs once each time the micro-controller starts
void setup()
{
	unsigned long startTime = millis();
	Serial.begin(9600);
	Serial.println();
	Serial.println();
	Serial.println("*******SETUP START***********");
	Serial.println();
	Serial.println();


	LOG("\nstart (setup)\n");
	
	pinMode(LED_BUILTIN, OUTPUT);
	//pinMode(46, OUTPUT);
	//digitalWrite(46, HIGH);

	//выход сирены аварийного перегрева котла
	pinMode(PIN_EMERGENCY_SIREN, OUTPUT); 
	digitalWrite(PIN_EMERGENCY_SIREN, HIGH);

	//настраиваем выходы управления насосами (по умолчанию насосы включены НЗ контактами реле)
	//ТТК
	pinMode(PIN_PUMP_TTK, OUTPUT);
	digitalWrite(PIN_PUMP_TTK, HIGH); //active level - LOW
	//Система
	pinMode(PIN_PUMP_SYS, OUTPUT);
	digitalWrite(PIN_PUMP_SYS, HIGH); //active level - LOW
	//Электрокотел, насос и сам котел
	pinMode(PIN_PUMP_EK, OUTPUT);
	digitalWrite(PIN_PUMP_EK, HIGH); //active level - LOW
	pinMode(PIN_CONTROL_EK, OUTPUT);
	digitalWrite(PIN_CONTROL_EK, HIGH); //active level - LOW

	//Выход подачи сигнала сброса ESP
	pinMode(PIN_RESET_ESP, OUTPUT);
	digitalWrite(PIN_RESET_ESP, HIGH);

	//инициализация RTC
	initSystemTime();

	//инициализация индикаторов TM1637
	init_TM1637(); //void

	//////load data from EEPROM Информация о целевой температуре системы (настройка и сохранение этой температуры в процедуре "indicator_TM1637_Output_temperature") 
	////g_tRoomSetpoint = (float)EEPROM.read(0x00); 


	//Инициализация датчиков температуры
	initTemp(); 

	//Тестирование моторизированных кранов
	initValve(); 

	//Настройка rx3/tx3 интерфейса
	initRxTxForNetSharing(9600);

	//Настройка rx2/tx2 интерфейса. Тестовый прием от Serial3
	//initRxTxForNetSharing2(115200);

	LOG("stop (setup) "); LOG((millis() - startTime)); LOG(" mc\n");
}

// Add the main program code into the continuous loop() function
void loop()
{
	unsigned long startTime = millis();
	//LOG("\nstart (loop)\n");
	int result = 0; //коды завершения вызываемых функций

	//////запуск проверки истечения циклов
	////timersWorks(); ///убрать при чеке само проверяется

	//ShowFPS(); //подсчет количества циклов loop за одну секунду.

	//обновление информации о системном времени на TM1637
	if (cycleDispSystemTime.check()) {
		result = indicator_TM1637_Output_time();
		if (result) {
			LOG("Error in indicator_TM1637_Output_time "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		}
	}

	//обновление информации с датчиков температуры
	if (cycleGetTemp.check()) {
		result = getTemperaturDS18B20();
		if (result) {
			LOG("Error in getTemperaturDS18B20 "); LOG2((uint16_t)result, BIN); LOG("\n");
			//обработка ошибки
			//...
		}
	}

	//Обновление информации о температуре на индикаторах TM1637
	if (cycleDispTempTM1637.check()) {
		result = indicator_TM1637_Output_temperature();
		if (result) {
			LOG("Error in indicator_TM1637_Output_temperature "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		} 
	}

	//управление краном ТТК (защита от холодной обратки)
	if (cycleTempControlTTK.check()) {
		result = temperatureControlTTK();
		if (result) {
			LOG("Error in temperatureControlTTK "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		}
	}

	//управление краном системы
	if (cycleTempControlSYS.check()) {
		result = temperatureControlSYS();
		if (result) {
			LOG("Error in temperatureControlSYS "); LOG(result); LOG("\n");
			//обработка ошибки
			//...
		}
	}

	// Обработка запросов поступающих от модуля ESP8266
		checkSerial();  //проверяем поступление команды по порту uart №3 от ESP

	// отпарвки команды своего присутствия на ESP через Serial3
	if (cyclecheckMegaAndESP.check()) {
		checkMegaAndESP();  //Отправка на модуль ESP команды о своем нормальном функционировании. Проверка нормального функционирования ESP.
		// перезапуск таймера вызова функции.
		cyclecheckMegaAndESP.clear();
		cyclecheckMegaAndESP.reStart();
	}

	digitalWrite(LED_BUILTIN, (digitalRead(LED_BUILTIN)?LOW:HIGH));
	
	//LOG("stop (loop) "); LOG((millis() - startTime)); LOG(" mc\n");
}

void ShowFPS()
{
	static uint32_t tm_m = 0;
	static uint32_t cnt_m = 0;
	cnt_m++;
	if ((millis() - tm_m) > 1000)
	{
		Serial.print("loop per sec: "); Serial.println(cnt_m);
		cnt_m = 0;
		tm_m = millis();
	}
}