#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "LoggingObserver.h"

using namespace std;

class GameEngine;

//new Tournament struct to hold/pass info
struct TournamentData {
	vector<string> mapList;
	vector<string> playerList;
	int numGames;
	int maxTurns;
};

class Command : public Subject, public ILoggable {
protected:
	unique_ptr<string> command;
	unique_ptr<string> effect;
	unique_ptr<bool> valid;

	//added for tournament mode
	unique_ptr<TournamentData> tournamentData;

public:
	Command(const string& command);
	Command(const Command& other);
	Command& operator=(const Command& other);
	~Command() = default;
	friend ostream& operator<<(ostream& os, const Command& cmd);

	string getCommandString() const;
	string getEffect() const;

	void saveEffect(const string& effect);

	// logging function
	std::string stringToLog() const override;

	bool isValid() const;
	void setValid(bool v);

	// Tournament Data Accessors/Setters
	void setTournamentData(const TournamentData& data) { tournamentData = make_unique<TournamentData>(data); }
	const TournamentData& getTournamentData() const { return *tournamentData; }
	bool hasTournamentData() const { return tournamentData != nullptr; }

};

class CommandProcessor : public Subject, public ILoggable {
public:
	CommandProcessor();
	CommandProcessor(const CommandProcessor& other);
	CommandProcessor& operator=(const CommandProcessor& other);
	friend ostream& operator<<(ostream& os, const CommandProcessor& cp);
	~CommandProcessor() = default;

	Command* getCommand(GameEngine& engine);
	bool validate(Command& command, GameEngine& engine);

	//added for tournament mode
	bool validateTournament(Command& command, const string& fullCommand, GameEngine& engine);

	//file parsing adapter function
	void runFromFile(const string& filename, GameEngine& engine);

	// logging function
	std::string stringToLog() const override;

protected:
	Command readCommand(GameEngine& engine);
	void saveCommand(const Command& command);

	unique_ptr<vector<unique_ptr<Command>>> commands;
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
	unique_ptr<ifstream> file;

public:
	FileCommandProcessorAdapter(const string& filename);
	FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
	FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);
	~FileCommandProcessorAdapter();
	Command* readCommand(GameEngine& engine);

};
