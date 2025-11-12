#include "CommandProcessing.h"
#include <sstream>
#include <fstream>
using namespace std;

//Command Class
//
//constructor
Command::Command(const string& command)
	: command(make_unique<string>(command)),
	effect(make_unique<string>("")),
	valid(make_unique<bool>(false))
{
};
//copy constructor
Command::Command(const Command& other)
	: Subject(other),
	command(make_unique<string>(*other.command)),
	effect(make_unique<string>(*other.effect)),
	valid(make_unique<bool>(*other.valid))
{
};
//assignment operator
Command& Command::operator=(const Command& other) {
	if (this != &other) {
		Subject::operator=(other);

		*command = *other.command;
		*effect = *other.effect;
		*valid = *other.valid;
	}
	return *this;
}

//assignment operator overload for printing
ostream& operator<<(ostream& os, const Command& cmd) {
	os << "Command: " << *cmd.command << " | Effect: " << *cmd.effect;
	return os;
}

void Command::saveEffect(const string& newEffect) {
	*effect = newEffect;

	// log the effect change to gamelog.txt
	notify(this);
}

string Command::getEffect() const {
	return *effect;
}

string Command::getCommandString() const {
	return *command;
}

// Returns a log string describing the Command's new saved effect.
std::string Command::stringToLog() const
{
	if (effect)
		return "[CMD] Saved a new command effect: { " + *effect + " }.";
	else
		return "[ERR::CMD] Attempted to log a command with no effect...";
}

bool Command::isValid() const {
	return *valid;
}

void Command::setValid(bool v) {
	*valid = v;
}

//CommandProcessor Class

//constructor
CommandProcessor::CommandProcessor() : commands(make_unique<vector<unique_ptr<Command>>>()) {}
//copy constructor
CommandProcessor::CommandProcessor(const CommandProcessor& other)
	: Subject(other), commands(make_unique<vector<unique_ptr<Command>>>()) {
	for (const auto& cmd : *other.commands) {
		commands->push_back(make_unique<Command>(*cmd));
	}
}
//assignment operator
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
	if (this != &other) {
		Subject::operator=(other);

		commands->clear();
		for (const auto& cmd : *other.commands) {
			commands->push_back(make_unique<Command>(*cmd));
		}
	}
	return *this;
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
	getline(cin, input);
	Command newCommand = Command(input);
	validate(newCommand, engine);
	return newCommand;
}

//adds command to end of commands vector
void CommandProcessor::saveCommand(const Command& command) {
	commands->push_back(make_unique<Command>(command));

	// log the saved command to gamelog.txt
	notify(this);
}

//validates command based on current game state
bool CommandProcessor::validate(Command& command, GameEngine& engine) {

	string fullCommand = command.getCommandString();
	string commandBase;

	// Use stringstream to reliably get the first word
	stringstream ss(fullCommand);
	ss >> commandBase; // This extracts the first word, ignoring leading whitespace

	// Handle empty input
	if (commandBase.empty()) {
		command.saveEffect("Error: No command entered.");
		return false; // Not valid
	}

	// Special case: 'quit' is always valid and doesn't need the engine
	if (commandBase == "quit") {
		command.saveEffect("Exiting...");
		return true;
	}

	// Since State is private, use helper functions to determine validity
	//"isCommandValid()" checks if the command is valid in the current state from the engine. Couldn't read state from here directly
	// due to encapsulation
	if (engine.isCommandValid(commandBase)) {
		command.setValid(true);
		command.saveEffect("Command '" + commandBase + "' is valid in state '" + engine.getCurrentStateName() + "'.");
		return true;
	}
	else {
		command.setValid(false);
		command.saveEffect("Invalid command: '" + commandBase + "' is not recognized in state '" + engine.getCurrentStateName() + "'.");
		return false;
	}
}

Command* CommandProcessor::getCommand(GameEngine& engine) {
	Command newCommand = readCommand(engine);
	saveCommand(newCommand);
	//return pointer to the last command in the vector
	return commands->back().get();
}


// Returns a log string stating the name of the new saved Command.
std::string CommandProcessor::stringToLog() const
{
	if (commands->empty())
		return "[ERR::CMDPROCESSOR] Attempted to log an empty command list...";
	else
	{
		std::ostringstream cmdAsStream{};
		cmdAsStream << *commands->back();
		return "[CMDPROCESSOR] Added a new command to a list: { " + cmdAsStream.str() + " }.";
	}
}


//FILE COMMAND PROCESSOR ADAPTER

//constructor, copy constructor, assignment operator, destructor

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string& filename)
	: CommandProcessor(), file(make_unique<ifstream>(filename)) {
	if (!file->is_open()) {
		throw runtime_error("Could not open file: " + filename);
	}
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other)
	: CommandProcessor(other), file(make_unique<ifstream>()) {
	// ifstream cannot be copied, so we just create a new empty ifstream
}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& other) {
	if (this != &other) {
		CommandProcessor::operator=(other);
		// ifstream cannot be copied, so we just create a new empty ifstream
		file = make_unique<ifstream>();
	}
	return *this;
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
	if (file && file->is_open()) {
		file->close();
	}
}

Command* FileCommandProcessorAdapter::readCommand(GameEngine& engine) {
	string line;
	while (getline(*file, line)) {
		if (line.empty()) {
			continue; // skip empty lines
		}

		Command* newCommand = new Command(line); 
		validate(*newCommand, engine);
		saveCommand(*newCommand); 

		return newCommand;

	}

	return nullptr; // End of file reached
}
