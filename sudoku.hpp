#ifndef SUDOOKU_H
#define SUDOOKU_H
#include <iostream>
#include <utility>


using namespace std;

class Sudoku{
    public:

        Sudoku(); // Constructor
        
    
    private:
        void create_board(string text_file); // Function to read puzzle from a text file and initialize the board
        void print_board(); // Function to print the current state of the Sudoku boar

        // Puzzle validation and solving helper functions
        bool valid_puzzle(int curr_board[9][9], pair<int, int> position);
        pair<int, int> valid_puzzle_helper(int curr_board[9][9]);
        bool puzzle_check(int curr_board[9][9], pair<int, int> position);
        bool puzzle_ready();
        

        pair<int, int> find_empty(int curr_board[9][9]); // Function to find an empty cell on the board
        bool valid_solution(int curr_board[9][9], int val, pair<int, int> position); // Function to check if a value can be placed at a specific position
        
        // Recursive backtracking solver       
        bool solve_helper(int curr_board[9][9]);
        void solve();

        // Functions for generating Sudoku puzzles
        void fill_diagonal();
        void fill_box(int row, int column);
        void remove_cell(int num_to_remove);
        bool is_unique(int curr_board[9][9]);
        void generate(); // Generates a Sudoku puzzle
                

        int board[9][9]; // The Sudoku puzzle board
        int option; // User option for puzzle creation or generation
        
        
};




#endif