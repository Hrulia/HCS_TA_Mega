// 
// 
// 

#include "Buttons.h"

void scanButtons()
{
	Button buttonReinitTemp(12, 15); // ����� button ��������� ����������� ������� ���������. ����� ���������� 15��*�� ������ ���������� �������. 
	extern  int *pTM1637_3;
	buttonReinitTemp.scanState();

	while (buttonReinitTemp.flagPress) {
		//����������� ������� ����������� �� ����� � ������������ �� ����������
		//*pTM1637_3.showNumberDecEx(g_tRoomSetpoint);

	}





}
