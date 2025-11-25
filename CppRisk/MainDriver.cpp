#include <iostream>
#include <limits>
#include <string>
#include <direct.h>
#include "GameEngine.h"
#include "CommandProcessing.h"

// Declare test functions
void testPlayerStrategies();
void testTournament();

int main(void)
{
    char directory[FILENAME_MAX];
    _getcwd(directory, FILENAME_MAX);
    std::cout << "--- CURRENT WORKING DIRECTORY ---" << std::endl;
    std::cout << directory << std::endl;
    std::cout << "--- Place MAP.txt in the folder above ---\n" << std::endl;

    char buffer[256];

    // run all driver test functions
    
    testPlayerStrategies();
    std::cout << "\n\nFinished testing PLAYER STRATEGIES. Enter any character to proceed to the next test. ";
    std::cin >> buffer;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    testTournament();
    std::cout << "\n\nFinished testing TOURNAMENT. Enter any character to finish the test. ";
    std::cin >> buffer;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return 0;
}