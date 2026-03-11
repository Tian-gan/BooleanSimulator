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
class ANDOperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return a && b; }
    string explain() { return "True only if BOTH inputs are true"; }
    string getName() { return "AND"; }
};

class OROperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return a || b; }
    string explain() { return "True if AT LEAST ONE input is true"; }
    string getName() { return "OR"; }
};

class NOTOperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return !a; }
    string explain() { return "Inverts the input"; }
    string getName() { return "NOT"; }
};

class XOROperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return a != b; }
    string explain() { return "True only if inputs are DIFFERENT"; }
    string getName() { return "XOR"; }
};

class NANDOperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return !(a && b); }
    string explain() { return "Opposite of AND"; }
    string getName() { return "NAND"; }
};

class NOROperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return !(a || b); }
    string explain() { return "Opposite of OR"; }
    string getName() { return "NOR"; }
};

// ========== Helper Functions ==========

// Convert string to uppercase
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

// ========== Core Expression Evaluator ==========
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

// ========== Operator Explainer ==========
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
int main() {
    cout << "*** BOOLEAN TRUTH TABLE SIMULATOR ***" << endl;
    cout << "\nEnter Boolean Expression (max 3 operators, variables A, B, C):" << endl;

    string expression;
    getline(cin, expression);

    // Explain operators
    explainOperators(expression);

    generateTruthTable(expression, cout);
    

    return 0;
}