// NTO1064 Student Project
// Boolean Expression & Truth Table Simulator
// Author: Gantian
// Header file - Class declarations for Boolean operators,
// BooleanExpression, and TruthTable

#ifndef BOOLEANOPERATOR_H
#define BOOLEANOPERATOR_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

// Forward declaration
bool evaluateExpression(string expr, bool a, bool b, bool c);

// ========== Abstract Base Class ==========
class BooleanOperator {
public:
    virtual bool evaluate(bool a, bool b) = 0; // Pure virtual
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
string toUpper(string s);
string trim(string s);
bool isWrappedInBrackets(string expr);

// ========== BooleanExpression Class ==========
class BooleanExpression {
private:
    string expression;

public:
    BooleanExpression(string expr);
    string getExpression();
    bool evaluate(bool a, bool b, bool c);
    int countOperators();
    void explainOperators();
    void explainOperatorsToFile(ofstream& file);
};

// ========== TruthTable Class ==========
class TruthTable {
private:
    BooleanExpression boolExpr;

public:
    TruthTable(BooleanExpression expr);
    vector<string> getSubExpressions(string expr);
    void generate(ostream& out);
};

#endif
