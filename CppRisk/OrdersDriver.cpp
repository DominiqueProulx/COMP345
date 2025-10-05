// -------------------------------
// COMP 345 - Fall 2025
// Risk Game Project
// Part 3: Orders List Driver
// File: OrdersDriver.cpp
// -------------------------------

#include "Orders.h"
#include <iostream>

using namespace std;

void testOrdersLists() {
    cout << "========================================" << endl;
    cout << "   TESTING ORDERS AND ORDERSLIST" << endl;
    cout << "========================================" << endl << endl;
    
    // ========================================
    // Test 1: Create orders of every kind
    // ========================================
    cout << "--- Test 1: Creating orders of every kind ---" << endl;
    
    Order* deploy = new Deploy();
    Order* advance = new Advance();
    Order* bomb = new Bomb();
    Order* blockade = new Blockade();
    Order* airlift = new Airlift();
    Order* negotiate = new Negotiate();
    
    cout << "Created 6 different order types." << endl << endl;
    
    // ========================================
    // Test 2: Create OrdersList and add orders
    // ========================================
    cout << "--- Test 2: Creating OrdersList and adding orders ---" << endl;
    
    OrdersList* ordersList = new OrdersList();
    
    ordersList->add(deploy);
    ordersList->add(advance);
    ordersList->add(bomb);
    ordersList->add(blockade);
    ordersList->add(airlift);
    ordersList->add(negotiate);
    
    cout << endl << *ordersList << endl;
    
    // ========================================
    // Test 3: Execute orders and show effects
    // ========================================
    cout << "--- Test 3: Executing orders ---" << endl;
    
    cout << "\nExecuting Deploy order:" << endl;
    deploy->execute();
    cout << *deploy << endl << endl;
    
    cout << "Executing Bomb order:" << endl;
    bomb->execute();
    cout << *bomb << endl << endl;
    
    cout << "Executing Negotiate order:" << endl;
    negotiate->execute();
    cout << *negotiate << endl << endl;
    
    // ========================================
    // Test 4: Test move() method
    // ========================================
    cout << "--- Test 4: Testing move() method ---" << endl;
    
    cout << "\nBefore moving:" << endl;
    cout << *ordersList << endl;
    
    cout << "\nMoving order from index 0 to index 3:" << endl;
    ordersList->move(0, 3);
    
    cout << "\nAfter moving:" << endl;
    cout << *ordersList << endl;
    
    // ========================================
    // Test 5: Test remove() method
    // ========================================
    cout << "--- Test 5: Testing remove() method ---" << endl;
    
    cout << "\nBefore removal:" << endl;
    cout << *ordersList << endl;
    
    cout << "\nRemoving order at index 2:" << endl;
    ordersList->remove(2);
    
    cout << "\nAfter removal:" << endl;
    cout << *ordersList << endl;
    
    // ========================================
    // Test 6: Test copy constructor
    // ========================================
    cout << "--- Test 6: Testing copy constructor ---" << endl;
    
    cout << "\nCreating a copy of the orders list:" << endl;
    OrdersList* copiedList = new OrdersList(*ordersList);
    
    cout << "\nOriginal list:" << endl;
    cout << *ordersList << endl;
    
    cout << "Copied list:" << endl;
    cout << *copiedList << endl;
    
    // ========================================
    // Test 7: Test assignment operator
    // ========================================
    cout << "--- Test 7: Testing assignment operator ---" << endl;
    
    OrdersList* assignedList = new OrdersList();
    assignedList->add(new Deploy());
    
    cout << "\nBefore assignment:" << endl;
    cout << "Assigned list: " << endl << *assignedList << endl;
    
    *assignedList = *ordersList;
    
    cout << "After assignment:" << endl;
    cout << "Assigned list: " << endl << *assignedList << endl;
    
    // ========================================
    // Test 8: Demonstrate validate() for all orders
    // ========================================
    cout << "--- Test 8: Demonstrating validate() for all order types ---" << endl;
    
    OrdersList* validationList = new OrdersList();
    validationList->add(new Deploy());
    validationList->add(new Advance());
    validationList->add(new Bomb());
    validationList->add(new Blockade());
    validationList->add(new Airlift());
    validationList->add(new Negotiate());
    
    cout << "\nValidating and executing all orders:" << endl;
    for (int i = 0; i < validationList->size(); i++) {
        Order* order = validationList->getOrder(i);
        cout << "\n--- Order " << (i + 1) << ": " << order->getType() << " ---" << endl;
        order->execute();
        cout << "Result: " << *order << endl;
    }
    
    // ========================================
    // Cleanup
    // ========================================
    cout << "\n--- Cleanup ---" << endl;
    cout << "Deleting all OrdersList objects..." << endl;
    
    delete ordersList;
    delete copiedList;
    delete assignedList;
    delete validationList;
    
    cout << "\nAll tests completed successfully!" << endl;
    cout << "========================================" << endl;
}