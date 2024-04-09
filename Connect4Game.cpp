// AERSP 424 FINAL PROJECT
// ALEXIS COLLINS, JULIA GRIFFITH
// CHATGPT WAS USED AS A RESOURCE TO COMPLETE THIS RPOJECT

// Connect4Game.cpp file

#include "Connect4Game.h" // Includes the header file for Connect4Game class definition

// Static members initialization in the .cpp file
const unsigned int Connect4Game::ROWS; // Initializes the number of rows in the game board
const unsigned int Connect4Game::COLUMNS; // Initializes the number of columns in the game board
const char Connect4Game::PLAYER_ONE_SYMBOL; // Initializes the symbol for player one
const char Connect4Game::PLAYER_TWO_SYMBOL; // Initializes the symbol for player two
const char Connect4Game::EMPTY; // Initializes the symbol for empty cells

Connect4Game::Connect4Game() {
    // Initialize the game board with the EMPTY symbol
    board = std::vector<std::vector<char>>(ROWS, std::vector<char>(COLUMNS, EMPTY));
}

void Connect4Game::printBoard() const {
    // Leveraging the overloaded << operator for printing
    std::cout << *this;
}

bool Connect4Game::dropDisc(unsigned int col, char player) {
    if (col < COLUMNS) { // Checks if the column is within the valid range
        for (int row = ROWS - 1; row >= 0; --row) { // Starts from the bottom row
            if (board[row][col] == EMPTY) { // Checks if the cell is empty
                board[row][col] = player; // Places the player's disc in the cell
                return true; // Returns true to indicate a successful drop
            }
        }
    }
    return false; // Returns false if the column is full or out of range, indicating the move wasn't made
}

bool Connect4Game::checkWin() const {
    // Check for wins horizontally, vertically, and both diagonal directions
    for (unsigned int row = 0; row < ROWS; ++row) {
        for (unsigned int col = 0; col < COLUMNS; ++col) {
            char cell = board[row][col]; // Fetches the current cell's symbol
            if (cell == EMPTY) continue; // Skips checking empty cells

            // Horizontal check
            if ((col + 3 & COLUMNS - 1) == col + 3 && // Ensures col + 3 doesn't overflow using bitwise AND
                cell == board[row][col + 1] &&
                cell == board[row][col + 2] &&
                cell == board[row][col + 3]) {
                return true;
            }

            // Vertical check
            if ((row + 3 & ROWS - 1) == row + 3 && // Ensures row + 3 doesn't overflow using bitwise AND
                cell == board[row + 1][col] &&
                cell == board[row + 2][col] &&
                cell == board[row + 3][col]) {
                return true;
            }

            // Diagonal (down-right) check
            if ((row + 3 & ROWS - 1) == row + 3 && (col + 3 & COLUMNS - 1) == col + 3 && // Ensures bounds check within array limits
                cell == board[row + 1][col + 1] &&
                cell == board[row + 2][col + 2] &&
                cell == board[row + 3][col + 3]) {
                return true;
            }

            // Diagonal (up-right) check
            if (int(row - 3) >= 0 && (col + 3 & COLUMNS - 1) == col + 3 && // Ensures row is not underflowing and col + 3 is within bounds
                cell == board[row - 1][col + 1] &&
                cell == board[row - 2][col + 2] &&
                cell == board[row - 3][col + 3]) {
                return true;
            }
        }
    }
    return false; // No win condition found, returns false
}

int Connect4Game::evaluateBoard() const {
    // Simplified evaluation: counts the non-empty cells
    int score = 0; // Initializes score counter
    for (const auto& row : board) { // Iterates through each row
        for (char cell : row) { // Iterates through each cell in the row
            if (cell != EMPTY) ++score; // Increments score for each non-empty cell
        }
    }
    return score; // Returns the total score
}

// Definition for the operator<< as a friend of Connect4Game to access private members
std::ostream& operator<<(std::ostream& os, const Connect4Game& game) {
    os << "Connect 4 Game Board:\n"; // Prints the game board header
    for (const auto& row : game.board) { // Iterates through each row of the board
        for (char cell : row) { // Iterates through each cell in the row
            os << cell << ' '; // Prints the cell's content followed by a space
        }
        os << '\n'; // Prints a newline at the end of each row
    }
    os << "Current Board Evaluation: " << game.evaluateBoard() << '\n'; // Prints the current board evaluation score
    return os; // Returns the ostream object
}


