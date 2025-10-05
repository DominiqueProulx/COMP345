#include "Cards.h"
#include "DummyOrder.h"
#include <iostream>

void testCards() {
	// Create a deck and initialize it
	Deck deck;
	deck.initializeDeck();
	deck.getSize();
	// Create a hand for a player
	Hand hand;
	hand.printHand();
	// Draw 5 cards from the deck to the player's hand
	for (int i = 0; i < 5; ++i) {
		deck.draw(&hand);
	}
	deck.getSize();
	hand.printHand();
	std::cout << "Hand size: " << hand.getSize() << std::endl;

    while (hand.getSize() > 0) {
        //begins to play cards beginning from index 0
        Card* cardToPlay = hand.getCard(0);

        if (cardToPlay) {
            //sets order for testing play function
            Order* returnedOrder = cardToPlay->play(hand, deck);

            if (returnedOrder) {
                std::cout << "Card's play() method returned an order of type: " << returnedOrder->getType() << "\n" << std::endl;

                // Delete Order to prevent memory leak
                delete returnedOrder;
                returnedOrder = nullptr;
            }

            hand.printHand();
            std::cout << "Hand size: " << hand.getSize() << std::endl;
        }

        else {
            std::cout << "Hand is empty, cannot test playing a card." << std::endl;
        }
    }

}