#include <iostream>
#include <string>
using namespace std;


class BooleanOperator {
public:
    virtual bool evaluate(bool a, bool b) = 0;  // 纯虚函数
    virtual string explain() = 0;               // 解释运算符
    virtual string getName() = 0;               // 返回名字
    virtual ~BooleanOperator() {}
};

// AND 
class ANDOperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return a && b; }
    string explain() { return "True only if BOTH inputs are true"; }
    string getName() { return "AND"; }
};

// OR 
class OROperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return a || b; }
    string explain() { return "True if AT LEAST ONE input is true"; }
    string getName() { return "OR"; }
};

// NOT 
class NOTOperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return !a; }  // 只用第一个输入
    string explain() { return "Inverts the input"; }
    string getName() { return "NOT"; }
};

// XOR 运算符
class XOROperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return a != b; }
    string explain() { return "True only if inputs are DIFFERENT"; }
    string getName() { return "XOR"; }
};

// NAND 
class NANDOperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return !(a && b); }
    string explain() { return "Opposite of AND"; }
    string getName() { return "NAND"; }
};

// NOR 
class NOROperator : public BooleanOperator {
public:
    bool evaluate(bool a, bool b) { return !(a || b); }
    string explain() { return "Opposite of OR"; }
    string getName() { return "NOR"; }
};

int main() {
    cout << "*** BOOLEAN TRUTH TABLE SIMULATOR ***" << endl;
    
    
    BooleanOperator* ops[] = {
        new ANDOperator(),
        new OROperator(),
        new NOTOperator(),
        new XOROperator(),
        new NANDOperator(),
        new NOROperator()
    };
    
    for (auto op : ops) {
        cout << op->getName() << ": " << op->explain() << endl;
    }
    
    return 0;
}