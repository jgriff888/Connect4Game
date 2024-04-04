#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <fstream>
#include <regex>

// Function prototypes
void displayBoard(const std::vector<std::vector<char>>& board);
bool dropPiece(std::vector<std::vector<char>>& board, int column, char player);
bool checkWin(const std::vector<std::vector<char>>& board, char player);
void saveGame(const std::vector<std::vector<char>>& board);
void loadGame(std::vector<std::vector<char>>& board);
void playerMove(std::vector<std::vector<char>>& board, int currentPlayer, std::mutex& mtx, bool& game_over);

class Connect4 {
public:
    Connect4() : board(ROWS, std::vector<char>(COLUMNS, '-')), currentPlayer(1), game_over(false) {}

    void playGame() {
        std::cout << "Welcome to Connect 4!\n";

        std::thread playerThread(&Connect4::playerMove, this, std::ref(board), currentPlayer, std::ref(mtx), std::ref(game_over));

        while (!game_over) {
            displayBoard(board); // Display the board first
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Delay for smooth display

            mtx.lock(); // Lock the mutex before checking game state

            if (checkWin(board, currentPlayer == 1 ? 'X' : 'O')) {
                std::cout << "Player " << currentPlayer << " wins!\n";
                game_over = true;
                mtx.unlock();
                break;
            }

            mtx.unlock(); // Unlock the mutex before switching players
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
        }

        playerThread.join();
    }

private:
    const int ROWS = 6;
    const int COLUMNS = 7;
    std::vector<std::vector<char>> board;
    int currentPlayer;
    bool game_over;
    std::mutex mtx;

    // Display the current state of the board
    void displayBoard(const std::vector<std::vector<char>>& board) {
        system("cls"); // Clear the console (Windows)

        for (int row = 0; row < board.size(); ++row) {
            for (int col = 0; col < board[row].size(); ++col) {
                std::cout << board[row][col] << ' ';
            }
            std::cout << '\n';
        }
    }

    // Drop a piece into the specified column
    bool dropPiece(std::vector<std::vector<char>>& board, int column, char player) {
        if (column < 0 || column >= board[0].size()) {
            return false; // Invalid column
        }
        for (int row = board.size() - 1; row >= 0; --row) {
            if (board[row][column] == '-') {
                board[row][column] = player;
                return true;
            }
        }
        return false; // Column is full
    }

    // Check if the current player has won the game
    bool checkWin(const std::vector<std::vector<char>>& board, char player) {
        // Check horizontally
        for (int row = 0; row < board.size(); ++row) {
            for (int col = 0; col <= board[row].size() - 4; ++col) {
                if (board[row][col] == player &&
                    board[row][col + 1] == player &&
                    board[row][col + 2] == player &&
                    board[row][col + 3] == player) {
                    return true;
                }
            }
        }

        // Check vertically
        for (int col = 0; col < board[0].size(); ++col) {
            for (int row = 0; row <= board.size() - 4; ++row) {
                if (board[row][col] == player &&
                    board[row + 1][col] == player &&
                    board[row + 2][col] == player &&
                    board[row + 3][col] == player) {
                    return true;
                }
            }
        }

        // Check diagonally (from top-left to bottom-right)
        for (int row = 0; row <= board.size() - 4; ++row) {
            for (int col = 0; col <= board[row].size() - 4; ++col) {
                if (board[row][col] == player &&
                    board[row + 1][col + 1] == player &&
                    board[row + 2][col + 2] == player &&
                    board[row + 3][col + 3] == player) {
                    return true;
                }
            }
        }

        // Check diagonally (from top-right to bottom-left)
        for (int row = 0; row <= board.size() - 4; ++row) {
            for (int col = 3; col < board[row].size(); ++col) {
                if (board[row][col] == player &&
                    board[row + 1][col - 1] == player &&
                    board[row + 2][col - 2] == player &&
                    board[row + 3][col - 3] == player) {
                    return true;
                }
            }
        }

        return false; // No win found

        return false;
    }

    // Save the current game state to a file
    void saveGame(const std::vector<std::vector<char>>& board) {
        std::ofstream file("save_game.txt");
        if (file.is_open()) {
            for (const auto& row : board) {
                for (char cell : row) {
                    file << cell << " ";
                }
                file << '\n';
            }
            file.close();
            std::cout << "Game saved successfully!\n";
        }
        else {
            std::cout << "Unable to save game!\n";
        }
    }

    // Load the game state from a file
    void loadGame(std::vector<std::vector<char>>& board) {
        std::ifstream file("save_game.txt");
        if (file.is_open()) {
            for (auto& row : board) {
                for (char& cell : row) {
                    file >> cell;
                }
            }
            file.close();
            std::cout << "Game loaded successfully!\n";
        }
        else {
            std::cout << "No saved game found!\n";
        }
    }

    // Function for player move handling
    void playerMove(std::vector<std::vector<char>>& board, int currentPlayer, std::mutex& mtx, bool& game_over) {
        while (!game_over) {
            mtx.lock();
            int column;
            std::string input;
            std::cout << "Player " << currentPlayer << ", enter column (0-6) to drop your piece (or type 'save' to save game): ";
            std::cin >> input;

            // Regular expression to match integers between 0 and 6
            std::regex integer_regex("[0-6]");

            if (std::regex_match(input, integer_regex)) {
                try {
                    column = std::stoi(input);
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid input. Please enter a valid column number (0-6).\n";
                    mtx.unlock();
                    continue;
                }

                if (dropPiece(board, column, currentPlayer == 1 ? 'X' : 'O')) {
                    if (checkWin(board, currentPlayer == 1 ? 'X' : 'O')) {
                        std::cout << "Player " << currentPlayer << " wins!\n";
                        game_over = true;
                    }
                    mtx.unlock();
                    break;
                }
                else {
                    std::cerr << "Invalid move. Please try again.\n";
                    mtx.unlock();
                }
            }
            else if (input == "save") {
                saveGame(board);
                mtx.unlock();
            }
            else {
                std::cerr << "Invalid input. Please enter a valid column number (0-6).\n";
                mtx.unlock();
            }
        }
    }
};

int main() {
    Connect4 game;
    game.playGame();

    return 0;
}