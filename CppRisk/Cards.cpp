#include "Cards.h"
#include "DummyOrder.h"
#include <iostream>
#include <random>

//constructor and destructor for base virtual class
Card::Card(const std::string& name) : name(name) {}

Card::~Card() {}

//overload for << operator
void Card::print(std::ostream& os) const {
    os << "Card: " << name;
}

//friend operator<< overload
std::ostream& operator<<(std::ostream& os, const Card& card) {
    card.print(os);
    return os;
}

//getter function
std::string Card::getName() const {
    return name;
}

//definitions for Bomb class - consructor, destructor, play, print
Bomb::Bomb() : Card("Bomb") {}
Bomb::~Bomb() {}

Order* Bomb::play(Hand& hand, Deck& deck) {
    std::cout << "Bomb card played" << std::endl;
	//returns to deck
    deck.returnCard(this);
    //removes from hand
	hand.removeCard(this);
    //returns order for order list
    return new Order("Bomb");
}

void Bomb::print(std::ostream& os) const {
    os << "Card: " << name;
}

//definitions for Reinforcement class - consructor, destructor, play, print
Reinforcement::Reinforcement() : Card("Reinforcement") {}
Reinforcement::~Reinforcement() {}

Order* Reinforcement::play(Hand& hand, Deck& deck) {
    std::cout << "Reinforcement card played" << std::endl;
    //returns to deck
    deck.returnCard(this);
    //removes from hand
    hand.removeCard(this);
    //returns order for order list
    return new Order("Reinforcement");
}

void Reinforcement::print(std::ostream& os) const {
    os << "Card: " << name;
}

//definitions for Blockade class - consructor, destructor, play, print
Blockade::Blockade() : Card("Blockade") {}
Blockade::~Blockade() {}

Order* Blockade::play(Hand& hand, Deck& deck) {
    std::cout << "Blockade card played" << std::endl;
    //returns to deck
    deck.returnCard(this);
    //removes from hand
    hand.removeCard(this);
    //returns order for order list
    return new Order("Blockade");
}

void Blockade::print(std::ostream& os) const {
    os << "Card: " << name;
}

//definitions for Airlift class - consructor, destructor, play, print
Airlift::Airlift() : Card("Airlift") {}
Airlift::~Airlift() {}

Order* Airlift::play(Hand& hand, Deck& deck) {
    std::cout << "Airlift card played" << std::endl;
    //returns to deck
    deck.returnCard(this);
    //removes from hand
    hand.removeCard(this);
    //returns order for order list
    return new Order("Airlift");
}

void Airlift::print(std::ostream& os) const {
    os << "Card: " << name;
}

//definitions for Diplomacy class - consructor, destructor, play, print
Diplomacy::Diplomacy() : Card("Diplomacy") {}
Diplomacy::~Diplomacy() {}

Order* Diplomacy::play(Hand& hand, Deck& deck) {
    std::cout << "Diplomacy card played" << std::endl;
    //returns to deck
    deck.returnCard(this);
    //removes from hand
    hand.removeCard(this);
    //returns order for order list
    return new Order("Diplomacy");
}

void Diplomacy::print(std::ostream& os) const {
    os << "Card: " << name;
}

//Deck Class constructors and destructors
Deck::Deck() {
    std::cout << "Deck created." << std::endl;
}

Deck::~Deck() {
    std::cout << "Deck destroyed." << std::endl;
    for (Card* card : deck) {
        delete card;
        card = nullptr;
    }
}

//initializes deck with 50 cards and shuffles
void Deck::initializeDeck() {
    // Create 10 of each card type
    for (int i = 0; i < 10; ++i) {
        deck.push_back(new Bomb());
        deck.push_back(new Reinforcement());
        deck.push_back(new Blockade());
        deck.push_back(new Airlift());
        deck.push_back(new Diplomacy());
    }
    std::cout << "Deck initialized with 50 cards." << std::endl;
    shuffle();
}

//uses random to shuffle the deck
void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    std::cout << "Deck shuffled." << std::endl;
}

//displays all information in deck
void Deck::printDeck() const {
    std::cout << "Deck contains:" << std::endl;
    for (const Card* card : deck) {
        std::cout << *card << std::endl;
    }
}

//draws last card from deck and adds to hand
Card* Deck::draw(Hand* hand) {
    if (deck.empty()) {
        std::cout << "Deck is empty. Cannot draw a card." << std::endl;
        return nullptr;
    }
	//creates a pointer to the last card in the deck
    Card* drawn_card = deck.back();
    //removes the last card of the deck
    deck.pop_back();
    //adds to hand
    hand->addCard(drawn_card);
	std::cout << "Card from deck added to player's hand." << std::endl;
    return drawn_card;
}

//adds card to back of deck and shuffles
void Deck::returnCard(Card* card) {
    deck.push_back(card);
    std::cout << "Returned " <<card->getName() << " to deck." << std::endl;
    shuffle();
}

void Deck::getSize() {
	std::cout << "Cards left in deck: " << deck.size() << std::endl;
}

//Hand constructors and desctructors
Hand::Hand() {
    std::cout << "Hand created." << std::endl;
}

Hand::~Hand() {
    std::cout << "Hand destroyed." << std::endl;
    for (Card* card : hand) {
        delete card;
        card = nullptr;
    }
}

//Hand functions
void Hand::addCard(Card* card) {
    //checks to see if hand is full, then adds card to end of vector
    if (hand.size() < MAX_HAND_SIZE) {
        hand.push_back(card);
    }
    else {
        std::cout << "Hand is full! Cannot draw another card." << std::endl;
        //delete to prevent memory leak
        delete card;
    }
}

//displays hand
void Hand::printHand() const {
    if (hand.empty()) {
        std::cout << "Hand is empty." << std::endl;
        return;
	}
    else {
        std::cout << "Hand contains:" << std::endl;
        for (const Card* card : hand) {
            std::cout << *card << std::endl;
        }
    }
    
}

//returns size of hand
int Hand::getSize() {
    return hand.size();
}

Card* Hand::getCard(int index) {
    if (index >= 0 && index < hand.size()) {
        return hand[index];
    } else {
        std::cout << "Invalid index. Cannot get card." << std::endl;
        return nullptr;
	}
}

//this would make more sense game design wise but i think it fails the rubric, i don't know. 
//Order* Hand::playFromHand(int index, Deck& deck) {
//        if (index < 0 || index >= hand.size()) {
//            std::cout << "Invalid index or hand empty. Cannot play card." << std::endl;
//            return nullptr;
//        }
//
//        // Get the card at the specified index from the private vector.
//        Card* cardToPlay = hand[index];
//
//        // Call the card's own play method. It will handle its removal
//        // from the hand and return to the deck.
//        return cardToPlay->play(*this, deck);
//    }

void Hand::removeCard(Card* card) {
    //creates an iterator to find the card in hand (start, end, value)
    std::vector<Card*>::iterator iterator = std::find(hand.begin(), hand.end(), card);
    if (iterator != hand.end()) {
        //removes card at the position of iterator
        hand.erase(iterator);
        std::cout << "Removed " << card->getName() << " from hand." << std::endl;
    } else {
        std::cout << "Card not found in hand." << std::endl;
	}
}