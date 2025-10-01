#include "Cards.h"
#include <iostream>
#include <random>

//constructor and destructor for base virtual class
Card::Card(const std::string& name) : name(name) {}

Card::~Card() {}

//overload for << operator
void Card::print(std::ostream& os) const {
    os << "Card: " << name;
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    card.print(os);
    return os;
}

//definitions for Bomb class
Bomb::Bomb() : Card("Bomb") {}
Bomb::~Bomb() {}

void Bomb::play() {
    std::cout << "Bomb card played" << std::endl;
};

void Bomb::print(std::ostream& os) const {
    os << "Card: " << name;
}

//definitions for Reinforcement class
Reinforcement::Reinforcement() : Card("Reinforcement") {}
Reinforcement::~Reinforcement() {}

void Reinforcement::play() {
    std::cout << "Reinforcement card played" << std::endl;
}

void Reinforcement::print(std::ostream& os) const {
    os << "Card: " << name;
}

//definitions for Blockade class
Blockade::Blockade() : Card("Blockade") {}
Blockade::~Blockade() {}

void Blockade::play() {
    std::cout << "Blockade card played" << std::endl;
}

void Blockade::print(std::ostream& os) const {
    os << "Card: " << name;
}

//definitions for Airlift class
Airlift::Airlift() : Card("Airlift") {}
Airlift::~Airlift() {}

void Airlift::play() {
    std::cout << "Airlift card played" << std::endl;
}

void Airlift::print(std::ostream& os) const {
    os << "Card: " << name;
}

//definitions for Diplomacy class
Diplomacy::Diplomacy() : Card("Diplomacy") {}
Diplomacy::~Diplomacy() {}

void Diplomacy::play() {
    std::cout << "Diplomacy card played" << std::endl;
}

void Diplomacy::print(std::ostream& os) const {
    os << "Card: " << name;
}

//Deck Class
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

void Deck::printDeck() const {
    std::cout << "Deck contains:" << std::endl;
    for (const Card* card : deck) {
        std::cout << *card << std::endl;
    }
}

Card* Deck::draw(Hand* hand) {
    if (deck.empty()) {
        std::cout << "Deck is empty. Cannot draw a card." << std::endl;
        return nullptr;
    }
	//creates a pointer to the last card in the deck
    Card* drawn_card = deck.back();
    //removes the last card of the deck
    deck.pop_back();
    hand->addCard(drawn_card);
	std::cout << "Card from deck added to player's hand." << std::endl;
    return drawn_card;
}

void Deck::returnCard(Card* card) {
    deck.push_back(card);
    std::cout << "Returned " <<card->getName() << " to deck." << std::endl;
    shuffle();
}

//Hand Class
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

void Hand::addCard(Card* card) {
    hand.push_back(card);
}

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

int Hand::getSize() {
    return hand.size();
}

void Hand::playFromHand(int index, Deck& deck) {
    if (hand.empty() || index < 0 || index >= hand.size()) {
        std::cout << "Invalid index or hand empty. Cannot play card." << std::endl;
        return;
	}

    Card* played_card = hand[index];
    played_card->play();

    //add back to deck
	deck.returnCard(played_card);

    //erase from hand
    hand.erase(hand.begin() + index);
}