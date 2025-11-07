#include <iostream>
#include <string>
#include <sstream> 
#include <vector>  
#include <algorithm>
#include <limits>

#include "GameEngine.h"        
#include "CommandProcessing.h" 
#include "Map.h"               
#include "Player.h"            
#include "Cards.h" 

using namespace std;

// Helper function to trim whitespace from the start of a string
string ltrim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : s.substr(start);
}

// --- Stub Functions for Main Play Loop ---
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
    engine.addChildTransition(playersAdded, "gamestart", play);

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
    engine.addChildTransition(issueOrders, "issueordersend", executeOrders);
    engine.addChildTransition(executeOrders, "execorder", executeOrders);
    engine.addChildTransition(executeOrders, "endexecorders", assignReinforcements);
    engine.addChildTransition(executeOrders, "win", win);
    engine.addChildTransition(win, "replay", startup);
    engine.addChildTransition(win, "end", end);

    // END child state
    auto final = engine.createState("quit", false);
    engine.addChildStates(end, { final });

    std::cout << "--- Setup complete. Active state is: "
        << engine.getActiveStatePtr()->getName() << " ---" << std::endl;
}


void testCommandLineProcessor() {
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

    // GAME LOOP
    while (true) {
        cout << "\n------------------------------------" << std::endl;
        cout << "Current State: " << engine.getActiveStatePtr()->getName() << std::endl;

        //  VALIDATION PHASE 
        // getCommand() calls readCommand() (which gets input and validate()
        Command* newCommand = processor.getCommand(engine);

        string fullCommandStr = newCommand->getCommandString();
        string baseCommand;
        string argument;
        stringstream ss(fullCommandStr);
        ss >> baseCommand; // Get the first word
        //get the second word. Use getline to get the REST of the string as the argument
        getline(ss, argument);
        // Trim leading whitespace from the argument
        argument = ltrim(argument);

        // Handle 'quit' 
        if (baseCommand == "quit") {
            if (fullCommandStr == "quit") {
                std::cout << "Exiting program." << std::endl;
				delete newCommand;
                break;
            }
        }

        std::cout << "Effect: " << newCommand->getEffect() << std::endl;

        // EXECUTION PHASE 
        // Check if the validate() passed
		bool isValid = newCommand->isValid();

        if (isValid) {
            bool executionSuccess = false;

            // if/else if block to handle each command
            if (baseCommand == "loadmap") {
                string errorMsg;
                Map* newMap = loader.load(argument, &errorMsg);
                if (newMap) {
                    delete gameMap; 
                    gameMap = newMap;
                    cout << " > SUCCESS: Map loaded." << std::endl;
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

            else if (baseCommand == "issueorder") {
                executionSuccess = true; 
            }
            else if (baseCommand == "endexecorders") {
                executionSuccess = true;
            }

            else if (baseCommand == "issueordersend") {
                executionSuccess = true; 
            }
            else if (baseCommand == "execorder") {
                executionSuccess = true;
            }

            else if (baseCommand == "win") {
                executionSuccess = true;
            }

            else if (baseCommand == "replay") {
                executionSuccess = true; 
                // reset the game state (delete players, map, etc.)
                std::cout << " > ACTION: Resetting game to 'start' state." << std::endl;
                delete gameMap;
                gameMap = nullptr;
                for (Player* p : players) { 
                    delete p; 
                    p = nullptr; 
                }
                players.clear();
                // Also reset the deck
                delete gameDeck;
                gameDeck = new Deck();
                gameDeck->initializeDeck();
            }

            // change the game state
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

    // CLEANUP
    delete gameMap;
    for (Player* p : players) delete p;
    delete gameDeck;
    players.clear();

}

void testFileCommandProcessor(const std::string& filename) {
    // Setup engine and file processor
    GameEngine engine;
    FileCommandProcessorAdapter fileProcessor(filename); // your adapter should open file in constructor
    setupGameStateMachine(engine);

    // Setup game objects
    MapLoader loader;
    Map* gameMap = nullptr;
    std::vector<Player*> players;
    Deck* gameDeck = new Deck();
    gameDeck->initializeDeck();

    // GAME LOOP
    while (true) {
        std::cout << "\n------------------------------------" << std::endl;
        std::cout << "Current State: " << engine.getActiveStatePtr()->getName() << std::endl;

        // Get next valid command from file
        Command* newCommand = fileProcessor.readCommand(engine);
        if (!newCommand) {
            std::cout << "End of command file reached." << std::endl;
            break; // stop at EOF
        }

        std::string fullCommandStr = newCommand->getCommandString();
        std::string baseCommand, argument;
        std::stringstream ss(fullCommandStr);
        ss >> baseCommand;
        std::getline(ss, argument);
        argument = ltrim(argument);

        // Handle quit BEFORE checking validity, since it's not valid in the game engine
        if (baseCommand == "quit" && fullCommandStr == "quit") {
            cout << "Quit order. Exiting program." << std::endl;
            delete newCommand;
            break;
        }

        if (!newCommand->isValid()) {
            std::cout << " > ERROR: Invalid command '" << newCommand->getCommandString() << "'" << std::endl;
            delete newCommand;
            continue; // Skip execution and state change
        }

        std::cout << "Effect: " << newCommand->getEffect() << std::endl;

        // Execute command
        bool executionSuccess = false;

        if (baseCommand == "loadmap") {
            std::string errorMsg;
            Map* newMap = loader.load(argument, &errorMsg);
            if (newMap) {
                delete gameMap;
                gameMap = newMap;
                std::cout << " > SUCCESS: Map loaded." << std::endl;
                executionSuccess = true;
            }
            else {
                std::cout << " > ERROR: " << errorMsg << std::endl;
            }
        }
        else if (baseCommand == "validatemap") {
            if (!gameMap) {
                std::cout << " > ERROR: No map loaded to validate." << std::endl;
            }
            else if (gameMap->validate()) {
                std::cout << " > SUCCESS: Map is valid." << std::endl;
                executionSuccess = true;
            }
            else {
                std::cout << " > ERROR: Map is invalid." << std::endl;
            }
        }
        else if (baseCommand == "addplayer") {
            if (argument.empty()) {
                std::cout << " > ERROR: No player name provided." << std::endl;
            }
            else {
                Player* newPlayer = new Player(argument, {}, gameDeck);
                players.push_back(newPlayer);
                std::cout << " > SUCCESS: Player '" << argument << "' added." << std::endl;
                executionSuccess = true;
            }
        }

        else if (baseCommand == "gamestart") {
            executionSuccess = startGame(gameMap, players);
        }
        else if (baseCommand == "issueorder") {
            executionSuccess = true;
        }
        else if (baseCommand == "endexecorders") {
            executionSuccess = true;
        }

        else if (baseCommand == "issueordersend") {
            executionSuccess = true;
        }
        else if (baseCommand == "execorder") {
            executionSuccess = true;
        }

        else if (baseCommand == "win") {
            executionSuccess = true;
        }

        else if (baseCommand == "replay") {
            std::cout << " > ACTION: Resetting game to 'start' state." << std::endl;
            delete gameMap;
            gameMap = nullptr;
            for (Player* p : players) delete p;
            players.clear();
            delete gameDeck;
            gameDeck = new Deck();
            gameDeck->initializeDeck();
            executionSuccess = true;
        }

        else {
            std::cout << " > ERROR: Unrecognized command '" << baseCommand << "'." << std::endl;
			executionSuccess = false; //prevents transition on invalid command
        }

        // Update engine state if execution succeeded
        if (executionSuccess) {
            engine.changeGameState(baseCommand);
        }
        else {
            std::cout << "--- Action failed. State not changed. ---" << std::endl;
        }

        delete newCommand;
        newCommand = nullptr;
    }

    // Cleanup
    delete gameMap;
    for (Player* p : players) delete p;
    delete gameDeck;
    players.clear();
}

void testCommandProcessor() {

    while (true) {
        cout << "=== Testing Command Line Processor ===" << endl;
        cout << "Choose to read from Command Line (1) or File (2) or Quit(0): ";
        int choice;
        cin >> choice;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flush leftover newline

        if (choice == 1) {
            testCommandLineProcessor();
        }

        else if (choice == 2) {
            cout << "Enter file name: ";
            string filename;
            cin >> filename;
            testFileCommandProcessor(filename);
        }

        else if (choice == 0) {
            cout << "Exiting Command Processor Test." << endl;
            break;
		}

        else {
            cout << "Invalid choice." << endl;
        }
    }

}
