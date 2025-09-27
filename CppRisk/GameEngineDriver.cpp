#include <iostream>
#include "GameEngine.h"

// for brevity
using GameState = GameEngine::State::StatePtr;

// Tests the GameEngine implementation by creating an FSM to replicate Risk and starting a loop to test state
// transitions using user-inputted commands.
void testGameStates()
{
	GameEngine engine;

	// adding states to recreate the state diagram
	GameState startup{ std::make_shared<GameEngine::State>("startup", true) };
	GameState play{ std::make_shared<GameEngine::State>("play", true) };
	GameState end{ std::make_shared<GameEngine::State>("end", true) };
	engine.addParentStates({ startup, play, end });

	// STARTUP
	GameState start{ std::make_shared<GameEngine::State>("start", false) };
	GameState mapLoaded{ std::make_shared<GameEngine::State>("map loaded", false) };
	GameState mapValidated{ std::make_shared<GameEngine::State>("map validated", false) };
	GameState playersAdded{ std::make_shared<GameEngine::State>("players added", false) };

	engine.addChildStates(startup, { start, mapLoaded, mapValidated, playersAdded });
	engine.addChildTransition(start, "loadmap", mapLoaded);
	engine.addChildTransition(mapLoaded, "loadmap", mapLoaded);
	engine.addChildTransition(mapLoaded, "validatemap", mapValidated);
	engine.addChildTransition(mapValidated, "addplayer", playersAdded);
	engine.addChildTransition(playersAdded, "addplayer", playersAdded);
	engine.addChildTransition(playersAdded, "assigncountries", play);
	engine.setActiveState(startup->getInitialSubstate());

	// PLAY
	GameState assignReinforcements{ std::make_shared<GameEngine::State>("assign reinforcements", false) };
	GameState issueOrders{ std::make_shared<GameEngine::State>("issue orders", false) };
	GameState executeOrders{ std::make_shared<GameEngine::State>("execute orders", false) };
	GameState win{ std::make_shared<GameEngine::State>("win", false) };

	engine.addChildStates(play, { assignReinforcements, issueOrders, executeOrders, win });
	engine.addChildTransition(assignReinforcements, "issueorder", issueOrders);
	engine.addChildTransition(issueOrders, "issueorder", issueOrders);
	engine.addChildTransition(issueOrders, "endissueorders", executeOrders);
	engine.addChildTransition(executeOrders, "execorder", executeOrders);
	engine.addChildTransition(executeOrders, "endexecorders", assignReinforcements);
	engine.addChildTransition(executeOrders, "win", win);
	engine.addChildTransition(win, "play", startup);
	engine.addChildTransition(win, "end", end);

	// END
	GameState final{ std::make_shared<GameEngine::State>("quit", false) };
	engine.addChildStates(end, { final });

	// console test loop
	std::cout << "----- testGameStates() -----";
	bool shouldContinue{ true };
	std::string cmd{};
	while (shouldContinue)
	{
		std::cout << "\n[" << engine.getActiveParentState()->getName() << "/" << engine.getActiveState()->getName()
			<< "] Enter a command to transition to a new state: ";
		cmd = engine.readCommand();

		engine.changeGameState(cmd);

		if (engine.isActiveStateFinal())
		{
			std::cout << "Reached a final state. Terminating test...";
			shouldContinue = false;
		}
	}
}
