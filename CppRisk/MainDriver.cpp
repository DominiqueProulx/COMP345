#include <iostream>
#include <string>
#include <direct.h>
// #define _CRTDBG_MAP_ALLOC
// #include <cstdlib>
// #include <crtdbg.h>

// Declare test functions
void testCommandProcessor();
//void testStartupPhase();
//void testMainGameLoop();
void testOrderExecution();
//void testLoggingObserver();

std::string buffer{};

int main(void)
{
	char buffer[FILENAME_MAX];
	_getcwd(buffer, FILENAME_MAX);
	std::cout << "--- CURRENT WORKING DIRECTORY ---" << std::endl;
	std::cout << buffer << std::endl;
	std::cout << "--- Place MAP.txt in the folder above ---\n" << std::endl;

	// run all driver test functions
	testCommandProcessor();
	std::cout << "\n\nFinished testing COMMAND PROCESSOR. Enter any character to proceed to the next test. ";
	std::cin >> buffer;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	/*
	testStartupPhase();
	std::cout << "\n\nFinished testing STARTUP PHASE. Enter any character to proceed to the next test. ";
	std::cin >> buffer;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	*/

	/*
	testMainGameLoop();
	std::cout << "\n\nFinished testing MAIN GAME LOOP. Enter any character to proceed to the next test. ";
	std::cin >> buffer;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	*/

	testOrderExecution();
	std::cout << "\n\nFinished testing ORDER EXECUTION. Enter any character to proceed to the next test. ";
	std::cin >> buffer;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	/*
	testLoggingObserver();
	std::cout << "\n\nFinished testing LOGGING OBSERVER. Enter any character to end the program. ";
	std::cin >> buffer;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	*/

	return 0;
}
