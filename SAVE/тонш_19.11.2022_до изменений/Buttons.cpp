// 
// 
// 

#include "Buttons.h"

void scanButtons()
{
	Button buttonReinitTemp(12, 15); // класс button позволяет фильтровать дребезг контактов. время фильтрации 15мс*на период повторения функции. 
	extern  int *pTM1637_3;
	buttonReinitTemp.scanState();

	while (buttonReinitTemp.flagPress) {
		//увеличиваем целевую температуру по кругу с отображением на индикаторе
		//*pTM1637_3.showNumberDecEx(g_tRoomSetpoint);

	}





}
