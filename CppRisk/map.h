#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>

class Territory;
class Continent;

class Map {
public:
    
    Map(const std::string& scroll, const std::string& author,
        const std::string& name, bool wp, bool wn);
    Map(const Map& other);
    Map& operator=(const Map& other);
    ~Map();

    
    void addTerritory(Territory* territory);
    void addContinent(Continent* continent);

    
    std::string getScroll() const;
    std::string getAuthor() const;
    std::string getName() const;
    bool getWrap() const;
    bool getWarn() const;
    const std::vector<Continent*>& getContinents() const;
    const std::vector<Territory*>& getTerritories() const;

    
    bool isValid() const;

    
    friend std::ostream& operator<<(std::ostream& out, const Map& map);

private:
    
    void dfs(Territory* start, std::unordered_set<Territory*>& visited) const;
    bool isConnected() const;
    bool areContinentsInterconnected() const;
    bool areTerritoriesUniqueToContinents() const;

private:
    
    std::string* scroll;
    std::string* author;
    std::string* name;
    bool* wrap;
    bool* warn;

    std::vector<Territory*> territories;
    std::vector<Continent*> continents;
};

#endif
