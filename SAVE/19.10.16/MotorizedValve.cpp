// 
// 
// 

#include "MotorizedValve.h"

// 0- t1, подача cистемы
// 1- t2, обратка системы
// 2- t3, отвод трехходового крана ТТ
// 3- t4, подача ТТК
// 4- t5, обратка ТТК
// 5- t6, верх ТА подача от ТТК
// 6- t7, низ ТА обратка ТТК
// 7- t8, верх ТА подача в систему
// 8- t9, отвод трехходового крана системы
// 9- t10, низ ТА, обратка системы
// 10-t11, верх ТА
// 11-t12, низ ТА
// 12-t13, температура в помещении
// 13-t14, температура на улице
// 14-t15, подача ЭК 
// 15-t16, обратка ЭК

const int tcrash = 90;	//аварийная температура(90 *С)
const int tdanger = 80;	//предаварийная температура(80 *С)
const int tneed = 50;	//требуемая температура обратки(50 * С)

int t_flueGas = 200; //температура дымовых газов ТТК котла  


extern bool failure; //сигнал наличия состояния аварийного перегрева котла




extern unsigned long timeSwitchValveTTK;//*******************************************************************




//Регулировка и поддержание оптимальной температуры ТТК (защита от холодной обратки)
int temperatureControlTTK() {
	unsigned long startTime = millis();	LOG("\nstart (temperatureControlTTK)\n");
	extern float temperature[];
	if (failure || (t_flueGas > 60) || ((int)temperature[3] > ((int)temperature[11] + 10))) { //котел топиться или вода в подаче котла нагрелась более чем на 10 *С выше температуры воды низа ТА
		//запуск насоса ТТК
		digitalWrite(PIN_PUMP_TTK, LOW); //active level - LOW
	}
	else{//остановка насоса ТТК
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


	if ((int)temperature[3] > tdanger) { //предаварийный перегрев котла
		if ((int)temperature[3] > tcrash) { //аварийный перегрев котла
			//включаем зуммер сирены
			///tone(PIN_EMERGENCY_SIREN, 3000, 300000);
			failure = true;
		}
		//полностью закрываем боковой отвод крана котла ТТК
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		//запуск насоса ТТК и системы
		digitalWrite(PIN_PUMP_TTK, LOW);
		digitalWrite(PIN_PUMP_SYS, LOW);
		return 0;
	}
	failure = false;
	if (((int)temperature[4] > (tneed + 3)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_ClOSE))) {// обратка горячая призакрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
		
		Serial.print("Peregrev obratki"); Serial.println("");
		
		digitalWrite(PIN_VALVE_TTK_ClOSE, LOW);//active level - LOW
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		delay((timeSwitchValveTTK) / 25);
		digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
		digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
	}
	else {
		Serial.print("else Peregrev obratki"); Serial.println("");
			if (((int)temperature[4] < (tneed -3)) && (digitalRead(PIN_VALVE_TTK_SIGNAL_OPEN))) {// обратка холодная приоткрываем (боковой отвод) кран на 1/25 максимального времени переключения крана
				
				Serial.print("Nedogrev obratki"); Serial.println("");
				
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);//active level - LOW
				digitalWrite(PIN_VALVE_TTK_OPEN, LOW);
				delay(timeSwitchValveTTK / 25);
				digitalWrite(PIN_VALVE_TTK_ClOSE, HIGH);
				digitalWrite(PIN_VALVE_TTK_OPEN, HIGH);
		}

	}
	// перезапуск таймера вызова функции регулировки температуры.
	extern myCycle cycleTempControlTTK;
	cycleTempControlTTK.clear();
	cycleTempControlTTK.reStart();
	LOG("stop (temperatureControlTTK) "); LOG((millis() - startTime)); LOG(" mc\n");
};