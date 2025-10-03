#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class Map;
class Continent;
class Territory;

class MapLoader {
public:
    MapLoader();
    MapLoader(const MapLoader& other);
    MapLoader& operator=(const MapLoader& other);
    ~MapLoader();

    Map* load(const std::string& path, std::string* errorOut = nullptr) const;

    friend std::ostream& operator<<(std::ostream& os, const MapLoader&);

private:
    static std::string trim(const std::string& s);
    static std::vector<std::string> splitCSV(const std::string& line);
};

#endif
