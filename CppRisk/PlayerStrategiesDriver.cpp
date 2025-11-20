#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::cin;
#include "Player.h"
#include "PlayerStrategies.h"
#include "GameEngine.h";

int testPlayerStrategy(void)
{
	GameEngine engine; //FLAG : remove this it's just for testing purposes
	GameEngine::initializeRiskFSM(engine); //FLAG : remove this it's just for testing purposes

	cout << "What kind of player do you want to create" << endl;
	cout << "1. Human Player" << endl;
	cout << "2. Benevolent Player" << endl;
	cout << "3. Cheater Player" << endl;

	int choice;
	cin >> choice;
	Player* player = new Player(); //FLAG : remove this it's just for testing purposes
	
	switch(choice)
	{

	case 1:
		cout << "Switching to Human Player Strategy" << endl;
		player->setStrategy(std::make_unique<HumanPlayerStrategy>(player));
		break;

	case 2 : 
		cout << "Switching to Benevolent Player Strategy" << endl;
		player->setStrategy(std::make_unique<BenevolentPlayerStrategy>(player));
		break;


	default:
		break;
	}


	cout << "testing issue Order:" << endl;
	player->issueOrder();

	return 0;


}