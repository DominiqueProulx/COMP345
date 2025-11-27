#include <iostream>
#include <string>
#include <direct.h>
#include "GameEngine.h"


// Declare test functions

void testStartupPhase(GameEngine& engine);
void testMainGameLoop(GameEngine& engine);
void testPlayerStrategy(void);

void testPlayerStrategies()
{
    char buffer[256];
    GameEngine engine;
    GameEngine::initializeRiskFSM(engine);
    

	bool gameDone = false;
	while (!gameDone) {

		testStartupPhase(engine);
		testMainGameLoop(engine);
		

		gameDone = engine.gameOver(std::cin, std::cout);

	}


}