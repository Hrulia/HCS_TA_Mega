// SystemManager.h

#ifndef _SYSTEMMANAGER_h
#define _SYSTEMMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SystemManagerClass
{
 protected:



 public:
	void init();
};

extern SystemManagerClass SystemManager;

#endif

