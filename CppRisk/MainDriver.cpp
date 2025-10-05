#include "Cards.h"

//forward declaration
void testCards();
#include "Map.h"
#include "Orders.h"
#include "GameEngine.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

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
	//testPlayers();
	//testOrdersLists();
	testCards();
	//testGameStates();
	testOrdersLists();
	//testCards();
	testGameStates();

	return 0;
}
