#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"

// Declare test functions
void testLoadMaps();
void testPlayers();
void testOrdersLists();
void testCards();
void testGameStates();

int main(void)
{
	// run all driver test functions

	testLoadMaps();
	testPlayers();
	testOrdersLists();
	testCards();
	testGameStates();

	return 0;
}
