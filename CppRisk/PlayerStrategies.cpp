
#include "PlayerStrategies.h"

// ============================================================================
// PLAYER STRATEGY CLASS IMPLEMENTATION
// ============================================================================
PlayerStrategies::PlayerStrategies(Player *p)
    : player(std::make_unique<Player>(*p)),
      strategyType(new std::string("Default"))
{
}

PlayerStrategies::~PlayerStrategies()
{
    delete strategyType;
}

std::ostream &operator<<(std::ostream &os, const PlayerStrategies &strategy)
{
    os << "Strategy Type: " << *strategy.strategyType << "\n";
    if (strategy.getPlayer())
        os << "Player: " << strategy.getPlayer()->getName();
    else
        os << "Player: (null)";
    return os;
}

// ============================================================================
// HUMAN STRATEGY IMPLEMENTATION
// ============================================================================

// Constructor
HumanPlayerStrategy::HumanPlayerStrategy(Player *p)
    : PlayerStrategies(p)
{
    delete strategyType; // remove Default
    strategyType = new std::string("Human");
}
// Destructor
HumanPlayerStrategy::~HumanPlayerStrategy() = default;
// Copy Constructor
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy &otherstrategy)
    : PlayerStrategies(otherstrategy.getPlayer())
{
    delete strategyType;
    strategyType = new std::string(*otherstrategy.strategyType);
}

std::ostream &operator<<(std::ostream &os, const HumanPlayerStrategy &strategy)
{
    os << "HumanPlayerStrategy for Player: ";
    if (strategy.getPlayer())
        os << strategy.getPlayer()->getName();
    else
        os << "(null)";
    return os;
}

// issueDeployOrder()
// Handles the issuance of a Deploy order by the player.
// Returns a pointer to the created Deploy order
Order *HumanPlayerStrategy::issueDeployOrder()
{

    std::cout << "\nWhich territories would you like to deploy armies to? " << std::endl;
    int i = 1;

    Territory *targetTerritory = player->choseFromToDefend();
    if (targetTerritory == nullptr)
    {
        std::cout << "Deploy order cancelled." << std::endl;
        return nullptr;
    }
    int armiesToDeploy;
    bool validInput = false;
    int reinforcementPool = player->getReinforcementPool();
    int pendingDeployments = player->getPendingDeployments();
    while (!validInput)
    {

        std::cout << "Enter number of armies to deploy to " << targetTerritory->getName()
                << " (max " << (reinforcementPool - pendingDeployments) << "): ";
        std::cin >> armiesToDeploy;

        // Handle invalid input types (non-numeric)
        if (std::cin.fail())
        {
            // Input was not an integer
            std::cin.clear();                                                   // clear the failbit
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        // Validate number of armies to deploy to the chosen territory
        if (armiesToDeploy < 1 || armiesToDeploy > (reinforcementPool - pendingDeployments))
        {
            std::cout << "Invalid number of armies. Please try again." << std::endl;
        }
        else
        {
            validInput = true;
        }
    }

    // Create Deploy order using the order factory
    Order *deployOrder = player->orderFactory(Player::OrderType::DEPLOY, nullptr, targetTerritory, nullptr, armiesToDeploy);

    // Update pending deployments to match the commited armies
    player->setPendingDeployments(pendingDeployments + armiesToDeploy);

    std::cout << armiesToDeploy << " armies deployed to " << targetTerritory->getName() << ". Remaining reinforcement pool: " << (reinforcementPool - pendingDeployments) << std::endl;
    return deployOrder;
}

// issueAdvanceOrder()
// Handles the issuance of an Advance order by the HumanPlayerStrategy
// Returns a pointer to the created Advance order
Order *HumanPlayerStrategy::issueAdvanceOrder()
{
    std::cout << "You want to advance armies to defend your own territories or to attack a neighboring territory,\n press\"d\"for defend and \"a\" for attack. press 0 to cancel." << std::endl;
    bool validInput = false;
    char choice;
    while (!validInput)
    {
        std::cin >> choice;

        // Handles defend choice
        if (choice == 'd' || choice == 'D')
        {
            Territory *fromTerritory = nullptr;
            Territory *toTerritory = nullptr;

            // Loop until a valid territory with armies is chosen to move armies from
            while (fromTerritory == nullptr || fromTerritory->getNumberOfArmies() < 1)
            {
                // chose the territory to move armies from
                std::cout << "chose the territory you want to move armies from: " << std::endl;
                fromTerritory = player->choseFromToDefend();
                if (fromTerritory == nullptr)
                {
                    std::cout << "Advance order cancelled." << std::endl;
                    return nullptr;
                }
                // Check if there are armies available to move
                if (fromTerritory->getNumberOfArmies() < 1)
                {
                    std::cout << "No armies available to move from " << fromTerritory->getName() << " Please chose another Territory or press 0 to skip your turn \n"
                              << std::endl;
                    int choice;
                    std::cin >> choice;

                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number." << std::endl;
                        continue;
                    }
                    if (choice == '0')
                    {
                        std::cout << "Advance order cancelled." << std::endl;
                        return nullptr; // Exit if user chooses to cancel
                    }
                }
            }

            // Chose the territory to move armies to
            std::cout << "chose the territory you want to move armies to: " << std::endl;
            toTerritory = player->choseFromAdjacent(fromTerritory, "defend");

            int armiestoMove;
            bool validArmiesInput = false;
            // Loop until a valid number of armies is entered
            while (!validArmiesInput)
            {
                int maxnumber = fromTerritory->getNumberOfArmies();
                std::cout << "How many armies do you wish to move? max: " << maxnumber << std::endl;
                std::cin >> armiestoMove;
                if (armiestoMove < 1 || armiestoMove > maxnumber)
                {
                    std::cout << "Invalid number of armies. Please try again." << std::endl;
                }
                else
                {
                    validArmiesInput = true;
                }
                // Create the Advance order using the order factory
                std::cout << "Creating an Advance order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
                Order *advanceOrder = player->orderFactory(Player::OrderType::DEPLOY, fromTerritory, toTerritory, nullptr, armiestoMove);

                validInput = true;
                return advanceOrder;
            }
        }
        // Handles attack choice
        else if (choice == 'a' || choice == 'A')
        {
            // chose the territory to move armies from
            Territory *fromTerritory = nullptr;
            Territory *toTerritory = nullptr;
            while (fromTerritory == nullptr || fromTerritory->getNumberOfArmies() < 1)
            {

                std::cout << "Choose the territory you want to move armies from: " << std::endl;
                fromTerritory = player->choseFromToDefend();
                if (fromTerritory == nullptr)
                {
                    return nullptr;
                } // Exit if user chooses to cancel
                if (fromTerritory->getNumberOfArmies() < 1)
                {
                    std::cout << "No armies available to move from " << fromTerritory->getName() << " Please choose another Territory " << std::endl;
                    int choice;
                    std::cin >> choice;
                    // Handle invalid input types (non-numeric)
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number." << std::endl;
                        continue;
                    }
                    // Exit if user chooses to cancel
                    if (choice == '0')
                    {
                        std::cout << "Advance order cancelled." << std::endl;
                        return nullptr;
                    }
                }
            }

            // list territories to attack
            std::cout << "chose the territory you want to move armies to: " << std::endl;
            toTerritory = player->choseFromAdjacent(fromTerritory, "attack");
            int armiestoMove;
            bool validArmiesInput = false;
            while (!validArmiesInput)
            {
                std::cout << "How many armies do you wish to move? " << std::endl;
                std::cin >> armiestoMove;

                // Handle invalid input types (non-numeric)
                if (std::cin.fail())
                {
                    // Input was not an integer
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                    continue;
                }
                // Validate number of armies to move
                if (armiestoMove < 1 || armiestoMove > fromTerritory->getNumberOfArmies())
                {
                    std::cout << "Invalid number of armies. Please try again." << std::endl;
                }
                else
                {
                    validArmiesInput = true;
                }
                // Create the Advance order using the order factory
                std::cout << "Creating an Advance order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
                Order *advanceOrder = player->orderFactory(Player::OrderType::ADVANCE, fromTerritory, toTerritory, nullptr, armiestoMove);

                validInput = true;
                return advanceOrder;
            }
        }
        else if (choice == '0')
        {
            std::cout << "Advance order cancelled." << std::endl; // Exit if user chooses to cancel
            validInput = true;
        }
        else
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    return nullptr;
}

// issueBombOrder()
// Handles the issuance of a Bomb order by the player.
// Returns a pointer to the created Bomb order
Order *HumanPlayerStrategy::issueBombOrder()
{
    // choose target territory
    std::cout << "choose the territory you want to bomb: " << std::endl;
    Territory *targetTerritory = player->choseFromToAttack();
    if (targetTerritory == nullptr)
    {
        std::cout << "Bomb order cancelled." << std::endl;
        return nullptr;
    }
    // Create Bomb order using the order factory
    std::cout << "Creating a Bomb order targeting " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order *bombOrder = player->orderFactory(Player::OrderType::BOMB, nullptr, targetTerritory, nullptr, 0);
    return bombOrder;
}

// issueBlockadeOrder()
// Handles the issuance of a Blockade order by the player.
//// Returns a pointer to the created Blockade order
Order *HumanPlayerStrategy::issueBlockadeOrder()
{
    // choose target territory
    std::cout << "choose the territory you want to blockade: " << std::endl;
    Territory *targetTerritory = player->choseFromToDefend();
    if (targetTerritory == nullptr)
    {
        std::cout << "Blockade order cancelled." << std::endl;
        return nullptr;
    }
    // Create Blockade order using the order factory
    std::cout << "Creating a Blockade order targeting " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order *blockadeOrder = player->orderFactory(Player::OrderType::BLOCKADE, nullptr, targetTerritory, new Player(), 0); // FLAG : need to specify neutral player after integration
    return blockadeOrder;
}

// issueAirliftOrder()
// Handles the issuance of an Airlift order by the player.
// Returns a pointer to the created Airlift order
Order *HumanPlayerStrategy::issueAirliftOrder()
{
    Territory *fromTerritory;
    Territory *toTerritory;

    do
    {
        // choose the origin territory
        std::cout << "Choose the territory you want to airlift armies from: " << std::endl;
        fromTerritory = player->choseFromToDefend();
        if (fromTerritory == nullptr)
        {
            std::cout << "Airlift order cancelled." << std::endl;
            return nullptr;
        }

        // choose target territory
        std::cout << "Choose the territory you want to airlift armies to: " << std::endl;
        toTerritory = player->choseFromToDefend();
        if (toTerritory == nullptr)
        {
            std::cout << "Airlift order cancelled." << std::endl;
            return nullptr;
        }
        if (fromTerritory == toTerritory)
        {
            std::cout << "Please try again, the territories cannot be the same." << std::endl;
        }
    } while (fromTerritory == toTerritory); // this compares the pointers , the == operator is not overloaded for Territory

    // chose number of armies
    int armiesToAirlift;
    bool validInput = false;
    while (!validInput)
    {
        std::cout << "Enter number of armies to airlift from " << fromTerritory->getName()
                  << " to " << toTerritory->getName() << " (max " << fromTerritory->getNumberOfArmies() << "): ";
        std::cin >> armiesToAirlift;

        // Handle invalid input types (non-numeric)
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
        if (armiesToAirlift < 1 || armiesToAirlift > fromTerritory->getNumberOfArmies())
        {
            std::cout << "Invalid number of armies. Please try again." << std::endl;
        }
        else
        {
            validInput = true;
        }
    }

    // Create Airlift order using the order factory
    std::cout << "Creating an Airlift order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order *airliftOrder = player->orderFactory(Player::OrderType::AIRLIFT, fromTerritory, toTerritory, nullptr, armiesToAirlift);
    return airliftOrder;
}

// issueNegotiateOrder()
// Handles the issuance of a Negotiate order by the player.
// Returns a pointer to the created Negotiate order
Order *HumanPlayerStrategy::issueNegotiateOrder()
{
    // choose the player to negotiate with
    std::cout << "Choose the territory of the player you want to negotiate with: " << std::endl;
    // chose the owner of a territory
    Territory *targetTerritory = player->choseFromToAttack();
    if (targetTerritory == nullptr)
    {
        std::cout << "Negociate order cancelled." << std::endl;
        return nullptr;
    }
    // Get the owner of the chosen territory - Negocation is between players not territories
    Player *owner = targetTerritory->getOwner();
    std::cout << "Creating a Negotiate order with player owning " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    // Create Negotiate order using the order factory
    Order *negotiateOrder = player->orderFactory(Player::OrderType::NEGOTIATE, nullptr, nullptr, owner, 0);
    return negotiateOrder;
}

void HumanPlayerStrategy::issueOrder()
{
    // First set both toDefend and toAttack list for the player
    std::cout << "\nPlease specify which of your territories to defend : " << std::endl;
    player->setTerritoriesToDefend(toDefend());
    std::cout << "\nPlease specify which territories you want to attack : " << std::endl;
    player->setTerritoriesToAttack(toAttack());

    std::cout << "\nHere are the possible orders you can issue" << std::endl;
    // Only deploy order is available if the reinforcement pool is not empty
    int reinforcementPool = player->getReinforcementPool();
    int pendingDeployments = player->getPendingDeployments();
    if ((reinforcementPool - pendingDeployments) != 0)
    {
        std::cout << "\nYour reinforcement pool is not empty, please Deploy your armies first" << std::endl;
        std::cout << (reinforcementPool - pendingDeployments) << " armies need to be deployed." << std::endl;
        // Deploy order to assign reinforcements to owned territories
        Order *deployOrder = issueDeployOrder();

        // Add to order list

        if (deployOrder != nullptr)
        {
            player->addOrderToOrderlist(deployOrder);
        }
    }
    else
    {

        // Other orders are available once the reinforcement pool is empty
        std::cout << "\nHere are the possible orders" << std::endl;
        std::cout << "0. End turn" << std::endl;
        std::cout << "1. Advance" << std::endl;
        std::cout << "From the cards available in your hand : \n"
                  << std::endl;
        Hand *playerHand = player->getHand();
        for (int i = 0; i < (*playerHand).getSize(); i++)
        {
            std::cout << (i + 2) << ". " << playerHand->getCard(i)->getName() << std::endl;
        }

        bool validInput = false;
        int choice;
        while (!validInput)
        {
            std::cout << "Please choose an order number" << std::endl;
            std::cin >> choice;
            // Handle invalid input types (non-numeric)
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number." << std::endl;
                continue;
            }

            if (choice < 0 || choice > (1 + (*playerHand).getSize()))
            {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
            else
            {
                validInput = true;
            }
        }
        if (choice == 0)
        {

            return;
        }
        else if (choice == 1)
        {
            // Advance order (to defend or attack)
            Order *advanceOrder = issueAdvanceOrder();
            if (advanceOrder != nullptr)
            {
                player->addOrderToOrderlist(advanceOrder);
            }
            else
            {
                std::cout << "There was a problem with the advance order, it was not added to the orderList" << std::endl;
            };
        }
        else
        {
            // Issue Order from hand
            std::string cardName = playerHand->getCard(choice - 2)->getName();
            std::cout << "Playing the card" << cardName << ", it will create the order and add it to the orders list" << std::endl;
            Deck *deck = (player->getDeck());

            Order *cardOrder = playerHand->getCard(choice - 2)->play(*playerHand, *deck, getPlayer());
            if (cardOrder != nullptr)
            {
                player->addOrderToOrderlist(cardOrder);
            }
            else
            {
                std::cout << "There was a problem with the card order, it was not added to the orderList" << std::endl;
            };
        }
    }
}

// toDefend()
// Player selects the territories they wish to defend this turn, in priority order.
// Returns a vector of Territory pointers representing the defend list.
std::vector<Territory *> *HumanPlayerStrategy::toDefend()
{

    const std::vector<Territory *> *territoriesOwned = player->getTerritories();
    int numberOfTerritories = territoriesOwned->size();

    // If player has no territories, allow to exit
    if (numberOfTerritories == 0)
    {
        std::cout << "You currently have no territories to defend. Returning to main menu..." << std::endl;
        return nullptr;
    }

    std::vector<Territory *> *territoriesToDefendThisturn = new std::vector<Territory *>(); // Create a new vector to hold territories to defend
    bool doneAdding = false;

    std::cout << "\nPlease add territories to defend in priority order." << std::endl;
    std::cout << "(Enter 0 to finish or -1 to cancel)\n"
              << std::endl;

    // Display owned territories
    for (int i = 0; i < numberOfTerritories; i++)
    {
        std::cout << i + 1 << ". " << (*territoriesOwned)[i]->getName() << " - " << (*territoriesOwned)[i]->getNumberOfArmies() << std::endl;
    }

    // Capture user input
    while (!doneAdding)
    {
        std::cout << "\nEnter the number of the territory to add to the defend list: ";
        int choice;
        std::cin >> choice;

        // Handle invalid input types (non-numeric)
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
        // returns the empty list if user cancels, allows a way out if there are no territories to defend.
        if (choice == -1)
        {
            std::cout << "You cancelled territory selection. Returning..." << std::endl;
            return territoriesToDefendThisturn;
        }
        // marks the end of the selection
        if (choice == 0)
        {
            if (territoriesToDefendThisturn->empty())
            {
                std::cout << "You must add at least one territory before finishing, or type -1 to cancel." << std::endl;
            }
            else
            {
                doneAdding = true;
            }
        }
        else if (choice < 1 || choice > numberOfTerritories)
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else
        {
            // add the chosen territory to the defend list if not already added
            Territory *territoryToAdd = (*territoriesOwned)[choice - 1];
            if (std::find(territoriesToDefendThisturn->begin(), territoriesToDefendThisturn->end(), territoryToAdd) == territoriesToDefendThisturn->end())
            {
                territoriesToDefendThisturn->push_back(territoryToAdd);
                std::cout << territoryToAdd->getName() << " added to defend list." << std::endl;
            }
            else
            {
                std::cout << "Territory already in defend list. Please choose another." << std::endl;
            }
        }
    }

    return territoriesToDefendThisturn;
}

// toAttack()
// Player selects the territories they wish to attack this turn, in priority order.
// Returns a vector of Territory pointers representing the attack list.
std::vector<Territory *> *HumanPlayerStrategy::toAttack()
{
    const std::vector<Territory *> *territoriesOwned = player->getTerritories();
    std::vector<Territory *> possibleTerritories;

    // Add all adjacent territories that are not owned by the player
    for (Territory *t : *territoriesOwned)
    {
        const std::vector<Territory *> &adjacentTerritories = t->getAdjacentTerritories();
        for (Territory *adjTerritory : adjacentTerritories)
        {
            if (std::find(possibleTerritories.begin(), possibleTerritories.end(), adjTerritory) == possibleTerritories.end() && (*(adjTerritory->getOwner()) != *player))
            {
                possibleTerritories.push_back(adjTerritory);
            }
        }
    }

    int numberOfPossibleTerritories = possibleTerritories.size();
    std::vector<Territory *> *territoriesToAttackThisturn = new std::vector<Territory *>();

    if (numberOfPossibleTerritories == 0)
    {
        std::cout << "Player has no territories to attack." << std::endl;
        return territoriesToAttackThisturn;
    }
    std::cout << "Please add territories to attack in priority (enter 0 to finish or cancel):" << std::endl;

    // Display possible territories to attack with the number of armies present on it
    for (int i = 0; i < numberOfPossibleTerritories; i++)
    {
        std::cout << i + 1 << ". " << possibleTerritories[i]->getName()
                  << " - " << possibleTerritories[i]->getNumberOfArmies() << " armies." << std::endl;
    }

    bool doneAdding = false;
    while (!doneAdding)
    {
        std::cout << "Enter the number of the territory to add to attack list (0 to finish/cancel): ";
        int choice;
        std::cin >> choice;

        // Handle invalid input types (non-numeric)
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        // Handle finish/cancel input
        if (choice == 0)
        {
            if (territoriesToAttackThisturn->empty())
            {
                std::cout << "No territories selected for attack. Returning empty list." << std::endl;
            }
            doneAdding = true;
        }
        // Validate choice range
        else if (choice < 1 || choice > numberOfPossibleTerritories)
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else
        {
            // Add the chosen territory to the attack list if not already added
            Territory *territoryToAdd = possibleTerritories[choice - 1];
            if (std::find(territoriesToAttackThisturn->begin(), territoriesToAttackThisturn->end(), territoryToAdd) == territoriesToAttackThisturn->end())
            {
                territoriesToAttackThisturn->push_back(territoryToAdd);
                std::cout << territoryToAdd->getName() << " added to attack list." << std::endl;
            }
            else
            {
                std::cout << "Territory already in attack list. Please choose another." << std::endl;
            }
        }
    }

    return territoriesToAttackThisturn; // Return the attack list
}

// ---------------------------------------------------------------------------
// BenevolentPlayerStrategy
// ---------------------------------------------------------------------------
// constructor
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *p)
    : PlayerStrategies(p)
{
    delete strategyType;
    strategyType = new std::string("Benevolent");
}
// Destructor
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() = default;
// Copy constrcutor
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &other)
    : PlayerStrategies(other.getPlayer())
{
    delete strategyType;
    strategyType = new std::string(*other.strategyType);
}
// instertion stream operator
std::ostream &operator<<(std::ostream &os, const BenevolentPlayerStrategy &strategy)
{
    os << "BenevolentPlayerStrategy for Player: ";
    if (strategy.getPlayer())
        os << strategy.getPlayer()->getName();
    else
        os << "(null)";
    return os;
}
std::vector<Territory *> *BenevolentPlayerStrategy::toAttack()
{
    // Benevolent player does not attack
    std::cout << "Benevolent player does not attack. No territories to attack." << std::endl;
    std::vector<Territory *> *territoriesToAttackThisturn = new std::vector<Territory *>();
    return territoriesToAttackThisturn; // Return an empty list
}

// for each territory in player terrytory list add the territories in order of ascending number of armies
std::vector<Territory *> *BenevolentPlayerStrategy::toDefend()
{
    // Get territories owned by the player
    const std::vector<Territory *> *territoriesOwned = player->getTerritories();
    if (territoriesOwned->empty())
    {
        std::cout << "Player has no territories to defend." << std::endl;
        return nullptr;
    }

    // Copy territories to a new vector
    std::vector<Territory *> *territoriesToDefendThisturn = new std::vector<Territory *>(*territoriesOwned);

    // Sort the territories in ascending order by number of armies
    std::sort(territoriesToDefendThisturn->begin(), territoriesToDefendThisturn->end(),
              [](Territory *t1, Territory *t2)
              {
                  return t1->getNumberOfArmies() < t2->getNumberOfArmies();
              });

    std::cout << "Territories to defend (weakest first):" << std::endl;
    for (Territory *t : *territoriesToDefendThisturn)
    {
        std::cout << t->getName() << " - " << t->getNumberOfArmies() << " armies" << std::endl;
    }

    return territoriesToDefendThisturn;
}

void BenevolentPlayerStrategy::issueOrder()
{

    // First set both toDefend and toAttack list for the player
    player->setTerritoriesToDefend(toDefend());
    player->setTerritoriesToAttack(toAttack());

    // Computer player deploys all their armies to their  weakest territories
    int reinforcementPool = player->getReinforcementPool();
    int pendingDeployments = player->getPendingDeployments();

    // Priority: deploy all reinforcements
    if (reinforcementPool - pendingDeployments != 0)
    {
        // Deploy order to assign reinforcements to owned territories
        Order *deployOrder = issueDeployOrder();
        // Add to order list
        if (deployOrder)
            player->addOrderToOrderlist(deployOrder);
        return;
    }

    Hand *hand = player->getHand();
    Deck *deck = player->getDeck();
    bool orderIssued = false;

    while (!orderIssued)
    {
        // Generate a random number between 0 and INT_MAX
        int totalChoices = 2 + hand->getSize();
        int randomNumber = std::rand() % totalChoices;

        // 0 cancel
        if (randomNumber == 0)
            return;

        // Advance order
        if (randomNumber == 1)
        {
            // Advance order (to defend only)
            Order *advanceOrder = issueAdvanceOrder();
            if (advanceOrder)
                player->addOrderToOrderlist(advanceOrder);
            else
                std::cout << "Advance order failed.\n";
            orderIssued = true;
            continue;
        }

        // Issue Order from hand
        int cardIndex = randomNumber - 2;
        Card *card = hand->getCard(cardIndex);

        // Reject BombCards for Benevolent strategy
        if (dynamic_cast<BombCard *>(card))
        {
            std::cout << "Benevolent player refuses BombCard. Choosing again.\n";
            orderIssued = false; // redundant but explicit
            continue;
        }

        // Create the order
        Order *cardOrder = card->play(*hand, *deck, getPlayer());
        if (cardOrder)
            player->addOrderToOrderlist(cardOrder);
        else
            std::cout << "Card order failed.\n";

        orderIssued = true;
    }
}
Order *BenevolentPlayerStrategy::issueDeployOrder()
{

    int i = 1;

    Territory *targetTerritory = player->getTerritoriesToDefend()->at(0); // Always choose the weakest territory
    if (targetTerritory == nullptr)
    {
        return nullptr;
    }
    int armiesToDeploy;
    int reinforcementPool = player->getReinforcementPool();
    int pendingDeployments = player->getPendingDeployments();
    // deploy all available armies to the weakest territory
    armiesToDeploy = reinforcementPool - pendingDeployments;
    return player->orderFactory(Player::OrderType::DEPLOY, nullptr, targetTerritory, nullptr, armiesToDeploy);
}
Order *BenevolentPlayerStrategy::issueAdvanceOrder()
{
    // Choose the strongest territory to move armies from
    Territory *fromTerritory = player->getTerritoriesToDefend()->back();

    // Choose the weakest territory as the initial target
    Territory *toTerritory = player->getTerritoriesToDefend()->front();

    // If the weakest territory is not adjacent, find the first adjacent territory
    if (!toTerritory->isAdjacentTo(fromTerritory))
    {
        bool foundAdjacent = false;
        for (Territory *potentialTarget : *player->getTerritoriesToDefend())
        {
            if (potentialTarget->isAdjacentTo(fromTerritory))
            {
                toTerritory = potentialTarget;
                foundAdjacent = true;
                break;
            }
        }
        if (!foundAdjacent)
        {
            std::cout << "No adjacent territory found in toDefend list for advance order." << std::endl;
            return nullptr;
        }
    }

    // Create and return the Advance order, moving half the armies
    return player->orderFactory(Player::OrderType::DEPLOY, fromTerritory, toTerritory, nullptr, fromTerritory->getNumberOfArmies() / 2);
}
Order *BenevolentPlayerStrategy::issueBlockadeOrder()
{

    Territory *targetTerritory = player->getTerritoriesToDefend()->at(0);
    if (targetTerritory == nullptr)
    {
        std::cout << "Blockade order cancelled." << std::endl;
        return nullptr;
    }
    // Create Blockade order using the order factory
    std::cout << "Creating a Blockade order targeting " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order *blockadeOrder = player->orderFactory(Player::OrderType::BLOCKADE, nullptr, targetTerritory, new Player(), 0); // FLAG : need to specify neutral player after integration
    return blockadeOrder;
}
Order *BenevolentPlayerStrategy::issueAirliftOrder()
{
    Territory *fromTerritory;
    Territory *toTerritory;

    // chose territoriy with the most armies and move to the weakest territory
    fromTerritory = player->getTerritoriesToDefend()->back();
    toTerritory = player->getTerritoriesToDefend()->at(0);
    int armiestoMove = fromTerritory->getNumberOfArmies() / 2; // move half the armies

    // Create Airlift order using the order factory
    std::cout << "Creating an Airlift order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order *airliftOrder = player->orderFactory(Player::OrderType::AIRLIFT, fromTerritory, toTerritory, nullptr, armiestoMove);
    return airliftOrder;
}
Order *BenevolentPlayerStrategy::issueNegotiateOrder()
{

    // chose the owner of a territory
    // Chose the territory of a player that is next to our most vulnerable territory
    Territory *targetTerritory = player->getTerritoriesToDefend()->at(0)->getAdjacentTerritories().at(0);

    if (targetTerritory == nullptr)
    {
        std::cout << "Negociate order cancelled." << std::endl;
        return nullptr;
    }
    // Get the owner of the chosen territory - Negocation is between players not territories
    Player *owner = targetTerritory->getOwner();
    std::cout << "Creating a Negotiate order with player owning " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    // Create Negotiate order using the order factory
    Order *negotiateOrder = player->orderFactory(Player::OrderType::NEGOTIATE, nullptr, nullptr, owner, 0);
    return negotiateOrder;
}

// ---------------------------------------------------------------------------
// CheaterPlayerStrategy
// ---------------------------------------------------------------------------
// constructor
cheaterPlayerStrategy::cheaterPlayerStrategy(Player *p)
    : PlayerStrategies(p)
{
    delete strategyType;
    strategyType = new std::string("Cheater");
}
// Destructor
cheaterPlayerStrategy::~cheaterPlayerStrategy() = default;
// Copy constrcutor
cheaterPlayerStrategy::cheaterPlayerStrategy(const cheaterPlayerStrategy &other)
    : PlayerStrategies(other.getPlayer())
{
    delete strategyType;
    strategyType = new std::string(*other.strategyType);
}

// instertion stream operator
std::ostream &operator<<(std::ostream &os, const cheaterPlayerStrategy &strategy)
{
    os << "CheaterPlayerStrategy for Player: ";
    if (strategy.getPlayer())
        os << strategy.getPlayer()->getName();
    else
        os << "(null)";
    return os;
}

// ---------------------------------------------------------------------------
// AggressivePlayerStrategy
// ---------------------------------------------------------------------------

// constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *p)
    : PlayerStrategies(p)
{
    delete strategyType;
    strategyType = new std::string("Aggressive");
}

// Destructor
AggressivePlayerStrategy::~AggressivePlayerStrategy() = default;

// Copy constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &other)
    : PlayerStrategies(other.getPlayer())
{
    delete strategyType;
    strategyType = new std::string(*other.strategyType);
}

// insertion stream operator
std::ostream &operator<<(std::ostream &os, const AggressivePlayerStrategy &strategy)
{
    os << "AggressivePlayerStrategy for Player: ";
    if (strategy.getPlayer())
        os << strategy.getPlayer()->getName();
    else
        os << "(null)";
    return os;
}

// ---------------------------------------------------------------------------
// Helper: strongest territory (most armies)
// ---------------------------------------------------------------------------
Territory *AggressivePlayerStrategy::getStrongestTerritory()
{
    Territory *strongest = nullptr;
    int maxArmies = -1;

    const std::vector<Territory *> *territoriesOwned = getPlayer()->getTerritories();
    if (!territoriesOwned || territoriesOwned->empty())
    {
        return nullptr;
    }

    for (Territory *t : *territoriesOwned)
    {
        if (!t)
            continue;
        int armies = t->getNumberOfArmies();
        if (armies > maxArmies)
        {
            maxArmies = armies;
            strongest = t;
        }
    }
    return strongest;
}

// ---------------------------------------------------------------------------
// toDefend: for aggressive, we defend strongest-first (descending armies)
// ---------------------------------------------------------------------------
std::vector<Territory *> *AggressivePlayerStrategy::toDefend()
{
    const std::vector<Territory *> *territoriesOwned = getPlayer()->getTerritories();
    if (!territoriesOwned || territoriesOwned->empty())
    {
        std::cout << "Aggressive player has no territories to defend.\n";
        return new std::vector<Territory *>();
    }

    auto *territoriesToDefendThisturn = new std::vector<Territory *>(*territoriesOwned);

    // Sort strongest first
    std::sort(
        territoriesToDefendThisturn->begin(),
        territoriesToDefendThisturn->end(),
        [](Territory *t1, Territory *t2)
        {
            return t1->getNumberOfArmies() > t2->getNumberOfArmies();
        });

    return territoriesToDefendThisturn;
}

// ---------------------------------------------------------------------------
// toAttack: all enemy territories adjacent to the strongest territory,
// ---------------------------------------------------------------------------
std::vector<Territory *> *AggressivePlayerStrategy::toAttack()
{
    auto *attackList = new std::vector<Territory *>();
    Territory *strong = getStrongestTerritory();
    if (!strong)
        return attackList;

    // getAdjacentTerritories returns a reference to a vector
    const std::vector<Territory *> &adj = strong->getAdjacentTerritories();

    for (Territory *t : adj)
    {
        // Compare owner against getPlayer() (Player*) – NOT the unique_ptr
        if (t && t->getOwner() != getPlayer())
        {
            attackList->push_back(t);
        }
    }

    // Smallest forves first
    std::sort(
        attackList->begin(),
        attackList->end(),
        [](Territory *a, Territory *b)
        {
            return a->getNumberOfArmies() < b->getNumberOfArmies();
        });

    return attackList;
}

// ---------------------------------------------------------------------------
// issueOrder
// 1) If any reinforcements not yet deployed -> DEPLOY on strongest
// 2) Otherwise:
//      - If there is a BombCard in hand -> play it (aggressive card)
//      - Else -> ADVANCE from strongest to an enemy (if possible)
// ---------------------------------------------------------------------------
void AggressivePlayerStrategy::issueOrder()
{
    Player* p = getPlayer();

    /* 1  –  build attack / defend lists  ----------------------------------- */
    p->setTerritoriesToDefend(toDefend());
    p->setTerritoriesToAttack(toAttack());

    const int reinforcemtn   = p->getReinforcementPool();
    const int pending= p->getPendingDeployments();

    /* 2  –  deploy everything to the strongest country  -------------------- */
    if (reinforcemtn - pending != 0)
    {
        Order* dep = issueDeployOrder();
        if (dep) p->addOrderToOrderlist(dep);
        return;                       // nothing else this turn
    }

    /* 3  –  aggressive card phase :  Bomb only  ---------------------------- */
    Hand* hand = p->getHand();
    Deck* deck = p->getDeck();

    if (hand && deck && hand->getSize() > 0)
    {
        for (std::size_t i = 0; i < hand->getSize(); ++i)
        {
            Card* c = hand->getCard(i);

            if (dynamic_cast<BombCard*>(c))          // aggressive card
            {
                std::cout << "[Aggressive] playing BombCard.\n";
                Order* bomb = c->play(*hand, *deck, p);
                if (bomb)
                {
                    p->addOrderToOrderlist(bomb);
                    return;                          // done for this turn
                }
                std::cout << "Bomb play failed – continuing.\n";
                break;                               // don’t try another Bomb
            }

            /*  explicitly skip  Blockade, Negotiate, Airlift  */
            if (dynamic_cast<BlockadeCard*>(c))
            {
                std::cout << "[Aggressive] skips BlockadeCard (defensive).\n";
                continue;
            }
            if (dynamic_cast<DiplomacyCard*>(c))
            {
                std::cout << "[Aggressive] skips NegotiateCard (prevents attacks).\n";
                continue;
            }
            if (dynamic_cast<AirliftCard*>(c))
            {
                std::cout << "[Aggressive] skips AirliftCard (not purely aggressive).\n";
                continue;
            }
        }
        // if we reach here : no suitable card was found
    }

    /* 4  –  fallback : always advance from strongest to enemy  ------------- */
    Order* adv = issueAdvanceOrder();
    if (adv)
        p->addOrderToOrderlist(adv);
    else
        std::cout << "[Aggressive] no valid advance possible this turn.\n";
}
// ---------------------------------------------------------------------------
// issueDeployOrder: deploy ALL available armies to the strongest territory
// ---------------------------------------------------------------------------
Order *AggressivePlayerStrategy::issueDeployOrder()
{
    Territory *targetTerritory = getStrongestTerritory();
    if (!targetTerritory)
    {
        std::cout << "Aggressive deploy: no strongest territory.\n";
        return nullptr;
    }

    int reinforcementPool = getPlayer()->getReinforcementPool();
    int pendingDeployments = getPlayer()->getPendingDeployments();
    int armiesToDeploy = reinforcementPool - pendingDeployments;

    if (armiesToDeploy <= 0)
    {
        return nullptr;
    }

    std::cout << "Aggressive player deploying " << armiesToDeploy
            << " armies to strongest territory " << targetTerritory->getName() << ".\n";

    // Update pending deployments
    getPlayer()->setPendingDeployments(pendingDeployments + armiesToDeploy);

    return getPlayer()->orderFactory(
        Player::OrderType::DEPLOY,
        nullptr,
        targetTerritory,
        nullptr,
        armiesToDeploy);
}

// ---------------------------------------------------------------------------
// issueAdvanceOrder: always advances from strongest territory to an enemy
// territory (first in toAttack list) with all but one army.
// Game engine will keep calling issueOrder(), so it keeps attacking until it
// cannot (no enemies adjacent / not enough armies).
// ---------------------------------------------------------------------------
Order *AggressivePlayerStrategy::issueAdvanceOrder()
{
    Territory *fromTerritory = getStrongestTerritory();
    if (!fromTerritory)
    {
        std::cout << "Aggressive advance: no strongest territory.\n";
        return nullptr;
    }

    if (fromTerritory->getNumberOfArmies() <= 1)
    {
        std::cout << "Aggressive advance: strongest territory has no spare armies.\n";
        return nullptr;
    }

    const std::vector<Territory *> *targets = getPlayer()->getTerritoriesToAttack();
    if (!targets || targets->empty())
    {
        std::cout << "Aggressive advance: no enemy territories adjacent to strongest.\n";
        return nullptr;
    }

    // Easiest enemy first (already sorted in toAttack)
    Territory *targetTerritory = (*targets)[0];

    int armiesToMove = fromTerritory->getNumberOfArmies() - 1; // keep 1 behind

    std::cout << "Aggressive player advancing " << armiesToMove
            << " armies from " << fromTerritory->getName()
            << " to enemy territory " << targetTerritory->getName() << ".\n";

    return getPlayer()->orderFactory(
        Player::OrderType::ADVANCE,
        fromTerritory,
        targetTerritory,
        nullptr,
        armiesToMove);
}

// ---------------------------------------------------------------------------
// issueBombOrder (if your header declares it; otherwise you can omit this)
// Chooses an enemy territory adjacent to strongest with the most armies.
// ---------------------------------------------------------------------------
Order *AggressivePlayerStrategy::issueBombOrder()
{
    Territory *strong = getStrongestTerritory();
    if (!strong)
    {
        std::cout << "Aggressive bomb: no strongest territory.\n";
        return nullptr;
    }

    const std::vector<Territory *> &adj = strong->getAdjacentTerritories();
    if (adj.empty())
    {
        std::cout << "Aggressive bomb: no adjacent territories.\n";
        return nullptr;
    }

    Territory *bestTarget = nullptr;
    int maxArmies = -1;

    for (Territory *t : adj)
    {
        if (t && t->getOwner() != getPlayer())
        {
            int armies = t->getNumberOfArmies();
            if (armies > maxArmies)
            {
                maxArmies = armies;
                bestTarget = t;
            }
        }
    }

    if (!bestTarget)
    {
        std::cout << "Aggressive bomb: no valid enemy target.\n";
        return nullptr;
    }

    std::cout << "Aggressive player issuing Bomb order on "
            << bestTarget->getName() << ".\n";

    return getPlayer()->orderFactory(
        Player::OrderType::BOMB,
        nullptr,
        bestTarget,
        nullptr,
        0);
}

// ----------------------------------------------------------
//  NeutralPlayerStrategy  --  full implementation
// ----------------------------------------------------------


NeutralPlayerStrategy::NeutralPlayerStrategy(Player* p)
    : PlayerStrategies(p) {
    delete strategyType;                       // inherit-safe: base inits to nullptr
    strategyType = new std::string("Neutral");
}

NeutralPlayerStrategy::~NeutralPlayerStrategy() = default;


void NeutralPlayerStrategy::issueOrder() {
    std::cout << "[Neutral] Player " << (getPlayer() ? getPlayer()->getName() : "(null)")
            << " takes no action.\n";
}

std::vector<Territory*>* NeutralPlayerStrategy::toAttack() {
    return new std::vector<Territory*>();
}

std::vector<Territory*>* NeutralPlayerStrategy::toDefend() {
    return new std::vector<Territory*>();
}


NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& other)
    : PlayerStrategies(other.getPlayer()) {
    delete strategyType;
    strategyType = new std::string(*other.strategyType);
}


std::ostream& operator<<(std::ostream& os, const NeutralPlayerStrategy& strategy) {
    os << "NeutralPlayerStrategy for Player: ";
    if (strategy.getPlayer())
        os << strategy.getPlayer()->getName();
    else
        os << "(null)";
    return os;
}
