// COMP 345 - Fall 2025
// Risk Game Project
// Part 2 : Player 
// File: PlayerDriver.cpp

#include "Player.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

void testPlayers() {
	//create a map 
	MapLoader loader;
	std::string error;
	Map* map = loader.load("MAP.txt", &error);

	if (!map) {
		std::cerr << "Failed to load map: " << error << std::endl;
		return ;
	}


	std::cout << "Map loaded successfully!\n" << std::endl;
	//std::cout << *map << std::endl;

	std::cout << "Creating the Deck of Cards" << std::endl;
	Deck deck;
	deck.initializeDeck();
	deck.getSize();
	
	std::cout << "Allocating Territories to players for testing purposes:\n";
	std::vector<TerritoriesWithArmies*> territoryPlayer1;
	std::vector<TerritoriesWithArmies*> territoryPlayer2;
	int count = 0;

	for (Territory* t : map->getTerritories()) {
		if(count % 2 == 0) {
			std::cout << " Player 2 - " << t->getName() << std::endl; 
			TerritoriesWithArmies* twa2 = new TerritoriesWithArmies(t, 3);
			territoryPlayer2.push_back(twa2);
		}
		else {
			std::cout << " Player 1 - " << t->getName() << std::endl;
			TerritoriesWithArmies* twa1 = new TerritoriesWithArmies(t, 3);
			territoryPlayer1.push_back(twa1);
		}
		count++;
		
	}

	

	std::cout << "------------------ Testing Player Class ------------------" << std::endl;
	
	
	std::cout << "\nWhat is the Player1 color" << std::endl;
	std::string color;
	std::cin >> color;


	Player player1(color, territoryPlayer1, &deck);
	std::cout << "\nPlayer 1 created with attributes : " << std::endl;
	std::cout <<  player1 << std::endl;

	std::cout << "\nCreate and set the player1 hand. " << std::endl;
	Hand player1Hand;
	// Draw 5 cards from the deck to the player's hand
	for (int i = 0; i < 5; ++i) {
		deck.draw(&player1Hand);
	}
	player1.setHand(player1Hand);

	std::cout << "\nWhat is the Player2 color" << std::endl;
	std::string color2;
	std::cin >> color2;


	Player player2(color, territoryPlayer2, &deck);
	std::cout << "\nPlayer 2 created with attributes : " << std::endl;
		std::cout << player2 << std::endl;

	std::cout << "\nCreate and set the player2 hand " << std::endl;
	Hand player2Hand;
		// Draw 5 cards from the deck to the player's hand
		for (int i = 0; i < 5; ++i) {
			deck.draw(&player2Hand);
		}
		player2.setHand(player2Hand);

	//setup the random generator using the current time
	std::srand(std::time(0));

	//Testing toDefend() method
	std::cout << "\nTesting toDefend() method for Player 1" << std::endl;
	std::vector<TerritoriesWithArmies*> territoriesToDefend1 = player1.toDefend();
	std::cout << "Player 1 has chosen to defend " << territoriesToDefend1.size() << " territories." << std::endl;
	std::cout << "The territories to defend are: " << std::endl; 
	int vectorsize3 = territoriesToDefend1.size();
	for (int i = 0; i < vectorsize3; i++) {
		std::cout << (territoriesToDefend1[i]->territory->getName()) << std::endl;
	}

	std::cout << "\nTesting toDefend() method for Player 2" << std::endl;
	std::vector<TerritoriesWithArmies*> territoriesToDefend2 = player2.toDefend();
	std::cout << "Player 2 has chosen to defend " << territoriesToDefend2.size() << " territories." << std::endl;
	std::cout << "The territories to defend are: " << std::endl;
	int vectorsize4 = territoriesToDefend2.size();
	for (int i = 0; i < vectorsize4; i++) {
		std::cout << (territoriesToDefend2[i]->territory->getName()) << std::endl;
	}

	//Testing toAttack() method
	std::cout << "\nTesting toAttack() method for Player 1" << std::endl;
	//std::vector<Territory*> territoriesToAttack1 = player1.toAttack(mapstub2);
	std::vector<Territory*> territoriesToAttack1 = player1.toAttack();
	std::cout << "Player 1 has chosen to Attack " << territoriesToAttack1.size() << " territories." << std::endl;
	std::cout << "The territories to Attack are: " << std::endl;
	int vectorsize5 = territoriesToAttack1.size();
	for (int i = 0; i < vectorsize5; i++) {
		std::cout << (territoriesToAttack1[i]->getName()) << std::endl;
	}

	//Testing toAttack() method
	std::cout << "\nTesting toAttack() method for Player 2" << std::endl;
	//std::vector<Territory*> territoriesToAttack2 = player2.toAttack(mapstub1);
	std::vector<Territory*> territoriesToAttack2 = player2.toAttack();
	std::cout << "Player 2 has chosen to Attack " << territoriesToAttack2.size() << " territories." << std::endl;
	std::cout << "The territories to Attack are: " << std::endl;
	int vectorsize6 = territoriesToAttack2.size();
	for (int i = 0; i < vectorsize6; i++) {
		std::cout << (territoriesToAttack2[i]->getName()) << std::endl;
	}



	//Testing issueOrder() method
	std::cout << "\nCreating cards and adding them to the hand of the player to test orders" << std::endl;
	

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
		std::cout << *(player1.getOrdersList()) << std::endl;
		/*std::vector<Order*> listOfPlayerOrders = *((*(player1.getOrdersList())).getOrdersList());

		while (!listOfPlayerOrders.empty() ) {
			Order* order = listOfPlayerOrders.back();
			std::cout << *order << std::endl;
		    listOfPlayerOrders.pop_back();
		}*/
		
		//cleanup 
		/*delete territory1;
		delete territory2;
		delete territory3;
		delete territory4;
		delete territory5;

		delete territory6;
		delete territory7;
		delete territory8;
		delete territory9;
		delete territory10;*/

		
		/*delete card1;
		delete card2;
		delete card3;
		delete card4;
		delete card5;*/

		delete map; // all territories will also be deleted
		


	std::cout << "\nEnd of Tests for Player Class "<< std::endl;

}

