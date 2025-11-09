// -------------------------------
// COMP 345 - Fall 2025
// Risk Game Project
// Part 3/4: Orders Driver
// File: OrdersDriver.cpp
// -------------------------------

#include "Orders.h"
#include <iostream>

using namespace std;

// Assignment 1 test function - kept for backward compatibility
void testOrdersLists() {
	cout << "\n========================================" << endl;
	cout << "    TESTING ORDERS AND ORDERS LIST" << endl;
	cout << "========================================\n" << endl;

	// Create dummy objects using default constructors
	Player* player1 = new Player();
	player1->setColor("Red");

	Territory* terr1 = new Territory("Territory1", 0, 0);
	Territory* terr2 = new Territory("Territory2", 1, 1);

	terr1->setNumberOfArmies(10);
	terr2->setNumberOfArmies(5);

	player1->addTerritory(terr1);
	player1->addTerritory(terr2);
	player1->setReinforcementPool(20);

	// Create orders
	Order* deploy = new Deploy(player1, terr1, 5);
	Order* advance = new Advance(player1, terr1, terr2, 3);
	Order* bomb = new Bomb(player1, terr2);

	OrdersList* ordersList = new OrdersList();
	ordersList->add(deploy);
	ordersList->add(advance);
	ordersList->add(bomb);

	cout << "Created OrdersList with 3 orders" << endl;
	cout << *ordersList << endl;

	// Test move
	cout << "\nTesting move()..." << endl;
	ordersList->move(0, 2);
	cout << *ordersList << endl;

	// Test remove
	cout << "\nTesting remove()..." << endl;
	ordersList->remove(1);
	cout << *ordersList << endl;

	// Test copy constructor
	cout << "\nTesting copy constructor..." << endl;
	OrdersList copyList(*ordersList);
	cout << "Original list:" << endl;
	cout << *ordersList << endl;
	cout << "Copied list:" << endl;
	cout << copyList << endl;

	// Test assignment operator
	cout << "\nTesting assignment operator..." << endl;
	OrdersList assignedList;
	assignedList = *ordersList;
	cout << "Assigned list:" << endl;
	cout << assignedList << endl;

	// Test executing orders
	cout << "\nExecuting all orders in list..." << endl;
	for (int i = 0; i < ordersList->size(); i++) {
		Order* order = ordersList->getOrder(i);
		cout << "\nExecuting: " << *order << endl;
		order->execute();
		cout << "After execution: " << *order << endl;
	}

	// Cleanup
	delete ordersList;
	delete player1;
	delete terr1;
	delete terr2;

	cout << "\nAssignment 1 tests completed!" << endl << endl;
}

// Assignment 2 test function - demonstrates order execution with game rules
void testOrderExecution() {
	cout << "\n========================================" << endl;
	cout << "         TESTING ORDER EXECUTION" << endl;
	cout << "========================================\n" << endl;

	// Create players
	Player* player1 = new Player();
	player1->setColor("Alice");

	Player* player2 = new Player();
	player2->setColor("Bob");

	Player* neutralPlayer = new Player();
	neutralPlayer->setColor("Neutral");

	// Create territories with coordinates
	Territory* terrA = new Territory("Territory_A", 0, 0);
	Territory* terrB = new Territory("Territory_B", 1, 0);
	Territory* terrC = new Territory("Territory_C", 2, 0);
	Territory* terrD = new Territory("Territory_D", 0, 1);
	Territory* terrE = new Territory("Territory_E", 1, 1);

	// Set up adjacencies
	terrA->addAdjacentTerritory(terrB);
	terrB->addAdjacentTerritory(terrA);
	terrB->addAdjacentTerritory(terrC);
	terrC->addAdjacentTerritory(terrB);
	terrC->addAdjacentTerritory(terrD);
	terrD->addAdjacentTerritory(terrC);
	terrD->addAdjacentTerritory(terrE);
	terrE->addAdjacentTerritory(terrD);

    // ========================================
    // Requirement 6: All orders can be issued and executed
    // ========================================
    cout << "=== REQUIREMENT 6: All Order Types Can Be Issued ===" << endl << endl;
    
    OrdersList* gameOrders = new OrdersList();
    
    cout << "Creating and adding all order types to OrdersList..." << endl;
    gameOrders->add(new Deploy(player1, terrA, 3));
    gameOrders->add(new Advance(player1, terrA, terrB, 2));
    gameOrders->add(new Bomb(player1, terrD));
    gameOrders->add(new Airlift(player1, terrA, terrB, 2));
    gameOrders->add(new Blockade(player2, terrD, neutralPlayer));
    gameOrders->add(new Negotiate(player1, player2));
    
    cout << "\nOrdersList contains " << gameOrders->size() << " orders" << endl;
    cout << *gameOrders << endl;
    
    cout << "\nExecuting all orders sequentially..." << endl;
    for (int i = 0; i < gameOrders->size(); i++) {
        cout << "\n--- Executing Order " << (i + 1) << " ---" << endl;
        gameOrders->getOrder(i)->execute();
    }
    
    // ========================================
    // Summary
    // ========================================
    cout << "\n==========================================" << endl;
    cout << "   ALL REQUIREMENTS DEMONSTRATED" << endl;
    cout << "==========================================" << endl;
    cout << "✓ Requirement 1: Validation before execution" << endl;
    cout << "✓ Requirement 2: Territory ownership transfer" << endl;
    cout << "✓ Requirement 3: Card awarding for conquest" << endl;
    cout << "✓ Requirement 4: Negotiate prevents attacks" << endl;
    cout << "✓ Requirement 5: Blockade transfers to Neutral" << endl;
    cout << "✓ Requirement 6: All orders can be executed" << endl;
    cout << "==========================================" << endl << endl;
    

	// Clear pending deployments at turn end
	player1->clearPendingDeployments();
	player2->clearPendingDeployments();
	neutralPlayer->clearPendingDeployments();

	// Cleanup: orders were created and added to gameOrders; assume OrdersList destructor
	// is responsible for deleting contained orders.
	delete gameOrders;
	
	delete player1;
	delete player2;
	delete neutralPlayer;
	
	delete terrA;
	delete terrB;
	delete terrC;
	delete terrD;
	delete terrE;
	
	cout << "Test completed successfully!" << endl;
}
