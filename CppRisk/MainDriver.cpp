#include <iostream>
#include <string>
#include <direct.h>
#include <crtdbg.h>
#include "GameEngine.h"
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>


// Declare test functions
void testCommandProcessor();
void testStartupPhase(GameEngine& engine);
void testMainGameLoop(GameEngine& engine);
void testOrderExecution();
void testLoggingObserver();


int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
	
	 char directory[FILENAME_MAX];
    _getcwd(directory, FILENAME_MAX);
    std::cout << "--- CURRENT WORKING DIRECTORY ---" << std::endl;
    std::cout << directory << std::endl;
    std::cout << "--- Place MAP.txt in the folder above ---\n" << std::endl;

    char buffer[256];
	GameEngine engine;
	GameEngine::initializeRiskFSM(engine);

    
	testStartupPhase(engine);
	std::cout << "\n\nFinished testing Startup Phase. Enter any character to proceed to the next test. ";
	std::cin >> buffer;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


	    return 0;

   
}
