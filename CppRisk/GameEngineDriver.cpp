#include <iostream>
#include "GameEngine.h"

// for brevity
using GameState = GameEngine::GameState;

// Tests the GameEngine implementation by creating an FSM to replicate Risk and starting a loop to test state
// transitions using user-inputted commands.
// The State implementation is hidden under GameEngine and can only be created through engine.createState
void testGameStates()
{
	GameEngine engine;

	// adding states to recreate the state diagram
	GameState startup{ engine.createState("startup", true) };
	GameState play{ engine.createState("play", true) };
	GameState end{ engine.createState("end", true) };
	engine.addParentStates({ startup, play, end });

	// STARTUP
	GameState start{ engine.createState("start", false) };
	GameState mapLoaded{ engine.createState("map loaded", false) };
	GameState mapValidated{ engine.createState("map validated", false) };
	GameState playersAdded{ engine.createState("players added", false) };

	engine.addChildStates(startup, { start, mapLoaded, mapValidated, playersAdded });
	engine.addChildTransition(start, "loadmap", mapLoaded);
	engine.addChildTransition(mapLoaded, "loadmap", mapLoaded);
	engine.addChildTransition(mapLoaded, "validatemap", mapValidated);
	engine.addChildTransition(mapValidated, "addplayer", playersAdded);
	engine.addChildTransition(playersAdded, "addplayer", playersAdded);
	engine.addChildTransition(playersAdded, "assigncountries", play);
	engine.setActiveState(startup->getInitialSubstatePtr());

	// PLAY
	GameState assignReinforcements{ engine.createState("assign reinforcements", false) };
	GameState issueOrders{ engine.createState("issue orders", false) };
	GameState executeOrders{ engine.createState("execute orders", false) };
	GameState win{ engine.createState("win", false) };

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
	GameState final{ engine.createState("quit", false) };
	engine.addChildStates(end, { final });

	// console test loop
	std::cout << "========================================\n";
	std::cout << "          TESTING GAME ENGINE\n";
	std::cout << "========================================\n";

	if (engine.getActiveParentStatePtr() == nullptr || engine.getActiveStatePtr() == nullptr)
	{
		std::cout << "ERR: No non-final states provided for this engine. Returning...";
		return;
	}

	bool shouldContinue{ true };
	std::string cmd{};
	while (shouldContinue)
	{
		std::cout << "\n[" << engine.getActiveParentStatePtr()->getName() << "/" << engine.getActiveStatePtr()->getName()
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
