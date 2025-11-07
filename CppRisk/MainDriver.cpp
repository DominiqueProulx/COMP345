#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"
#include "CommandProcessing.h"
#include <direct.h>

// Declare test functions
//void testLoadMaps();
//void testPlayers();
//void testOrdersLists();
//void testCards();
//void testGameStates();
void testCommandProcessor();

std::string buffer{};

int main(void)
{
	// run all driver test functions
	//testLoadMaps();
	//std::cout << "\n\nFinished testing MAPS. Enter any character to proceed to the next test. ";
	//std::cin >> buffer;
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//testPlayers();
	//std::cout << "\n\nFinished testing PLAYERS. Enter any character to proceed to the next test. ";
	//std::cin >> buffer;
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//testOrdersLists();
	//std::cout << "\n\nFinished testing ORDER LISTS. Enter any character to proceed to the next test. ";
	//std::cin >> buffer;
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//testCards();
	//std::cout << "\n\nFinished testing CARDS. Enter any character to proceed to the next test. ";
	//std::cin >> buffer;
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//testGameStates();
	//std::cout << "\n\nFinished testing GAME ENGINE. Enter any character to end the program. ";
	//std::cin >> buffer;
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	char buffer[FILENAME_MAX];
	_getcwd(buffer, FILENAME_MAX);
	std::cout << "--- CURRENT WORKING DIRECTORY ---" << std::endl;
	std::cout << buffer << std::endl;
	std::cout << "--- Place MAP.txt in the folder above ---" << std::endl;

	testCommandProcessor();

	return 0;
}

