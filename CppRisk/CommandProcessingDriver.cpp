#include <iostream>
#include <string>
#include <sstream> 
#include <vector>  

#include "GameEngine.h"        
#include "CommandProcessing.h" 
#include "Map.h"               
#include "Player.h"            
#include "Cards.h" 

using namespace std;

// --- Stub Functions for Main Play Loop ---
// This is the stub for the "assigncountries" command.
bool startGame(Map* map, vector<Player*>& players) {
    std::cout << " > EXECUTING: Assigning countries..." << std::endl;
    if (!map) {
        std::cout << " > ERROR: Cannot start game, no map is loaded." << std::endl;
        return false;
    }
    if (players.size() < 2) {
        std::cout << " > ERROR: Cannot start game, not enough players (minimum 2)." << std::endl;
        return false;
    }
    // In a real program, you'd distribute territories, assign armies, etc.
    std::cout << " > SUCCESS: Game started. Countries assigned. Moving to 'assign reinforcement' phase." << std::endl;
    return true;
}


void setupGameStateMachine(GameEngine& engine)
{
    // Parent states
    auto startup = engine.createState("startup", true);
    auto play = engine.createState("play", true);
    auto end = engine.createState("end", true);

    engine.addParentStates({ startup, play, end });

    // STARTUP child states
    auto start = engine.createState("start", false);
    auto mapLoaded = engine.createState("map loaded", false);
    auto mapValidated = engine.createState("map validated", false);
    auto playersAdded = engine.createState("players added", false);

    engine.addChildStates(startup, { start, mapLoaded, mapValidated, playersAdded });

    engine.addChildTransition(start, "loadmap", mapLoaded);
    engine.addChildTransition(mapLoaded, "loadmap", mapLoaded);
    engine.addChildTransition(mapLoaded, "validatemap", mapValidated);
    engine.addChildTransition(mapValidated, "addplayer", playersAdded);
    engine.addChildTransition(playersAdded, "addplayer", playersAdded);
    engine.addChildTransition(playersAdded, "assigncountries", play);

    // Set initial state for the startup parent
    engine.setActiveState(startup->getInitialSubstatePtr());

    // PLAY child states
    auto assignReinforcements = engine.createState("assign reinforcements", false);
    auto issueOrders = engine.createState("issue orders", false);
    auto executeOrders = engine.createState("execute orders", false);
    auto win = engine.createState("win", false);

    engine.addChildStates(play, { assignReinforcements, issueOrders, executeOrders, win });

    engine.addChildTransition(assignReinforcements, "issueorder", issueOrders);
    engine.addChildTransition(issueOrders, "issueorder", issueOrders);
    engine.addChildTransition(issueOrders, "endissueorders", executeOrders);
    engine.addChildTransition(executeOrders, "execorder", executeOrders);
    engine.addChildTransition(executeOrders, "endexecorders", assignReinforcements);
    engine.addChildTransition(executeOrders, "win", win);
    engine.addChildTransition(win, "play", startup);
    engine.addChildTransition(win, "end", end);

    // END child state
    auto final = engine.createState("quit", false);
    engine.addChildStates(end, { final });

    std::cout << "--- Setup complete. Active state is: "
        << engine.getActiveStatePtr()->getName() << " ---" << std::endl;
}


void testCommandProcessor() {
    //setup engine and processor
	GameEngine engine;
	CommandProcessor processor;
	setupGameStateMachine(engine);

    //setup play objects map, players, deck
    MapLoader loader;
    Map* gameMap = nullptr; 
    vector<Player*> players; 
    Deck* gameDeck = new Deck();
    gameDeck->initializeDeck();

    // --- GAME LOOP (Test Driver) ---
    // This loop waits for user input.
    while (true) {
        std::cout << "\n------------------------------------" << std::endl;
        std::cout << "Current State: " << engine.getActiveStatePtr()->getName() << std::endl;

        // --- VALIDATION PHASE ---
        // getCommand() calls readCommand() (which gets input) and validate()
        Command* newCommand = processor.getCommand(engine);

        string fullCommandStr = newCommand->getCommandString();
        string baseCommand;
        string argument;
        stringstream ss(fullCommandStr);
        ss >> baseCommand; // Get the first word
        ss >> argument;    // Get the second word (if it exists)

        // Handle 'quit' immediately
        if (baseCommand == "quit") {
            if (fullCommandStr == "quit") {
                std::cout << "Exiting program." << std::endl;
				delete newCommand;
                break;
            }
        }

        std::cout << "Effect: " << newCommand->getEffect() << std::endl;

        // --- EXECUTION PHASE ---
        // Check if the validation *passed*
        bool isValid = (newCommand->getEffect().find("Invalid") == string::npos &&
            newCommand->getEffect().find("Error") == string::npos);

        if (isValid) {
            bool executionSuccess = false;

            // if/else if block to handle each command
            if (baseCommand == "loadmap") {
                string errorMsg;
                Map* newMap = loader.load(argument, &errorMsg);
                if (newMap) {
                    delete gameMap; 
                    gameMap = newMap;
                    std::cout << " > SUCCESS: Map '" << gameMap->getName() << "' loaded." << std::endl;
                    executionSuccess = true;
                }
                else {
                    std::cout << " > ERROR: " << errorMsg << std::endl;
                    executionSuccess = false;
                }
            }
            else if (baseCommand == "validatemap") {
                if (!gameMap) {
                    std::cout << " > ERROR: No map loaded to validate." << std::endl;
                    executionSuccess = false;
                }
                else {
                    if (gameMap->validate()) {
                        std::cout << " > SUCCESS: Map is valid." << std::endl;
                        executionSuccess = true;
                    }
                    else {
                        std::cout << " > ERROR: Map is invalid." << std::endl;
                        executionSuccess = false;
                    }
                }
            }
            else if (baseCommand == "addplayer") {
                if (argument.empty()) {
                    std::cout << " > ERROR: No player name provided." << std::endl;
                    executionSuccess = false;
                }
                else {
                    // Create an empty vector of territories to pass to the constructor
                    std::vector<Territory*> initialTerritories;
                    // Use the constructor that accepts a name (color) and the shared deck
                    Player* newPlayer = new Player(argument, initialTerritories, gameDeck);
                    players.push_back(newPlayer);
                    std::cout << " > SUCCESS: Player '" << argument << "' added. Total players: " << players.size() << std::endl;
                    executionSuccess = true;
                }
            }
            else if (baseCommand == "gamestart") {
                executionSuccess = startGame(gameMap, players);
            }
            else if (baseCommand == "replay") {
                executionSuccess = true; // Simple state change
                // You would also reset the game state (delete players, map, etc.)
                std::cout << " > ACTION: Resetting game to 'start' state." << std::endl;
                delete gameMap;
                gameMap = nullptr;
                for (Player* p : players) { delete p; }
                players.clear();
                // Also reset the deck
                delete gameDeck;
                gameDeck = new Deck();
                gameDeck->initializeDeck();
            }

            // If the specific action was successful, change the game state
            if (executionSuccess) {
                engine.changeGameState(baseCommand);
            }
            else {
                std::cout << "--- Action failed. State not changed. ---" << std::endl;
            }
        }

        delete newCommand;
		newCommand = nullptr;
    }

    // --- CLEANUP ---
    delete gameMap;
    gameMap = nullptr;

    for (Player* p : players) {
        delete p;
    }
    players.clear();

    // Delete the shared deck
    delete gameDeck;
    gameDeck = nullptr;

}