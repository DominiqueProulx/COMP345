// COMP 345 - Fall 2025
// Risk Game Project
// Assignment 3 - Part 1: Player Strategies
// Comprehensive Test Driver for All Player Strategies

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "GameEngine.h"
#include "Player.h"
#include "PlayerStrategies.h"
#include "Cards.h"
#include "Orders.h"
#include "Map.h"
#include "LoggingObserver.h"

using namespace std;

// Function to display the current game state
void displayGameState(const vector<Player*>& players) {
    cout << "\n========== CURRENT GAME STATE ==========" << endl;
    for (Player* player : players) {
        cout << "\n" << player->getName() << " (";
        
        // Display strategy type
        PlayerStrategies* strategy = player->getStrategy();
        if (dynamic_cast<HumanPlayerStrategy*>(strategy)) {
            cout << "Human";
        } else if (dynamic_cast<AggressivePlayerStrategy*>(strategy)) {
            cout << "Aggressive";
        } else if (dynamic_cast<BenevolentPlayerStrategy*>(strategy)) {
            cout << "Benevolent";
        } else if (dynamic_cast<NeutralPlayerStrategy*>(strategy)) {
            cout << "Neutral";
        } else {
            cout << "Unknown";
        }
        cout << " Strategy)" << endl;
        
        cout << "  Territories owned: " << player->getTerritories()->size() << endl;
        cout << "  Reinforcement pool: " << player->getReinforcementPool() << endl;
        cout << "  Territories: ";
        for (Territory* t : *player->getTerritories()) {
            cout << t->getName() << "(" << t->getNumberOfArmies() << ") ";
        }
        cout << endl;
    }
    cout << "========================================\n" << endl;
}

// Function to check if a player has won (owns all territories)
bool checkWinCondition(Player* player, Map* map) {
    return player->getTerritories()->size() == map->getTerritories().size();
}

// Function to simulate one turn for a player
void executeTurn(Player* player, Map* map, vector<Player*>& players) {
    cout << "\n--- " << player->getName() << "'s Turn ---" << endl;
    
    // Phase 1: Reinforcement
    int reinforcements = 3 + (player->getTerritories()->size() / 3); // Basic reinforcement formula
    player->setReinforcementPool(reinforcements);
    cout << player->getName() << " receives " << reinforcements << " reinforcements." << endl;
    
    // Phase 2: Issue Orders
    cout << "\nIssuing orders..." << endl;
    int orderCount = 0;
    int maxOrders = 5; // Limit orders per turn to prevent infinite loops
    
    while (player->getReinforcementPool() - player->getPendingDeployments() > 0 && orderCount < maxOrders) {
        player->issueOrder();
        orderCount++;
    }
    
    // Issue additional orders (advance, cards, etc.)
    if (orderCount < maxOrders) {
        player->issueOrder(); // One more order after deployments
    }
    
    // Phase 3: Execute Orders
    cout << "\nExecuting orders..." << endl;
    OrdersList* orders = player->getOrdersList();
    while (orders->size() > 0) {
        Order* order = orders->getOrder(0);
        if (order) {
            cout << "Executing: " << *order << endl;
            order->execute();
            orders->remove(0);
        }
    }
    
    // Reset for next turn
    player->clearPendingDeployments();
    player->resetDefendAndAttack();
    
    // Draw a card if conquered territory this turn
    if (player->hasConqueredThisTurn()) {
        Deck* deck = player->getDeck();
        if (deck) {
            deck->draw(player->getHand());
            cout << player->getName() << " draws a card for conquering territory." << endl;
        }
        player->setConqueredThisTurn(false);
    }
}

// Test function for each strategy
void testPlayerStrategies() {
    cout << "========================================" << endl;
    cout << "   TESTING ALL PLAYER STRATEGIES" << endl;
    cout << "========================================\n" << endl;
    
    // Initialize random seed
    srand(time(nullptr));
    
    // Load the map
    MapLoader loader;
    string error;
    Map* map = loader.load("MAPA3.txt", &error);
    
    if (!map || !map->validate()) {
        cerr << "Failed to load or validate map: " << error << endl;
        return;
    }
    
    cout << "Map loaded successfully!" << endl;
    cout << "Territories: " << map->getTerritories().size() << endl;
    cout << "Continents: " << map->getContinents().size() << endl << endl;
    
    // Create deck
    Deck* deck = new Deck();
    deck->initializeDeck();
    
    // Create players with different strategies
    vector<Player*> players;
    
    // Test configuration selection
    cout << "Select test configuration:" << endl;
    cout << "1. Human vs Aggressive" << endl;
    cout << "2. Aggressive vs Benevolent" << endl;
    cout << "3. Benevolent vs Neutral" << endl;
    cout << "4. All AI Players (Aggressive, Benevolent, Neutral)" << endl;
    cout << "5. Custom (choose strategies for 2-4 players)" << endl;
    cout << "Enter choice (1-5): ";
    
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (choice) {
        case 1: {
            // Human vs Aggressive
            Player* human = new Player(deck, make_unique<HumanPlayerStrategy>(new Player()));
            human->setColor("Blue");
            players.push_back(human);
            
            Player* aggressive = new Player(deck, make_unique<AggressivePlayerStrategy>(new Player()));
            aggressive->setColor("Red");
            players.push_back(aggressive);
            break;
        }
        case 2: {
            // Aggressive vs Benevolent
            Player* aggressive = new Player(deck, make_unique<AggressivePlayerStrategy>(new Player()));
            aggressive->setColor("Red");
            players.push_back(aggressive);
            
            Player* benevolent = new Player(deck, make_unique<BenevolentPlayerStrategy>(new Player()));
            benevolent->setColor("Green");
            players.push_back(benevolent);
            break;
        }
        case 3: {
            // Benevolent vs Neutral
            Player* benevolent = new Player(deck, make_unique<BenevolentPlayerStrategy>(new Player()));
            benevolent->setColor("Green");
            players.push_back(benevolent);
            
            Player* neutral = new Player(deck, make_unique<NeutralPlayerStrategy>(new Player()));
            neutral->setColor("Yellow");
            players.push_back(neutral);
            break;
        }
        case 4: {
            // All AI Players
            Player* aggressive = new Player(deck, make_unique<AggressivePlayerStrategy>(new Player()));
            aggressive->setColor("Red");
            players.push_back(aggressive);
            
            Player* benevolent = new Player(deck, make_unique<BenevolentPlayerStrategy>(new Player()));
            benevolent->setColor("Green");
            players.push_back(benevolent);
            
            Player* neutral = new Player(deck, make_unique<NeutralPlayerStrategy>(new Player()));
            neutral->setColor("Yellow");
            players.push_back(neutral);
            break;
        }
        case 5: {
            // Custom configuration
            cout << "How many players (2-4)? ";
            int numPlayers;
            cin >> numPlayers;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (numPlayers < 2) numPlayers = 2;
            if (numPlayers > 4) numPlayers = 4;
            
            for (int i = 0; i < numPlayers; i++) {
                cout << "\nPlayer " << (i + 1) << " strategy:" << endl;
                cout << "1. Human" << endl;
                cout << "2. Aggressive" << endl;
                cout << "3. Benevolent" << endl;
                cout << "4. Neutral" << endl;
                cout << "Enter choice: ";
                
                int strategyChoice;
                cin >> strategyChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                Player* newPlayer = nullptr;
                string color;
                
                cout << "Enter color for Player " << (i + 1) << ": ";
                getline(cin, color);
                
                switch (strategyChoice) {
                    case 1:
                        newPlayer = new Player(deck, make_unique<HumanPlayerStrategy>(new Player()));
                        break;
                    case 2:
                        newPlayer = new Player(deck, make_unique<AggressivePlayerStrategy>(new Player()));
                        break;
                    case 3:
                        newPlayer = new Player(deck, make_unique<BenevolentPlayerStrategy>(new Player()));
                        break;
                    case 4:
                        newPlayer = new Player(deck, make_unique<NeutralPlayerStrategy>(new Player()));
                        break;
                    default:
                        cout << "Invalid choice. Creating Neutral player." << endl;
                        newPlayer = new Player(deck, make_unique<NeutralPlayerStrategy>(new Player()));
                        break;
                }
                
                newPlayer->setColor(color);
                players.push_back(newPlayer);
            }
            break;
        }
        default:
            cout << "Invalid choice. Using default: Aggressive vs Benevolent" << endl;
            Player* aggressive = new Player(deck, make_unique<AggressivePlayerStrategy>(new Player()));
            aggressive->setColor("Red");
            players.push_back(aggressive);
            
            Player* benevolent = new Player(deck, make_unique<BenevolentPlayerStrategy>(new Player()));
            benevolent->setColor("Green");
            players.push_back(benevolent);
            break;
    }
    
    // Distribute territories among players
    cout << "\n--- Distributing Territories ---" << endl;
    vector<Territory*> territories = map->getTerritories();
    int territoryIndex = 0;
    
    while (territoryIndex < territories.size()) {
        for (Player* player : players) {
            if (territoryIndex < territories.size()) {
                Territory* t = territories[territoryIndex++];
                player->addTerritory(t);
                t->setOwner(player);
                t->setNumberOfArmies(3); // Start with 3 armies per territory
                cout << player->getName() << " receives " << t->getName() << endl;
            }
        }
    }
    
    // Give each player initial cards
    cout << "\n--- Distributing Initial Cards ---" << endl;
    for (Player* player : players) {
        for (int i = 0; i < 2; i++) {
            deck->draw(player->getHand());
        }
        cout << player->getName() << " draws 2 cards." << endl;
    }
    
    // Initial reinforcements
    cout << "\n--- Initial Reinforcements ---" << endl;
    for (Player* player : players) {
        player->setReinforcementPool(10);
        cout << player->getName() << " receives 10 initial reinforcements." << endl;
    }
    
    // Display initial state
    displayGameState(players);
    
    // MAIN GAME LOOP
    cout << "\n========== STARTING GAME LOOP ==========" << endl;
    int turnNumber = 0;
    int maxTurns = 30; // Prevent infinite games
    bool gameOver = false;
    Player* winner = nullptr;
    
    while (!gameOver && turnNumber < maxTurns) {
        turnNumber++;
        cout << "\n========== TURN " << turnNumber << " ==========" << endl;
        
        for (Player* player : players) {
            // Check if player still has territories
            if (player->getTerritories()->empty()) {
                cout << player->getName() << " has been eliminated!" << endl;
                continue;
            }
            
            // Execute turn
            executeTurn(player, map, players);
            
            // Display state after turn
            displayGameState(players);
            
            // Check win condition
            if (checkWinCondition(player, map)) {
                winner = player;
                gameOver = true;
                break;
            }
            
            // Allow for manual game termination
            if (dynamic_cast<HumanPlayerStrategy*>(player->getStrategy())) {
                cout << "\nContinue game? (y/n): ";
                char cont;
                cin >> cont;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cont == 'n' || cont == 'N') {
                    gameOver = true;
                    break;
                }
            }
        }
        
        // Remove eliminated players
        players.erase(
            remove_if(players.begin(), players.end(),
                [](Player* p) { return p->getTerritories()->empty(); }),
            players.end()
        );
        
        // Check if only one player remains
        if (players.size() == 1) {
            winner = players[0];
            gameOver = true;
        }
    }
    
    // Game Over
    cout << "\n========== GAME OVER ==========" << endl;
    if (winner) {
        cout << "WINNER: " << winner->getName() << " (";
        PlayerStrategies* strategy = winner->getStrategy();
        if (dynamic_cast<HumanPlayerStrategy*>(strategy)) {
            cout << "Human";
        } else if (dynamic_cast<AggressivePlayerStrategy*>(strategy)) {
            cout << "Aggressive";
        } else if (dynamic_cast<BenevolentPlayerStrategy*>(strategy)) {
            cout << "Benevolent";
        } else if (dynamic_cast<NeutralPlayerStrategy*>(strategy)) {
            cout << "Neutral";
        }
        cout << " Strategy) wins!" << endl;
    } else {
        cout << "Game ended after " << maxTurns << " turns with no winner." << endl;
    }
    
    // Cleanup
    for (Player* player : players) {
        delete player;
    }
    delete deck;
    delete map;
}

// Test individual strategy behaviors
void testIndividualStrategies() {
    cout << "\n========================================" << endl;
    cout << "   TESTING INDIVIDUAL STRATEGY BEHAVIORS" << endl;
    cout << "========================================\n" << endl;
    
    // Create a simple test setup
    MapLoader loader;
    string error;
    Map* map = loader.load("MAPA3.txt", &error);
    
    if (!map) {
        cerr << "Failed to load map for individual tests." << endl;
        return;
    }
    
    Deck* deck = new Deck();
    deck->initializeDeck();
    
    // Test 1: Aggressive Strategy
    cout << "\n--- Testing Aggressive Strategy ---" << endl;
    {
        Player* aggressive = new Player(deck, make_unique<AggressivePlayerStrategy>(new Player()));
        aggressive->setColor("Red");
        
        // Give some territories
        int count = 0;
        for (Territory* t : map->getTerritories()) {
            if (count++ < 3) {
                aggressive->addTerritory(t);
                t->setOwner(aggressive);
                t->setNumberOfArmies(5);
            }
        }
        
        cout << "Aggressive player owns " << aggressive->getTerritories()->size() << " territories." << endl;
        
        // Test toAttack - should return enemy territories
        vector<Territory*>* toAttack = aggressive->toAttack();
        cout << "Territories to attack: " << toAttack->size() << endl;
        
        // Test toDefend - should return strongest territory
        vector<Territory*>* toDefend = aggressive->toDefend();
        cout << "Territories to defend (strongest first): ";
        if (!toDefend->empty()) {
            cout << toDefend->at(0)->getName() << endl;
        }
        
        delete toAttack;
        delete toDefend;
        delete aggressive;
    }
    
    // Test 2: Benevolent Strategy
    cout << "\n--- Testing Benevolent Strategy ---" << endl;
    {
        Player* benevolent = new Player(deck, make_unique<BenevolentPlayerStrategy>(new Player()));
        benevolent->setColor("Green");
        
        // Give some territories with varying army counts
        int count = 0;
        int armies[] = {2, 5, 10};
        for (Territory* t : map->getTerritories()) {
            if (count < 3) {
                benevolent->addTerritory(t);
                t->setOwner(benevolent);
                t->setNumberOfArmies(armies[count]);
                count++;
            }
        }
        
        cout << "Benevolent player owns " << benevolent->getTerritories()->size() << " territories." << endl;
        
        // Test toAttack - should return empty
        vector<Territory*>* toAttack = benevolent->toAttack();
        cout << "Territories to attack: " << toAttack->size() << " (should be 0)" << endl;
        
        // Test toDefend - should return weakest territories first
        vector<Territory*>* toDefend = benevolent->toDefend();
        cout << "Territories to defend (weakest first): ";
        if (!toDefend->empty()) {
            for (Territory* t : *toDefend) {
                cout << t->getName() << "(" << t->getNumberOfArmies() << ") ";
            }
            cout << endl;
        }
        
        delete toAttack;
        delete toDefend;
        delete benevolent;
    }
    
    // Test 3: Neutral Strategy
    cout << "\n--- Testing Neutral Strategy ---" << endl;
    {
        Player* neutral = new Player(deck, make_unique<NeutralPlayerStrategy>(new Player()));
        neutral->setColor("Yellow");
        
        // Give some territories
        int count = 0;
        for (Territory* t : map->getTerritories()) {
            if (count++ < 2) {
                neutral->addTerritory(t);
                t->setOwner(neutral);
                t->setNumberOfArmies(3);
            }
        }
        
        cout << "Neutral player owns " << neutral->getTerritories()->size() << " territories." << endl;
        
        // Test toAttack - should return empty
        vector<Territory*>* toAttack = neutral->toAttack();
        cout << "Territories to attack: " << toAttack->size() << " (should be 0)" << endl;
        
        // Test toDefend - should return empty
        vector<Territory*>* toDefend = neutral->toDefend();
        cout << "Territories to defend: " << toDefend->size() << " (should be 0)" << endl;
        
        // Test issueOrder - should do nothing
        cout << "Testing issueOrder (should take no action):" << endl;
        neutral->issueOrder();
        
        delete toAttack;
        delete toDefend;
        delete neutral;
    }
    
    // Cleanup
    delete deck;
    delete map;
}

// Main test function
int main2() {
    cout << "========================================" << endl;
    cout << "   PLAYER STRATEGIES TEST DRIVER" << endl;
    cout << "========================================\n" << endl;
    
    cout << "Select test mode:" << endl;
    cout << "1. Test full game with strategies" << endl;
    cout << "2. Test individual strategy behaviors" << endl;
    cout << "3. Run both tests" << endl;
    cout << "Enter choice (1-3): ";
    
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            testPlayerStrategies();
            break;
        case 2:
            testIndividualStrategies();
            break;
        case 3:
            testIndividualStrategies();
            cout << "\n\nPress Enter to continue to full game test...";
            cin.get();
            testPlayerStrategies();
            break;
        default:
            cout << "Invalid choice. Running full game test." << endl;
            testPlayerStrategies();
            break;
    }
    
    cout << "\n========================================" << endl;
    cout << "   TEST DRIVER COMPLETED" << endl;
    cout << "========================================" << endl;
    
    return 0;
}