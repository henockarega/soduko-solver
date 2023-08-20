#ifndef SUDOKU_CPP
#define SUDOKU_CPP
#include "sudoku.hpp"
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>

/**
 * @brief Constructor for the Sudoku class.
 *
 * This constructor initializes the Sudoku solver program and provides the user with two options:
 * 1. To enter their own Sudoku puzzle to be solved.
 * 2. To generate a Sudoku puzzle for the user to solve.
 *
 * Depending on the user's choice, the program either reads a puzzle from a file, solves it, and prints the solution,
 * or generates a Sudoku puzzle, allows the user to view the puzzle, and then solves and prints the solution.
 * The constructor handles user input, error checking, and puzzle validation.
 */
Sudoku::Sudoku() {

    // Display options for the user
    cout << "Welcome to sudoku solver!" << endl;
    cout << "Press 1 if you want enter your own sudoku puzzle." << endl;
    cout << "Press 2 if you want me to generate a sudoku puzzle for you to solve." << endl;
    cout << "Choice: ";
    cin >> option;
    
    cout << endl;
    
    // Handle invalid user choices
    while( option != 1 && option !=2 ) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cerr << "ERROR: please press either 1 (solve puzzle) or 2 (generate puzzle)." << endl;
        cout << "Choice: ";
        cin >> option;
        cout << endl;
    }

    // If user chooses to enter a own sudoku puzzle to be solved
    if(option == 1){
        string file_in;

        cout << "Enter the text file name that holds the sudoku puzzle." << endl;
        cout << "Reading from: ";
        cin >> file_in; 

        ifstream File;
        File.open(file_in);

        // Handle invalid text file
        if(!File.is_open()) {
            cerr << "ERROR: Failed to open the file, check if file exists and try again." << endl;
            File.close();
            exit(EXIT_FAILURE);
        }

        cout << endl;

        cout <<"- - - - - - - - - - - - CREATING BOARD - - - - - - - - - - - - " << endl;
        cout << endl;
        
        // reads from the file and creates the board
        create_board(file_in);

        // Checks if the puzzle provided is valid
        if(puzzle_ready()){
            print_board();

            cout << endl;
            cout <<"- - - - - - - - - - - - SOLVING BOARD - - - - - - - - - - - - " << endl;
            cout << endl;

            // Solve and print the solution
            solve();
            print_board();
        }
        // Prints board to show user the error in the puzzle
        else{
            print_board();
            cout << endl;
            cerr << "ERROR: Invalid puzzle. repeated number in the horizontal, vertical, or box." << endl;
            
        }
        
    }

    // If user chooses to generate a puzzle
    else{
        
        srand(time(nullptr));
        cout <<"- - - - - - - - - - - - CREATING BOARD - - - - - - - - - - - - " << endl;
        cout << endl;

        // Generate a puzzle and print it
        generate();
       
        cout << "Press ENTER when you are ready for the answer. ";
        cin.get();
        cin.ignore();

        cout << endl;
        
        cout <<"- - - - - - - - - - - - SOLVING BOARD - - - - - - - - - - - - " << endl;
        cout << endl;

        // Solve and print the solution
        solve();
        print_board();
    }

    cout << endl;
}


/// @brief Reads the puzzle values from the text file and populate the board. Also makes sure that all 81 values are provided and 
///         values are between 0 and 9.
/// @param text_file Takes in a string type that holds the name of the text file with the puzzle in it.
void Sudoku::create_board(string text_file){
    
    int value;
    ifstream File;
    File.open(text_file);

    // Read the puzzle values from the text file and populate the board
    for(int row = 0; row < 9; row++){
        for(int column = 0; column < 9; column++){
            if((File >> value) && (value >= 0) && (value < 10)){
                board[row][column] = value;
            }
            else{
                // Handles invalid values and exits the program
                if(value < 0 || value >= 10){
                    cerr << "ERROR: The puzzle must be filled with numbers ranging from 0 to 9." << endl;
                }
                // Handles invalid case where the board had less than 81 values 
                else{
                    cerr << "ERROR: Please make sure you provide the full puzzle in the " << text_file 
                            << " file, 81 numbers separated by space." << endl;
                }
                File.close();
                exit(EXIT_FAILURE);
            }
        }
    }

    File.close();

}

/// @brief This function formats and prints the current state of the Sudoku board to the console
void Sudoku::print_board(){
 
    for(int row = 0; row < 9; row++) {
        if(row % 3 == 0 && row != 0) {
            cout << "- - - - - - - - - - - - " << endl;
        }

        for(int column = 0; column < 9; column++) {

            if(column % 3 == 0 && column != 0) {
                cout << " | ";
            }

            if(column == 8) {
                cout << to_string(board[row][column]);
            }
            else{
                cout << to_string(board[row][column]) << " ";
            }
        }
            cout << endl;  
    }
    
    cout << endl;
    
}


/*                                     Code for checking if the provided puzzle is valid                                         */


/// @brief Checks if the puzzle violates the rule of repeated value in a horizontal, vertical or box area.
/// @param curr_board Takes in the current board as a 2D array.
/// @param position Takes in the position of the value as a pair to check if values are repeated
/// @return True if valid, False if not
bool Sudoku::valid_puzzle(int curr_board[9][9], pair<int, int> position) {
    //check through each column in the row to see if the value exists more than once
    int val = curr_board[position.first][position.second];

    for(int column = 0; column < 9; column++){
        if( (curr_board[position.first][column] == val) &&  position.second != column) {
            return false;
        }
    }

    //check through each row in the column to see if the value exists more than once
    for(int row = 0; row < 9; row++){
        if( (curr_board[row][position.second]) == val && position.first != row) {
            return false;
        }
    }

    //check box
    int box_x = position.first / 3;
    int box_y = position.second / 3;

    for(int row = box_x * 3; row < (box_x * 3) + 3; row++) {
        for(int column = box_y * 3; column < (box_y * 3) + 3; column++){
            
            if( (curr_board[row][column] == val) && (row != position.first) && (column != position.second)){
                return false;
            }
        }
    } 
    return true;
}


/// @brief Returns the position of the first non-zero value in the puzzle.
/// @param curr_board Takes in the current board as a 2D array.
/// @return Returns the position of the first non-zero value in the puzzle.
pair<int, int> Sudoku::valid_puzzle_helper(int curr_board[9][9]){
    for(int row = 0; row < 9; row++){
        for(int column = 0; column < 9; column++){
            if(curr_board[row][column] > 0) {
                return make_pair(row, column);
            }
        }
    }
    return make_pair(-1, -1);
}


/// @brief Calls the valid_puzzle_helper to get the position of a non-zero, calls valid_puzzle to check if the column, row, and
///          box in that position follow the sudoku rule of non-repeated value. If its invalid, returns false and and if its valid
///          it returns true and cames a recurisve call to puzzle_check. Recursive ends when all non-zero values are checked or 
///          when the puzzle violates the rule.
/// @param curr_board Takes in the current board as a 2D array.
/// @param position akes in the position of the value as a pair to check if values are repeated
/// @return True if valid, False if not
bool Sudoku::puzzle_check(int curr_board[9][9], pair<int, int> position){

    // Checks if non-zero value exists
    if(valid_puzzle_helper(curr_board).first == -1){
        return true;
    }
    else{

        // Check if the column, row, and box in that position follow the sudoku rule of non-repeated value.
        if(valid_puzzle(curr_board, valid_puzzle_helper(curr_board))){

            // Set that position to 0.
            curr_board[position.first][position.second] = 0;

            // Recursive call to puzzle_check
            if(puzzle_check(curr_board, valid_puzzle_helper(curr_board))){
                return true;
            }
            else{
                return false;
            }
            
        }
        // Return false if the column, row, and box in that position does not follow the sudoku rule of non-repeated value.
        return false;
    }
}


/// @brief Creates a back up board to check if all the non-zero values are valid. Does this by calling puzzle check.
/// @return returns true if valid, false if not
bool Sudoku::puzzle_ready(){
   
    // Creates and populate the back_up 2D array
    int back_up[9][9];
    for(int row = 0; row < 9; row++){
        for(int column = 0; column < 9; column ++){
            back_up[row][column] = board[row][column];
        }
    }
    
    // Calls the puzzle_check function to see if puzzle is valid.
    if(puzzle_check(back_up, valid_puzzle_helper(back_up))){
        return true;
    }
    else{
        return false;
    }
}

/*                                             Code for solving a sudoku puzzle                                                   */

/// @brief Find an empty cell in the Sudoku board.
///         This function searches through the provided curr_board to find the first empty cell (represented by the value 0).
///         It iterates through each row and column, checking the value of each cell. If an empty cell is found, the row and column
///         indices are returned as a pair. If no empty cell is found, the function returns (-1, -1) to indicate that the board is full.
///
/// @param curr_board The current state of the Sudoku board.
/// @return A pair of integers representing the row and column indices of an empty cell, or (-1, -1) if no empty cell is found.
pair<int, int> Sudoku::find_empty(int curr_board[9][9]) {
    
    for(int row = 0; row < 9; row++){
        for(int column = 0; column < 9; column++){
            if(curr_board[row][column] == 0) {
                return make_pair(row, column); // Return the indices of the empty cell.
            }
        }
    }
    return make_pair(-1, -1); // Return (-1, -1) if no empty cell is found.
}

/// @brief Check if placing a value in a certain position is a valid move in the Sudoku board.
///         This function checks the validity of placing the given `val` in the specified `position` of the Sudoku board (`curr_board`).
///         It performs three checks to ensure the placement is valid: it checks the row, column, and 3x3 box in which the position is located.
///         If placing the value in the position violates any Sudoku rules (same value already exists in the row, column, or box),
///         the function returns false. Otherwise, it returns true to indicate that the placement is valid.
///
/// @param curr_board The current state of the Sudoku board.
/// @param val The value to be placed in the position.
/// @param position The row and column indices of the position to check.
/// @return True if placing the value in the position is a valid move, otherwise false.
bool Sudoku::valid_solution(int curr_board[9][9], int val, pair<int, int> position){
    
    //check through each column in the row to see if the value exists more than once
    for(int column = 0; column < 9; column++){
        if( (curr_board[position.first][column] == val) &&  position.second != column) {
            return false;
        }
    }

    //check through each row in the column to see if the value exists more than once
    for(int row = 0; row < 9; row++){
        if( (curr_board[row][position.second]) == val && position.first != row) {
            return false;
        }
    }

    //check box
    int box_x = position.first / 3;
    int box_y = position.second / 3;

    for(int row = box_x * 3; row < (box_x * 3) + 3; row++) {
        for(int column = box_y * 3; column < (box_y * 3) + 3; column++){
            
            if( (curr_board[row][column] == val) && (row != position.first) && (column != position.second)){
                return false;
            }
        }
    } 
    return true;
}

/// @brief Recursive helper function to solve the Sudoku puzzle.
///         This function uses a backtracking algorithm to solve the Sudoku puzzle represented by the `curr_board`.
///         It iterates through each empty cell of the board, and for each cell, it tries placing a value from 1 to 9.
///         It calls the `valid_solution` function to determine if placing the value is valid according to Sudoku rules.
///         If a valid value is found, it places the value and recursively calls itself to solve the remaining puzzle.
///         If the puzzle is successfully solved, it returns true. If no valid value is found, or the puzzle
///         cannot be solved, it backtracks by setting the cell value back to 0 and continues with other possibilities.
///         If no solution is found after trying all possibilities, it returns false to indicate failure in solving.
///
/// @param curr_board The current state of the Sudoku board.
/// @return True if the puzzle is solved successfully, otherwise false.
bool Sudoku::solve_helper(int curr_board[9][9]) {
    

    if(find_empty(curr_board).first == -1){
        return true; // Puzzle is solved when there are no more empty cells.
    }
    else{
        int row = find_empty(curr_board).first;
        int column = find_empty(curr_board).second;
    
        for(int val = 1; val < 10; val++){
            
            if(valid_solution(curr_board, val, find_empty(curr_board))){ // Checks if value is valid
                curr_board[row][column] = val; // Place the valid value in the cell.
                
                if(solve_helper(curr_board)){
                    return true; // Recursively solve the remaining puzzle.
                }
                else{
                    curr_board[row][column] = 0; // Backtrack by resetting the cell value.
                }
            }
        }
        return false; // No valid value found for the current cell, backtrack further.
    }
}

/// @brief Calls the solve_helper and passes in the board.
void Sudoku::solve(){
    solve_helper(board);
}


/*                                            Code for generating a sudoku puzzle                                                 */


/// @brief Fill the diagonal boxes of the Sudoku board with valid values.
///         It uses the `fill_box` function to fill each individual 3x3 box of the diagonal.
void Sudoku::fill_diagonal(){

    // Populate all value to zero
    for(int row = 0; row < 9; row++){
        for(int column = 0; column < 9; column++){
            board[row][column] = 0;
        }
    }

    // Populate the the diagonal boxes bay calling the fill_box function.
    for(int i = 0; i < 9; i += 3){
        fill_box(i, i);
    }
}

/// @brief Fill a 3x3 box of the Sudoku board with valid values.
///         This function fills a 3x3 box of the Sudoku board specified by the given starting
///         row and column indices with valid values such that each row, column, and 3x3 box
///         contains distinct numbers from 1 to 9.
///
/// @param row The starting row index of the 3x3 box.
/// @param column The starting column index of the 3x3 box.
void Sudoku::fill_box(int row, int column){

    // Prepare a vector of values from 1 to 9 in random order.
    vector<int> value;
    for(int i = 1; i < 10; i++){
        value.push_back(i);
    }

    random_device rd;
    auto rng = default_random_engine(rd());
    shuffle(value.begin(), value.end(), rng); 

    int value_index = 0;

    // Fill the 3x3 box with the shuffled values.
    for(int i = row; i < row + 3; i++){
        for(int j = column; j < column + 3; j++){
            board[i][j] = value[value_index];
            value_index++;
        }
    }
}

/// @brief Remove cells from the Sudoku board to create a puzzle.
///         This function removes a specified number of cells from the Sudoku board to create a puzzle
///         while ensuring that the resulting puzzle still has a unique solution. It iterates through the
///         board and randomly removes cells, then checks if the puzzle remains unique by attempting to solve
///         it with the removed cell. If the puzzle becomes non-unique, the removed cell is restored.
///
/// @param num_to_remove The number of cells to remove from the board.
void Sudoku::remove_cell(int num_to_remove){
    
    while(num_to_remove > 0) {

        // Generate random row and column indices.
        int row = rand() % 9;
        int column = rand() % 9;

        if(board[row][column] != 0){
            int temp = board[row][column];
            board[row][column] = 0;

            // Create a backup of the current board.
            int backup_board[9][9];

            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    backup_board[i][j] = board[i][j];
                }
            }

            // Check if removing the cell results in a non-unique solution.
            if(!is_unique(backup_board)){
                board[row][column] = temp; // Restore the removed cell.
            }
            else{
                board[row][column] = 0; // Cell successfully removed.
            }
            num_to_remove--;
        }
    }
}


/// @brief Checks if a Sudoku puzzle has a unique solution.
///         This function attempts to solve the given Sudoku puzzle and determines if it has a unique solution.
///         It uses the `solve_helper` function to solve the puzzle and counts the number of solutions found.
///         If more than one solution is found during the solving process, it returns false to indicate
///         that the puzzle has multiple solutions. If only one solution is found, it returns true to indicate
///         that the puzzle has a unique solution.
///
/// @param curr_board The current state of the Sudoku board.
/// @return True if the puzzle has a unique solution, false if it has multiple solutions.
bool Sudoku::is_unique(int curr_board[9][9]){

    int solutions = 0;
    solve_helper(curr_board);

    // Continue solving to find additional solutions
    while (find_empty(curr_board).first != -1) {

        int row = find_empty(curr_board).first;
        int column = find_empty(curr_board).second;
    
        for (int val = 1; val <= 9; val++) {
            if (valid_solution(curr_board, val, make_pair(row, column))) {
                curr_board[row][column] = val;
                
                if (solve_helper(curr_board)) {
                    solutions++;
                    if (solutions > 1) {
                        return false; // Multiple solutions found
                    }
                }

                curr_board[row][column] = 0;
            }
        }
        
    }
    return true; // Unique solution found
}

/// @brief Generates a Sudoku puzzle for the user to solve.
///
/// This function generates a Sudoku puzzle by following these steps:
/// 1. Determine the number of cells to be removed from the solved puzzle (num_to_remove).
/// 2. Fill the diagonal boxes to create a valid partially-filled puzzle using the fill_diagonal function.
/// 3. Solve the partially-filled puzzle using the solve function to ensure a unique solution.
/// 4. Remove a specified number of cells (num_to_remove) randomly from the puzzle while ensuring
///    that the puzzle still has a unique solution using the remove_cell function.
/// 5. Print the generated puzzle using the print_board function.
void Sudoku::generate(){

    // Determine the number of cells to remove from the solved puzzle
    int num_to_remove = rand() % 20 + 30;

    // Step 1: Fill the diagonal boxes to create a valid partially-filled puzzle
    fill_diagonal();

    // Step 2: Solve the partially-filled puzzle to ensure a unique solution
    solve();

    // Step 3: Remove a specified number of cells while maintaining uniqueness
    remove_cell(num_to_remove);

    // Step 4: Print the generated puzzle
    print_board();
}

#endif