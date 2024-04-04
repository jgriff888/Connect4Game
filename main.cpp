// AERSP 424 FINAL PROJECT
// ALEXIS COLLINS, JULIA GRIFFITH
// CHATGPT WAS USED AS A RESOURCE TO COMPLETE THIS RPOJECT

// main file

#include "Connect4Game.h" // Includes the Connect4Game class definition
#include <iostream> // For standard input and output
#include <limits> // To use std::numeric_limits
#include <memory> // For smart pointers

int main() {
    std::unique_ptr<Connect4Game> game = std::make_unique<Connect4Game>(); // Creates an instance of Connect4Game with automatic memory management

    unsigned int currentPlayer = 1; // Initializes the currentPlayer to player 1
    bool gameWon = false; // Flag to keep track of game state (won or not)
    unsigned short moves = 0; // Counter for the number of moves made in the game

    std::cout << "Welcome to Connect 4!\n"; // Prints welcome message

    // Lambda function for switching players
    auto switchPlayers = [&currentPlayer]() -> void {
        currentPlayer = currentPlayer == 1 ? 2 : 1; // Switches currentPlayer between 1 and 2
    };

    while (!gameWon && moves < Connect4Game::ROWS * Connect4Game::COLUMNS) { // Continues the game until it's won or the board is full
        std::cout << *game; // Uses the overloaded << operator to print the game board

        char symbol = currentPlayer == 1 ? Connect4Game::PLAYER_ONE_SYMBOL : Connect4Game::PLAYER_TWO_SYMBOL; // Determines the current player's symbol

        bool validMove = false; // Flag to check for valid move
        unsigned int column; // Variable to store player's column choice
        while (!validMove) { // Repeats until a valid move is made
            std::cout << "Player " << currentPlayer << ", enter a column (0-6): "; // Prompts the current player to enter a column
            std::cin >> column; // Takes input for column

            // Input validation
            if (std::cin.fail() || column >= Connect4Game::COLUMNS) { // Checks for input failure or invalid column
                std::cin.clear(); // Clears cin's error flags
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignores incorrect input
                std::cout << "Invalid input, please enter a number between 0 and 6.\n"; // Prompts for valid input
                continue; // Continues to the next iteration of the loop
            }

            validMove = game->dropDisc(column, symbol); // Attempts to drop a disc into the column
            if (!validMove) { // Checks if the move was invalid
                std::cout << "This column is full, try another one.\n"; // Informs player the column is full
            }
        }

        moves++; // Increments the move counter
        gameWon = game->checkWin(); // Checks if the last move won the game

        if (gameWon) { // If the game is won
            std::cout << *game; // Prints the final board
            std::cout << "Player " << currentPlayer << " wins!\n"; // Announces the winner
        } else if (moves == Connect4Game::ROWS * Connect4Game::COLUMNS) { // Checks if the game is a draw
            std::cout << "The game is a draw!\n"; // Announces a draw
            break; // Exits the loop
        }

        switchPlayers(); // Switches to the other player
    }

    return 0; // Ends the program
}

