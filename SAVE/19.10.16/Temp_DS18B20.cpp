
#include "Temp_DS18B20.h"

//#include "myCycle.h"
//#include ".\libraries\OneWire\OneWire.h"


#define NUMBER_OF_DS18B20 16//Number of sensor connected

enum { RESOLUSHION_9bit = 0x1F, RESOLUSHION_10bit = 0x3F,RESOLUSHION_11bit = 0x5F,RESOLUSHION_12bit = 0x7F};
#define RESOLUSHION_DS18B20 RESOLUSHION_11bit //1F-9bit, 3F-10bit, 5F-11bit, 7F-12bit
#define DELAY_READ_DS18B20 400 // 9bit-93.75ms; 10-187.5ms; 11-375ms; 12-750ms 


/*������� ��� ds18b20*/
//ROM commands
#define SKIP_ROM 0xCC			//������� ROM (����� ��������� ��������)
//Functional commands
#define CONVERT_TEMPERATURE 0x44			//convert temperature
#define WRITE_SCRATCHPAD 0x4E	//Write Scratchpad
#define COPY_SCRATCHPAD 0x48	//Copy Scratchpad
#define READ_SCRATCHPAD 0xBE	//Read Scratchpad [BEh])
//


//������ �������� ���������� ���������
float temperature[NUMBER_OF_DS18B20];
////unsigned int temperature[NUMBER_OF_DS18B20];

//����������� ������ ������������ �������� (uint8_t)
const uint8_t  ds18b20_address[NUMBER_OF_DS18B20][8] = {
	{ 0x28, 0xB8, 0x45, 0xA3, 0x09, 0x00, 0x00, 0x2C },  // 0- t1, ������ c������
	{ 0x28, 0xFD, 0x10, 0xC0, 0x04, 0x00, 0x00, 0xFD },  // 1- t2, ������� �������
	{ 0x28, 0xFF, 0xA8, 0xA5, 0x60, 0x18, 0x02, 0xEE },  // 2- t3, ����� ������������ ����� ���
	{ 0x28, 0xFF, 0x09, 0x64, 0x68, 0x18, 0x01, 0x61 },  // 3- t4, ������ ���
	{ 0x28, 0xFF, 0x19, 0xE1, 0x67, 0x18, 0x01, 0x1A },  // 4- t5, ������� ���
	{ 0x28, 0xFF, 0xF9, 0xA9, 0x60, 0x18, 0x02, 0xA4 },  // 5- t6, ���� �� ������ �� ���
	{ 0x28, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 6- t7, ��� �� ������� ���
	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 7- t8, ���� �� ������ � �������
	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 8- t9, ����� ������������ ����� �������
	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 9- t10, ��� ��, ������� �������
	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 10-t11, ���� ��
	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 11-t12, ��� ��
	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 12-t13, ����������� � ���������
	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 13-t14, ����������� �� �����
	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 14-t15, ������ �� 
	{ 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 15-t16, ������� ��
}; 


OneWire ds18b20(PIN_DS18B20);  // ������ ��� ������ �� ����� ��������� �����������
myCycle TimerConvTemp(DELAY_READ_DS18B20, false); //������ �������� �������� �������������� �����������

////void findAllconnectDS18B20();

//��������������� ������������� � ������������ ���� �������� �����������
int initTemp()
{
	LOG("\nstart (initTemp)\n");
	int result = 0; //���� ���������� ���������� �������
	
	//� ������ ������� ���������� ������ ���� ������������ ��������
	if (DEBUG == 1) {
		findAllconnectDS18B20(); //��� �� ��������������� ���������� �� ��� ��������� �������
	}

	//��������� �������
	//�������� CRC � �������� ������
	//////uint8_t calcCRC;
	//////calcCRC = ds18b20.crc8(ds, 7);
	//////if (ds18b20.crc8(ds, 7) != ds[7])
	//////{
	//////	LOG("CRC ��������� � � ������ �� �����! "); LOG2(ds18b20.crc8(ds, 7), HEX); LOG(" - "); LOG2(ds[7], HEX); LOG("\n");
	//////	temperature[currentNumberOfDS18B20] = 777; continue;
	//////}
	//////else
	//////{
	//////	LOG("CRC OK (");  LOG2(ds[7], HEX); LOG(")\n");
	//////}

	////	result = getTemperaturDS18B20(ds18b20);
	////�������////if (result) {
	////	Serial.print("Error in... "); Serial.print(result); ; LOG("\n");  //��������� ������
	////}

	LOG("\nstop (initTemp)\n");
	return 0;
}


int getTemperaturDS18B20()
//int getTemperaturDS18B20(OneWire& ds18b20)
{
	/*
	return error codes:
	��������������� ��� ������ 1 � ���������� errCod �������� � ��������� ��� ���������� ����������� ������� � ���� �� �������.
	������, ����������� ����� ������� ��������� �������� �� ��������, ������ -99,00*�
	*/
	unsigned long startTime = millis();	LOG("\nstart (getTemperaturDS18B20)\n");
		uint16_t errCod = 0;

		byte data[9]; //bufer for scratchpad
		int i;
		int currentNumberOfDS18B20;

		//��������� ���������� �������� �������
		TimerConvTemp.check();
		if (!TimerConvTemp.active()) {  //���� �� ������� ������� �������������� �����������
			//���� ������� �� �������������� ����������� ���� �������� �����
			if (ds18b20.reset() != 1) {
				errCod = 0xFF; LOG("Error reset 1-Wire bus (May be the sensor not connected?)\n"); 
			}
			ds18b20.write(SKIP_ROM);//��������� �� ���� ��������
			ds18b20.write(CONVERT_TEMPERATURE);// ������ �������������� (��� ����������� �������) */
			//��������� ������ �������� �������������� �����������
			TimerConvTemp.setActive(true);
			}
		else {
			if (TimerConvTemp.go()) { //������� ����������� ����������� ��������, ����� ���������
				//��������� � ������� ������ �����������
				TimerConvTemp.setActive(false);
				TimerConvTemp.clear();
				// ���������� ������� ������ ������� ������ ������������.
				extern myCycle cycleGetTemp;
				cycleGetTemp.clear();
				cycleGetTemp.reStart();
				
				//������������ ������ �� ���� �������� � ���������� �����������
		currentNumberOfDS18B20 = 0;
		for (auto &dsAddr : ds18b20_address)  //&dsAddr ����� ��� 'arr[8]'
		{
			LOG("\nSensor temperature "); LOG(currentNumberOfDS18B20); 
			LOG("\nAddress DS18B20: "); LOG2(dsAddr[0], HEX); LOG(" "); LOG2(dsAddr[1], HEX); LOG(" "); LOG2(dsAddr[2], HEX); LOG(" "); LOG2(dsAddr[3], HEX); LOG(" "); LOG2(dsAddr[4], HEX); LOG(" "); LOG2(dsAddr[5], HEX); LOG(" "); LOG2(dsAddr[6], HEX); LOG(" "); LOG2(dsAddr[7], HEX); LOG("\n");
			ds18b20.reset();
			ds18b20.select(dsAddr);
			LOG("Read screatchpad: ");
			ds18b20.write(READ_SCRATCHPAD);

			for (i = 0; i < 9; i++) {// we need 9 bytes
				data[i] = ds18b20.read();	LOG2(data[i], HEX); LOG(" ");
			}
			if (ds18b20.crc8(data, 8) != data[8]){
				LOG("\nAnswer with incorrect CRC! Calculation CRC: "); LOG2(ds18b20.crc8(data, 7), HEX); LOG("\n");
				errCod |= (1<<currentNumberOfDS18B20); //��������� ����� ��������������� �������� ������ �������( ����� ������������ ���������� ������ � �������)
				//��������� ����� ������� ���������� ����������� ������ -99,00 *� 
				data[1] = 0xF9; data[0] = 0xD0;
				/*currentNumberOfDS18B20++; continue;*/
			}
			else { LOG("\nAnswer CRC is correct\n"); }

			//������������ ����������� � ����������� �� ��������� ��� �������� ����������
			int16_t raw = (data[1] << 8) | data[0];
			byte cfg = (data[4] & 0x60); //������� 5 � 6 ���� �������� ������������, ��� �������� ���������� �������
			// at lower resolution, the low bits are undefined, so let's zero them //�������� �� �������� ������� ����
			if (cfg == 0x00) raw = raw & ~7;		// 9 bit resolution, 93.75 ms
			else if (cfg == 0x20) raw = raw & ~3;	// 10 bit res, 187.5 ms
			else if (cfg == 0x40) raw = raw & ~1;	// 11 bit res, 375 ms
													// default is 12 bit resolution, 750 ms conversion time

			//float celsius = (float)raw / 16.0;
			//float celsius2= 1.0 * raw/ 16 + (float(raw % 16)) * 1.0 / 16;   // (1.0 * raw/ 16)-'��� ����� �����. ������������ (raw>>4)   /*����������� �� ���������!*/

			//Save temperature value in the temperature array
			temperature[currentNumberOfDS18B20] =  (float)raw / 16.0; LOG("Temperature: "); LOG(temperature[currentNumberOfDS18B20]); LOG(" *C: \n");

			////temperature[currentNumberOfDS18B20] = (raw*100) / 16; LOG("Temperature: "); LOG(temperature[currentNumberOfDS18B20]); LOG(" *C: \n");
			////LOG("raw: "); LOG(raw); LOG("\n");

			////LOG("TemperatureFloat: "); LOG((float)raw / 16.0); LOG("\n");

			currentNumberOfDS18B20++;
			} //for
		
			}



		}
		LOG("stop (getTemperaturDS18B20) "); LOG((millis() - startTime)); LOG(" mc\n");
		return errCod;
}


/*
����� ������� ���� ������������ ��������. 
� ��� �� ��������� ���������� ��� ���������� �������  
/* �������� ��� �� ��������� ����������� ������ ��� */
void findAllconnectDS18B20()
{
	LOG("\nstart (findAllconnectDS18B20)\n");
	// ��� ����� ����������
	//  ds.reset(); // ����� ����
	//  ds.select(addr2); //��������� �����
	//  ds.write(0x4E); // ���������� �������� ������
	//  ds.write(0x7F); // Th �������� ����������� ���� 128���
	//  ds.write(0xFF); //Tl �������� ����������� ��� -128���
	//  ds.write(0x60); // 0x60 12-��� ����������, 0x00 -9��� ����������

	uint8_t addr[8];
	uint8_t data[12];
	uint8_t totalSensor = 0;
	//� ����� ���� ������ ���� ������������ ��������
	while (ds18b20.search(addr))
	{
		//������� ����� �������
		for (uint8_t i = 0; i < 8; i++) {
			LOG2(addr[i], HEX); LOG("  ");
		}
		if (OneWire::crc8(addr, 7) != addr[7]) { LOG("   CRC_FAULT "); }
		else { LOG("   CRC_OK "); }
		//��������� ����������� ���������� ���������� �������
		ds18b20.reset();
		//device selection
		ds18b20.select(addr);
		ds18b20.write(WRITE_SCRATCHPAD); //Write Scratchpad
		ds18b20.write(0xFF);
		ds18b20.write(0xFF);
		ds18b20.write(RESOLUSHION_DS18B20);   
		ds18b20.write(COPY_SCRATCHPAD); //������� ������� � ���

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

