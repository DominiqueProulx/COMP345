#include "Map.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void testLoadMaps() {
	MapLoader loader;
	vector<string> paths = {
		"MAP.txt",
		"MAP2.txt",
		"test.txt"
	};

	cout << "========================================" << endl;
	cout << "      TESTING MAPS AND MAPLOADER" << endl;
	cout << "========================================" << endl << endl;

	for (const auto& p : paths) {
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
		cout << "------------------------------------------\n";
	}

	cout << endl;
}