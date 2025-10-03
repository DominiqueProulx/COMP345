#include "MapLoader.h"
#include "map.h"
#include "continent.h"
#include "country.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
using namespace std;

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

    if (!map->isValid()) {
        if (errorOut) *errorOut = "Invalid map check failed).";
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

vector<string> MapLoader::splitCSV(const string& line) {
    vector<string> out; string cur;
    istringstream ss(line);
    while (getline(ss, cur, ',')) out.push_back(cur);
    return out;
}
