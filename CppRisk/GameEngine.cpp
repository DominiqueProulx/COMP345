#include "GameEngine.h"
#include "map.h"
#include "Player.h"
#include "Cards.h"
#include "map.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>
#include "CommandProcessing.h"
#include <memory>
#include <limits>

/* -------------------------- */
/* -- STATE IMPLEMENTATION -- */
/* -------------------------- */

// Default constrcutor, intiializes an empty state object with no data.
GameEngine::State::State()
{
    name = new std::string("");
    transitions = new StateMap;
    substates = new StateList;
    initialSubstate = nullptr;
    parent = new bool(false);
}

// Initializes a new empty State object with a specified name and parent status.
GameEngine::State::State(const std::string& name, bool isParent)
{
    this->name = new std::string(name);
    transitions = new StateMap;
    substates = new StateList;
    initialSubstate = nullptr;
    this->parent = new bool(isParent);
}

// Duplicates an existing State into a new distinct object.
GameEngine::State::State(const State& other)
{
    name = new std::string(*other.name);
    transitions = new StateMap(*other.transitions);
    substates = new StateList(*other.substates);
    initialSubstate = other.initialSubstate;
    parent = new bool(*other.parent);
}

// Destroys this State instance and clears all dynamic memory.
// State pointers are NOT DESTROYED, as they are owned and deleted by the GameEngine itself.
GameEngine::State::~State()
{
    delete name;
    delete transitions;
    delete substates;
    // initial substate is left alone
    delete parent;
}

// Reassigns this State instance to a new State object by deep copying all fields.
GameEngine::State& GameEngine::State::operator=(const State& other)
{
    // short-circuit self-assignment
    if (this == &other)
        return *this;

    // deep copy other's fields and clear old memory
    delete name;
    name = new std::string(*other.name);

    delete transitions;
    transitions = new StateMap(*other.transitions);

    delete substates;
    substates = new StateList(*other.substates);

    // do not clear memory for initial substate, it is owned by GameEngine
    this->initialSubstate = other.initialSubstate;

    delete parent;
    parent = new bool(*other.parent);

    return *this;
}

// Prints a complete summary of a State object's fields.
std::ostream& operator<<(std::ostream& os, const GameEngine::State& state)
{
    os << "STATE [" << state.getName() << "]:\n";

    if (state.isParent())
    {
        if (state.getInitialSubstatePtr() != nullptr)
            os << "\tPARENT NODE (Initial: " << state.getInitialSubstatePtr()->getName() << ")\n";
        else
            os << "\tPARENT NODE (Initial: NULL)\n";
        os << "\tCHILDREN:\n";

        for (const auto& s : state.getSubstates())
            os << "\t\t" << s->getName() << " (" << s->getTransitions().size() << " transitions)\n";
    }

    os << "\tTRANSITIONS TO (" << state.getTransitions().size() << "):\n";
    for (const auto& [cmd, ptr] : state.getTransitions())
    {
        os << "\t\t" << '[' << ptr->getName() << "] via " << cmd << '\n';
    }

    return os;
}

// Adds an existing State object to this State's list of substates.
void GameEngine::State::addSubstate(State* newState)
{
    substates->push_back(newState);
}

// Adds a new transition (command -> destination State) to this State's list of transitions.
void GameEngine::State::addTransition(const std::string& cmd, State* dest)
{
    (*transitions)[cmd] = dest;
}

// Return this State's name.
std::string GameEngine::State::getName() const
{
    return *name;
}

// Returns this State's list of substates.
const GameEngine::State::StateList& GameEngine::State::getSubstates() const
{
    return *substates;
}

// Returns this State's list of transitions.
const GameEngine::State::StateMap& GameEngine::State::getTransitions() const
{
    return *transitions;
}

// Returns this State's initial substate.
GameEngine::State* GameEngine::State::getInitialSubstatePtr() const
{
    return initialSubstate;
}

// Returns true if this state is a parent state and holds other substates.
bool GameEngine::State::isParent() const
{
    return *parent;
}

// Sets this State's initial substate to a new state.
void GameEngine::State::setInitialSubstate(State* state)
{
    initialSubstate = state;
}

// Returns the destination State of a transition for a given command. If the command is invalid, nullptr is returned.
GameEngine::State* GameEngine::State::resolveTransition(const std::string& cmd) const
{
    const auto& result{ (*transitions).find(cmd) };
    return result == (*transitions).end() ? nullptr : result->second;
}

/* -------------------------------- */
/* -- GAME ENGINE IMPLEMENTATION -- */
/* -------------------------------- */

// Initializes an empty GameEngine object.
GameEngine::GameEngine()
{
    activeParentState = nullptr;
    activeState = nullptr;
    parentStates = new std::vector<State*>();
    states = new std::vector<State*>();

    //A2_P2
    map = nullptr;
    mapLoader = new MapLoader();
    players = new std::vector <Player*>();
    deck = new Deck();
    deck->initializeDeck();

}

// Duplicates an existing GameEngine into a new distinct object.
GameEngine::GameEngine(const GameEngine& other)
{
    activeParentState = other.activeParentState;
    activeState = other.activeState;
    parentStates = new std::vector<State*>(*other.parentStates);
    states = new std::vector<State*>(*other.states);

    //A2_P2
    map = other.map ? new Map(*other.map) : nullptr;
    mapLoader = new MapLoader(*other.mapLoader);
    players = new std::vector<Player*>(*other.players);
    deck = other.deck ? new Deck(*other.deck) : nullptr;

}

// Destroys a GameEngine object and ALL State objects created using this engine.
GameEngine::~GameEngine()
{
    delete parentStates;

    // delete all State objects
    for (const auto& s : *states)
        delete s;

    delete states;

    //A2_P2
    if (deck) delete deck;
    if (map) delete map;
    if (mapLoader) delete mapLoader;
    if (deck) delete deck;

    if (players) {
        for (auto* p : *players) delete p;
        delete players;
    }
}

// Reassigns this GameEngine instance to a new GameEngine object by deep copying all fields.
GameEngine& GameEngine::operator=(const GameEngine& other)
{
    // short-circuit self-assignment
    if (this == &other)
        return *this;

    // deep copy other's fields and clear old memory
    activeParentState = other.activeParentState;
    activeState = other.activeState;

    delete parentStates;
    parentStates = new std::vector<State*>(*other.parentStates);

    delete states;
    states = new std::vector<State*>(*other.states);

    if (deck) delete deck;
    deck = other.deck ? new Deck(*other.deck) : nullptr;

    if (map) delete map;
    map = other.map ? new Map(*other.map) : nullptr;

    if (mapLoader) delete mapLoader;
    mapLoader = new MapLoader(*other.mapLoader);

    if (players) {
        for (auto* p : *players) delete p;
        delete players;
    }
    players = new std::vector<Player*>(*other.players);

    return *this;
}

// Prints a complete summary of a GameEngine object's fields.
std::ostream& operator<<(std::ostream& os, const GameEngine& engine)
{
    os << "GAME ENGINE:\n";

    if (engine.activeParentState != nullptr)
        os << "\tACTIVE PARENT STATE: " << engine.activeParentState->getName() << " (Initial: " << engine.activeParentState->getInitialSubstatePtr()->getName() << ")\n";

    if (engine.activeState != nullptr)
        os << "\tACTIVE SUBSTATE: " << engine.activeState->getName() << '\n';

    os << "\tPARENT STATES (" << engine.parentStates->size() << "):\n";
    for (const auto& s : *engine.parentStates)
        os << "\t\t" << s->getName() << "(" << s->getSubstates().size() << " substates)\n";

    os << "\tSTARTUP: players=" << (engine.players ? engine.players->size() : 0)
        << ", map=" << (engine.map ? "loaded" : "null")
        << ", deck=" << (engine.deck ? "ok" : "null") << '\n';

    return os;
}

// Creates and returns a new State reference. Reference is stored and owned under this engine.
GameEngine::State* GameEngine::createState(const std::string& name, bool isParent)
{
    State* s = new State(name, isParent);
    states->push_back(s);
    return s;
}

// Adds a new top-level state to the GameEngine's state list. If no states have been added yet, the first is set to
// the active parent state.
void GameEngine::addParentStates(const std::initializer_list<State*>& states)
{
    State::StateList& parents{ *parentStates };

    for (const auto& state : states)
    {
        // set an initial state if the list is empty
        if (parents.size() == 0)
            activeParentState = state;

        parents.push_back(state);
    }
}

// Adds a new substate to an existing parent's state list. If no states have been added to the parent yet, the first
// is set as the initial state.
void GameEngine::addChildStates(State* parent, const std::initializer_list<State*>& states)
{
    State& parentState = *parent;

    for (const auto& state : states)
    {
        // set an initial substate if no states have been added yet
        if (parentState.getInitialSubstatePtr() == nullptr)
            parentState.setInitialSubstate(state);

        parentState.addSubstate(state);
    }
}

// Adds a new transition from one existing State to another via a command string.
void GameEngine::addChildTransition(State* from, const std::string& cmd, State* to)
{
    from->addTransition(cmd, to);
}

// Initializes the Finite State Machine (FSM) for C++ Risk based on the assignment's diagram.
void GameEngine::initializeRiskFSM(GameEngine& engine)
{
    // adding parent states to recreate the state diagram
    GameState startup{ engine.createState("startup", true) };
    GameState play{ engine.createState("play", true) };
    GameState end{ engine.createState("end", true) };
    engine.addParentStates({ startup, play, end });

    // STARTUP
    GameState start{ engine.createState("start", false) };
    GameState mapLoaded{ engine.createState("map loaded", false) };
    GameState mapValidated{ engine.createState("map validated", false) };
    GameState playersAdded{ engine.createState("players added", false) };

    engine.addChildStates(startup, { start, mapLoaded, mapValidated, playersAdded });
    engine.addChildTransition(start, "loadmap", mapLoaded);
    engine.addChildTransition(mapLoaded, "loadmap", mapLoaded);
    engine.addChildTransition(mapLoaded, "validatemap", mapValidated);
    engine.addChildTransition(mapValidated, "addplayer", playersAdded);
    engine.addChildTransition(playersAdded, "addplayer", playersAdded);
    engine.addChildTransition(playersAdded, "gamestart", play);
    engine.setActiveState(startup->getInitialSubstatePtr());

    // PLAY
    GameState assignReinforcements{ engine.createState("assign reinforcements", false) };
    GameState issueOrders{ engine.createState("issue orders", false) };
    GameState executeOrders{ engine.createState("execute orders", false) };
    GameState win{ engine.createState("win", false) };

    engine.addChildStates(play, { assignReinforcements, issueOrders, executeOrders, win });
    engine.addChildTransition(assignReinforcements, "issueorder", issueOrders);
    engine.addChildTransition(issueOrders, "issueorder", issueOrders);
    engine.addChildTransition(issueOrders, "issueordersend", executeOrders);
    engine.addChildTransition(executeOrders, "execorder", executeOrders);
    engine.addChildTransition(executeOrders, "endexecorders", assignReinforcements);
    engine.addChildTransition(executeOrders, "win", win);
    engine.addChildTransition(win, "replay", startup);
    engine.addChildTransition(win, "end", end);

    // END
    GameState final{ engine.createState("quit", false) };
    engine.addChildStates(end, { final });
}

// Returns this GameEngine's active substate.
GameEngine::State* GameEngine::getActiveStatePtr() const
{
    return activeState;
}

// Returns this GameEngine's active parent (top-level) state.
GameEngine::State* GameEngine::getActiveParentStatePtr() const
{
    return activeParentState;
}

// Sets this GameEngine's active substate.
void GameEngine::setActiveState(State* state)
{
    activeState = state;
}

// Sets this GameEngine's active parent state.
void GameEngine::setActiveParentState(State* state)
{
    activeParentState = state;
}

// Reads and returns a single string of user input.
std::string GameEngine::readCommand() const
{
    std::string cmd{};
    std::cin >> cmd;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flush buffer for hanging input by user
    return cmd;
}

// Accepts a string command and updates the game's state based on available transitions.
// If no transitions by that name exist, an error message is printed to the console.
void GameEngine::changeGameState(const std::string& cmd)
{
    auto newState = activeState->resolveTransition(cmd);
    if (newState)
    {
        // handle case where substate transitions to new parent state
        if (newState->isParent())
        {
            activeParentState = newState;
            activeState = newState->getInitialSubstatePtr();
        }
        else
            activeState = newState;

        std::cout << "SUCCESS: transitioned to new state (" << activeState->getName() << ") via '" << cmd << "'.\n";
    }
    else
        std::cerr << "ERR: command '" << cmd << "' does not exist for active state (" << activeState->getName() << ").\n";
}

// Returns true if the active state is considered a final state (defined by no outgoing transitions).
bool GameEngine::isActiveStateFinal() const
{
    return activeState->getTransitions().size() == 0;
}

//helper commands for validate function in command processor
//If the command is allowed in the current state, it returns the State of the next state
bool GameEngine::isCommandValid(const std::string& cmd) const {
    if (!activeState) return false;
    return (activeState->resolveTransition(cmd) != nullptr);
}

std::string GameEngine::getCurrentStateName() const {
    if (!activeState) return "Unknown";
    return activeState->getName();
}

std::string GameEngine::getParentStateName() const {
    if (!activeParentState) return "Unknown";
    return activeParentState->getName();
}

void GameEngine::startupPhase(std::istream& in, std::ostream& out) {
    using std::numeric_limits;
    using std::streamsize;

    out << "Choose to read from Command Line (1) or File (2) or Quit (0): ";

    int choice;
    if (!(in >> choice)) {
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        out << "Invalid input.\n";
        return;
    }
    in.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) {

        out << "=== STARTUP PHASE ===\n"
            << "Commands:\n"
            << "  loadmap <filename>\n"
            << "  validatemap\n"
            << "  addplayer <playername>\n"
            << "  gamestart\n"
            << "  replay \n"
            << "  quit   \n";

        CommandProcessor cp;
        for (;;) {
            Command* c = cp.getCommand(*this);
            if (!c) { out << "No more commands.\n"; break; }
            processStartupCommand(c->getCommandString(), out);
            if (getActiveStatePtr() && getActiveStatePtr()->getName() == "quit") break;
        }
        return;
    }

    if (choice == 2) {

        out << "Enter file name (e.g., commands.txt): ";
        std::string filename;
        in >> filename;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            FileCommandProcessorAdapter f(filename);
            for (;;) {
                Command* raw = f.readCommand(*this);
                if (!raw) { out << "No more commands (input ended).\n"; break; }
                processStartupCommand(raw->getCommandString(), out);
                delete raw;
                if (getActiveStatePtr() && getActiveStatePtr()->getName() == "quit") break;
            }
        }
        catch (const std::exception& e) {
            out << "Error: " << e.what() << '\n';
        }
        return;
    }

    if (choice == 0) {
        out << "Exiting Command Processor Test.\n";
        return;
    }

    out << "Invalid choice.\n";
}

void GameEngine::processStartupCommand(const std::string& full, std::ostream& out) {
    std::istringstream ss(full);
    std::string verb, arg;
    ss >> verb;
    std::getline(ss, arg);
    if (!arg.empty() && arg.front() == ' ') arg.erase(0, 1);

    if (verb.empty()) { out << "Invalid command.\n"; return; }

    if (verb == "replay") {
        if (!isCommandValid("replay")) { out << "State invalid.\n"; return; }
        out << "Replaying... transitioning to start.\n";
        changeGameState("replay");
        return;
    }
    if (verb == "quit") {
        if (!isCommandValid("quit")) { out << "State invalid.\n"; return; }
        out << "Exiting program.\n";
        changeGameState("quit");
        return;
    }

    if (verb == "loadmap") {
        if (!isCommandValid("loadmap")) { out << "State invalid.\n"; return; }
        if (arg.empty()) { out << "Enter valid file name\n"; return; }
        if (cmdLoadMap(arg, out)) changeGameState("loadmap");
        return;
    }

    if (verb == "validatemap") {
        if (!isCommandValid("validatemap")) { out << "State invalid.\n"; return; }
        if (cmdValidateMap(out)) changeGameState("validatemap");
        return;
    }

    if (verb == "addplayer") {
        if (!isCommandValid("addplayer")) { out << "State invalid.\n"; return; }
        if (arg.empty()) { out << "Usage: addplayer <playername>\n"; return; }
        if (cmdAddPlayer(arg, out)) changeGameState("addplayer");
        return;
    }

    if (verb == "gamestart") {
        if (!isCommandValid("gamestart")) { out << "State invalid.\n"; return; }
        if (!players || players->size() < 2 || players->size() > 6) {
            out << "Need 2–6 players before 'gamestart'.\n"; return;
        }
        if (cmdGameStart(out)) {
            changeGameState("gamestart");
        }
        return;
    }

    out << "Invalid command.\n";
}

bool GameEngine::cmdLoadMap(const std::string& filename, std::ostream& out)
{
    std::string error;
    Map* loaded = mapLoader->load(filename, &error);
    if (!loaded) {
        out << "Failed to load map: " << error << "\n";
        return false;
    }
    if (map) delete map;
    map = loaded;
    out << " Loaded map '" << map->getName() << "' with "
        << map->getTerritories().size() << " territories.\n";
    return true;
}

bool GameEngine::cmdValidateMap(std::ostream& out)
{
    if (!map) { out << "No map loaded.\n"; return false; }
    bool ok = map->validate();
    out << (ok ? "Map validation succeeded.\n" : "Map validation failed.\n");
    return ok;
}

bool GameEngine::cmdAddPlayer(const std::string& name, std::ostream& out)
{
    if (players->size() >= 6) { out << "Max 6 players reached.\n"; return false; }
    Player* p = new Player();
    p->setColor(name); //There's no name in player
    players->push_back(p);
    out << "Player added: " << name << " (Total " << players->size() << ")\n";
    return true;
}

bool GameEngine::cmdGameStart(std::ostream& out)
{
    if (!map || !map->validate()) {
        out << "Map must be loaded and validated first.\n";
        return false;
    }
    if (players->size() < 2) {
        out << "At least 2 players required.\n";
        return false;
    }

    fairDistributeTerritories(out);
    randomizePlayerOrder(out);
    grant50Reinforcements(out);
    initialCardDraws(out);

    out << "GameStart complete.\n";
    return true;
}

void GameEngine::fairDistributeTerritories(std::ostream& out)
{
    if (!map || !players || players->empty()) {
        out << "No map or players to distribute.\n";
        return;
    }

    const auto& ts = map->getTerritories();
    if (ts.empty()) {
        out << "No territories to distribute.\n";
        return;
    }

    for (auto* p : *players) {
        auto* owned = const_cast<std::vector<Territory*>*>(p->getTerritories());
        if (owned) owned->clear();
    }

    std::size_t i = 0;
    for (auto* t : ts) {
        Player* owner = (*players)[i % players->size()];
        auto* owned = const_cast<std::vector<Territory*>*>(owner->getTerritories());
        if (owned) owned->push_back(t);
        ++i;
    }

    out << "Distributed " << ts.size() << " territories among " << players->size() << " players.\n";

    for (auto* p : *players) {
        cout << *p;
    }
}

void GameEngine::randomizePlayerOrder(std::ostream& out)
{
    if (!players) { out << "No players list.\n"; return; }
    std::mt19937 rng(42);
    std::shuffle(players->begin(), players->end(), rng);

    out << "Randomized Play Order (Seed=42):\n";
    for (std::size_t i = 0; i < players->size(); ++i) {
        const std::string* label = (*players)[i]->getColor(); // using color as name
        out << "  " << (i + 1) << ") " << (label ? *label : std::string{ "(unnamed)" }) << "\n";
    }
}

void GameEngine::grant50Reinforcements(std::ostream& out)
{
    if (!players) { out << "No players to grant reinforcements.\n"; return; }

    for (auto* p : *players) {
        p->setReinforcementPool(50);
    }
    out << "Granted each player 50 reinforcement armies.\n";
}

void GameEngine::initialCardDraws(std::ostream& out)
{

    if (!players) { out << "No players to draw.\n"; return; }

    for (auto* p : *players) {
        p->getHand();
        deck->draw(p->getHand());
        deck->draw(p->getHand());
    }
    out << "Each player drew 2 cards.\n";
}

