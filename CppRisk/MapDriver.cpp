#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include <iostream>
#include <limits>  

using namespace std;

// Declare test driver functions
void testLoadMaps();
void testPlayers();
void testOrdersLists();
void testCards();
void testGameStates();
void testOrderExecution();  // Assignment 2 test

int main(void) {
    cout << "\n========================================" << endl;
    cout << "    COMP345 - RISK GAME - MAIN DRIVER" << endl;
    cout << "========================================\n" << endl;

    cout << "Select which part to test:" << endl;
    cout << "1. Map Loader" << endl;
    cout << "2. Player" << endl;
    cout << "3. Orders Lists" << endl;
    cout << "4. Cards" << endl;
    cout << "5. Game Engine" << endl;
    cout << "6. Order Execution (Assignment 2)" << endl;
    cout << "0. Exit" << endl;

    int choice;
    while (true) {
        cout << "\nEnter your choice: ";
        cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) break;

        switch (choice) {
            case 1:
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                testLoadMaps();
                break;
            case 2:
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                testPlayers();
                break;
            case 3:
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                testOrdersLists();
                break;
            case 4:
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                testCards();
                break;
            case 5:
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                testGameStates();
                break;
            case 6:
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                testOrderExecution();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    cout << "\nExiting program..." << endl;
    return 0;
}