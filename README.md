# Red-Black Tree Implementation in C++

This project provides a C++ implementation of a templated **Red-Black Tree** (RBT) data structure. Red-black trees are a
type of self-balancing binary search tree that guarantees O(log n) time complexity for insertion, deletion, and lookup
operations.

## Table of Contents

- [Features](#features)
- [API Documentation](#api-documentation)
- [Example Usage](#example-usage)
- [Installation](#installation)
- [Contributing](#contributing)
- [License](#license)

## Features

- **Templated Implementation:** Works with any data type.
- **Self-Balancing Insertion:** Ensures tree balance via color-flipping and rotations.
- **Flexible Comparisons:** Allows custom comparator functions for complex or non-arithmetic types.
- **Search Capability:** Locate nodes using custom or default comparison logic.
- **Traversal Methods:** Supports in-order and pre-order traversal.
- **String Representation:** Provides detailed metrics such as size, height, log2(size), and visual tree output.
- **Memory Management:** Automatically deallocates all nodes on destruction.

## API Documentation

### Namespace: `RBT`

### Class: `RedBlackTree<T>`

#### Template Parameters:

- `T`: The type of data stored in the tree.

#### Public Methods

##### Constructors:

- `RedBlackTree()`: Constructs an empty red-black tree.

##### Destructor:

- `~RedBlackTree()`: Frees all nodes and associated memory.

##### Tree Properties:

- `size_t get_ize() const`: Returns the number of nodes in the tree.
- `size_t get_height() const`: Returns the height of the tree.
- `bool is_empty() const`: Checks whether the tree is empty.

##### Insertion:

- `void push(T data, bool (*cmp)(T, T) = nullptr)`: Inserts a new element. If no comparator is given, uses standard
  arithmetic comparison.

##### Deletion:

- `void erase()`: Clears the entire tree and deallocates all memory.

##### Search:

- `Node* find(T data, bool (*cmp_equal)(T, T) = nullptr, bool (*cmp_lgreater)(T, T) = nullptr) const`: Finds a node with
  the given value, optionally using custom comparators.

##### Traversal:

- `std::vector<Node*> get_preorder() const`: Returns nodes in pre-order traversal.
- `std::vector<Node*> get_inorder() const`: Returns nodes in in-order traversal.

##### Visualization:

- `std::string to_str(unsigned int limit = 0, std::string (*fun_str)(T) = nullptr) const`: Returns a formatted string
  representation of the tree with optional traversal depth and custom data formatting.

## Example Usage

```cpp
#include "RBT.h"
#include <iostream>
#include <string>

std::string int_to_string(int data) {
    return std::to_string(data);
}

int main() {
    RBT::RedBlackTree<int> tree;

    tree.push(10);
    tree.push(20);
    tree.push(30);
    tree.push(15);

    std::cout << tree.to_str(10, int_to_string) << std::endl;

    auto node = tree.find(20);
    if (node) {
        std::cout << "Found: " << node->data << std::endl;
    } else {
        std::cout << "Not found." << std::endl;
    }

    tree.erase();
    return 0;
}
```

## Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/nonofare/RedBlackTree.git
   ```

2. **Include the `RBT.h` file in your project.**

3. **Compile your project with a compiler that supports C++17 or later.**

## Contributing

Please open an issue or submit a pull request for any improvements or bug fixes.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
