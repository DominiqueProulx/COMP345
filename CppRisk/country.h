#ifndef COUNTRY_H
#define COUNTRY_H

#include <string>
#include <vector>
#include <iostream>

class Territory {
public:
    
    Territory(const std::string& n, int xCord, int yCord);
    Territory(const Territory& other);
    Territory& operator=(const Territory& other);
    ~Territory();

    
    void addAdjacentTerritory(Territory* territory);
    const std::vector<Territory*>& getAdjacentTerritories() const;

    
    std::string getName() const;
    int getX() const;
    int getY() const;
    int getNumberOfArmies() const;

    
    void setName(const std::string& n);
    void setX(int xCord);
    void setY(int yCord);
    void setNumberOfArmies(int numOfArmies);

    
    friend std::ostream& operator<<(std::ostream& out, const Territory& territory);

private:
    
    std::string* name;
    int* x;
    int* y;
    int* numberOfArmies;
    std::vector<Territory*> adjacentTerritories;
};

#endif
