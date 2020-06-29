#include "Application.h"
#include <crtdbg.h>

int main(){
	Application app;
	app.Run();
	app.Exit();
	_CrtDumpMemoryLeaks(); //To check for memory leaks
}