#ifndef CONTINENT_H
#define CONTINENT_H

#include <string>
#include <vector>
#include <iostream>

class Territory;

class Continent {
public:
    
    Continent(const std::string& n, int bonus);
    Continent(const Continent& other);
    Continent& operator=(const Continent& other);
    ~Continent();

    
    void addTerritory(Territory* territory);
    const std::vector<Territory*>& getTerritories() const;

    
    int getBonus() const;
    std::string getName() const;

    
    friend std::ostream& operator<<(std::ostream& out, const Continent& continent);

private:
    std::string* name;
    int* bonus;
    std::vector<Territory*> territories;
};

#endif
