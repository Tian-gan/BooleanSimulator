// NTO1064 Student Project
// Boolean Expression & Truth Table Simulator
// Author: Gantian
// Implementation file - Class method definitions

#include "BooleanOperator.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

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

// Check if entire expression is wrapped in matching brackets
bool isWrappedInBrackets(string expr) {
    if (expr.front() != '(' || expr.back() != ')') return false;
    int level = 0;
    for (int i = 0; i < (int)expr.length(); i++) {
        if (expr[i] == '(') level++;
        if (expr[i] == ')') level--;
        if (level == 0 && i < (int)expr.length() - 1) return false;
    }
    return true;
}

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

    int level = 0;
    int opPos = -1;
    string opFound = "";

    // First pass: look for OR / NOR at top level
    for (int i = 0; i < (int)expr.length(); i++) {
        if (expr[i] == '(') level++;
        if (expr[i] == ')') level--;
        if (level == 0) {
            if (expr.substr(i, 3) == "NOR") { opPos = i; opFound = "NOR"; break; }
            if (expr.substr(i, 2) == "OR")  { opPos = i; opFound = "OR";  break; }
        }
    }

    // Second pass: look for AND / NAND / XOR if no OR found
    if (opPos == -1) {
        level = 0;
        for (int i = 0; i < (int)expr.length(); i++) {
            if (expr[i] == '(') level++;
            if (expr[i] == ')') level--;
            if (level == 0) {
                if (expr.substr(i, 4) == "NAND") { opPos = i; opFound = "NAND"; break; }
                if (expr.substr(i, 3) == "AND")  { opPos = i; opFound = "AND";  break; }
                if (expr.substr(i, 3) == "XOR")  { opPos = i; opFound = "XOR";  break; }
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

// ========== BooleanExpression Methods ==========

BooleanExpression::BooleanExpression(string expr) {
    expression = expr;
}

string BooleanExpression::getExpression() {
    return expression;
}

bool BooleanExpression::evaluate(bool a, bool b, bool c) {
    return evaluateExpression(expression, a, b, c);
}

int BooleanExpression::countOperators() {
    string expr = toUpper(expression);
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

void BooleanExpression::explainOperators() {
    string expr = toUpper(expression);
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

void BooleanExpression::explainOperatorsToFile(ofstream& file) {
    string expr = toUpper(expression);
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
}

// ========== TruthTable Methods ==========

TruthTable::TruthTable(BooleanExpression expr) : boolExpr(expr) {}

vector<string> TruthTable::getSubExpressions(string expr) {
    vector<string> subs;
    string upper = expr;
    transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    // Find NOT sub-expressions
    size_t pos = 0;
    while ((pos = upper.find("NOT", pos)) != string::npos) {
        size_t start = pos;
        size_t end = pos + 4;
        if (upper[end] == '(') {
            int level = 0;
            for (size_t i = end; i < upper.length(); i++) {
                if (upper[i] == '(') level++;
                if (upper[i] == ')') level--;
                if (level == 0) { end = i + 1; break; }
            }
        } else {
            end = pos + 5;
        }
        subs.push_back(expr.substr(start, end - start));
        pos += 3;
    }

    // Find AND/NAND sub-expressions
    pos = 0;
    while ((pos = upper.find("AND", pos)) != string::npos) {
        int level = 0;
        int leftStart = pos - 2;
        while (leftStart > 0) {
            if (upper[leftStart] == ')') level++;
            if (upper[leftStart] == '(') level--;
            if (level == 0 && upper[leftStart] == ' ' && leftStart < (int)pos - 1) break;
            leftStart--;
        }
        size_t rightEnd = pos + 4;
        level = 0;
        if (rightEnd < upper.length() && upper[rightEnd] == '(') {
            for (size_t i = rightEnd; i < upper.length(); i++) {
                if (upper[i] == '(') level++;
                if (upper[i] == ')') level--;
                if (level == 0) { rightEnd = i + 1; break; }
            }
        } else {
            rightEnd = pos + 6;
        }
        string sub = expr.substr(leftStart, rightEnd - leftStart);
        while (!sub.empty() && (sub.front() == ' ' || sub.front() == '(')) sub = sub.substr(1);
        while (!sub.empty() && (sub.back() == ' ' || sub.back() == ')')) sub.pop_back();
        subs.push_back(sub);
        pos += 3;
    }

    // Find OR/NOR sub-expressions
    pos = 0;
    while ((pos = upper.find(" OR ", pos)) != string::npos) {
        subs.push_back(expr);
        pos += 4;
    }

    // Always add the full expression as last column
    subs.push_back(expr);

    // Remove duplicates
    vector<string> unique;
    for (auto& s : subs) {
        bool found = false;
        for (auto& u : unique) {
            if (u == s) { found = true; break; }
        }
        if (!found) unique.push_back(s);
    }
    return unique;
}

void TruthTable::generate(ostream& out) {
    string expr = boolExpr.getExpression();
    vector<string> subExprs = getSubExpressions(expr);

    // Print header
    out << "\nGenerating Truth Table..." << endl;
    out << "| A | B | C |";
    for (auto& s : subExprs) out << " " << s << " |";
    out << endl;

    // Print separator
    out << "|---|---|---|";
    for (auto& s : subExprs) {
        string sep(s.length() + 2, '-');
        out << sep << "|";
    }
    out << endl;

    // Print rows
    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {
            for (int c = 0; c <= 1; c++) {
                out << "| " << a << " | " << b << " | " << c << " |";
                for (auto& s : subExprs) {
                    bool res = evaluateExpression(s, a, b, c);
                    int pad = s.length() / 2;
                    string spaces(pad, ' ');
                    out << spaces << " " << res << " " << spaces << "|";
                }
                out << endl;
            }
        }
    }
}
