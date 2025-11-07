#include <iostream>
#include <fstream>
#include "GameEngine.h"
#include "Orders.h"
#include "CommandProcessing.h"

// Tests that the LogObserver MVC pattern is implemented correctly by demonstrating that certain actions performed by
// ILoggable and Subject objects are recorded in gamelog.txt.
void testLoggingObserver()
{
	std::cout << "========================================\n";
	std::cout << "         TESTING LOG OBSERVERS\n";
	std::cout << "========================================\n" << std::endl;

	// clear gamelog.txt before each test
	std::ofstream gamelog{ "gamelog.txt", std::ios::trunc };
	if (gamelog.is_open())
		gamelog.close();

	LogObserver obConsoleView{};

	// 1 - log Command effect saving
	Command cmd{ "testcommand" };

	std::cout << "-- 1. \nAttaching an observer to the Command.\n";
	cmd.attachObserver(&obConsoleView);

	std::cout << "Executing Command::saveEffect()..." << std::endl;
	cmd.saveEffect("Test action completed successfully");

	// 2 - log command processor saving command objects
	CommandProcessor cmdProc{};
	FileCommandProcessorAdapter fileCmdProc{ "commands.txt" };
	GameEngine engine{};
	GameEngine::initializeRiskFSM(engine);

	std::cout << "\n--2. \nAttaching an observer to the CommandProcessor and FileCommandProcessorAdapter.\n";
	cmdProc.attachObserver(&obConsoleView);
	fileCmdProc.attachObserver(&obConsoleView);

	std::cout << "Executing CommandProcessor::saveCommand()...\n";
	cmdProc.getCommand(engine); // saveCommand is called implicitly through this function
	std::cout << "Excecuting FileCommandProcessorAdapter::saveCommand()..." << std::endl;
	fileCmdProc.readCommand(engine); // same here

	// 3 - log OrderList list changes
	OrdersList olist{};
	Order* orderPtr{ new Deploy() };

	std::cout << "\n-- 3.\nAttaching an observer to the OrderList.\n";
	olist.attachObserver(&obConsoleView);

	std::cout << "Executing OrdersList::add()..." << std::endl;
	olist.add(orderPtr);

	// 4 - log Order execution effect

	std::cout << "\n-- 4.\nAttaching an observer to the DeployOrder.\n";
	orderPtr->attachObserver(&obConsoleView);

	std::cout << "Executing Order::execute()..." << std::endl;
	orderPtr->execute();

	// 5 - log GameEngine state changes
	std::cout << "\n-- 5.\nAttaching an observer to the GameEngine.\n";
	engine.attachObserver(&obConsoleView);

	std::cout << "Executing GameEngine::changeGameState()..." << std::endl;
	engine.changeGameState("loadmap");

	std::cout << "\nAll 5 logged functions have been called. Please check gamelog.txt to see the results." << std::endl;
}
