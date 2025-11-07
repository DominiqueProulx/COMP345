#include "GameEngine.h"
#include <iostream>

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
}

// Duplicates an existing GameEngine into a new distinct object.
GameEngine::GameEngine(const GameEngine& other)
{
	activeParentState = other.activeParentState;
	activeState = other.activeState;
	parentStates = new std::vector<State*>(*other.parentStates);
	states = new std::vector<State*>(*other.states);
}

// Destroys a GameEngine object and ALL State objects created using this engine.
GameEngine::~GameEngine()
{
	delete parentStates;

	// delete all State objects
	for (const auto& s : *states)
		delete s;

	delete states;
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