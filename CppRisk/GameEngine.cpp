#include "GameEngine.h"
#include <iostream>

/* -------------------------- */
/* -- STATE IMPLEMENTATION -- */
/* -------------------------- */

// Default constrcutor, intiializes an empty state object with no data.
GameEngine::State::State()
{
	this->name = nullptr;
	this->transitions = std::make_unique<std::unordered_map<std::string, std::weak_ptr<State>>>();
	this->substates = std::make_unique<std::vector<StatePtr>>();
	this->initialSubstate = nullptr;
	this->parent = nullptr;
}

// Initializes a new empty State object with a specified name and parent status.
GameEngine::State::State(const std::string& name, bool isParent)
{
	this->name = std::make_unique<std::string>(name);
	this->transitions = std::make_unique<std::unordered_map<std::string, std::weak_ptr<State>>>();
	this->substates = std::make_unique<std::vector<StatePtr>>();
	this->initialSubstate = nullptr;
	this->parent = std::make_unique<bool>(isParent);
}

// Duplicates an existing State into a new distinct object. Substate pointers are shared.
GameEngine::State::State(const State& other)
{
	this->name = std::make_unique<std::string>(*(other.name));

	this->transitions = std::make_unique<std::unordered_map<std::string, std::weak_ptr<State>>>();
	for (const auto& [cmd, dest] : *(other.transitions))
		(*this->transitions)[cmd] = dest;

	this->substates = std::make_unique<std::vector<StatePtr>>();
	for (const auto& ptr : *(other.substates))
		(*this->substates).push_back(ptr);

	this->initialSubstate = other.initialSubstate;
	this->parent = std::make_unique<bool>(*other.parent);
}

// Destroys this State instance. Dynamic memory is cleared automatically via smart pointers.
GameEngine::State::~State()
{
}

// Reassigns this State instance to a new State object by deep copying all fields (except shared_ptrs).
GameEngine::State& GameEngine::State::operator=(const State& other)
{
	// short-circuit self-assignment
	if (this == &other)
		return *this;

	// deep copy other's fields (except the shared_ptrs)
	this->name = std::make_unique<std::string>(*other.name);
	this->transitions = std::make_unique<std::unordered_map<std::string, std::weak_ptr<State>>>(*other.transitions);
	this->substates = std::make_unique<std::vector<StatePtr>>(*other.substates);
	this->initialSubstate = other.initialSubstate;
	this->parent = std::make_unique<bool>(*other.parent);

	return *this;
}

// Prints a complete summary of a State object's fields.
std::ostream& operator<<(std::ostream& os, const GameEngine::State& state)
{
	os << "STATE [" << state.getName() << "]:\n";

	if (state.isParent())
	{
		os << "\tPARENT NODE (Initial: " << (*state.getInitialSubstate()).getName() << ")\n";
		os << "\tCHILDREN:\n";

		for (const auto& s : *state.getSubstates())
			os << "\t\t" << s->getName() << " (" << s->getTransitions()->size() << " transitions)\n";
	}

	os << "\tTRANSITIONS TO (" << state.getTransitions()->size() << "):\n";
	for (const auto& [cmd, ptr] : *state.getTransitions())
	{
		if (auto s = ptr.lock())
			os << "\t\t" << '[' << s->getName() << "] via " << cmd << '\n';
	}

	return os;
}

// Adds an existing State object to this State's list of substates.
void GameEngine::State::addSubstate(StatePtr newState)
{
	(*this->substates).push_back(newState);
}

// Adds a new transition (command -> destination State) to this State's list of transitions.
void GameEngine::State::addTransition(const std::string& cmd, StatePtr dest)
{
	(*this->transitions)[cmd] = dest;
}

// Return this State's name.
std::string GameEngine::State::getName() const
{
	return *this->name;
}

// Returns this State's list of substates.
const GameEngine::State::StateList& GameEngine::State::getSubstates() const
{
	return this->substates;
}

// Returns this State's list of transitions.
const GameEngine::State::StateMap& GameEngine::State::getTransitions() const
{
	return this->transitions;
}

// Returns this State's initial substate.
const GameEngine::State::StatePtr& GameEngine::State::getInitialSubstate() const
{
	return this->initialSubstate;
}

// Returns true if this state is a parent state and holds other substates.
bool GameEngine::State::isParent() const
{
	return *this->parent;
}

// Sets this State's initial substate to a new state.
void GameEngine::State::setInitialSubstate(StatePtr state)
{
	this->initialSubstate = state;
}

// Returns the destination State of a transition for a given command. If the command is invalid, nullptr is returned.
GameEngine::State::StatePtr GameEngine::State::resolveTransition(const std::string& cmd) const
{
	const auto& result{ (*this->transitions).find(cmd) };
	return result == (*this->transitions).end() ? nullptr : result->second.lock();
}

/* -------------------------------- */
/* -- GAME ENGINE IMPLEMENTATION -- */
/* -------------------------------- */

// Initializes an empty GameEngine object.
GameEngine::GameEngine()
{
	this->activeParentState = nullptr;
	this->activeState = nullptr;
	this->parentStates = std::make_unique<std::vector<State::StatePtr>>();
}

// Duplicates an existing GameEngine into a new distinct object. State pointers are shared.
GameEngine::GameEngine(const GameEngine& other)
{
	this->activeParentState = other.activeParentState;
	this->activeState = other.activeState;
	this->parentStates = std::make_unique<std::vector<State::StatePtr>>(*other.parentStates);
}

// Destroys a GameEngine object. Dynamic memory is cleared automatically via smart pointers.
GameEngine::~GameEngine()
{
}

// Reassigns this GameEngine instance to a new GameEngine object by deep copying all fields (except shared_ptrs).
GameEngine& GameEngine::operator=(const GameEngine& other)
{
	// short-circuit self-assignment
	if (this == &other)
		return *this;

	// deep copy other's fields (except the shared_ptrs)
	this->activeParentState = other.activeParentState;
	this->activeState = other.activeState;
	this->parentStates = std::make_unique<std::vector<State::StatePtr>>(*other.parentStates);

	return *this;
}

// Prints a complete summary of a GameEngine object's fields.
std::ostream& operator<<(std::ostream& os, const GameEngine& engine)
{
	os << "GAME ENGINE:\n";

	os << "\tACTIVE PARENT STATE: " << engine.activeParentState->getName() << " (Initial: " << engine.activeParentState->getInitialSubstate()->getName() << ")\n";
	os << "\tACTIVE SUBSTATE: " << engine.activeState->getName() << '\n';
	os << "\tPARENT STATES (" << engine.parentStates->size() << "):\n";

	for (const auto& s : *engine.parentStates)
		os << "\t\t" << s->getName() << "(" << s->getSubstates()->size() << " substates)\n";

	return os;
}

// Adds a new top-level state to the GameEngine's state list. If no states have been added yet, the first is set to
// the active parent state.
void GameEngine::addParentStates(std::initializer_list<State::StatePtr> states)
{
	std::vector<GameEngine::State::StatePtr>& parentStates{ *this->parentStates };

	for (const auto& state : states)
	{
		// set an initial state if the list is empty
		if (parentStates.size() == 0)
			this->activeParentState = state;

		parentStates.push_back(state);
	}
}

// Adds a new substate to an existing parent's state list. If no states have been added to the parent yet, the first
// is set as the initial state.
void GameEngine::addChildStates(State::StatePtr parent, std::initializer_list<State::StatePtr> states)
{
	State& parentState = *parent;

	for (const auto& state : states)
	{
		// set an initial substate if no states have been added yet
		if (parentState.getInitialSubstate() == nullptr)
			parentState.setInitialSubstate(state);

		parentState.addSubstate(state);
	}
}

// Adds a new transition from one existing State to another via a command string.
void GameEngine::addChildTransition(State::StatePtr from, const std::string& cmd, State::StatePtr to)
{
	from->addTransition(cmd, to);
}

// Returns this GameEngine's active substate.
const GameEngine::State::StatePtr& GameEngine::getActiveState() const
{
	return this->activeState;
}

// Returns this GameEngine's active parent (top-level) state.
const GameEngine::State::StatePtr& GameEngine::getActiveParentState() const
{
	return this->activeParentState;
}

// Sets this GameEngine's active substate.
void GameEngine::setActiveState(State::StatePtr state)
{
	this->activeState = state;
}

// Sets this GameEngine's active parent state.
void GameEngine::setActiveParentState(State::StatePtr state)
{
	this->activeParentState = state;
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
	auto newState = this->activeState->resolveTransition(cmd);
	if (newState)
	{
		// handle case where substate transitions to new parent state
		if (newState->isParent())
		{
			this->activeParentState = newState;
			this->activeState = newState->getInitialSubstate();
		}
		else
			this->activeState = newState;

		std::cout << "SUCCESS: transitioned to new state (" << this->activeState->getName() << ") via '" << cmd << "'.\n";
	}
	else
		std::cerr << "ERR: command '" << cmd << "' does not exist for active state (" << this->activeState->getName() << ").\n";
}

// Returns true if the active state is considered a final state (defined by no outgoing transitions).
bool GameEngine::isActiveStateFinal() const
{
	return this->activeState->getTransitions()->size() == 0;
}
