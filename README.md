# FILE: /cppRopeSTL/cppRopeSTL/README.md

# cppRopeSTL

## Overview

`cppRopeSTL` is a C++ implementation of the Rope data structure, which is used for efficient string manipulation. This project demonstrates various operations such as concatenation, insertion, deletion, and substring extraction on ropes.

## Features

- Efficient string manipulation
- Concatenation of ropes
- Insertion of strings at specified indices
- Deletion of substrings
- Replacement of substrings
- Extraction of substrings
- Printing the rope
- Counting the number of nodes in the rope

## Classes

### RopeNode

A node in the Rope data structure.

#### Public Members

- `std::string data`: Leaf node data
- `int weight`: Weight of the left subtree
- `std::shared_ptr<RopeNode> left, right`: Pointers to left and right children

#### Constructors

- `RopeNode(const std::string &str)`: Constructor for leaf nodes
- `RopeNode(std::shared_ptr<RopeNode> leftChild, std::shared_ptr<RopeNode> rightChild)`: Constructor for internal nodes

### Rope

A Rope data structure for efficient string manipulation.

#### Public Methods

- `Rope(const std::string &str)`: Constructor
- `void concatenate(Rope &other)`: Concatenate another Rope
- `std::string substring(int start, int length)`: Get a substring of the Rope
- `void insert(int index, const std::string &str)`: Insert a string at a given index
- `void erase(int start, int length)`: Erase a part of the Rope
- `void replace(int start, int length, const std::string &newStr)`: Replace a part of the Rope with a new string
- `void print()`: Print the Rope
- `int length()`: Get the total length of characters in the Rope
- `int nodeCount()`: Get the total number of nodes in the Rope

## Usage

### Example

```cpp
#include <iostream>
#include "rope.h"

int main() {
    Rope rope("Hello, World!");
    rope.print(); // Should print "Hello, World!"

    rope.replace(7, 5, "Universe");
    rope.print(); // Should print "Hello, Universe!"

    rope.insert(5, ",");
    rope.print(); // Should print "Hello,, Universe!"

    rope.erase(5, 1);
    rope.print(); // Should print "Hello, Universe!"

    std::string result = rope.substring(7, 8);
    std::cout << "Substring: " << result << std::endl; // Should print "Universe"

    Rope rope2(" Welcome!");
    rope.concatenate(rope2);
    rope.print(); // Should print "Hello, Universe! Welcome!"

    std::cout << "Length: " << rope.length() << std::endl; // Should print 23
    std::cout << "Node count: " << rope.nodeCount() << std::endl; // Should print 3

    return 0;
}
```

## Building

To build the project, use the following commands:

```sh
mkdir build
cd build
cmake ..
make
```

## Running Tests

To run the tests, execute the compiled binary:

```sh
./main
```

## License

This project is licensed under the MIT License.