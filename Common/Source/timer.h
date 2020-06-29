#pragma once
#include <windows.h>

class StopWatch{
	private:
		LARGE_INTEGER frequency, prevTime, currTime;
		double LIToSecs(LARGE_INTEGER&);
		UINT wTimerRes;
	public:
		StopWatch();
		~StopWatch();
		void startTimer(), waitUntil(long long);
		double getElapsedTime(); //Get time in seconds since the last call to this function 
};