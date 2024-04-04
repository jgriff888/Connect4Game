// AERSP 424 FINAL PROJECT
// ALEXIS COLLINS, JULIA GRIFFITH
// CHATGPT WAS USED AS A RESOURCE TO COMPLETE THIS RPOJECT

// Connect4Game.h file 

#ifndef CONNECT4GAME_H // If CONNECT4GAME_H is not defined,
#define CONNECT4GAME_H // define CONNECT4GAME_H to prevent double inclusion.

#include <vector> // Include the vector header for using vectors.
#include <iostream> // Include the iostream header for input/output operations.

// Base class for games
class Game {
public:
    virtual void printBoard() const = 0; // Pure virtual function to print the game board, must be overridden.
    virtual bool dropDisc(unsigned int col, char player) = 0; // Pure virtual function to drop a disc, must be overridden.
    virtual bool checkWin() const = 0; // Pure virtual function to check for a win, must be overridden.
    virtual ~Game() {} // Virtual destructor to allow derived class destructors to be called.
};

class Connect4Game : public Game { // Connect4Game inherits from Game.
public:
    static const unsigned int ROWS = 6; // Constant integer for number of rows in the game board.
    static const unsigned int COLUMNS = 7; // Constant integer for number of columns in the game board.
    static const char PLAYER_ONE_SYMBOL = 'X'; // Constant character for player one's symbol.
    static const char PLAYER_TWO_SYMBOL = 'O'; // Constant character for player two's symbol.
    static const char EMPTY = '.'; // Constant character for empty slots in the game board.
    
    Connect4Game(); // Constructor declaration.
    void printBoard() const override; // Override the pure virtual printBoard function from Game.
    bool dropDisc(unsigned int col, char player) override; // Override the pure virtual dropDisc function from Game.
    bool checkWin() const override; // Override the pure virtual checkWin function from Game.
    int evaluateBoard() const; // Function to evaluate the board, not part of Game.

    // Template function declaration for filling an array.
    template<typename T>
    static void fillArray(T* array, T value, std::size_t size);

    // Friend declaration for operator<< allowing it to access private members of Connect4Game.
    friend std::ostream& operator<<(std::ostream& os, const Connect4Game& game);

private:
    std::vector<std::vector<char>> board; // The game board, a 2D vector of chars.
};

#endif // CONNECT4GAME_H // End of the header guard.

