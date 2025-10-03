#include "Player.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

void testPlayers() {
	std::cout << "------------------ Testing Player Class ------------------" << std::endl;
	Territory* territory1 = new Territory("Rohan");
	Territory* territory2 = new Territory("Gondor");
	Territory* territory3 = new Territory("Rivendell");
	Territory* territory4 = new Territory("Mirkwood");
	Territory* territory5 = new Territory("Osgiliath");

	std::vector<Territory*> mapstub1;
	mapstub1.push_back(territory1);
	mapstub1.push_back(territory2);
	mapstub1.push_back(territory3);
	mapstub1.push_back(territory4);
	mapstub1.push_back(territory5);

	TerritoriesWithArmies* twa1 = new TerritoriesWithArmies( territory1, 3 );
	TerritoriesWithArmies* twa2 = new TerritoriesWithArmies( territory2, 3 );
	TerritoriesWithArmies* twa3 = new TerritoriesWithArmies( territory3, 3 );
	TerritoriesWithArmies* twa4 = new TerritoriesWithArmies( territory4, 3 );
	TerritoriesWithArmies* twa5 = new TerritoriesWithArmies( territory5, 3 );

	std::vector<TerritoriesWithArmies*> intitialTerritories1;
	intitialTerritories1.push_back(twa1);
	intitialTerritories1.push_back(twa2);
	intitialTerritories1.push_back(twa3);
	intitialTerritories1.push_back(twa4);
	intitialTerritories1.push_back(twa5);

	


	std::cout << "\nWhat is the Player1 color" << std::endl;
	std::string color;
	std::cin >> color;


	Player player1(color, intitialTerritories1);
	std::cout << "\nPlayer 1 created with attributes : " << std::endl;
	std::cout <<  player1 << std::endl;


	Territory* territory6 = new Territory("Mordor");
	Territory* territory7 = new Territory("Angmar");
	Territory* territory8 = new Territory("Minas Morgul");
	Territory* territory9 = new Territory("Dol Guldur");
	Territory* territory10 = new Territory("Isengard");

	std::vector<Territory*> mapstub2;
	mapstub2.push_back(territory6);
	mapstub2.push_back(territory7);
	mapstub2.push_back(territory8);
	mapstub2.push_back(territory9);
	mapstub2.push_back(territory10);


	TerritoriesWithArmies* twa6 = new TerritoriesWithArmies(territory6, 3);
	TerritoriesWithArmies* twa7 = new TerritoriesWithArmies(territory7, 3 );
	TerritoriesWithArmies* twa8 = new TerritoriesWithArmies(territory8, 3 );
	TerritoriesWithArmies* twa9 = new TerritoriesWithArmies(territory9, 3 );
	TerritoriesWithArmies* twa10 = new TerritoriesWithArmies(territory10, 3 );

	std::vector<TerritoriesWithArmies*> intitialTerritories2;
	intitialTerritories2.push_back(twa6);
	intitialTerritories2.push_back(twa7);
	intitialTerritories2.push_back(twa8);
	intitialTerritories2.push_back(twa9);
	intitialTerritories2.push_back(twa10);


	std::cout << "\nWhat is the Player2 color" << std::endl;
	std::string color2;
	std::cin >> color2;


	Player player2(color, intitialTerritories2);
	std::cout << "\nPlayer 2 created with attributes : " << std::endl;
		std::cout << player2 << std::endl;

	
	//setup the random generator using the current time
	std::srand(std::time(0));

	//Testing toDefend() method
	std::cout << "\nTesting toDefend() method for Player 1" << std::endl;
	std::vector<TerritoriesWithArmies*> territoriesToDefend1 = player1.toDefend();
	std::cout << "Player 1 has chosen to defend " << territoriesToDefend1.size() << " territories." << std::endl;
	std::cout << "The territories are: " << std::endl; 
	int vectorsize3 = territoriesToDefend1.size();
	for (int i = 0; i < vectorsize3; i++) {
		std::cout << *(territoriesToDefend1[i]->territory->name) << std::endl;
	}



	std::cout << "\nTesting toDefend() method for Player 2" << std::endl;
	std::vector<TerritoriesWithArmies*> territoriesToDefend2 = player2.toDefend();
	std::cout << "Player 2 has chosen to defend " << territoriesToDefend2.size() << " territories." << std::endl;
	std::cout << "The territories are: " << std::endl;
	int vectorsize4 = territoriesToDefend2.size();
	for (int i = 0; i < vectorsize4; i++) {
		std::cout << *(territoriesToDefend2[i]->territory->name) << std::endl;
	}

	//Testing toAttack() method
	std::cout << "\nTesting toAttack() method for Player 1" << std::endl;
	std::vector<Territory*> territoriesToAttack1 = player1.toAttack(mapstub2);
	std::cout << "Player 1 has chosen to Attack " << territoriesToAttack1.size() << " territories." << std::endl;
	std::cout << "The territories are: " << std::endl;
	int vectorsize5 = territoriesToAttack1.size();
	for (int i = 0; i < vectorsize5; i++) {
		std::cout << *(territoriesToAttack1[i]->name) << std::endl;
	}

	//Testing toAttack() method
	std::cout << "\nTesting toAttack() method for Player 2" << std::endl;
	std::vector<Territory*> territoriesToAttack2 = player2.toAttack(mapstub1);
	std::cout << "Player 2 has chosen to Attack " << territoriesToAttack2.size() << " territories." << std::endl;
	std::cout << "The territories are: " << std::endl;
	int vectorsize6 = territoriesToAttack2.size();
	for (int i = 0; i < vectorsize6; i++) {
		std::cout << *(territoriesToAttack2[i]->name) << std::endl;
	}



	//Testing issueOrder() method
	std::cout << "Creating cards and adding them to the hand of the player to test orders" << std::endl;
	//creating some cards and adding them to the player's hand for testing
	Cards* card1 = new Cards("bomb");
	Cards* card2 = new Cards("bomb");
	Cards* card3 = new Cards("blockade");
	Cards* card4 = new Cards("airlift");
	Cards* card5 = new Cards("negotiate");
	(*(player1.getHand())).addCard(card1);
	(*(player1.getHand())).addCard(card2);
	(*(player1.getHand())).addCard(card3);
	(*(player1.getHand())).addCard(card4);
	(*(player1.getHand())).addCard(card5);

	std::cout << "\nTesting issueOrder() method for Player 1" << std::endl;
	bool endOrder = false;
	while(!endOrder) {
		player1.issueOrder();
		std::cout << "Do you want to issue another order ? (y/n)" << std::endl;
		char answer;
		std::cin >> answer;
		if (answer == 'n' || answer == 'N') {
			endOrder = true;
		}
	}

		std::cout << "Here is the list of all the orders that are part of the player's order list: " <<  std::endl;
		std::queue<DummyOrders*> listOfPlayerOrders = *((*(player1.getOrdersList())).getQueue());
		while (!listOfPlayerOrders.empty() ) {
			DummyOrders* order = listOfPlayerOrders.front();
			std::cout << *order << std::endl;
		    listOfPlayerOrders.pop();
		}
		
		//cleanup 
		delete territory1;
		delete territory2;
		delete territory3;
		delete territory4;
		delete territory5;

		delete territory6;
		delete territory7;
		delete territory8;
		delete territory9;
		delete territory10;

		
		delete card1;
		delete card2;
		delete card3;
		delete card4;
		delete card5;



	std::cout << "\nEnd of Tests for Player Class "<< std::endl;

}

