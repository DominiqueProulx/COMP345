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
    testStartupPhase(engine);
    testMainGameLoop(engine);

}