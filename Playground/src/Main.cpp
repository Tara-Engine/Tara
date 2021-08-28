/*
A Tara application, for testing purposes.


*/
#include "Tara.h"



int main(int argc, char** argv) {
	Tara::Application::Get()->Init(1200, 700, "Tara Playground Application!");
	Tara::Application::Get()->Run();
	return 0;
}