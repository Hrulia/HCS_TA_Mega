// 
// 
// 

#include "Net.h"
#include ".\libraries\SPI\src\SPI.h"
//#include ".\libraries\Ethernet\src\Ethernet.h"
#include <Ethernet.h>

uint8_t mac[]= { 0xAE, 0xB2, 0x26, 0xE4, 0x4A, 0x5C };		// MAC-�����
byte ip[] = { 192, 168, 0, 177 };		// IP-�����
//byte _Dns[4];	// ����� DNS-�������
//byte _gateway[4];// ����� �������� �����
//byte _subnet[4];	// ����� �������
uint16_t serverPort=7777; //���� ��� ������� �������

// Initialize the Ethernet server library
// with the IP address and port you want to use
EthernetServer server(serverPort);


//�������������� ������� ������
int initNet(){
	LOG("\nstart (initNet)\n");
	int errCod = 0;

	// ������ ���� ��� �������� ����������
	//server(serverPort);

	// start the Ethernet connection and the server:
	// Ethernet.begin(mac, ip, myDns, gateway, subnet);  // ������������� �����������
	Ethernet.begin(mac, ip);
	////���� ����� DHCP
	////if (Ethernet.begin(mac) == 0) {
	////	Serial.println("Failed to configure using DHCP");
	////	while (true); // �������� �� ������
	////}

	// ���������� ������ ������ �������� �������� ����������.
	server.begin();
	LOG("Server start is at ");
	LOG(Ethernet.localIP());  LOG(":");LOG(serverPort); LOG("\n");

	LOG("stop (initNet)\n");
	return errCod;
}

//��������� �������� �� Net ��������
int NetRequests() {
	//Serial.print("\nstart (NetRequests)\n");
	int errCod = 0;

	// listen for incoming clients
	EthernetClient client = server.available();
	if (client) {
		Serial.print("Connect new client"); Serial.print("\n");
		while (client.connected()){
			uint8_t buf[5] = { 0,0,0,0,0 }; //�������� ����� ��� ����������� ������� (������ ������ 5 ����)
			if (client.available() >= 1){
				Serial.println("Available: "); Serial.println(client.available());
				//read one byte
				buf[0] = client.read();
				Serial.println("bufer[0]: "); Serial.println((char)buf[0]);

				// parsing the request and response
				//    if (buf[0] == "#" && buf[0]^buf[1]^buf[2]^buf[3]^0x5E == buf[5]) //with check CS
				if (buf[0] == '#'){ //������� ������ ������� �������
					//finishing read the rest bytes of request
					client.read((uint8_t*)(buf + 1), 4);
					////for (int i = 1; i < 5; i++)
					////	{
					////		buf[i] = client.read();
					////	}
					Serial.println("bufer all: ");
					Serial.println((char*)buf);

					Serial.println("bufer[1] "); Serial.println((char)buf[1]);
					switch (buf[1]) 
					{
						case 'T': //������ ����������� �� �������
						{
							int sensorNumber = buf[2];
							uint8_t answer[9] = { 0x24,0x54,buf[2],buf[3] };
							
							////�������� float ����� uart. http//avr-start.ru/?p=4557
							////float adc_data = 0.333;
							////uint8_t i = 0;
							////uint8_t *ptr = (uint8_t *)&adc_data;
							////while (i < 4)
							////{
							////	uart_putchar(*ptr); //��������� ����
							////	ptr++; //������� �� ���������
							////	i++; //��������� �������
							////}	

							temperature[sensorNumber] = 22.33;	//delete	
							//��������� �������� 1 ���� �� �������� ���������� � ������� float(����������� 4 �����)
							uint8_t *ptr = (uint8_t *)&temperature[sensorNumber];
							
							answer[4] = (*ptr);		//1����
							answer[5] = (*(++ptr));	//2���� � �.�.
							answer[6] = (*(++ptr));
							answer[7] = (*(++ptr));	
							
							/**/
							//������ �� ������� ���������
							float ptrFlout = *(float*)&answer[4];
							Serial.print("temperature: "); Serial.println(ptrFlout);
							/**/

							answer[8] = calcCS(answer, 9);
							client.write(answer,9);

							Serial.println("Respons");
							Serial.write(answer, 9);
							Serial.println();

							break;
						}
						//case 't': //������ ����������� �� ������� � ����� � ASCII ��������
						//{//Serial.println ("floattemperature");
						//	float temperature; //����������� � ������� DS18B20 2 ����
						//	int itemSensor; //����� ������a
						//	itemSensor = buf[2] - 0x30;
						//	temperature = fget_temperatur_ds18b20(ds18b20_addres[itemSensor], DS18B20_PIN);
						//	Serial.print("$t"); Serial.write(buf[2]); Serial.write(buf[3]); Serial.print(": "); Serial.print(temperature); Serial.print("�C\n");
						//	break;
						//}
						//case 'D': //Find all DS18B20
						//{
						//	//find_DS18B20(DS18B20_PIN);
						//	byte addr[8];
						//	ds18b20.begin(DS18B20_PIN);
						//	ds18b20.reset_search();
						//	while (ds18b20.search(addr))
						//	{
						//		Serial.write(addr, sizeof(addr));
						//		//Serial.print("\n");
						//	}
						//	break;
						//}
						default:Serial.println("Unknown command");
					}
					//disconnecting the client
					client.stop();
					Serial.println("The client disconnected");
				}
			}
		}
	}
		// ���������� ������� ������ ������� ��������� �������� �������� �� ���� �� ��������.
		extern myCycle cycleNetRequests;
		cycleNetRequests.clear();
		cycleNetRequests.reStart();

		//Serial.println("\nstop (NetRequests)");
		return errCod;
}
	

	//calculating Control Sum
	byte calcCS(byte* array1, int n)
	{
		//Serial.println("Calculating CRC");
		byte i = 0;
		byte tempCS = 0x5E;
		while (i < n)
		{
			tempCS ^= array1[i];
			i++;
		}
		//Serial.print("CRC "); Serial.println((char)tempCS);
		return tempCS;
	}
