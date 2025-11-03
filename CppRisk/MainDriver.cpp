#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"
#include <limits>

using namespace std;

// Declare test functions
// void testLoadMaps();
// void testPlayers();
void testOrdersLists();
// void testCards();
// void testGameStates();
void testOrderExecution();

std::string buffer{};

// int main(void)
// {
// 	// run all driver test functions
// 	testLoadMaps();
// 	std::cout << "\n\nFinished testing MAPS. Enter any character to proceed to the next test. ";
// 	std::cin >> buffer;
// 	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// 	testPlayers();
// 	std::cout << "\n\nFinished testing PLAYERS. Enter any character to proceed to the next test. ";
// 	std::cin >> buffer;
// 	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// 	testOrdersLists();
// 	std::cout << "\n\nFinished testing ORDER LISTS. Enter any character to proceed to the next test. ";
// 	std::cin >> buffer;
// 	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// 	testCards();
// 	std::cout << "\n\nFinished testing CARDS. Enter any character to proceed to the next test. ";
// 	std::cin >> buffer;
// 	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// 	testGameStates();
// 	std::cout << "\n\nFinished testing GAME ENGINE. Enter any character to end the program. ";
// 	std::cin >> buffer;
// 	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// 	return 0;
// }

int main(void) {
    cout << "\n========================================" << endl;
    cout << "    TESTING ORDERS - Assignment 2" << endl;
    cout << "========================================\n" << endl;

    cout << "Select which test to run:" << endl;
    cout << "1. Orders Lists (Assignment 1)" << endl;
    cout << "2. Order Execution (Assignment 2)" << endl;
    cout << "0. Exit" << endl;

    int choice;
    while (true) {
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
            case 1:
                testOrdersLists();
                break;
            case 2:
                testOrderExecution();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    cout << "\nExiting program..." << endl;
    return 0;
}