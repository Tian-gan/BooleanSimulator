# Boolean Expression & Truth Table Simulator
**Author:** Gantian | **Module:** NTO1064

## How to compile and run
```
g++ main.cpp BooleanOperator.cpp -o simulator
```

## How to use
- **1** - Enter a new expression (e.g. `(A AND B) OR (NOT C)`)
- **2** - Load from a saved file
- **3** - Exit

Supports: AND, OR, NOT, XOR, NAND, NOR | Variables: A, B, C | Max 3 operators

## Class Structure
- **BooleanOperator** - abstract base class
- **ANDOperator, OROperator, NOTOperator, XOROperator, NANDOperator, NOROperator** - derived classes
- **BooleanExpression** - stores, evaluates and explains expressions
- **TruthTable** - generates truth table with intermediate columns

## File Structure

- **`BooleanOperator.h`** — Class declarations
- **`BooleanOperator.cpp`** — Method implementations
- **`main.cpp`** — User interaction, file saving and loading
  
## Design Decisions

- **Inheritance & Polymorphism**: Each operator (AND, OR, NOT etc.) is its own class that inherits from the abstract base class `BooleanOperator`. This makes it easy to add new operators in the future.

- **Encapsulation**: All operator logic is encapsulated inside the `BooleanExpression` class, including counting operators and explaining them. This keeps the main function clean and simple.

- **Recursive Evaluator**: The expression evaluator works recursively to handle nested brackets. Operator priority order is: NOT first, then AND/NAND/XOR, then OR/NOR.

- **Intermediate Columns**: The truth table shows intermediate steps (e.g. `NOT C` and `A AND B` before the final result), making it easier to understand how the expression is evaluated.

- **File Handling**: Results are saved using `ofstream` and loaded using `ifstream`, so users can save and revisit their expressions later.

- **Case-insensitive Input**: All input is converted to uppercase before processing, so users can type `and`, `AND`, or `And` and it will work the same way.
## Development Checklist
- Let the user input an expression - main.cpp - 06/03/2026
- Verify that the input is valid - main.cpp - 11/03/2026
- Detect the operators - main.cpp - 06/03/2026
- Explain to the user how each operator functions - main.cpp - 06/03/2026
- Generate truth table - main.cpp - 06/03/2026
  - inputs for table
  - outputs for table
  - layout of table
- Export results to text file - main.cpp - 11/03/2026
- Load a saved expression from text file - main.cpp - 11/03/2026
- Add BooleanExpression and TruthTable classes - main.cpp - 24/03/2026
- Add input validation and error handling - main.cpp - 11/03/2026
- Add intermediate columns to truth table - main.cpp - 25/03/2026
- Encapsulate operators into BooleanExpression class - main.cpp - 26/03/2026
