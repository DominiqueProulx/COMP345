#pragma once
#include <string>
#include <vector>
#iinclude <iostream>
using namespace std;

class Command {
protected:
	string* command;
	string* effect;

public:
	Command(const string& command);
	Command(const Command& other);
	Command& operator=(const Command& other);
	~Command();
	friend ostream& operator<<(ostream& os, const Command& cmd);

	string saveEffect() const;

};

class CommandProcessor {
public:
	CommandProcessor();
	CommandProcessor(const CommandProcessor& other);
	CommandProcessor& operator=(const CommandProcessor& other);
	friend ostream& operator<<(ostream& os, const CommandProcessor& cp);
	~CommandProcessor();

	Command* getCommand();
	bool validate(Command& command);

protected:
	string readCommand();
	void saveCommand(const Command& command);

	vector<Command*>* commands;
};
