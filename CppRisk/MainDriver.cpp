#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"

// A1 test functions
void testLoadMaps();
void testPlayers();
void testOrdersLists();
void testCards();
void testGameStates();

// A2 test functions
void testLoggingObserver();

std::string buffer{};
constexpr int ASSIGN_TEST_NUM = 2;

int main(void)
{
	// run all driver test functions

	// A1 TEST CODE
	if (ASSIGN_TEST_NUM == 1)
	{
		testLoadMaps();
		std::cout << "\n\nFinished testing MAPS. Enter any character to proceed to the next test. ";
		std::cin >> buffer;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		testPlayers();
		std::cout << "\n\nFinished testing PLAYERS. Enter any character to proceed to the next test. ";
		std::cin >> buffer;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		testOrdersLists();
		std::cout << "\n\nFinished testing ORDER LISTS. Enter any character to proceed to the next test. ";
		std::cin >> buffer;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		testCards();
		std::cout << "\n\nFinished testing CARDS. Enter any character to proceed to the next test. ";
		std::cin >> buffer;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		testGameStates();
		std::cout << "\n\nFinished testing GAME ENGINE. Enter any character to end the program. ";
		std::cin >> buffer;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	else if (ASSIGN_TEST_NUM == 2)
	{
		// other test methods TBA

		testLoggingObserver();
		std::cout << "\n\nFinished testing LOG OBSERVER. Enter any character to end the program. ";
		std::cin >> buffer;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	return 0;
}
