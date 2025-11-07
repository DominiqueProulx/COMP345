#include <algorithm>
#include <fstream>
#include <iostream>
#include "LoggingObserver.h"

/* ---------------------------- */
/* -- SUBJECT IMPLEMENTATION -- */
/* ---------------------------- */

// Called by concrete Subjects in the call chain, initializes the observer list.
Subject::Subject()
{
	observers = new std::vector<Observer*>{};
}

// Destroys this Subject instance and deletes the memory for the observer vector.
// Subject does NOT have an ownership relationship with its Observers, it does not clear the Observer memory.
Subject::~Subject()
{
	if (observers) delete observers;
}

// Copies a subject subject by shallow copying the observers array.
// As Subjects do not own their observers, the observers vector CANNOT be deep copied.
Subject::Subject(const Subject& other)
{
	observers = new std::vector<Observer*>{ *other.observers };
}

// Copy-assigns a Subject instance, the observers vector is shallow copied.
Subject& Subject::operator=(const Subject& other)
{
	if (this == &other) return *this;

	delete observers;
	observers = new std::vector<Observer*>{ *other.observers };

	return *this;
}

// Prints the quantity of observers that this Subject instance has.
std::ostream& operator<<(std::ostream& os, const Subject& sub)
{
	return os << "This subject has " << sub.observers->size() << " attached observers.";
}

// Notifies each registered observer that an update has occured with a model. The subject should usually pass itself.
void Subject::notify(ILoggable* model) const
{
	if (!model) return;

	for (const auto& ob : *observers)
		ob->update(model);
}

// Registers a specified Observer to this Subject.
void Subject::attachObserver(Observer* observer)
{
	observers->push_back(observer);
}

// De-registers a specific Observer if it exists within this Subject's observer list.
void Subject::detachObserver(Observer* observer)
{
	std::vector<Observer*>& list{ *observers };

	const auto& observerPos = std::find(list.begin(), list.end(), observer);
	if (observerPos != list.end())
		list.erase(observerPos);
}

/* --------------------------------- */
/* -- LOG OBSERVER IMPLEMENTATION -- */
/* --------------------------------- */

// Opens 'gamelog.txt' and appends a model-specific update string. Executed when this observer is notified of a change.
void LogObserver::update(ILoggable* model)
{
	std::string toLog{ model->stringToLog() };

	std::ofstream logFile{ "gamelog.txt", std::ios::app };
	if (logFile.is_open())
	{
		logFile << toLog << '\n';
		logFile.close();
	}
	else
	{
		std::cerr << "Failed to open gamelog.txt. Terminating the application..." << std::endl;
		exit(-1);
	}
}

// Prints a simple statement saying a LogObserver has been printed (these objects store no data to print).
std::ostream& operator<<(std::ostream& os, const LogObserver& log)
{
	return os << "Printed a log observer.";
}
