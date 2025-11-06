#include "Orders.h"
#include "Player.h"
#include "Cards.h"
#include <iostream>
#include <random>
#include <algorithm>  

static const int handSizeValue = 5;
const int* Hand::MAX_HAND_SIZE = &handSizeValue;


//constructor and destructor for base virtual class
Card::Card(const std::string& name) : name(new std::string(name)) {}

Card::~Card() {
    delete name;
    name = nullptr;
}

//friend operator<< overload for printing
std::ostream& operator<<(std::ostream& os, const Card& card) {
	os << "Card: " << *card.name;
    return os;
}

//getter function
std::string Card::getName() const {
    return *name;
}

//definitions for Bomb class - consructor, destructor, play, print
BombCard::BombCard() : Card("Bomb") {}
BombCard::~BombCard() {}
Card* BombCard::copy() const {
    return new BombCard(*this);
}

Order* BombCard::play(Hand& hand, Deck& deck, Player* player) {
    std::cout << "Bomb card played" << std::endl;
	//returns to deck
    deck.returnCard(this);
    //removes from hand
	hand.removeCard(this);
    //returns order for order list  
    Order* bombOrder = player->issueBombOrder();
    return bombOrder;
}

//definitions for Reinforcement class - consructor, destructor, play, print
ReinforcementCard::ReinforcementCard() : Card("Reinforcement") {}
ReinforcementCard::~ReinforcementCard() {}
Card* ReinforcementCard::copy() const {
    return new ReinforcementCard(*this);
}

Order* ReinforcementCard::play(Hand& hand, Deck& deck, Player* player) {
    std::cout << "Reinforcement card played" << std::endl;
    //returns to deck
    deck.returnCard(this);
    //removes from hand
    hand.removeCard(this);
    //returns order for order list
    Order* deployOrder = player->issueDeployOrder();
    return deployOrder;
}

//definitions for Blockade class - consructor, destructor, play, print
BlockadeCard::BlockadeCard() : Card("Blockade") {}
BlockadeCard::~BlockadeCard() {}
Card* BlockadeCard::copy() const {
    return new BlockadeCard(*this);
}

Order* BlockadeCard::play(Hand& hand, Deck& deck, Player* player) {
    std::cout << "Blockade card played" << std::endl;
    //returns to deck
    deck.returnCard(this);
    //removes from hand
    hand.removeCard(this);
    //returns order for order list
    Order* blockadeOrder = player->issueBlockadeOrder();
    return blockadeOrder;
}

//definitions for Airlift class - consructor, destructor, play, print
AirliftCard::AirliftCard() : Card("Airlift") {}
AirliftCard::~AirliftCard() {}
Card* AirliftCard::copy() const {
    return new AirliftCard(*this);
}

Order* AirliftCard::play(Hand& hand, Deck& deck, Player* player) {
    std::cout << "Airlift card played" << std::endl;
    //returns to deck
    deck.returnCard(this);
    //removes from hand
    hand.removeCard(this);
    //returns order for order list
    Order* airliftOrder = player->issueAirliftOrder();
    return airliftOrder;
}

//definitions for Diplomacy class - consructor, destructor, play, print
DiplomacyCard::DiplomacyCard() : Card("Diplomacy") {}
DiplomacyCard::~DiplomacyCard() {}
Card* DiplomacyCard::copy() const {
    return new DiplomacyCard(*this);
}

Order* DiplomacyCard::play(Hand& hand, Deck& deck, Player* player) {
    std::cout << "Diplomacy card played" << std::endl;
    //returns to deck
    deck.returnCard(this);
    //removes from hand
    hand.removeCard(this);
    //returns order for order list
    Order* negociatetOrder = player->issueNegotiateOrder();
	return negociatetOrder;
}

//Deck Class constructors and destructors
Deck::Deck() : deck(new std::vector<Card*>()) {
    std::cout << "Deck created." << std::endl;
}

Deck::~Deck() {
    std::cout << "Deck destroyed." << std::endl;
    for (Card* card : *deck) {
        delete card;
    }
    deck->clear();
    delete deck;
    deck = nullptr;
}

//copy constructor for deep copy
Deck::Deck(const Deck& copy) : deck(new std::vector<Card*>()) {
    for (Card* card : *copy.deck) {
        deck->push_back(card->copy());
    }
    std::cout << "Deck copied." << std::endl;
}

//initializes deck with 50 cards and shuffles
void Deck::initializeDeck() {
    // Create 10 of each card type
    for (int i = 0; i < 10; ++i) {
        deck->push_back(new BombCard());
        deck->push_back(new ReinforcementCard());
        deck->push_back(new BlockadeCard());
        deck->push_back(new AirliftCard());
        deck->push_back(new DiplomacyCard());
    }
    std::cout << "Deck initialized with 50 cards." << std::endl;
    shuffle();
}

//uses random to shuffle the deck
void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck->begin(), deck->end(), g);
    std::cout << "Deck shuffled." << std::endl;
}

//displays all information in deck
void Deck::printDeck() const {
    std::cout << "Deck contains:" << std::endl;
    for (const Card* card : *deck) {
        std::cout << *card << std::endl;
    }
}

//draws last card from deck and adds to hand
Card* Deck::draw(Hand* hand) {
    if (deck->empty()) {
        std::cout << "Deck is empty. Cannot draw a card." << std::endl;
        return nullptr;
    }
	//creates a pointer to the last card in the deck
    Card* drawn_card = deck->back();
    //removes the last card of the deck
    deck->pop_back();
    //adds to hand
    hand->addCard(drawn_card);
	std::cout << "Card from deck added to player's hand." << std::endl;
    return drawn_card;
}

//adds card to back of deck and shuffles
void Deck::returnCard(Card* card) {
    deck->push_back(card);
    std::cout << "Returned " <<card->getName() << " to deck." << std::endl;
    shuffle();
}

void Deck::getSize() {
	std::cout << "Cards left in deck: " << deck->size() << std::endl;
}

//Hand constructors and desctructors
Hand::Hand() : hand(new std::vector<Card*>()) {
    std::cout << "Hand created." << std::endl;
}

Hand::~Hand() {
    std::cout << "Hand destroyed." << std::endl;
    for (Card* card : *hand) {
        delete card;
    }
    hand->clear();
    delete hand;
    hand = nullptr;
}

//copy constructor for deep copy
Hand::Hand(const Hand& copy) : hand(new std::vector<Card*>()) {
    for (Card* card : *copy.hand) {
        hand->push_back(card->copy());
    }
    std::cout << "Hand copied." << std::endl;
}

//Hand functions
void Hand::addCard(Card* card) {
    //checks to see if hand is full, then adds card to end of vector
    if (hand->size() < *MAX_HAND_SIZE) {
        hand->push_back(card);
    }
    else {
        std::cout << "Hand is full! Cannot draw another card." << std::endl;
        //delete to prevent memory leak
        delete card;
    }
}

//displays hand
void Hand::printHand() const {
    if (hand->empty()) {
        std::cout << "Hand is empty." << std::endl;
        return;
	}
    else {
        std::cout << "Hand contains:" << std::endl;
        for (const Card* card : *hand) {
            std::cout << *card << std::endl;
        }
    }
    
}

//returns size of hand
int Hand::getSize() {
    return hand->size();
}

Card* Hand::getCard(int index) {
    if (index >= 0 && index < hand->size()) {
        return (*hand)[index];
    } else {
        std::cout << "Invalid index. Cannot get card." << std::endl;
        return nullptr;
	}
}

void Hand::removeCard(Card* card) {
    //creates an iterator to find the card in hand (start, end, value)
    std::vector<Card*>::iterator iterator = std::find(hand->begin(), hand->end(), card);
    if (iterator != hand->end()) {
        //removes card at the position of iterator
        hand->erase(iterator);
        std::cout << "Removed " << card->getName() << " from hand." << std::endl;
    } else {
        std::cout << "Card not found in hand." << std::endl;
	}
}