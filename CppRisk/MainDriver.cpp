#include "PlayerDriver.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// run all driver test functions
	//testLoadMaps();
	testPlayers();
	//testOrdersLists();
	//testCards();
	//testGameStates();


	return 0;
}
