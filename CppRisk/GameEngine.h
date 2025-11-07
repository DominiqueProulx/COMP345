#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <vector>
#include <iostream>

class Player;
class Deck;
class Territory;
class Map;
class MapLoader;

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
		friend std::ostream& operator<<(std::ostream& os, const State& state);

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

    Map*                                    map;
    MapLoader*                              mapLoader;
    std::vector<Player*>*                   players;
    Deck*                                   deck;
    std::unordered_map<Player*, int>*       reinforcementPool; //Jackson to test for now

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
	static void initializeRiskFSM(GameEngine& engine);

	// accessors
	State* getActiveStatePtr() const;
	State* getActiveParentStatePtr() const;
	void setActiveState(State* state);
	void setActiveParentState(State* state);

	std::string readCommand() const;
	void changeGameState(const std::string& cmd);
	bool isActiveStateFinal() const;

	//bea added helper functions for command processing
	bool isCommandValid(const std::string& cmd) const;
	std::string getCurrentStateName() const;
	std::string getParentStateName() const;

    void startupPhase( std::istream& in=std::cin,std::ostream& out=std::cout);

    const std::vector<Player*>* getPlayers() const { return players; }

    friend std::ostream& operator<<(std::ostream& os, const GameEngine& engine);

private:
        
    bool cmdLoadMap(const std::string& filename, std::ostream& out);
    bool cmdValidateMap(std::ostream& out);
    bool cmdAddPlayer(const std::string& name, std::ostream& out);
    bool cmdGameStart(std::ostream& out);

    void fairDistributeTerritories(std::ostream& out);
    void randomizePlayerOrder(std::ostream& out);
    void grant50Reinforcements(std::ostream& out);
    void initialCardDraws(std::ostream& out);


};

#endif
