// NTO1064 Student Project
// Boolean Expression & Truth Table Simulator
// Author: Gantian
// Description: An interactive Boolean expression simulator
// using Object-Oriented Programming in C++
// Main application file - handles user interaction, file save/load

#include "BooleanOperator.h"
#include <iostream>
#include <fstream>
using namespace std;

// ========== File Save ==========
// Saves the expression, operator explanations and truth table to a text file
// Uses ofstream to write to the file
void saveToFile(string expression) {
    cout << "\nEnter filename to save (e.g. output.txt): ";
    string filename;
    cin >> filename;

    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return;
    }

    file << "*** BOOLEAN TRUTH TABLE SIMULATOR ***" << endl;
    file << "Expression: " << expression << endl;

    // Write operator explanations to file
    BooleanExpression boolExpr(expression);
    boolExpr.explainOperatorsToFile(file);

    // Write truth table to file
    TruthTable tt(boolExpr);
    tt.generate(file);

    file.close();
    cout << "Saved successfully to " << filename << "!" << endl;
}

// ========== File Load ==========
// Loads a previously saved expression from a text file
// Uses ifstream to read the file and regenerates the truth table
void loadFromFile() {
    cout << "\nEnter filename to load: ";
    string filename;
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return;
    }

    // Find the expression line
    string line;
    string expression = "";
    while (getline(file, line)) {
        if (line.substr(0, 11) == "Expression:") {
            expression = trim(line.substr(12));
            break;
        }
    }
    file.close();

    if (expression.empty()) {
        cout << "Error: No expression found in file!" << endl;
        return;
    }

    cout << "\nLoaded Expression: " << expression << endl;
    BooleanExpression boolExpr(expression);
    boolExpr.explainOperators();
    TruthTable tt(boolExpr);
    tt.generate(cout);
}

// ========== Main Function ==========
// Entry point of the program
// Displays a menu and handles user input in a loop
// Supports entering expressions, loading from file, and exiting
int main() {
    cout << "*** BOOLEAN TRUTH TABLE SIMULATOR ***" << endl;

    while (true) {
        cout << "\n--- MENU ---" << endl;
        cout << "1. Enter new expression" << endl;
        cout << "2. Load expression from file" << endl;
        cout << "3. Exit" << endl;
        cout << "Choose (1/2/3): ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "\nEnter Boolean Expression (max 3 operators, variables A, B, C):" << endl;
            string expression;
            getline(cin, expression);

            // Check valid input
            string exprCheck = toUpper(expression);
            bool hasVariable = (exprCheck.find('A') != string::npos ||
                                exprCheck.find('B') != string::npos ||
                                exprCheck.find('C') != string::npos);
            if (!hasVariable) {
                cout << "Error: Must contain at least one variable (A, B, C)!" << endl;
                continue;
            }

            // Check operator count
            BooleanExpression tempExpr(expression);
            if (tempExpr.countOperators() > 3) {
                cout << "Error: Too many operators! Maximum 3 allowed." << endl;
                continue;
            }

            BooleanExpression boolExpr(expression);
            boolExpr.explainOperators();
            TruthTable tt(boolExpr);
            tt.generate(cout);

            cout << "\nWould you like to save this to a file? (Y/N): ";
            char save;
            cin >> save;
            if (save == 'Y' || save == 'y') {
                saveToFile(expression);
            }

        } else if (choice == 2) {
            loadFromFile();

        } else if (choice == 3) {
            cout << "Goodbye!" << endl;
            break;
        }
    }

    return 0;
}
