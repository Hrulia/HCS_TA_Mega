// SystemTime.h

#ifndef _SYSTEMTIME_h
#define _SYSTEMTIME_h

#include "HeaderHCS_TA.h"
//#include ".\libraries\RTClib\RTClib.h"

int initSystemTime();

void ISR_UpdateSystemTime();

#endif

