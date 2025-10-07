#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"
#include <crtdbg.h>

// Declare test functions
void testLoadMaps();
void testPlayers();
void testOrdersLists();
void testCards();
void testGameStates();

int main(void)
{
	
	// run all driver test functions


	//testPlayers();
	try {
		testPlayers();
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	catch (...) {
		std::cerr << "Unknown exception!\n";
	}


	testLoadMaps();
	testOrdersLists();
	testCards();
	testGameStates();
	

	return 0;
}
