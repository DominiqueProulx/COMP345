#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

class Map;
class Continent;
class Territory;

//MAP LOADER-----------------------------------------------------------------------------------------------------------------------
class MapLoader {
public:
//constructors, destructor, assignment operator
    MapLoader();
    MapLoader(const MapLoader& other);
    MapLoader& operator=(const MapLoader& other);
    ~MapLoader();

    Map* load(const std::string& path, std::string* errorOut = nullptr) const;

    friend std::ostream& operator<<(std::ostream& os, const MapLoader&);

private:
//helper functions
    static std::string trim(const std::string& s);
    static std::vector<std::string> splitCSV(const std::string& line);
};

//TERRITORY-----------------------------------------------------------------------------------------------------------------------
class Territory {
public:
    //
    Territory(const std::string& n, int xCord, int yCord);
    Territory(const Territory& other);
    Territory& operator=(const Territory& other);
    ~Territory();

    
    void addAdjacentTerritory(Territory* territory);
    const std::vector<Territory*>& getAdjacentTerritories() const;

    //getters and setters
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
    std::vector<Territory*>* adjacentTerritories;
};

//CONTINENT-----------------------------------------------------------------------------------------------------------------------
class Continent {
public:
    //constructors, destructor, assignment operator
    Continent(const std::string& n, int bonus);
    Continent(const Continent& other);
    Continent& operator=(const Continent& other);
    ~Continent();

    
    void addTerritory(Territory* territory);
    const std::vector<Territory*>& getTerritories() const;

    //getters
    int getBonus() const;
    std::string getName() const;

    
    friend std::ostream& operator<<(std::ostream& out, const Continent& continent);

private:
    std::string* name;
    int* bonus;
    std::vector<Territory*>* territories;
};

//MAP-----------------------------------------------------------------------------------------------------------------------
class Map {
public:
    //constructors, destructor, assignment operator
    Map(const std::string& scroll, const std::string& author,
        const std::string& name, bool wp, bool wn);
    Map(const Map& other);
    Map& operator=(const Map& other);
    ~Map();

    
    void addTerritory(Territory* territory);
    void addContinent(Continent* continent);

    //getters
    std::string getScroll() const;
    std::string getAuthor() const;
    std::string getName() const;
    bool getWrap() const;
    bool getWarn() const;
    const std::vector<Continent*>& getContinents() const;
    const std::vector<Territory*>& getTerritories() const;

    
    bool validate() const;

    
    friend std::ostream& operator<<(std::ostream& out, const Map& map);

private:
    //helper functions for validation
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

    std::vector<Territory*>* territories;
    std::vector<Continent*>* continents;
};

#endif
