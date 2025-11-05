#include "map.h"
// #include "country.h"
// #include "continent.h"
// #include "MapLoader.h"
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

//CONTINENT-----------------------------------------------------------------------------------------------------------------------
//normal constructor
Continent::Continent(const string& n, int b)
    : name(new string(n)), bonus(new int(b)), territories(new vector<Territory*>()) {}

//copy constructor
Continent::Continent(const Continent& other)
    : name(new string(*other.name)),
    bonus(new int(*other.bonus)),
    territories(new vector<Territory*>(*other.territories)) {}

//assignment operator 
Continent& Continent::operator=(const Continent& other) {
    if (this != &other) {
        *name  = *other.name;
        *bonus = *other.bonus;
        *territories = *other.territories;
    }
    return *this;
}

//destructor
Continent::~Continent() {
    delete name;
    delete bonus;
    delete territories;
}

//add territory if not null or present already
void Continent::addTerritory(Territory* territory) {
    if (!territory) return;
    if (find(territories->begin(), territories->end(), territory) == territories->end()) {
        territories->push_back(territory);
    }
}

const vector<Territory*>& Continent::getTerritories() const { 
    return *territories; 
}

int Continent::getBonus() const { return *bonus; }
string Continent::getName() const { return *name; }

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


//TERRITORY-----------------------------------------------------------------------------------------------------------------------
//Regular constructor
Territory::Territory(const string& n, int xCord, int yCord)
    : name(new string(n)),
    x(new int(xCord)),
    y(new int(yCord)),
    numberOfArmies(new int(0)),
    adjacentTerritories(new vector<Territory*>()),
    owner(nullptr) {}  // Assignment 2 addition

// Copy constructor w/shallow copy for adjacency, for new territories
Territory::Territory(const Territory& other)
    : name(new string(*other.name)),
    x(new int(*other.x)),
    y(new int(*other.y)),
    numberOfArmies(new int(*other.numberOfArmies)),
    adjacentTerritories(new vector<Territory*>(*other.adjacentTerritories)),
    owner(other.owner) {}  // Assignment 2 addition

//when territory already exists
Territory& Territory::operator=(const Territory& other) {
    if (this != &other) {
        *name            = *other.name;
        *x               = *other.x;
        *y               = *other.y;
        *numberOfArmies  = *other.numberOfArmies;
        *adjacentTerritories = *other.adjacentTerritories;
        owner = other.owner;  // Assignment 2 addition
    }
    return *this;
}

//destructor
Territory::~Territory() {
    delete name;
    delete x;
    delete y;
    delete numberOfArmies;
    delete adjacentTerritories;
    // owner is not deleted - it's managed by the game, not Territory
}

//add neighboring territory if not null, itself, or already present
void Territory::addAdjacentTerritory(Territory* territory) {
    if (!territory || territory == this) return;
    if (find(adjacentTerritories->begin(), adjacentTerritories->end(), territory)
        == adjacentTerritories->end()) {
        adjacentTerritories->push_back(territory);
    }
}

const vector<Territory*>& Territory::getAdjacentTerritories() const {
    return *adjacentTerritories;
}

string Territory::getName() const { return *name; }
int Territory::getX() const { return *x; }
int Territory::getY() const { return *y; }
int Territory::getNumberOfArmies() const { return *numberOfArmies; }

void Territory::setName(const string& n) { *name = n; }
void Territory::setX(int xCord) { *x = xCord; }
void Territory::setY(int yCord) { *y = yCord; }
void Territory::setNumberOfArmies(int numOfArmies) { *numberOfArmies = numOfArmies; }

// Assignment 2 additions for order execution
bool Territory::isAdjacentTo(Territory* other) const {
    if (!other) return false;
    for (Territory* adj : *adjacentTerritories) {
        if (adj == other) {
            return true;
        }
    }
    return false;
}

Player* Territory::getOwner() const {
    return owner;
}

void Territory::setOwner(Player* newOwner) {
    owner = newOwner;
}

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


//MAP-----------------------------------------------------------------------------------------------------------------------
//normal constructor
Map::Map(const string &scr, const string &auth,
        const string &nm, bool wp, bool wn)
    : scroll(new string(scr)),
    author(new string(auth)),
    name(new string(nm)),
    wrap(new bool(wp)),
    warn(new bool(wn)),
    territories(new vector<Territory*>()),
    continents(new vector<Continent*>()) {}

//copy constructor
Map::Map(const Map &other)
    : scroll(new string(*other.scroll)),
    author(new string(*other.author)),
    name(new string(*other.name)),
    wrap(new bool(*other.wrap)),
    warn(new bool(*other.warn)),
    territories(new vector<Territory*>()),
    continents(new vector<Continent*>())
{
    //deep copy continents
    unordered_map<const Continent *, Continent *> cMap;
    for (auto *oc : *other.continents)
    {
        auto *nc = new Continent(oc->getName(), oc->getBonus());
        continents->push_back(nc);
        cMap[oc] = nc;
    }
    //deep copy territories
    unordered_map<const Territory *, Territory *> tMap;
    for (auto *ot : *other.territories)
    {
        auto *nt = new Territory(ot->getName(), ot->getX(), ot->getY());
        nt->setNumberOfArmies(ot->getNumberOfArmies());
        territories->push_back(nt);
        tMap[ot] = nt;
    }
    // new deep copy continents assign to new deep copy territories
    for (auto *oc : *other.continents)
    {
        Continent *nc = cMap[oc];
        for (auto *ot : oc->getTerritories())
        {
            nc->addTerritory(tMap[ot]);
        }
    }
    // new deep copy territories assign to new deep copy adjacent territories
    for (auto *ot : *other.territories)
    {
        Territory *nt = tMap[ot];
        for (auto *oa : ot->getAdjacentTerritories())
        {
            nt->addAdjacentTerritory(tMap[oa]);
        }
    }
}

Map &Map::operator=(const Map &other)
{
    if (this != &other)
    {
        for (auto *t : *territories)
            delete t;
        for (auto *c : *continents)
            delete c;
        territories->clear();
        continents->clear();

        *scroll = *other.scroll;
        *author = *other.author;
        *name = *other.name;
        *wrap = *other.wrap;
        *warn = *other.warn;

        // deep copy continents 
        unordered_map<const Continent *, Continent *> cMap;
        for (auto *oc : *other.continents)
        {
            auto *nc = new Continent(oc->getName(), oc->getBonus());
            continents->push_back(nc);
            cMap[oc] = nc;
        }
        // deep copy territories
        unordered_map<const Territory *, Territory *> tMap;
        for (auto *ot : *other.territories)
        {
            auto *nt = new Territory(ot->getName(), ot->getX(), ot->getY());
            nt->setNumberOfArmies(ot->getNumberOfArmies());
            territories->push_back(nt);
            tMap[ot] = nt;
        }
        // new deep copy continents assign to new deep copy territories
        for (auto *oc : *other.continents)
        {
            Continent *nc = cMap[oc];
            for (auto *ot : oc->getTerritories())
            {
                nc->addTerritory(tMap[ot]);
            }
        }
        // new deep copy territories assign to new deep copy adjacent territories
        for (auto *ot : *other.territories)
        {
            Territory *nt = tMap[ot];
            for (auto *oa : ot->getAdjacentTerritories())
            {
                nt->addAdjacentTerritory(tMap[oa]);
            }
        }
    }
    return *this;
}

//destructor
Map::~Map()
{
    for (auto *t : *territories)
        delete t;
    for (auto *c : *continents)
        delete c;
    delete scroll;
    delete author;
    delete name;
    delete wrap;
    delete warn;
    delete territories;
    delete continents;
}

void Map::addTerritory(Territory *territory)
{
    if (!territory)
        return;
    if (find(territories->begin(), territories->end(), territory) == territories->end())
    {
        territories->push_back(territory);
    }
}

void Map::addContinent(Continent *continent)
{
    if (!continent)
        return;
    if (find(continents->begin(), continents->end(), continent) == continents->end())
    {
        continents->push_back(continent);
    }
}

string Map::getScroll() const { return *scroll; }
string Map::getAuthor() const { return *author; }
string Map::getName() const { return *name; }
bool Map::getWrap() const { return *wrap; }
bool Map::getWarn() const { return *warn; }

const vector<Continent *> &Map::getContinents() const { return *continents; }
const vector<Territory*>& Map::getTerritories() const { return *territories; }



void Map::dfs(Territory *start, unordered_set<Territory *> &visited) const
{
    if (!start || visited.count(start))
        return;
    queue<Territory *> q;
    visited.insert(start);
    q.push(start);
    while (!q.empty())
    {
        auto *cur = q.front();
        q.pop();
        for (auto *nb : cur->getAdjacentTerritories())
        {
            if (nb && !visited.count(nb))
            {
                visited.insert(nb);
                q.push(nb);
            }
        }
    }
}

bool Map::isConnected() const
{
    if (territories->empty())
        return false;
    unordered_set<Territory *> visited;
    dfs((*territories)[0], visited);
    return visited.size() == territories->size();
}

bool Map::areContinentsInterconnected() const
{
    
    for (auto *c : *continents)
    {
        const auto &ts = c->getTerritories();
        if (ts.empty())
            continue; 
        unordered_set<Territory *> allowed(ts.begin(), ts.end());
        unordered_set<Territory *> visited;

        
        queue<Territory *> q;
        q.push(ts[0]);
        visited.insert(ts[0]);
        while (!q.empty())
        {
            Territory *cur = q.front();
            q.pop();
            for (auto *nb : cur->getAdjacentTerritories())
            {
                if (!nb || !allowed.count(nb))
                    continue;
                if (!visited.count(nb))
                {
                    visited.insert(nb);
                    q.push(nb);
                }
            }
        }
        if (visited.size() != ts.size())
            return false;
    }
    return true;
}

bool Map::areTerritoriesUniqueToContinents() const
{
    
    for (auto *t : *territories)
    {
        int count = 0;
        for (auto *c : *continents)
        {
            const auto &ts = c->getTerritories();
            count += static_cast<int>(count_if(ts.begin(), ts.end(),
                                               [&](auto *item)
                                            { return item == t; }));
        }
        if (count != 1)
            return false;
    }
    return true;
}

bool Map::validate() const
{
    if (territories->empty())
        return false;
    if (!isConnected())
        return false;
    if (!areContinentsInterconnected())
        return false;
    if (!areTerritoriesUniqueToContinents())
        return false;
    return true;
}

ostream &operator<<(ostream &out, const Map &map)
{
    out << "Map{name=" << map.getName()
        << ", author=" << map.getAuthor()
        << ", scroll=" << map.getScroll()
        << ", wrap=" << (map.getWrap() ? "true" : "false")
        << ", warn=" << (map.getWarn() ? "true" : "false")
        << "}\nContinents:\n";
    for (auto *c : map.getContinents())
        out << "  " << *c << "\n";
    out << "Territories:\n";
    for (auto *t : map.getTerritories())
        out << "  " << *t << "\n";
    return out;
}
//MAPLOADER-----------------------------------------------------------------------------------------------------------------------

namespace {
    bool parseInt(const string& s, int& out) {
        try {
            size_t idx = 0; int v = stoi(s, &idx);
            if (idx != s.size()) return false;
            out = v; return true;
        } catch (...) { return false; }
    }
    bool parseBool(const string& s, bool& out) {
        string t = s; for (auto& ch: t) ch = tolower(ch);
        if (t == "yes" || t == "true" || t == "1") { out = true; return true; }
        if (t == "no"  || t == "false"|| t == "0") { out = false; return true; }
        return false;
    }
}

MapLoader::MapLoader() = default;
MapLoader::MapLoader(const MapLoader&) = default;
MapLoader& MapLoader::operator=(const MapLoader&) = default;
MapLoader::~MapLoader() = default;

Map* MapLoader::load(const string& path, string* errorOut) const {
    ifstream in(path);
    if (!in) {
        if (errorOut) *errorOut = "Cannot open file: " + path;
        return nullptr;
    }


    string mapName = "Unknown";
    string author  = "Unknown";
    string scroll  = "none";
    bool wrap = false;
    bool warn = false;


    unordered_map<string, int> contBonus;
    struct Row { int x; int y; string continent; vector<string> adj; };
    unordered_map<string, Row> rows;

    enum Sec { NONE, CONTINENTS, TERRITORIES };
    Sec sec = NONE;

    string line;
    while (getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0]==';' || line[0]=='#') continue;


        if (line == "[Map]") { sec = NONE; continue; }
        if (line == "[Continents]") { sec = CONTINENTS; continue; }
        if (line == "[Territories]") { sec = TERRITORIES; continue; }
        if (!line.empty() && line.front()=='[' && line.back()==']') { sec = NONE; continue; }


        auto eq = line.find('=');
        if (eq != string::npos && sec == NONE) {
            string key = trim(line.substr(0, eq));
            string val = trim(line.substr(eq+1));
            if (key == "author") author = val;
            else if (key == "wrap") parseBool(val, wrap);
            else if (key == "warn") parseBool(val, warn);
            else if (key == "scroll") scroll = val;
            else if (key == "name") mapName = val;
            continue;
        }

        if (sec == CONTINENTS) {
            auto pos = line.find('=');
            if (pos == string::npos) continue;
            string cname = trim(line.substr(0, pos));
            string bstr  = trim(line.substr(pos+1));
            int b=0; if (!parseInt(bstr, b)) continue;
            contBonus[cname] = b;
        } else if (sec == TERRITORIES) {
            auto parts = splitCSV(line);
            if (parts.size() < 4) continue;
            string tname = trim(parts[0]);
            int x=0, y=0;
            parseInt(trim(parts[1]), x);
            parseInt(trim(parts[2]), y);
            string cName = trim(parts[3]);
            vector<string> adj;
            for (size_t i=4;i<parts.size();++i) {
                string a = trim(parts[i]);
                if (!a.empty()) adj.push_back(a);
            }
            rows[tname] = {x,y,cName,adj};
        }
    }


    Map* map = new Map(scroll, author, mapName, wrap, warn);

    unordered_map<string, Continent*> cptrs;
    for (const auto& kv : contBonus) {
        auto* c = new Continent(kv.first, kv.second);
        cptrs[kv.first] = c;
        map->addContinent(c);
    }

    unordered_map<string, Territory*> tptrs;
    for (const auto& kv : rows) {
        const string& tname = kv.first;
        const auto& r = kv.second;
        auto* t = new Territory(tname, r.x, r.y);
        tptrs[tname] = t;
        map->addTerritory(t);

        auto itc = cptrs.find(r.continent);
        if (itc != cptrs.end()) {
            itc->second->addTerritory(t);
        }
    }


    for (const auto& kv : rows) {
        Territory* t = tptrs[kv.first];
        for (const auto& aname : kv.second.adj) {
            auto ita = tptrs.find(aname);
            if (ita != tptrs.end()) {
                t->addAdjacentTerritory(ita->second);
                ita->second->addAdjacentTerritory(t);
            }
        }
    }

    if (!map->validate()) {
        if (errorOut) *errorOut = "Invalid map (validation check failed).";
        delete map;
        return nullptr;
    }
    return map;
}

ostream& operator<<(ostream& os, const MapLoader&) {
    os << "MapLoader{}";
    return os;
}

string MapLoader::trim(const string& s) {
    size_t b=0, e=s.size();
    while (b<e && isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e>b && isspace(static_cast<unsigned char>(s[e-1]))) --e;
    return s.substr(b, e-b);
}

// Split a CSV line into parts
vector<string> MapLoader::splitCSV(const string& line) {
    vector<string> out; 
    string cur;
    istringstream ss(line);
    while (getline(ss, cur, ',')) 
        out.push_back(cur);
    return out;
}