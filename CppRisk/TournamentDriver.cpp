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

void testTournament() {

    cout << "\n======================================================\n";
    cout << "  TOURNAMENT COMMAND VALIDATION DRIVER \n";
    cout << "======================================================\n";
    cout << "Enter 'quit' to exit the test loop.\n\n";

    // Example valid command for reference:
    cout << "Valid Command Format:\n";
    cout << "tournament -M map1.map,map2.map -P aggressive,benevolent -G 3 -D 30\n\n";

    CommandProcessor processor;
    GameEngine engine;
    GameEngine::initializeRiskFSM(engine);

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "quit") {
            break;
        }
        if (input.empty()) {
            continue;
        }

        // Create Command object
        Command cmd(input);

        // Validate the command
        bool validated = processor.validate(cmd, engine);

        // Output results
        if (validated) {
            cout << "Validation: Success.\n";
            cout << "Effect: " << cmd.getEffect() << "\n";

            // Show that the data was successfully stored in the Command object
            if (cmd.hasTournamentData()) {
                const TournamentData& data = cmd.getTournamentData();

                cout << "--- Parsed & Stored Tournament Data ---\n";
                cout << "  Maps: " << data.mapList.size() << " [ ";
                for (const auto& m : data.mapList) cout << m << " ";
                cout << "]\n";
                cout << "  Strategies: " << data.playerList.size() << " [ ";
                for (const auto& p : data.playerList) cout << p << " ";
                cout << "]\n";
                cout << "  Games: " << data.numGames << "\n";
                cout << "  Max Turns: " << data.maxTurns << "\n";
                cout << "---------------------------------------\n";

                cout << "\n\n--- STARTING TOURNAMENT ---" << endl;

                engine.tournamentGameLoop(data);
            }
        }
        else {
            cout << "Validation: Failed.\n";
            cout << "Reason: " << cmd.getEffect() << "\n";
        }
    }
}
