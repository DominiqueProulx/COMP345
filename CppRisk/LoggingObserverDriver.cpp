#include <iostream>
#include <fstream>
#include "GameEngine.h"
#include "Orders.h"

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

	// !!!!!!!!!!!!!!!!!!!
	// TBA - CommandProcessor and Command logic, awaiting Pt. 1 integration
	// !!!!!!!!!!!!!!!!!!!

	// 1 - log OrderList list changes
	OrdersList olist{};
	Deploy deployOrder{};
	Order* orderPtr{ &deployOrder };

	std::cout << "-- 1.\nAttaching an observer to the OrderList.\n";
	olist.attachObserver(&obConsoleView);

	std::cout << "Executing OrdersList::add()..." << std::endl;
	olist.add(orderPtr);

	// 2 - log Order execution effect

	std::cout << "\n-- 2.\nAttaching an observer to the DeployOrder.\n";
	deployOrder.attachObserver(&obConsoleView);

	std::cout << "Executing Order::execute()..." << std::endl;
	deployOrder.execute();

	// 3 - log GameEngine state changes
	GameEngine engine{};
	GameEngine::initializeRiskFSM(engine);

	std::cout << "\n-- 3.\nAttaching an observer to the GameEngine.\n";
	engine.attachObserver(&obConsoleView);

	std::cout << "Executing GameEngine::changeGameState() twice..." << std::endl;
	engine.changeGameState("loadmap");
	engine.changeGameState("validatemap");

	std::cout << "\nAll 3 logged functions have been called. Please check gamelog.txt to see the results." << std::endl;
}
