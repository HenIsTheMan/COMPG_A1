#pragma once
#include "timer.h"

class Application{
	public:
		Application();
		~Application();
		bool hideCursor;
		void Run(), Exit();
		static bool IsKeyPressed(unsigned short);
	private:
		StopWatch m_timer;
};