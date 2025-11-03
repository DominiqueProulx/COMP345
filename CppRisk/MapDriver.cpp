// -------------------------------
// COMP 345 - Fall 2025
// Risk Game Project
// Part 1: Map
// File: MapDriver.cpp
// -------------------------------

#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"
#include <limits>  
using namespace std;

void testLoadMaps() {
    cout << "========================================" << endl;
    cout << "      TESTING MAPS AND MAPLOADER" << endl;
    cout << "========================================" << endl << endl;

    MapLoader loader;
    
    vector<string> paths = {
        "MAP.txt",
        "MAP2.txt",
        "test.map"
    };

    cout << "Testing " << paths.size() << " map files..." << endl << endl;

    for (const auto& p : paths) {
        cout << "--- Loading " << p << " ---" << endl;
        
        string err;
        Map* m = loader.load(p, &err);
        
        if (m) {
            cout << "[OK] Loaded: " << p << "\n";
            cout << *m << "\n";
            cout << "  -> validate(): " << (m->validate() ? "SUCCESS" : "FAILURE") << "\n";
            delete m;
        }
        else {
            cout << "[ERR] " << p << " => " << err << "\n";
        }
        cout << "------------------------------------------\n" << endl;
    }

    cout << "========================================" << endl;
    cout << "   MAP TESTING COMPLETED" << endl;
    cout << "========================================" << endl << endl;
}