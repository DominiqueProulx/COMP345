#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <memory>
#include <string>
#include <unordered_map>

/* -- GAME ENGINE OBJECT DEFINITION -- */
class GameEngine
{
private:
	/* -- STATE OBJECT DEFINITION (GAME ENGINE COMPONENT) -- */
	class State
	{
	public:
		// custom type aliases for readability
		using StateMap = std::unordered_map<std::string, State*>;
		using StateList = std::vector<State*>;

		State();
		State(const std::string& name, bool isParent);
		State(const State& other);
		~State();

		// operators
		State& operator=(const State& other);
		friend std::ostream& operator<<(std::ostream& os, State& state);

		// builder functions
		void addSubstate(State* newState);
		void addTransition(const std::string& cmd, State* dest);

		// accessors
		std::string getName() const;
		const StateList& getSubstates() const;
		const StateMap& getTransitions() const;
		State* getInitialSubstatePtr() const;
		bool isParent() const;
		void setInitialSubstate(State* state);

		State* resolveTransition(const std::string& cmd) const;

	private:
		std::string* name{};
		StateMap* transitions{};
		StateList* substates{};
		State* initialSubstate{};
		bool* parent;
	};
	/* ----------------------------------------------------- */

	State* activeState;
	State* activeParentState;
	State::StateList* parentStates;
	State::StateList* states;

public:
	using GameState = State*; // exposed publically so states can be initialized but not used directly externally

	GameEngine();
	GameEngine(const GameEngine& other);
	~GameEngine();

	// operators
	GameEngine& operator=(const GameEngine& other);
	friend std::ostream& operator<<(std::ostream& os, const GameEngine::State& state); // forwarded for visibility
	friend std::ostream& operator<<(std::ostream& os, const GameEngine& engine);

	// builder functions
	State* createState(const std::string& name, bool isParent);
	void addParentStates(const std::initializer_list<State*>& states);
	void addChildStates(State* parent, const std::initializer_list<State*>& states);
	void addChildTransition(State* from, const std::string& cmd, State* to);

	// accessors
	State* getActiveStatePtr() const;
	State* getActiveParentStatePtr() const;
	void setActiveState(State* state);
	void setActiveParentState(State* state);

	std::string readCommand() const;
	void changeGameState(const std::string& cmd);
	bool isActiveStateFinal() const;

};

#endif
