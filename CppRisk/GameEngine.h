#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <memory>
#include <string>
#include <unordered_map>

/* -- GAME ENGINE DRIVER (FOR TESTING) -- */
void testGameStates();

/* -- GAME ENGINE OBJECT DEFINITION -- */
class GameEngine
{
public:
	/* -- STATE OBJECT DEFINITION (GAME ENGINE COMPONENT) -- */
	class State
	{
	public:
		// custom type aliases for readability
		using StatePtr = std::shared_ptr<State>;
		using StateMap = std::unique_ptr<std::unordered_map<std::string, std::weak_ptr<State>>>;
		using StateList = std::unique_ptr<std::vector<StatePtr>>;

		State();
		State(const std::string& name, bool isParent);
		State(const State& other);
		~State();

		// operators
		State& operator=(const State& other);
		friend std::ostream& operator<<(std::ostream& os, const State& state);

		// builder functions
		void addSubstate(StatePtr newState);
		void addTransition(const std::string& cmd, StatePtr dest);

		// accessors
		std::string getName() const;
		const StateList& getSubstates() const;
		const StateMap& getTransitions() const;
		const StatePtr& getInitialSubstate() const;
		bool isParent() const;
		void setInitialSubstate(StatePtr state);

		StatePtr resolveTransition(const std::string& cmd) const;

	private:
		std::unique_ptr<std::string> name{};
		StateMap transitions{};
		StateList substates{};
		StatePtr initialSubstate{};
		std::unique_ptr<bool> parent;
	};
	/* ----------------------------------------------------- */

private:
	State::StatePtr activeState;
	State::StatePtr activeParentState;
	State::StateList parentStates;

public:
	GameEngine();
	GameEngine(const GameEngine& other);
	~GameEngine();

	// operators
	GameEngine& operator=(const GameEngine& other);
	friend std::ostream& operator<<(std::ostream& os, const GameEngine::State& state); // forwarded for visibility
	friend std::ostream& operator<<(std::ostream& os, const GameEngine& engine);

	// builder functions
	void addParentStates(std::initializer_list<State::StatePtr> states);
	void addChildStates(State::StatePtr parent, std::initializer_list<State::StatePtr> states);
	void addChildTransition(State::StatePtr from, const std::string& cmd, State::StatePtr to);

	// accessors
	const State::StatePtr& getActiveState() const;
	const State::StatePtr& getActiveParentState() const;
	void setActiveState(State::StatePtr state);
	void setActiveParentState(State::StatePtr state);

	std::string readCommand() const;
	void changeGameState(const std::string& cmd);
	bool isActiveStateFinal() const;

};

#endif
