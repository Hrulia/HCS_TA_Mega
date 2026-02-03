/*
   Функция, возвращающая количество свободной оперативной памяти (SRAM)
	 Примечание: данный способ проверки свободной оперативной памяти
	 работает некорректно в случае фрагментации памяти!
*/

extern int __bss_end;
extern void *__brkval;
// Функция, возвращающая количество свободного ОЗУ
int memoryFree() {
  int freeValue;
  if ((int)__brkval == 0)
    freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);
  return freeValue;
}
