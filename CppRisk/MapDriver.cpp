#include <iostream>
#include <limits>
#include "Orders.h"

using namespace std;

// Declare only the test functions that exist
void testOrdersLists();
void testOrderExecution();

int main(void) {
    cout << "\n========================================" << endl;
    cout << "    COMP345 - TESTING ORDERS" << endl;
    cout << "========================================\n" << endl;

    cout << "Select which test to run:" << endl;
    cout << "1. Orders Lists (Assignment 1)" << endl;
    cout << "2. Order Execution (Assignment 2)" << endl;
    cout << "0. Exit" << endl;

    int choice;
    while (true) {
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

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