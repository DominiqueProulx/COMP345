#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "GameEngine.h"
using namespace std;

class Command {
protected:
	unique_ptr<string> command;
	unique_ptr<string> effect;

public:
	Command(const string& command);
	Command(const Command& other);
	Command& operator=(const Command& other);
	~Command() = default;
	friend ostream& operator<<(ostream& os, const Command& cmd);

	string getCommandString() const {};
	string getEffect() const;

	void saveEffect(const string& effect);
	string getEffect() const;

};

class CommandProcessor {
public:
	CommandProcessor();
	CommandProcessor(const CommandProcessor& other);
	CommandProcessor& operator=(const CommandProcessor& other);
	friend ostream& operator<<(ostream& os, const CommandProcessor& cp);
	~CommandProcessor() = default;

	Command* getCommand(GameEngine& engine);
	bool validate(Command& command, GameEngine& engine);

protected:
	Command readCommand();
	void saveCommand(const Command& command);

	unique_ptr<vector<unique_ptr<Command>>> commands;
};
