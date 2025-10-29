#include <iostream>
#include <vector>
#include <string>
#include "CommandProcessing.h"
using namespace std;

//Command Class
//
//constructor
Command::Command(const string& command) : command(new string(command)), effect(new string("")) {};
//copy constructor
Command::Command(const Command& other) : command(new string(*other.command)), effect(new string(*other.effect)) {};
//assignment operator
Command& Command::operator=(const Command& other) {
	if (this != &other) {
		delete command;
		delete effect;
		command = new string(*other.command);
		effect = new string(*other.effect);
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
	if (this->effect != nullptr) {
		delete this->effect;
	}
	this->effect = new string(effect);
}

string Command::getEffect() const {
	return *effect;
}
