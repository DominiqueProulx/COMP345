#include <iostream>
#include <string>
#include <direct.h>
#include "GameEngine.h"
#include <memory>

// Declare test functions
void testCommandProcessor();
void testStartupPhase(GameEngine& engine);
void testMainGameLoop(GameEngine& engine);
void testOrderExecution();
void testLoggingObserver();
void testPlayerStrategy();

int main(void)
{
    char directory[FILENAME_MAX];
    _getcwd(directory, FILENAME_MAX);
    std::cout << "--- CURRENT WORKING DIRECTORY ---" << std::endl;
    std::cout << directory << std::endl;
    std::cout << "--- Place MAP.txt in the folder above ---\n" << std::endl;

    char buffer[256];
    GameEngine engine;
    GameEngine::initializeRiskFSM(engine);

    testPlayerStrategy();

    //// run all driver test functions
    //testCommandProcessor();
    //std::cout << "\n\nFinished testing COMMAND PROCESSOR. Enter any character to proceed to the next test. ";
    //std::cin >> buffer;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //testStartupPhase(engine);
    //std::cout << "\n\nFinished testing Startup Phase. Enter any character to proceed to the next test. ";
    //std::cin >> buffer;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //testMainGameLoop(engine);
    //std::cout << "\n\nFinished testing Main Game Phase. Enter any character to proceed to the next test. ";
    //std::cin >> buffer;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //testOrderExecution();
    //std::cout << "\n\nFinished testing ORDER EXECUTION. Enter any character to proceed to the next test. ";
    //std::cin >> buffer;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //testLoggingObserver();
    //std::cout << "\n\nFinished testing LOGGING OBSERVER. Enter any character to end the program. ";
    //std::cin >> buffer;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return 0;
}
