#include "country.h"
#include <algorithm>
using namespace std;

//Regular constructor
Territory::Territory(const string& n, int xCord, int yCord)
    : name(new string(n)),
    x(new int(xCord)),
    y(new int(yCord)),
    numberOfArmies(new int(0)),
    adjacentTerritories() {}

// Copy constructor w/shallow copy for adjacency, for new territories
Territory::Territory(const Territory& other)
    : name(new string(*other.name)),
    x(new int(*other.x)),
    y(new int(*other.y)),
    numberOfArmies(new int(*other.numberOfArmies)),
    adjacentTerritories(other.adjacentTerritories) {}

//when territory already exists
Territory& Territory::operator=(const Territory& other) {
    if (this != &other) {
        *name            = *other.name;
        *x               = *other.x;
        *y               = *other.y;
        *numberOfArmies  = *other.numberOfArmies;
        adjacentTerritories = other.adjacentTerritories;
    }
    return *this;
}

//destructor
Territory::~Territory() {
    delete name;
    delete x;
    delete y;
    delete numberOfArmies;
}

//add neighboring territory if not null, itself, or already present
void Territory::addAdjacentTerritory(Territory* territory) {
    if (!territory || territory == this) return;
    if (find(adjacentTerritories.begin(), adjacentTerritories.end(), territory)
        == adjacentTerritories.end()) {
        adjacentTerritories.push_back(territory);
    }
}

const vector<Territory*>& Territory::getAdjacentTerritories() const {
    return adjacentTerritories;
}

string Territory::getName() const { return *name; }
int Territory::getX() const { return *x; }
int Territory::getY() const { return *y; }
int Territory::getNumberOfArmies() const { return *numberOfArmies; }

void Territory::setName(const string& n) { *name = n; }
void Territory::setX(int xCord) { *x = xCord; }
void Territory::setY(int yCord) { *y = yCord; }
void Territory::setNumberOfArmies(int numOfArmies) { *numberOfArmies = numOfArmies; }

//pretty print (e.g look into <boost/pfr.hpp>)
ostream& operator<<(ostream& out, const Territory& territory) {
    out << "Territory{name=" << territory.getName()
        << ", x=" << territory.getX()
        << ", y=" << territory.getY()
        << ", armies=" << territory.getNumberOfArmies()
        << ", adj=[";
    const auto& adj = territory.getAdjacentTerritories();
    for (size_t i = 0; i < adj.size(); ++i) {
        out << adj[i]->getName();
        if (i + 1 < adj.size()) out << ", ";
    }
    out << "]}";
    return out;
}
