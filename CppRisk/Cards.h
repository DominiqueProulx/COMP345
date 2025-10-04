#pragma once
#include <iostream>
#include <string>
#include <vector> 

class Order; //forward declaration for DummyOrder class
class Deck; //forward declaration for Deck class
class Hand; //forward declaration for Hand class

//declaration for virtual parent class 
class Card {
public:
	Card(const std::string& name);
	virtual ~Card();
	virtual Order* play(Hand& hand, Deck& deck) = 0;
	virtual void print(std::ostream& os) const;

	//getter function
	std::string getName() const;

	//friend operator<< overload
	friend std::ostream& operator<<(std::ostream& os, const Card& card);

protected:
	std::string name;
};

//declarations for child classes that inherit from Card
class Bomb : public Card {
public:
	Bomb();
	~Bomb();
	Order* play(Hand& hand, Deck& deck) override;
	void print(std::ostream& os) const override;
};

class Reinforcement : public Card {
	public:
	Reinforcement();
	~Reinforcement();
	Order* play(Hand& hand, Deck& deck) override;
	void print(std::ostream& os) const override;
};

class Blockade : public Card {
	public:
	Blockade();
	~Blockade();
	Order* play(Hand& hand, Deck& deck) override;
	void print(std::ostream& os) const override;
};

class Airlift : public Card {
	public:
	Airlift();
	~Airlift();
	Order* play(Hand& hand, Deck& deck) override;
	void print(std::ostream& os) const override;
};

class Diplomacy : public Card {
	public:
	Diplomacy();
	~Diplomacy();
	Order* play(Hand& hand, Deck& deck) override;
	void print(std::ostream& os) const override;
};

class Hand {
public:
	//constructor and destructor
	Hand();
	~Hand();

	//functions
	void addCard(Card* card);
	//Order* playFromHand(int index, Deck& deck);
	Card* getCard(int index);
	void printHand() const;
	int getSize();
	void removeCard(Card* card);

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
	void getSize();
	void printDeck() const;

//vector that holds cards
private:
	std::vector<Card*> deck;
};