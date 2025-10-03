#include "continent.h"
#include "country.h"
#include <algorithm>
using namespace std;

//normal constructor
Continent::Continent(const string& n, int b)
    : name(new string(n)), bonus(new int(b)), territories() {}

//copy constructor
Continent::Continent(const Continent& other)
    : name(new string(*other.name)),
    bonus(new int(*other.bonus)),
    territories(other.territories) {}

//assignment operator 
Continent& Continent::operator=(const Continent& other) {
    if (this != &other) {
        *name  = *other.name;
        *bonus = *other.bonus;
        territories = other.territories;
    }
    return *this;
}

//destructor
Continent::~Continent() {
    delete name;
    delete bonus;
    
}

//add territory if not null or present already
void Continent::addTerritory(Territory* territory) {
    if (!territory) return;
    if (find(territories.begin(), territories.end(), territory) == territories.end()) {
        territories.push_back(territory);
    }
}

const vector<Territory*>& Continent::getTerritories() const { return territories; }

int Continent::getBonus() const { return *bonus; }
string Continent::getName() const { return *name; }

//pretty print
ostream& operator<<(ostream& out, const Continent& continent) {
    out << "Continent{name=" << continent.getName()
        << ", bonus=" << continent.getBonus()
        << ", territories=[";
    const auto& ts = continent.getTerritories();
    for (size_t i = 0; i < ts.size(); ++i) {
        out << ts[i]->getName();
        if (i + 1 < ts.size()) out << ", ";
    }
    out << "]}";
    return out;
}
