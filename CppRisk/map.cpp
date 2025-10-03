#include "map.h"
#include "country.h"
#include "continent.h"
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <unordered_map>

using namespace std;

//normal constructor
Map::Map(const string &scr, const string &auth,
        const string &nm, bool wp, bool wn)
    : scroll(new string(scr)),
    author(new string(auth)),
    name(new string(nm)),
    wrap(new bool(wp)),
    warn(new bool(wn)),
    territories(),
    continents() {}

//copy constructor
Map::Map(const Map &other)
    : scroll(new string(*other.scroll)),
    author(new string(*other.author)),
    name(new string(*other.name)),
    wrap(new bool(*other.wrap)),
    warn(new bool(*other.warn)),
    territories(),
    continents()
{
    //deep copy continents
    unordered_map<const Continent *, Continent *> cMap;
    for (auto *oc : other.continents)
    {
        auto *nc = new Continent(oc->getName(), oc->getBonus());
        continents.push_back(nc);
        cMap[oc] = nc;
    }
    //deep copy territories
    unordered_map<const Territory *, Territory *> tMap;
    for (auto *ot : other.territories)
    {
        auto *nt = new Territory(ot->getName(), ot->getX(), ot->getY());
        nt->setNumberOfArmies(ot->getNumberOfArmies());
        territories.push_back(nt);
        tMap[ot] = nt;
    }
    // new deep copy continents assign to new deep copy territories
    for (auto *oc : other.continents)
    {
        Continent *nc = cMap[oc];
        for (auto *ot : oc->getTerritories())
        {
            nc->addTerritory(tMap[ot]);
        }
    }
    // new deep copy territories assign to new deep copy adjacent territories
    for (auto *ot : other.territories)
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
        for (auto *t : territories)
            delete t;
        for (auto *c : continents)
            delete c;
        territories.clear();
        continents.clear();

        *scroll = *other.scroll;
        *author = *other.author;
        *name = *other.name;
        *wrap = *other.wrap;
        *warn = *other.warn;

        // deep copy continents 
        unordered_map<const Continent *, Continent *> cMap;
        for (auto *oc : other.continents)
        {
            auto *nc = new Continent(oc->getName(), oc->getBonus());
            continents.push_back(nc);
            cMap[oc] = nc;
        }
        // deep copy territories
        unordered_map<const Territory *, Territory *> tMap;
        for (auto *ot : other.territories)
        {
            auto *nt = new Territory(ot->getName(), ot->getX(), ot->getY());
            nt->setNumberOfArmies(ot->getNumberOfArmies());
            territories.push_back(nt);
            tMap[ot] = nt;
        }
        // new deep copy continents assign to new deep copy territories
        for (auto *oc : other.continents)
        {
            Continent *nc = cMap[oc];
            for (auto *ot : oc->getTerritories())
            {
                nc->addTerritory(tMap[ot]);
            }
        }
        // new deep copy territories assign to new deep copy adjacent territories
        for (auto *ot : other.territories)
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
    for (auto *t : territories)
        delete t;
    for (auto *c : continents)
        delete c;
    delete scroll;
    delete author;
    delete name;
    delete wrap;
    delete warn;
}

void Map::addTerritory(Territory *territory)
{
    if (!territory)
        return;
    if (find(territories.begin(), territories.end(), territory) == territories.end())
    {
        territories.push_back(territory);
    }
}

void Map::addContinent(Continent *continent)
{
    if (!continent)
        return;
    if (find(continents.begin(), continents.end(), continent) == continents.end())
    {
        continents.push_back(continent);
    }
}

string Map::getScroll() const { return *scroll; }
string Map::getAuthor() const { return *author; }
string Map::getName() const { return *name; }
bool Map::getWrap() const { return *wrap; }
bool Map::getWarn() const { return *warn; }

const vector<Continent *> &Map::getContinents() const { return continents; }
const vector<Territory*>& Map::getTerritories() const { return territories; }



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
    if (territories.empty())
        return false;
    unordered_set<Territory *> visited;
    dfs(territories.front(), visited);
    return visited.size() == territories.size();
}

bool Map::areContinentsInterconnected() const
{
    
    for (auto *c : continents)
    {
        const auto &ts = c->getTerritories();
        if (ts.empty())
            continue; 
        unordered_set<Territory *> allowed(ts.begin(), ts.end());
        unordered_set<Territory *> visited;

        
        queue<Territory *> q;
        q.push(ts.front());
        visited.insert(ts.front());
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
    
    for (auto *t : territories)
    {
        int count = 0;
        for (auto *c : continents)
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

bool Map::isValid() const
{
    if (territories.empty())
        return false;
    if (!isConnected())
        return false;
    if (!areContinentsInterconnected())
        return false;
    if (!areTerritoriesUniqueToContinents())
        return false;
    return true;
}

//pretty print
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
