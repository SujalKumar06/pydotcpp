# pydotcpp

**`pydotcpp`** is a tree-walking Python interpreter written completely in C++.

It works by scanning and parsing Python source code to construct an Abstract Syntax Tree (AST), which it then directly evaluates using a tree-walking execution process.

## Features

Currently, `pydotcpp` supports a core subset of Python's syntax and behavior:

* **Variables and Assignment:** Standard dynamic variable bindings.
* **Arithmetic and Logical Operations:** Standard math operations (`+`, `-`, `*`, `/`, `//`, `%`, `**`), logical operations (`or`, `and`, `not`) and comparison operations (`==`, `!=`, `>`, `<`, `<=`, `>=`). 
* **Conditionals:** `if`, `elif` and `else` branching mechanisms.
* **Loops:** `while` loops for iteration.
* **Control Flow:** `break` and `continue` statements to control loop execution.
* **Functions:** Definition of custom functions with parameters, scoped variables, and `return` statements.
* **Data Structures:** Support for Python `Lists`.

### Example Supported Code

```python
def sum_even_numbers(limit):
    total = 0
    i = 0
    while i < limit:
        if i % 2 == 0:
            total = total + i
        i = i + 1
    return total

my_list = [1, 2, sum_even_numbers(10)]
```

---

## Prerequisites

To build and run `pydotcpp`, you will need the following installed on your system:
* A C++ compiler (e.g., `g++` or `clang++`)
* `make` (GNU Make)
* `clang-format` (Optional, for code formatting during development)

## Building and Running

The project includes a `Makefile` to simplify building, running, and testing the interpreter. All compiled objects and executables are safely placed in the `build/` directory.

* **Build the interpreter:**
  ```bash
  make
  ```
  *This compiles the source code and creates the executable at `build/pydotcpp`.*

* **Build and run immediately:**
  ```bash
  make run
  ```

* **Clean build artifacts:**
  ```bash
  make clean
  ```
  *This removes the `build/` directory and all compiled `.o` and `.d` files.*

## Testing

The project is broken down into modular tests that verify the different stages of the interpreter pipeline. You can run these tests individually:

* **Test the Lexer:** (Validates token generation)
  ```bash
  make test-lexer
  ```
* **Test the Parser:** (Validates Abstract Syntax Tree generation)
  ```bash
  make test-parser
  ```
* **Test the Evaluator:** (Validates the tree-walking execution logic)
  ```bash
  make test-evaluator
  ```

## Development and Formatting

If you are contributing to or modifying the C++ source code, you can use the built-in formatting targets to maintain a consistent code style:

* **Auto-format code:**
  ```bash
  make format
  ```
  *Runs `clang-format -i` across all `.cpp` and `.h` files in the `src/` directory.*

* **Check formatting (CI/Dry-run):**
  ```bash
  make check-format
  ```
  *Verifies that the code complies with the formatting rules without making actual changes. Useful for continuous integration pipelines.*

## Project Structure

* `src/` - Core interpreter implementation (Lexer, Parser, Evaluator, AST definitions, Runtime definitions).
* `tests/` - C++ test files for the interpreter's discrete components.
* `entrypoint.cpp` - The main entry point for the compiled executable.
* `Makefile` - Build, test, and format configuration.
