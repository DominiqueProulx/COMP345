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

	TerritoriesWithArmies* twa1 = new TerritoriesWithArmies{ territory1, 3 };
	TerritoriesWithArmies* twa2 = new TerritoriesWithArmies{ territory2, 3 };
	TerritoriesWithArmies* twa3 = new TerritoriesWithArmies{ territory3, 3 };
	TerritoriesWithArmies* twa4 = new TerritoriesWithArmies{ territory4, 3 };
	TerritoriesWithArmies* twa5 = new TerritoriesWithArmies{ territory5, 3 };

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
	std::cout << "Player 1 ID: " << *(player1.getID()) << std::endl;
	std::cout << "Player 1 Color: " << *(player1.getColor()) << std::endl;
	std::cout << "Player 1 Territories: " << std::endl;
	int vectorsize1 = (*(player1.getTerritories())).size();
	std::vector<TerritoriesWithArmies*> player1Territories = *(player1.getTerritories());
	for (int i = 0; i < vectorsize1; i++) {
		std::cout << *(player1Territories[i]->territory->name) << std::endl;
	}

	Territory* territory6 = new Territory("Mordor");
	Territory* territory7 = new Territory("Angmar");
	Territory* territory8 = new Territory("Minas Morgul");
	Territory* territory9 = new Territory("Dol Guldur");
	Territory* territory10 = new Territory("Isengard");

	TerritoriesWithArmies* twa6 = new TerritoriesWithArmies{ territory6, 3 };
	TerritoriesWithArmies* twa7 = new TerritoriesWithArmies{ territory7, 3 };
	TerritoriesWithArmies* twa8 = new TerritoriesWithArmies{ territory8, 3 };
	TerritoriesWithArmies* twa9 = new TerritoriesWithArmies{ territory9, 3 };
	TerritoriesWithArmies* twa10 = new TerritoriesWithArmies{ territory10, 3 };

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
	std::cout << "Player 2 ID: " << *(player2.getID()) << std::endl;
	std::cout << "Player 2 Color: " << *(player2.getColor()) << std::endl;
	std::cout << "Player 2 Territories: " << std::endl;
	int vectorsize2 = (*(player2.getTerritories())).size();
	std::vector<TerritoriesWithArmies*> player2Territories = *(player2.getTerritories());
	for (int i = 0; i < vectorsize2; i++) {
		std::cout << *(player2Territories[i]->territory->name) << std::endl;
	}

	
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

}

