
#include "map.h"
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
	testOrdersLists();
	//testCards();
	testGameStates();

	return 0;
}