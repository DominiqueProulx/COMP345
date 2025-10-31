#include <iostream>
#include <vector>
#include <string>
#include "CommandProcessing.h"
#include "GameEngine.h"
using namespace std;

//Command Class
//
//constructor
Command::Command(const string& command) : command(make_unique<string>(command)), effect(make_unique<string>("")) {};
//copy constructor
Command::Command(const Command& other) : command(make_unique<string>(*other.command)), effect(make_unique<string>(*other.effect)) {};
//assignment operator
Command& Command::operator=(const Command& other) {
	if (this != &other) {
		delete command;
		delete effect;
		command = make_unique<string>(*other.command);
		effect = make_unique<string>(*other.effect);
	}
	return *this;
}
//desctructor
Command::~Command() {
	delete command;
	command = nullptr;
	delete effect;
	effect = nullptr;
}
//assignment operator overload for printing
ostream& operator<<(ostream& os, const Command& cmd) {
	os << "Command: " << *cmd.command << " | Effect: " << *cmd.effect;
	return os;
}

void Command::saveEffect(string& effect) const {
	this->effect = make_unique<string>(effect);
}

string Command::getEffect() const {
	return *effect;
}

string Command::getCommandString() const {
	return *command;
}

//CommandProcessor Class

//constructor
CommandProcessor::CommandProcessor() : commands(new vector<Command*>()) {}
//copy constructor
CommandProcessor::CommandProcessor(const CommandProcessor& other) : commands(new vector<Command*>()){
	for (const auto& cmd : *other.commands) {
		commands->push_back(make_unique<Command>(*cmd));
	}
}
//assignment operator
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
	if (this != &other) {
		commands->clear();
		for (const auto& cmd : *other.commands) {
			commands->push_back(make_unique<Command>(*cmd));
		}
	}
	return *this;
}
//desctructor
CommandProcessor::~CommandProcessor() {
	for (Command* cmd : *commands) {
		delete cmd;
	}
	commands->clear();
	delete commands;
	commands = nullptr;
}

//assignment operator overload for printing
ostream& operator<<(ostream& os, const CommandProcessor& cp) {
	os << "Command Processor with " << cp.commands->size() << " commands." << endl;
	for (const auto& cmd : *cp.commands) {
		os << *cmd << endl;
	}
	return os;
}

//reads command from users input
Command CommandProcessor::readCommand(GameEngine& engine) {
	string input;
	cout << "Enter command: ";
	cin >> input;
	Command newCommand = Command(input);
	validate(newCommand, engine);
	return newCommand;
}

//adds command to end of commands vector
void CommandProcessor::saveCommand(const Command& command) {
	commands->push_back(make_unique<Command>(command));
}

//validates command based on current game state
bool CommandProcessor::validate(Command& command, GameEngine& engine) {
	//Get the state pointer from the engine
	GameEngine::State* currentState = engine.getActiveStatePtr();

	//safety check to make sure there is a state
	if (!currentState) {
		command.saveEffect("No active state in game engine.");
		return false;
	}

	string fullCommand = command.getCommandString();

	//extract first part of command (before any spaces)
	string commandBase = fullCommand.substr(0, fullCommand.find(' '));

	//resolveTrasition returns a *State if valid, nullptr if invalid
	if (currentState->resolveTransition(commandBase) != nullptr) {
		command.saveEffect("Command '" + commandBase + "' is valid in state '" + currentState->getName() + "'.");
		return true;
	}
	else {
		command.saveEffect("Command '" + commandBase + "' is invalid in state '" + currentState->getName() + "'.");
		return false;
	}
}

Command* CommandProcessor::getCommand(GameEngine& engine) {
	return readCommand(engine);
}