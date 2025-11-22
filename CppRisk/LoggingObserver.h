#pragma once
#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H

#include <string>
#include <vector>

/* -- LOGGABLE INTERFACE DEFINITION -- */
class ILoggable
{
public:
	virtual ~ILoggable() = default;
	virtual std::string stringToLog() const = 0;
};

/* -- ASBTRACT OBSERVER DEFINITION -- */
class Observer
{
public:
	virtual ~Observer() = default;
	virtual void update(ILoggable* model) = 0;
};

/* -- CONCRETE OBSERVER DEFINITION (LOG OBSERVER) -- */
class LogObserver : public Observer
{
public:
	LogObserver() = default;
	~LogObserver() = default;
	LogObserver(const LogObserver& other) = default;

	LogObserver& operator=(const LogObserver& other) = default;
	friend std::ostream& operator<<(std::ostream& os, const LogObserver& log);

	void update(ILoggable* model) override;
};

/* -- SUBJECT DEFINITION -- */
class Subject
{
protected:
	std::vector<Observer*>* observers;

	Subject(); // prevent subject from being instantiated as a lone class

	// observer notification function
	void notify(ILoggable* model) const;

public:
	virtual ~Subject();
	Subject(const Subject& other);

	// operators
	Subject& operator=(const Subject& other);
	friend std::ostream& operator<<(std::ostream& os, const Subject& sub);

	// observer management functions
	void attachObserver(Observer* observer);
	void detachObserver(Observer* observer);
};

#endif
