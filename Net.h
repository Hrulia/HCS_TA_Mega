// Net.h

#ifndef _NET_h
#define _NET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "HeaderHCS_TA.h"

//������ ���������� �� �������� DS18B20. ���������� �� =  NUMBER_OF_DS18B20
// [0]- t1, ������ c������
// [1]- t2, ������� �������
// [2]- t3, ����� ������������ ����� ���
// [3]- t4, ������ ���
// [4]- t5, ������� ���
// [5]- t6, ���� �� ������ �� ���
// [6]- t7, ��� �� ������� ���
// [7- t8, ���� �� ������ � �������
// [8]- t9, ����� ������������ ����� �������
// [9]- t10, ��� ��, ������� �������
// [10]-t11, ���� ��
// [11]-t12, ��� ��
// [12]-t13, ����������� � ���������
// [13]-t14, ����������� �� �����
// [14]-t15, ������ �� 
// [15]-t16, ������� ��
extern float temperature[];


	 //byte _mac[6];	// MAC-�����
	 //byte _ip[4];	// IP-�����
	 //byte _Dns[4];	// ����� DNS-�������
	 //byte _gateway[4];	// ����� �������� �����
	 //byte _subnet[4];	// ����� �������
	 //uint16_t _serverPort; //���� ��� ������� �������

	 //�������������� ������ ������� ������
	int initNet();

	//��������� �������� �� Net ��������
	int NetRequests();

	//calculate Control Sum
	byte calcCS(byte* array1, int n);

//
//class NetClass
//{
//
// protected:
//	 ///byte _mac[6];		// MAC-�����
//	 ///byte _ip[4];		// IP-�����
//	 //byte _Dns[4];	// ����� DNS-�������
//	 //byte _gateway[4];// ����� �������� �����
//	 //byte _subnet[4];	// ����� �������
//	 uint16_t _serverPort; //���� ��� ������� �������
//
// public:
//	 NetClass();
//
//
//	void init();
//};

//extern NetClass Net;

#endif

