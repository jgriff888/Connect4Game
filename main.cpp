// AERSP 424 FINAL PROJECT
// ALEXIS COLLINS, JULIA GRIFFITH
// CHATGPT WAS USED AS A RESOURCE TO COMPLETE THIS RPOJECT

// main file
#include "Connect4Game.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <sstream>

std::mutex gameMutex; // Mutex for synchronizing access to the game
std::condition_variable cv; // Condition variable for turn management
std::atomic<unsigned int> currentPlayer(1); // Player 1 starts
std::atomic<bool> gameWon(false); // Flag to track if the game has been won
std::atomic<bool> gameReady(false); // Flag to control the game flow
unsigned short moves = 0; // Counter for the number of moves made in the game

// Function to write game results to a file
void writeGameResultToFile(const std::string& result) {
    std::ofstream file("game_result.txt");
    if (file.is_open()) {
        file << result;
        file.close();
    } else {
        std::cerr << "Failed to open file for writing.\n";
    }
}

void announceAndWriteResult(const std::unique_ptr<Connect4Game>& game, unsigned int player) {
    std::ostringstream resultStream;
    // Ensures the game board is included before announcing the winner.
    resultStream << *game; // Append the final board to the stream
    resultStream << "Player " << player << " wins with a Connect Four!\n"; // Append the winner announcement to the stream

    std::lock_guard<std::mutex> lock(gameMutex); // Ensure exclusive access to console and file write
    std::cout << resultStream.str(); // Print result to console
    writeGameResultToFile(resultStream.str()); // Write the result to a file

    gameWon = true; // Set game as won
    gameReady = true; // Signal threads to exit
    cv.notify_all(); // Wake up all waiting threads
}

// Function for handling player moves
void playerMove(std::unique_ptr<Connect4Game>& game, unsigned int player) {
    while (!gameReady.load()) {
        std::unique_lock<std::mutex> lock(gameMutex);
        cv.wait(lock, [player] { return currentPlayer.load() == player || gameReady.load(); });

        if (gameReady) break; // Exit if the game is over or ready to exit

        // Show the current state of the game board at the start of each turn
        std::cout << *game;

        char symbol = player == 1 ? Connect4Game::PLAYER_ONE_SYMBOL : Connect4Game::PLAYER_TWO_SYMBOL;

        bool validMove = false;
        unsigned int column;
        while (!validMove && !gameReady.load()) {
            std::cout << "Player " << player << ", enter a column (0-6): ";
            std::cin >> column;

            if (std::cin.fail() || column >= Connect4Game::COLUMNS) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input, please enter a number between 0 and 6.\n";
                continue;
            }

            validMove = game->dropDisc(column, symbol);
            if (!validMove) {
                std::cout << "This column is full, try another one.\n";
            }
        }

        if (validMove) {
            moves++; // Increments the move counter
            if (game->checkWin()) { // Checks if the last move won the game
                // Print the final game board state before announcing the winner
                std::cout << *game;
                std::cout << "Player " << player << " wins with a Connect Four!\n";
                
                gameWon = true;
                gameReady = true; // Set game as over to exit other player's loop
                cv.notify_all(); // Wake up all waiting threads
                
                // Optionally, write the result to a file
                std::ostringstream resultStream;
                resultStream << *game;
                resultStream << "Player " << player << " wins with a Connect Four!\n";
                writeGameResultToFile(resultStream.str());

                break; // Exit loop on win
            }
        }

        if (!gameWon) {
            currentPlayer = currentPlayer == 1 ? 2 : 1; // Switch players
            cv.notify_all(); // Notify other threads
        }
    }
}


int main() {
    std::unique_ptr<Connect4Game> game = std::make_unique<Connect4Game>();

    // Create threads for player 1 and player 2
    std::thread player1Thread(playerMove, std::ref(game), 1);
    std::thread player2Thread(playerMove, std::ref(game), 2);

    // Wait for both threads to finish
    player1Thread.join();
    player2Thread.join();

    return 0;
}
