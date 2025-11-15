
#include "PlayerStrategies.h"
// ============================================================================
// PLAYER STRATEGY CLASS IMPLEMENTATION
// ============================================================================
PlayerStrategies::PlayerStrategies(Player* p) : player(p) {};

// ============================================================================
// HUMAN STRATEGY IMPLEMENTATION
// ============================================================================


HumanPlayerStrategy::HumanPlayerStrategy(Player* p) : PlayerStrategies(p) {}
//Destructor is default - player uses smart pointer
HumanPlayerStrategy::~HumanPlayerStrategy() = default;

void HumanPlayerStrategy::issueOrder() {

            //First set both toDefend and toAttack list for the player
            std::cout << "\nPlease specify which of your territories to defend : " << std::endl;
            player->setTerritoriesToDefend(toDefend());
            std::cout << "\nPlease specify which territories you want to attack : " << std::endl;
            player->setTerritoriesToAttack(toAttack());
        

        std::cout << "\nHere are the possible orders you can issue" << std::endl;
        // Only deploy order is available if the reinforcement pool is not empty
		int reinforcementPool = player->getReinforcementPool();
		int pendingDeployments = player->getPendingDeployments();
        if ((reinforcementPool - pendingDeployments) != 0) {
            std::cout << "\nYour reinforcement pool is not empty, please Deploy your armies first" << std::endl;
            std::cout << (reinforcementPool - pendingDeployments) << " armies need to be deployed." << std::endl;
            // Deploy order to assign reinforcements to owned territories
            Order* deployOrder = player->issueDeployOrder();
         
            //Add to order list 

            if (deployOrder != nullptr) { player->addOrderToOrderlist(deployOrder); }
    

        }
        else {

            // Other orders are available once the reinforcement pool is empty
            std::cout << "\nHere are the possible orders" << std::endl;
            std::cout << "0. End turn" << std::endl;
            std::cout << "1. Advance" << std::endl;
            std::cout << "From the cards available in your hand : \n" << std::endl;
			Hand* playerHand = player->getHand();
            for (int i = 0; i < (*playerHand).getSize(); i++) {
                std::cout << (i + 2) << ". " << playerHand->getCard(i)->getName() << std::endl;
            }

            bool validInput = false;
            int choice;
            while (!validInput) {
                std::cout << "Please choose an order number" << std::endl;
                std::cin >> choice;
                // Handle invalid input types (non-numeric)
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                    continue;
                }

                if (choice < 0 || choice >(1 + (*playerHand).getSize())) {
                    std::cout << "Invalid choice. Please try again." << std::endl;
                }
                else {
                    validInput = true;
                }
            }
            if (choice == 0) {

                return;
            }
            else if (choice == 1) {
                // Advance order (to defend or attack)
                Order* advanceOrder = player->issueAdvanceOrder();
                if (advanceOrder != nullptr) { player->addOrderToOrderlist(advanceOrder); }
                else { std::cout << "There was a problem with the advance order, it was not added to the orderList" << std::endl; };
            }
            else {
                // Issue Order from hand
                std::string cardName = playerHand->getCard(choice - 2)->getName();
                std::cout << "Playing the card" << cardName << ", it will create the order and add it to the orders list" << std::endl;
				Deck* deck = (player->getDeck());

                Order* cardOrder = playerHand->getCard(choice - 2)->play(*playerHand, *deck, getPlayer());
                if (cardOrder != nullptr) { player->addOrderToOrderlist(cardOrder); }
                else { std::cout << "There was a problem with the card order, it was not added to the orderList" << std::endl; };
            }
        }
    }

