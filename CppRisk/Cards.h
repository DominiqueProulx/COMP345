#pragma once
#include <iostream>
#include <string>
#include <vector> 

class Order; //forward declaration for DummyOrder class
class Deck; //forward declaration for Deck class

//declaration for virtual parent class 
class Card {
public:
	Card(const std::string& name);
	virtual ~Card();
	virtual Order* play() = 0;
	virtual void print(std::ostream& os) const;

	//getter function
	std::string getName() const { return name; }

	//friend operator<< overload
	friend std::ostream& operator<<(std::ostream& os, const Card& card);

protected:
	std::string name;
};

class Bomb : public Card {
public:
	Bomb();
	~Bomb();
	Order* play() override;
	void print(std::ostream& os) const override;
};

class Reinforcement : public Card {
	public:
	Reinforcement();
	~Reinforcement();
	Order* play() override;
	void print(std::ostream& os) const override;
};

class Blockade : public Card {
	public:
	Blockade();
	~Blockade();
	Order* play() override;
	void print(std::ostream& os) const override;
};

class Airlift : public Card {
	public:
	Airlift();
	~Airlift();
	Order* play() override;
	void print(std::ostream& os) const override;
};

class Diplomacy : public Card {
	public:
	Diplomacy();
	~Diplomacy();
	Order* play() override;
	void print(std::ostream& os) const override;
};

class Hand {
public:
	//constructor and destructor
	Hand();
	~Hand();

	//functions
	void addCard(Card* card);
	void playFromHand(int index, Deck& deck);
	void printHand() const;
	int getSize();

//vector that holds hand cards
private:
	std::vector<Card*> hand;
	static const int MAX_HAND_SIZE = 5;
};

class Deck {

	//constructor and destructor
public:
	Deck();
	~Deck();

	//functions
	void initializeDeck();
	void shuffle();
	Card* draw(Hand* hand);
	void returnCard(Card* card);
	void printDeck() const;

//vector that holds cards
private:
	std::vector<Card*> deck;
};