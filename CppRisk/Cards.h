#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

class Order; //forward declaration for DummyOrder class
class Deck; //forward declaration for Deck class
class Hand; //forward declaration for Hand class
class Player; //forward declaration for Hand class

//declaration for virtual parent class 
class Card {
public:
	Card(const std::string& name);
	virtual ~Card();
	virtual Order* play(Hand& hand, Deck& deck, Player* player) = 0;
	virtual Card* copy() const = 0;

	//getter function
	std::string getName() const;

	//friend operator<< overload
	friend std::ostream& operator<<(std::ostream& os, const Card& card);

protected:
	std::string* name;
};

//declarations for child classes that inherit from Card
class BombCard : public Card {
public:
	BombCard();
	~BombCard();
	Order* play(Hand& hand, Deck& deck, Player* player) override;
	Card* copy() const override;
	
};

class ReinforcementCard : public Card {
	public:
	ReinforcementCard();
	~ReinforcementCard();
	Order* play(Hand& hand, Deck& deck, Player* player) override;
	Card* copy() const override;
};

class BlockadeCard : public Card {
	public:
	BlockadeCard();
	~BlockadeCard();
	Order* play(Hand& hand, Deck& deck, Player* player) override;
	Card* copy() const override;
};

class AirliftCard : public Card {
	public:
	AirliftCard();
	~AirliftCard();
	Order* play(Hand& hand, Deck& deck, Player* player) override;
	Card* copy() const override;
};

class DiplomacyCard : public Card {
	public:
	DiplomacyCard();
	~DiplomacyCard();
	Order* play(Hand& hand, Deck& deck, Player* player) override;
	Card* copy() const override;
};

class Hand {
public:
	//constructor and destructor and copier
	Hand();
	~Hand();
	Hand(const Hand& copy);

	//functions
	void addCard(Card* card);
	Card* getCard(int index);
	void printHand() const;
	int getSize();
	void removeCard(Card* card);

//vector that holds hand cards
private:
	std::vector<Card*>* hand;
	static const int* MAX_HAND_SIZE;
};

class Deck {

	//constructor and destructor and copier
public:
	Deck();
	~Deck();
	Deck(const Deck& copy);

	//functions
	void initializeDeck();
	void shuffle();
	Card* draw(Hand* hand);
	void returnCard(Card* card);
	void getSize();
	void printDeck() const;

//vector that holds cards
private:
	std::vector<Card*>* deck;
};