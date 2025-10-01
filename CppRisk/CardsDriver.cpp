#include "Cards.h"
#include <iostream>

void testCards() {
	// Create a deck and initialize it
	Deck deck;
	deck.initializeDeck();
	deck.printDeck();
	// Create a hand for a player
	Hand hand;
	// Draw 5 cards from the deck to the player's hand
	for (int i = 0; i < 5; ++i) {
		deck.draw(&hand);
	}
	hand.printHand();
	std::cout << "Hand size: " << hand.getSize() << std::endl;
	// Play the first card in the hand
	hand.playFromHand(0, deck);
	hand.printHand();
	std::cout << "Hand size: " << hand.getSize() << std::endl;

	hand.playFromHand(0, deck);
	hand.printHand();
	std::cout << "Hand size: " << hand.getSize() << std::endl;

	hand.playFromHand(0, deck);
	hand.printHand();
	std::cout << "Hand size: " << hand.getSize() << std::endl;

	hand.playFromHand(0, deck);
	hand.printHand();
	std::cout << "Hand size: " << hand.getSize() << std::endl;

	hand.playFromHand(0, deck);
	hand.printHand();
	std::cout << "Hand size: " << hand.getSize() << std::endl;

	hand.playFromHand(0, deck);
	hand.printHand();
	std::cout << "Hand size: " << hand.getSize() << std::endl;
}