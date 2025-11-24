#include "GameEngine.h"
#include "map.h"
#include "Player.h"
#include "Cards.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
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
GameEngine::State::State(const std::string &name, bool isParent)
{
    this->name = new std::string(name);
    transitions = new StateMap;
    substates = new StateList;
    initialSubstate = nullptr;
    this->parent = new bool(isParent);
}

// Duplicates an existing State into a new distinct object.
GameEngine::State::State(const State &other)
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
GameEngine::State &GameEngine::State::operator=(const State &other)
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
std::ostream &operator<<(std::ostream &os, const GameEngine::State &state)
{
    os << "STATE [" << state.getName() << "]:\n";

    if (state.isParent())
    {
        if (state.getInitialSubstatePtr() != nullptr)
            os << "\tPARENT NODE (Initial: " << state.getInitialSubstatePtr()->getName() << ")\n";
        else
            os << "\tPARENT NODE (Initial: NULL)\n";
        os << "\tCHILDREN:\n";

        for (const auto &s : state.getSubstates())
            os << "\t\t" << s->getName() << " (" << s->getTransitions().size() << " transitions)\n";
    }

    os << "\tTRANSITIONS TO (" << state.getTransitions().size() << "):\n";
    for (const auto &[cmd, ptr] : state.getTransitions())
    {
        os << "\t\t" << '[' << ptr->getName() << "] via " << cmd << '\n';
    }

    return os;
}

// Adds an existing State object to this State's list of substates.
void GameEngine::State::addSubstate(State *newState)
{
    substates->push_back(newState);
}

// Adds a new transition (command -> destination State) to this State's list of transitions.
void GameEngine::State::addTransition(const std::string &cmd, State *dest)
{
    (*transitions)[cmd] = dest;
}

// Return this State's name.
std::string GameEngine::State::getName() const
{
    return *name;
}

// Returns this State's list of substates.
const GameEngine::State::StateList &GameEngine::State::getSubstates() const
{
    return *substates;
}

// Returns this State's list of transitions.
const GameEngine::State::StateMap &GameEngine::State::getTransitions() const
{
    return *transitions;
}

// Returns this State's initial substate.
GameEngine::State *GameEngine::State::getInitialSubstatePtr() const
{
    return initialSubstate;
}

// Returns true if this state is a parent state and holds other substates.
bool GameEngine::State::isParent() const
{
    return *parent;
}

// Sets this State's initial substate to a new state.
void GameEngine::State::setInitialSubstate(State *state)
{
    initialSubstate = state;
}

// Returns the destination State of a transition for a given command. If the command is invalid, nullptr is returned.
GameEngine::State *GameEngine::State::resolveTransition(const std::string &cmd) const
{
    const auto &result{(*transitions).find(cmd)};
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
    parentStates = new std::vector<State *>();
    states = new std::vector<State *>();

    // A2_P2
    map = nullptr;
    mapLoader = new MapLoader();
    players = new std::vector<Player *>();
    deck = new Deck();
    deck->initializeDeck();
}

// Duplicates an existing GameEngine into a new distinct object.
GameEngine::GameEngine(const GameEngine &other) : Subject(other)
{
    activeParentState = other.activeParentState;
    activeState = other.activeState;
    parentStates = new std::vector<State *>(*other.parentStates);
    states = new std::vector<State *>(*other.states);

    // A2_P2
    map = other.map ? new Map(*other.map) : nullptr;
    mapLoader = new MapLoader(*other.mapLoader);
    players = new std::vector<Player *>(*other.players);
    deck = other.deck ? new Deck(*other.deck) : nullptr;
}

// Destroys a GameEngine object and ALL State objects created using this engine.
GameEngine::~GameEngine()
{
    delete parentStates;
    parentStates = nullptr;

    // delete all State objects
    for (const auto &s : *states)
    {
        delete s;
    }
    if (states)
    {
        delete states;
        states = nullptr;
    }

    // A2_P2
    if (deck)
        delete deck;
    deck = nullptr;
    if (map)
        delete map;
    map = nullptr;
    if (mapLoader)
        delete mapLoader;
    mapLoader = nullptr;

    if (players)
    {
        for (auto *p : *players)
            delete p;
        delete players;
        players = nullptr;
    }
}

// Reassigns this GameEngine instance to a new GameEngine object by deep copying all fields.
GameEngine &GameEngine::operator=(const GameEngine &other)
{
    // short-circuit self-assignment
    if (this == &other)
        return *this;

    Subject::operator=(other);

    // deep copy other's fields and clear old memory
    activeParentState = other.activeParentState;
    activeState = other.activeState;

    if (deck)
        delete deck;
    deck = other.deck ? new Deck(*other.deck) : nullptr;

    if (map)
        delete map;
    map = other.map ? new Map(*other.map) : nullptr;

    if (mapLoader)
        delete mapLoader;
    mapLoader = new MapLoader(*other.mapLoader);

    if (players)
    {
        for (auto *p : *players)
            delete p;
        delete players;
    }
    players = new std::vector<Player *>(*other.players);

    return *this;
}

// Prints a complete summary of a GameEngine object's fields.
std::ostream &operator<<(std::ostream &os, const GameEngine &engine)
{
    os << "GAME ENGINE:\n";

    if (engine.activeParentState != nullptr)
        os << "\tACTIVE PARENT STATE: " << engine.activeParentState->getName() << " (Initial: " << engine.activeParentState->getInitialSubstatePtr()->getName() << ")\n";

    if (engine.activeState != nullptr)
        os << "\tACTIVE SUBSTATE: " << engine.activeState->getName() << '\n';

    os << "\tPARENT STATES (" << engine.parentStates->size() << "):\n";
    for (const auto &s : *engine.parentStates)
        os << "\t\t" << s->getName() << "(" << s->getSubstates().size() << " substates)\n";

    os << "\tSTARTUP: players=" << (engine.players ? engine.players->size() : 0)
       << ", map=" << (engine.map ? "loaded" : "null")
       << ", deck=" << (engine.deck ? "ok" : "null") << '\n';

    return os;
}

// Creates and returns a new State reference. Reference is stored and owned under this engine.
GameEngine::State *GameEngine::createState(const std::string &name, bool isParent)
{
    State *s = new State(name, isParent);
    states->push_back(s);
    return s;
}

// Adds a new top-level state to the GameEngine's state list. If no states have been added yet, the first is set to
// the active parent state.
void GameEngine::addParentStates(const std::initializer_list<State *> &states)
{
    State::StateList &parents{*parentStates};

    for (const auto &state : states)
    {
        // set an initial state if the list is empty
        if (parents.size() == 0)
            activeParentState = state;

        parents.push_back(state);
    }
}

// Adds a new substate to an existing parent's state list. If no states have been added to the parent yet, the first
// is set as the initial state.
void GameEngine::addChildStates(State *parent, const std::initializer_list<State *> &states)
{
    State &parentState = *parent;

    for (const auto &state : states)
    {
        // set an initial substate if no states have been added yet
        if (parentState.getInitialSubstatePtr() == nullptr)
            parentState.setInitialSubstate(state);

        parentState.addSubstate(state);
    }
}

// Adds a new transition from one existing State to another via a command string.
void GameEngine::addChildTransition(State *from, const std::string &cmd, State *to)
{
    from->addTransition(cmd, to);
}

// Initializes the Finite State Machine (FSM) for C++ Risk based on the assignment's diagram.
void GameEngine::initializeRiskFSM(GameEngine &engine)
{
    // adding parent states to recreate the state diagram
    GameState startup{engine.createState("startup", true)};
    GameState play{engine.createState("play", true)};
    GameState end{engine.createState("end", true)};
    engine.addParentStates({startup, play, end});

    // STARTUP
    GameState start{engine.createState("start", false)};
    GameState mapLoaded{engine.createState("map loaded", false)};
    GameState mapValidated{engine.createState("map validated", false)};
    GameState playersAdded{engine.createState("players added", false)};

    engine.addChildStates(startup, { start, mapLoaded, mapValidated, playersAdded });

    //added for Tournament mode
    engine.addChildTransition(start, "tournament", play);
  
    engine.addChildTransition(start, "loadmap", mapLoaded);
    engine.addChildTransition(mapLoaded, "loadmap", mapLoaded);
    engine.addChildTransition(mapLoaded, "validatemap", mapValidated);
    engine.addChildTransition(mapValidated, "addplayer", playersAdded);
    engine.addChildTransition(playersAdded, "addplayer", playersAdded);
    engine.addChildTransition(playersAdded, "gamestart", play);

    engine.setActiveState(startup->getInitialSubstatePtr());

    // PLAY
    GameState assignReinforcements{engine.createState("assign reinforcements", false)};
    GameState issueOrders{engine.createState("issue orders", false)};
    GameState executeOrders{engine.createState("execute orders", false)};
    GameState win{engine.createState("win", false)};

    engine.addChildStates(play, {assignReinforcements, issueOrders, executeOrders, win});
    engine.addChildTransition(assignReinforcements, "issueorder", issueOrders);
    engine.addChildTransition(assignReinforcements, "draw", win); // special transition for tournament mode
    engine.addChildTransition(issueOrders, "issueorder", issueOrders);
    engine.addChildTransition(issueOrders, "issueordersend", executeOrders);
    engine.addChildTransition(executeOrders, "execorder", executeOrders);
    engine.addChildTransition(executeOrders, "endexecorders", assignReinforcements);
    engine.addChildTransition(executeOrders, "win", win);
    engine.addChildTransition(win, "replay", startup);
    engine.addChildTransition(win, "quit", end);

    // END
    GameState final{engine.createState("quit", false)};
    engine.addChildStates(end, {final});
}

// Returns a log string describing the GameEngine's current state.
std::string GameEngine::stringToLog() const
{
    return "[ENGINE] Transitioned to new state: { " + activeParentState->getName() + "/" + activeState->getName() + " }.";
}

// Returns this GameEngine's active substate.
GameEngine::State *GameEngine::getActiveStatePtr() const
{
    return activeState;
}

// Returns this GameEngine's active parent (top-level) state.
GameEngine::State *GameEngine::getActiveParentStatePtr() const
{
    return activeParentState;
}

// Sets this GameEngine's active substate.
void GameEngine::setActiveState(State *state)
{
    activeState = state;
}

// Sets this GameEngine's active parent state.
void GameEngine::setActiveParentState(State *state)
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


int GameEngine::attachStrategyFromMenu(Player *p)
{
    std::cout << "Choose strategy for player '" << *(p->getColor()) << "':\n"
            << "1 - Human\n"
            << "2 - Aggressive\n"
            << "3 - Benevolent\n"
            << "4 - Neutral\n"
            << "5 - Cheater\n"
            << "Enter choice: ";

    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 5)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Enter 1–5: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flush line

    switch (choice)
    {
    case 1:
        p->setStrategy(std::make_unique<HumanPlayerStrategy>(p));
        break;
    case 2:
        p->setStrategy(std::make_unique<AggressivePlayerStrategy>(p));
        break;
    case 3:
        p->setStrategy(std::make_unique<BenevolentPlayerStrategy>(p));
        break;
    case 4:
        p->setStrategy(std::make_unique<NeutralPlayerStrategy>(p));
        break;
      case 5: p->setStrategy(std::make_unique<cheaterPlayerStrategy>(p)); 
        break;
    }
    return choice;
}


// Accepts a string command and updates the game's state based on available transitions.
// If no transitions by that name exist, an error message is printed to the console.
void GameEngine::changeGameState(const std::string &cmd)
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

        // log the state transition to gamelog.txt
        notify(this);
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
    if (!activeState || cmd == "draw") return false;

    return (activeState->resolveTransition(cmd) != nullptr);
}

std::string GameEngine::getCurrentStateName() const
{
    if (!activeState)
        return "Unknown";
    return activeState->getName();
}

std::string GameEngine::getParentStateName() const
{
    if (!activeParentState)
        return "Unknown";
    return activeParentState->getName();
}

void GameEngine::startupPhase(std::istream &in, std::ostream &out)
{
    using std::numeric_limits;
    using std::streamsize;

    out << "Choose to read from Command Line (1) or File (2) or Quit (0): ";

    int choice;
    if (!(in >> choice))
    {
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        out << "Invalid input.\n";
        return;
    }
    in.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1)
    {

        out << "=== STARTUP PHASE ===\n"
            << "Commands:\n"
            << "  loadmap <filename>\n"
            << "  validatemap\n"
            << "  addplayer <playername>\n"
            << "  gamestart\n";

        CommandProcessor cp;
        for (;;)
        {
            Command *c = cp.getCommand(*this);
            if (!c)
            {
                out << "No more commands.\n";
                break;
            }
            processStartupCommand(c->getCommandString(), out);

            // Exit Setup Phase if we reach play state
            if (getActiveParentStatePtr() && getActiveParentStatePtr()->getName() == "play")
            {
                out << "Game setup complete. Starting main game loop...\n";
                break;
            }

            if (getActiveStatePtr() && getActiveStatePtr()->getName() == "quit")
                break;
        }
        return;
    }

    if (choice == 2)
    {

        out << "Enter file name (e.g., commands.txt): ";
        std::string filename;
        in >> filename;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        try
        {
            FileCommandProcessorAdapter f(filename);
            for (;;)
            {
                Command *raw = f.readCommand(*this);
                if (!raw)
                {
                    out << "No more commands (input ended).\n";
                    break;
                }
                processStartupCommand(raw->getCommandString(), out);
                delete raw;

                if (getActiveParentStatePtr() && getActiveParentStatePtr()->getName() == "play")
                {
                    out << "Game setup complete. Starting main game loop...\n";
                    break;
                }

                if (getActiveStatePtr() && getActiveStatePtr()->getName() == "quit")
                    break;
            }
        }
        catch (const std::exception &e)
        {
            out << "Error: " << e.what() << '\n';
        }
        return;
    }

    if (choice == 0)
    {
        out << "Exiting Command Processor Test.\n";
        return;
    }

    out << "Invalid choice.\n";
}

void GameEngine::processStartupCommand(const std::string &full, std::ostream &out)
{
    std::istringstream ss(full);
    std::string verb, arg;
    ss >> verb;
    std::getline(ss, arg);
    if (!arg.empty() && arg.front() == ' ')
        arg.erase(0, 1);

    if (verb.empty())
    {
        out << "Invalid command.\n";
        return;
    }

    if (verb == "replay")
    {
        if (!isCommandValid("replay"))
        {
            out << "State invalid.\n";
            return;
        }
        out << "Replaying... transitioning to start.\n";
        changeGameState("replay");
        return;
    }
    if (verb == "quit")
    {
        if (!isCommandValid("quit"))
        {
            out << "State invalid.\n";
            return;
        }
        out << "Exiting program.\n";
        changeGameState("quit");
        return;
    }

    if (verb == "loadmap")
    {
        if (!isCommandValid("loadmap"))
        {
            out << "State invalid.\n";
            return;
        }
        if (arg.empty())
        {
            out << "Enter valid file name\n";
            return;
        }
        if (cmdLoadMap(arg, out))
            changeGameState("loadmap");
        return;
    }

    if (verb == "validatemap")
    {
        if (!isCommandValid("validatemap"))
        {
            out << "State invalid.\n";
            return;
        }
        if (cmdValidateMap(out))
            changeGameState("validatemap");
        return;
    }

    if (verb == "addplayer")
    {
        if (!isCommandValid("addplayer"))
        {
            out << "State invalid.\n";
            return;
        }
        if (arg.empty())
        {
            out << "Usage: addplayer <playername>\n";
            return;
        }
        if (cmdAddPlayer(arg, out))
            changeGameState("addplayer");
        return;
    }

    if (verb == "gamestart")
    {
        if (!isCommandValid("gamestart"))
        {
            out << "State invalid.\n";
            return;
        }
        if (!players || players->size() < 2 || players->size() > 6)
        {
            out << "Need 2�6 players before 'gamestart'.\n";
            return;
        }
        if (cmdGameStart(out))
        {
            changeGameState("gamestart");
        }
        return;
    }

    out << "Invalid command.\n";
}

bool GameEngine::cmdLoadMap(const std::string &filename, std::ostream &out)
{
    std::string error;
    Map *loaded = mapLoader->load(filename, &error);
    if (!loaded)
    {
        out << "Failed to load map: " << error << "\n";
        return false;
    }
    if (map)
        delete map;
    map = loaded;
    out << " Loaded map '" << map->getName() << "' with "
        << map->getTerritories().size() << " territories.\n";
    return true;
}

bool GameEngine::cmdValidateMap(std::ostream &out)
{
    if (!map)
    {
        out << "No map loaded.\n";
        return false;
    }
    bool ok = map->validate();
    out << (ok ? "Map validation succeeded.\n" : "Map validation failed.\n");
    return ok;
}

bool GameEngine::cmdAddPlayer(const std::string &name, std::ostream &out)
{
    if (players->size() >= 6)
    {
        out << "Max 6 players reached.\n";
        return false;
    }

    Player *p = new Player(deck);
    p->setColor(name); // still works as "name"
    int choice = GameEngine::attachStrategyFromMenu(p);

    players->push_back(p);
    out << "Player added: " << name
        << " [Strategy chosen: " << choice << "] "
        << "(Total " << players->size() << ")\n";

    return true;
}

bool GameEngine::cmdGameStart(std::ostream &out)
{
    if (!map || !map->validate())
    {
        out << "Map must be loaded and validated first.\n";
        return false;
    }
    if (players->size() < 2)
    {
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

void GameEngine::fairDistributeTerritories(std::ostream &out)
{
    if (!map || !players || players->empty())
    {
        out << "No map or players to distribute.\n";
        return;
    }

    const auto &ts = map->getTerritories();
    if (ts.empty())
    {
        out << "No territories to distribute.\n";
        return;
    }

    for (auto *p : *players)
    {
        auto *owned = const_cast<std::vector<Territory *> *>(p->getTerritories());
        if (owned)
            owned->clear();
    }

    std::size_t i = 0;
    for (auto *t : ts)
    {
        Player *owner = (*players)[i % players->size()];
        t->setOwner(owner);
        auto *owned = const_cast<std::vector<Territory *> *>(owner->getTerritories());
        if (owned)
            owned->push_back(t);
        ++i;
    }

    out << "Distributed " << ts.size() << " territories among " << players->size() << " players.\n";

    for (auto *p : *players)
    {
        // cout << *p;
    }

    // ========= RANDOMLY SET ARMIES PER TERRITORY =========
    std::srand(static_cast<unsigned>(time(nullptr)));

    for (auto *t : ts)
    {
        int randomArmies = 1 + (std::rand() % 5); // random 1–5 armies
        t->setNumberOfArmies(randomArmies);
    }

    out << "Distributed " << ts.size() << " territories among "
        << players->size() << " players.\n";

    // for (auto* p : *players) {
    //     std::cout << *p;
    // }
}

void GameEngine::randomizePlayerOrder(std::ostream &out)
{
    if (!players)
    {
        out << "No players list.\n";
        return;
    }
    std::mt19937 rng(42);
    std::shuffle(players->begin(), players->end(), rng);

    out << "Randomized Play Order (Seed=42):\n";
    for (std::size_t i = 0; i < players->size(); ++i)
    {
        const std::string *label = (*players)[i]->getColor(); // using color as name
        out << "  " << (i + 1) << ") " << (label ? *label : std::string{"(unnamed)"}) << "\n";
    }
}

void GameEngine::grant50Reinforcements(std::ostream &out)
{
    if (!players)
    {
        out << "No players to grant reinforcements.\n";
        return;
    }

    for (auto *p : *players)
    {
        p->setReinforcementPool(50);
    }
    out << "Granted each player 50 reinforcement armies.\n";
    for (auto *p : *players)
    {
        // std::cout << *p;
    }
}

void GameEngine::initialCardDraws(std::ostream &out)
{

    if (!players)
    {
        out << "No players to draw.\n";
        return;
    }

    for (auto *p : *players)
    {
        p->getHand();
        deck->draw(p->getHand());
        deck->draw(p->getHand());
    }
    out << "Each player drew 2 cards.\n";
}

// Checks if player still has territories, if the player is removed from the game
bool GameEngine::validatePlayerStays(Player *player)
{
    if (player->getTerritories()->size() == 0)
    {

        std::cout << "Player " << player->getName() << " has no territories left and is eliminated from the game." << std::endl;
        auto playerIndex = std::find(players->begin(), players->end(), player);
        if (playerIndex != players->end())
        {
            delete *playerIndex;         // delete the player
            players->erase(playerIndex); // remove the pointer from the vector
        }
        else
        {
            std::cerr << "ERROR: Player " << player->getName() << " not found in active players list." << std::endl;
        }
        return false;
    }
    else
    {
        std::cout << "Player " << player->getName() << " still has territories and remains in the game." << std::endl;
        return true;
    }
}

// Main game loop

/*Reinforcement phase :
    - Allocates the number of armies to players based on territories and continue bonus
    - Resets the turn variables( negotionations, conquered this turn, pendingdeployment)
    - Draws card back up to 2
*/
bool GameEngine::reinforcementPhase()
{
    std::cout << "---------------------------" << std::endl;
    std::cout << "Reinforcement Phase" << std::endl;
    std::cout << "---------------------------" << std::endl;
    for (Player *player : *players)
    {

        // Draw Cards back up
        std::cout << "drawing cards back up " << std::endl;
        player->drawBackUpCards();

        // Clear Order effects from last turn
        player->clearNegotiations();
        player->setConqueredThisTurn(false);
        player->clearPendingDeployments();

        // Calculate reinforcements based on territories owned
        int numTerritories = player->getTerritories()->size();
        int reinforcements = std::max(3, numTerritories / 3); // minimum of 3 reinforcements per turn
        std::cout << "Player " << player->getName() << " receives " << reinforcements << " base reinforcements." << std::endl;

        // Calculate Continent bonuses
        for (Continent *c : getMap()->getContinents())
        {
            bool ownsAll = true;
            for (Territory *t : c->getTerritories())
            {
                if (t->getOwner() != player)
                {
                    ownsAll = false;
                    break;
                }
            }
            if (ownsAll)
            {
                reinforcements += c->getBonus();
                std::cout << "Player " << player->getName() << " receives " << c->getBonus() << " bonus reinforcements for owning continent " << c->getName() << "." << std::endl;
            }
        }
        // Add reinforcements to player's pool
        player->addToReinforcementPool(reinforcements);
    }
    return true;
}

/*Issue Order phase :
    - player has to build the toDefend and toAttack list first
    - player has to deploy all reinforcement armies first
    - player can then issue advance or card orders.
 */
bool GameEngine::issueOrdersPhase()
{
    std::cout << "---------------------------" << std::endl;
    std::cout << "Issue Orders  Phase" << std::endl;
    std::cout << "---------------------------" << std::endl;

    std::cout << "Issuing Orders Phase begins." << std::endl;
    std::unordered_map<Player *, bool> playersDoneIssuing;
    for (Player *player : *players)
    {
        std::cout << "reset toDefend and ToAttack lists for the new issue Order Phase" << std::endl;
        playersDoneIssuing[player] = false; // initialize all players as not done issuing orders
        player->resetDefendAndAttack();     // reset the toDefend and toAttack lists for the new turn
        player->setPendingDeployments(0);   // reset the pending deployment for the phase
        cout << *player << endl;
    }

    bool allPlayersDone = false;
    while (!allPlayersDone)
    {
        // Player issue orders in round robin fashion
        for (Player *player : *players)
        {
            if (!playersDoneIssuing[player])
            {
                std::cout << "---------------------------------------------------------" << std::endl;
                std::cout << "---------- Player " << player->getName() << " turn ------" << std::endl;
                std::cout << "---------------------------------------------------------" << std::endl;
                std::cout << "Player " << player->getName() << "'s turn to issue an order." << std::endl;

                player->issueOrder();

                // verifies if player is done with his orders in this phase
                std::cout << "Does Player " << player->getName() << " want to issue another order? (y/n)" << std::endl;
                char answer;
                std::cin >> answer;
                if (answer == 'n' || answer == 'N')
                {
                    char switchAns;
                    std::cout << "Do you want to switch strategy for Player "
                    << player->getName() << "? (1 = yes / 0 = keep): ";
                    while (!(std::cin >> switchAns) || (switchAns != '0' && switchAns != '1'))
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Enter 0 or 1: ";
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (switchAns == '1')
                        GameEngine::attachStrategyFromMenu(player);
                    playersDoneIssuing[player] = true;
                }
                else
                {

                    playersDoneIssuing[player] = false; // still not done issuing orders
                }
            }
        }
        allPlayersDone = true;
        // Check if all players are done issuing orders
        for (const auto &[player, done] : playersDoneIssuing)
        {
            if (!done)
            {
                allPlayersDone = false; // found someone still issuing, the issue order phase will continue
                break;
            }
        }

        if (allPlayersDone)
        {
            std::cout << "All players have finished issuing orders." << std::endl; // the issue order phase will end for this turn.
            return true;
        }
    }
    return false;
}

/*Executes Orders
    - Executes orders in Orderlist.
    - executes one order per player in round robin fashion until all orders are executed.
*/
bool GameEngine::executeOrdersPhase()
{
    std::unordered_map<Player *, bool> playersDoneExecuting;
    for (Player *player : *players)
    {
        playersDoneExecuting[player] = false;
    }

    std::cout << "---------------------------" << std::endl;
    std::cout << "Execute Orders Phase" << std::endl;
    std::cout << "---------------------------" << std::endl;

    std::cout << "Executing Orders Phase begins." << std::endl;
    bool noMoreOrders = false;
    while (!noMoreOrders)
    {
        for (Player *player : *players)
        {
            if (playersDoneExecuting[player])
                continue;

            Order *nextOrder = player->getNextOrderToExecute();
            if (nextOrder != nullptr)
            {
                std::cout << "Player " << player->getName() << " is executing order" << std::endl;
                nextOrder->execute();
                player->removeExecutedOrder(); // Remove order from Orderlist after execution
            }
            else
            {
                playersDoneExecuting[player] = true;
                std::cout << "Player " << player->getName() << " has no more orders to execute." << std::endl;
            }
        }
        // check if all players are done executing orders
        noMoreOrders = true;
        for (const auto &[player, done] : playersDoneExecuting)
        {
            if (!done)
            {
                noMoreOrders = false; // found someone still has orders to execute, execute orders phase continues
                break;
            }
        }

        if (noMoreOrders)
        {
            std::cout << "All players have finished executing orders." << std::endl; // ExecuteOrders phase ends for this turn
            return true;
        }
    }
    return false;
}

/*Main Game Loop
Until game ends:
    -Reinforcement Phase
    -Issue Orders Phase
    -Execute Orders Phase
        checks if a player is eliminated
        checks if a player has won , if so, moves to the win state
*/
void GameEngine::mainGameLoop()
{
    bool gameContinues = true;
    while (gameContinues)
    {
        if (reinforcementPhase())
        {
            changeGameState("issueorder");
        }
        if (issueOrdersPhase())
        {
            changeGameState("issueordersend");
        }
        else
        {
            changeGameState("issueorder");
        }

        if (executeOrdersPhase())
        {
            for (Player *player : *players)
            {
                validatePlayerStays(player);
            }
            // check if a player has won
            if (players->size() == 1)
            {
                std::cout << "*****---------------------------------------------------*****" << std::endl;
                std::cout << "Player " << (*players)[0]->getName() << " has won the game!" << std::endl;
                std::cout << "*****---------------------------------------------------*****" << std::endl;
                gameContinues = false;
                changeGameState("win"); // Game state transition to win , the game has ended
            }
            else
            {
                changeGameState("endexecorders");
            } // Go back to reinformcement phase for another turn.
        }
        else
        {
            changeGameState("execorder");
        }
    }
    gameOver(std::cin, std::cout);
}

// executes the tournament games automatically and reports their execution in the log file
void GameEngine::tournamentGameLoop(const TournamentData& td)
{
    std::unordered_map<int, std::vector<std::string>> gameResults{};
    int nbMaps{ static_cast<int>(td.mapList.size()) };

    // run a tourney loop for each map
    for (int i{}; i < nbMaps; i++)
    {
        // run a game loop on each game per map
        for (int j{}; j < td.numGames; j++)
        {
            std::cout << "\nStarting new tourney round..." << std::endl;
            std::cout << "\n===== GAME " << (j + 1) << " ON " << td.mapList[i] << " =====";
            std::cout << "\n[INFO]: Initiating startup phase...\n" << std::endl;

            /* HANDLE THE STARTUP PHASE AUTONOMOUSLY */
            processStartupCommand("loadmap " + td.mapList[i], std::cout);
            processStartupCommand("validatemap", std::cout);
            
            // TODO: double check that this adds a player with the correct strategy.. for now this is passing the strategy as the player's name. check with jack's branch for this
            for (const std::string& plr : td.playerList)
                processStartupCommand("addplayer " + plr, std::cout);

            processStartupCommand("gamestart", std::cout);
            std::cout << "\n[INFO]: Completed startup, progressing to play phase...\n" << std::endl;

            /* AUTOMATED PLAYER STRATEGY GAME LOOP */
            int currentTurn{ 1 };
            while (currentTurn <= td.maxTurns && players->size() != 1)
            {
                std::cout << "== TURN " << currentTurn << " ==" << std::endl;

                // TODO - implement the play phase for the strategies, should play autonomously 
                // IN THE CASE OF A DRAW - im assuming the loop will end on 'assignreinforcements', so I added a special state that allows us to break
                // from the play loop into the ending state. if this is not the case, be sure to update it!!

                currentTurn++;
            }
   
            /* HANDLE GAME RESULT, UPDATE RESULTS MAP AND RESTART */
            if (players->size() == 1) {}
                // TODO: uncomment when the PlayerStrategies have been integrated, getStrategyType needs to be added but its a simple getter!
                // gameResults[i].push_back((*players)[0]->getStrategy()->getStrategyType());
            else
            {
                gameResults[i].push_back(to_string(players->size()) + "-plr Draw");
                changeGameState("draw"); // force-break from the play phase to the win state
            }

            // replay if there are more games to play, otherwise set the game engine to the final state
            if (i == nbMaps - 1 && j == td.numGames - 1)
                changeGameState("quit");
            else
                changeGameState("replay");

            /* TOURNEY ROUND RESOURCE CLEANUP */
            std::cout << "\n[INFO]: Completed tourney round, cleaning up resources...\n" << std::endl;
            for (auto* player : *players) delete player;
            players->clear();
            if (map) { delete map; }
            map = nullptr;
        }
    }
    
    /* FULL TOURNEY RESOURCE CLEANUP */
    if (deck) { delete deck; }
    deck = nullptr;

    // log the result of each match to the log file
    std::cout << "\n[INFO]: The tournament has completed! See the results in tourney-results.txt.\n" << std::endl;
    logTournamentResults(td, gameResults);
}

// logs the results of the tournement
void GameEngine::logTournamentResults(const TournamentData& data, const std::unordered_map<int, std::vector<std::string>>& results)
{
    std::ofstream logFile{ "tourney-results.txt", std::ios::trunc };
    if (logFile.is_open())
    {
        logFile << "Tournament Mode:\nM: ";

        // map names
        for (int i{}; i < data.mapList.size() - 1; i++)
            logFile << data.mapList[i] << ", ";
        logFile << data.mapList[data.mapList.size() - 1];

        // strategy names
        logFile << "\nP: ";
        for (int i{}; i < data.playerList.size() - 1; i++)
            logFile << data.playerList[i] << ", ";
        logFile << data.playerList[data.playerList.size() - 1];

        // nb games and max turns
        logFile << "\nG: " << data.numGames;
        logFile << "\nD: " << data.maxTurns;

        // results table
        logFile << "\n\nResults:\n";
        logFile << std::left << std::setw(13) << "";
        for (int i{ 1 }; i <= data.numGames; i++)
            logFile << std::setw(14) << "Game " + to_string(i);

        for (int i{}; i < data.mapList.size(); i++)
        {
            logFile << std::setw(14) << "\nMap " + to_string(i + 1);

            for (int j{}; j < data.numGames; j++)
                logFile << std::setw(14) << results.at(i)[j];
        }
        
        logFile << std::endl;
        logFile.close();
    }
    else
    {
        std::cerr << "Failed to open tourney-results.txt. Terminating the application..." << std::endl;
        exit(-1);
    }

}

// Handles the end of a game once on "win" state., allowing the user to input command to replay or quit.
void GameEngine::gameOver(std::istream &in, std::ostream &out)
{
    out << "\n=== GAME OVER ===\n";
    out << "Commands:\n"
        << "  replay \n"
        << "  quit  \n";

    for (;;)
    {
        std::cout << "> ";
        std::string userInput;
        std::getline(std::cin, userInput);

        // Parse and clean command
        std::istringstream inputStream(userInput);
        std::string command, argument;
        inputStream >> command;
        std::getline(inputStream, argument);

        if (!argument.empty() && argument.front() == ' ')
            argument.erase(0, 1);

        if (command.empty())
        {
            out << "Invalid command.\n";
            continue;
        }

        if (command == "replay")
        {
            if (!isCommandValid("replay"))
            {
                out << "Replay not allowed from this state.\n";
                continue;
            }
            out << "Restarting the game...\n";
            changeGameState("replay");

            // Clean up for a fresh start
            for (auto *player : *players)
                if (player)
                {
                    delete player;
                }
            players->clear();
            if (map)
            {
                delete map;
            }
            map = nullptr;

            startupPhase(std::cin, out);
            return;
        }
        else if (command == "quit")
        {
            if (!isCommandValid("end") && !isCommandValid("quit"))
            {
                out << "Quit not allowed from this state.\n";
                continue;
            }
            out << "Exiting game. Goodbye!\n";
            changeGameState("end");
            // Clean up
            for (auto *player : *players)
                delete player;
            players->clear();
            if (map)
            {
                delete map;
            }
            map = nullptr;
            if (deck)
            {
                delete deck;
            }
            deck = nullptr;
            return;
        }
        else
        {
            out << "Invalid command. Type 'replay' or 'quit'.\n";
        }
    }
}
