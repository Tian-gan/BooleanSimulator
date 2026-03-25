#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

// ========== Abstract Base Class ==========
class BooleanOperator {
public:
    virtual bool evaluate(bool a, bool b) = 0;
    virtual string explain() = 0;
    virtual string getName() = 0;
    virtual ~BooleanOperator() {}
};

// ========== Derived Classes ==========
// AND operator - returns true only if both inputs are true
class ANDOperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return a && b; }
    string explain() { return "True only if BOTH inputs are true"; }
    string getName() { return "AND"; }
};
// OR operator - returns true if at least one input is true
class OROperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return a || b; }
    string explain() { return "True if AT LEAST ONE input is true"; }
    string getName() { return "OR"; }
};
// NOT operator - inverts the input value
class NOTOperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return !a; }
    string explain() { return "Inverts the input"; }
    string getName() { return "NOT"; }
};
// XOR operator - returns true only if inputs are different
class XOROperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return a != b; }
    string explain() { return "True only if inputs are DIFFERENT"; }
    string getName() { return "XOR"; }
};
// NAND operator - opposite of AND
class NANDOperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return !(a && b); }
    string explain() { return "Opposite of AND"; }
    string getName() { return "NAND"; }
};
// NOR operator - opposite of OR
class NOROperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return !(a || b); }
    string explain() { return "Opposite of OR"; }
    string getName() { return "NOR"; }
};

// ========== Helper Functions ==========

// Convert string to uppercase
// Used to make input case-insensitive (e.g. "and" becomes "AND")
string toUpper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

// Trim spaces from both ends
string trim(string s) {
    int start = 0;
    int end = s.length() - 1;
    while (start <= end && s[start] == ' ') start++;
    while (end >= start && s[end] == ' ') end--;
    return s.substr(start, end - start + 1);
}

// Forward declaration
bool evaluateExpression(string expr, bool a, bool b, bool c);

// Check if entire expression is wrapped in matching brackets
bool isWrappedInBrackets(string expr) {
    if (expr.front() != '(' || expr.back() != ')') return false;
    int level = 0;
    for (int i = 0; i < (int)expr.length(); i++) {
        if (expr[i] == '(') level++;
        if (expr[i] == ')') level--;
        // If level hits 0 before the end, brackets don't wrap everything
        if (level == 0 && i < (int)expr.length() - 1) return false;
    }
    return true;
}
// ========== BooleanExpression Class ==========
// This class stores and manages a Boolean expression string.
// It provides an interface to evaluate the expression
// with given values for variables A, B and C.
class BooleanExpression {
private:
    string expression;  // Stores the expression string

public:
    // Constructor - takes the expression as input
    BooleanExpression(string expr) {
        expression = expr;
    }

    // Returns the stored expression
    string getExpression() {
        return expression;
    }

    // Evaluates the stored expression with given variable values
    // Returns true or false based on the result
    bool evaluate(bool a, bool b, bool c) {
        return evaluateExpression(expression, a, b, c);
    }
};

// ========== TruthTable Class ==========
class TruthTable {
private:
    BooleanExpression boolExpr;  // Stores the expression object

public:
    // Constructor - takes a BooleanExpression object
    TruthTable(BooleanExpression expr) : boolExpr(expr) {}

    // Generates and displays the truth table
    void generate(ostream& out) {
        out << "\nGenerating Truth Table..." << endl;
        out << "| A | B | C | Result |" << endl;
        out << "|---|---|---|--------|" << endl;

        for (int a = 0; a <= 1; a++) {
            for (int b = 0; b <= 1; b++) {
                for (int c = 0; c <= 1; c++) {
                    bool result = boolExpr.evaluate(a, b, c);
                    out << "| " << a << " | " << b << " | " << c
                        << " |   " << result << "    |" << endl;
                }
            }
        }
    }
};
// ========== Core Expression Evaluator ==========
// Recursively evaluates a Boolean expression
// Parameters: expr = the expression string, a/b/c = variable values
// Returns the boolean result of the expression
bool evaluateExpression(string expr, bool a, bool b, bool c) {
    expr = trim(toUpper(expr));

    // Remove outer brackets
    while (isWrappedInBrackets(expr)) {
        expr = trim(expr.substr(1, expr.length() - 2));
    }

    // Base case: single variable
    if (expr == "A") return a;
    if (expr == "B") return b;
    if (expr == "C") return c;

    // Handle NOT operator
    if (expr.substr(0, 3) == "NOT") {
        string inner = trim(expr.substr(3));
        return !evaluateExpression(inner, a, b, c);
    }

    // Find operator at the TOP level (not inside brackets)
    // Scan left to right, find OR first (lowest priority), then AND
    int level = 0;
    int opPos = -1;
    string opFound = "";

    // First pass: look for OR / NOR at top level
    for (int i = 0; i < (int)expr.length(); i++) {
        if (expr[i] == '(') level++;
        if (expr[i] == ')') level--;

        if (level == 0) {
            if (expr.substr(i, 3) == "NOR") {
                opPos = i; opFound = "NOR"; break;
            }
            if (expr.substr(i, 2) == "OR") {
                opPos = i; opFound = "OR"; break;
            }
        }
    }

    // Second pass: look for AND / NAND / XOR if no OR found
    if (opPos == -1) {
        level = 0;
        for (int i = 0; i < (int)expr.length(); i++) {
            if (expr[i] == '(') level++;
            if (expr[i] == ')') level--;

            if (level == 0) {
                if (expr.substr(i, 4) == "NAND") {
                    opPos = i; opFound = "NAND"; break;
                }
                if (expr.substr(i, 3) == "AND") {
                    opPos = i; opFound = "AND"; break;
                }
                if (expr.substr(i, 3) == "XOR") {
                    opPos = i; opFound = "XOR"; break;
                }
            }
        }
    }

    // Split left and right, evaluate recursively
    if (opPos != -1) {
        string left  = trim(expr.substr(0, opPos));
        string right = trim(expr.substr(opPos + opFound.length()));
        bool L = evaluateExpression(left,  a, b, c);
        bool R = evaluateExpression(right, a, b, c);

        if (opFound == "AND")  return L && R;
        if (opFound == "OR")   return L || R;
        if (opFound == "XOR")  return L != R;
        if (opFound == "NAND") return !(L && R);
        if (opFound == "NOR")  return !(L || R);
    }

    return false;
}
// ========== Check operator count ==========
// Returns the number of operators found in the expression
int countOperators(string expr) {
    expr = toUpper(expr);
    int count = 0;
    int i = 0;
    while (i < (int)expr.length()) {
        if (expr.substr(i, 4) == "NAND") { count++; i += 4; }
        else if (expr.substr(i, 3) == "NOR") { count++; i += 3; }
        else if (expr.substr(i, 3) == "XOR") { count++; i += 3; }
        else if (expr.substr(i, 3) == "NOT") { count++; i += 3; }
        else if (expr.substr(i, 3) == "AND") { count++; i += 3; }
        else if (expr.substr(i, 2) == "OR")  { count++; i += 2; }
        else i++;
    }
    return count;
}
// ========== Operator Explainer ==========
// Detects which operators are used in the expression
// Prints a clear explanation for each operator found
void explainOperators(string expr) {
    expr = toUpper(expr);
    cout << "\nOperators Detected and Explained:" << endl;

    if (expr.find("NAND") != string::npos)
        cout << "- NAND: Opposite of AND" << endl;
    else if (expr.find("AND") != string::npos)
        cout << "- AND: True only if BOTH inputs are true" << endl;

    if (expr.find("NOR") != string::npos)
        cout << "- NOR: Opposite of OR" << endl;
    else if (expr.find("OR") != string::npos)
        cout << "- OR: True if AT LEAST ONE input is true" << endl;

    if (expr.find("NOT") != string::npos)
        cout << "- NOT: Inverts the input" << endl;

    if (expr.find("XOR") != string::npos)
        cout << "- XOR: True only if inputs are DIFFERENT" << endl;
}

// ========== Main Function ==========
// ========== Truth Table Generator ==========
void generateTruthTable(string expression, ostream& out) {
    out << "\nGenerating Truth Table..." << endl;
    out << "| A | B | C | Result |" << endl;
    out << "|---|---|---|--------|" << endl;

    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {
            for (int c = 0; c <= 1; c++) {
                bool result = evaluateExpression(expression, a, b, c);
                out << "| " << a << " | " << b << " | " << c
                    << " |   " << result << "    |" << endl;
            }
        }
    }
}
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
    string expr = expression;
    transform(expr.begin(), expr.end(), expr.begin(), ::toupper);
    file << "\nOperators Detected and Explained:" << endl;
    if (expr.find("NAND") != string::npos)
        file << "- NAND: Opposite of AND" << endl;
    else if (expr.find("AND") != string::npos)
        file << "- AND: True only if BOTH inputs are true" << endl;
    if (expr.find("NOR") != string::npos)
        file << "- NOR: Opposite of OR" << endl;
    else if (expr.find("OR") != string::npos)
        file << "- OR: True if AT LEAST ONE input is true" << endl;
    if (expr.find("NOT") != string::npos)
        file << "- NOT: Inverts the input" << endl;
    if (expr.find("XOR") != string::npos)
        file << "- XOR: True only if inputs are DIFFERENT" << endl;

    // Write truth table to file
    generateTruthTable(expression, file);

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
            expression = line.substr(12);
            break;
        }
    }
    file.close();

    if (expression.empty()) {
        cout << "Error: No expression found in file!" << endl;
        return;
    }

    cout << "\nLoaded Expression: " << expression << endl;
    explainOperators(expression);
    BooleanExpression boolExpr(expression);
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
            if (countOperators(expression) > 3) {
                cout << "Error: Too many operators! Maximum 3 allowed." << endl;
                continue;
            }

            explainOperators(expression);
            BooleanExpression boolExpr(expression);
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