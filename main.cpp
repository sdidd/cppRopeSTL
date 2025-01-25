#include <iostream>
#include "rope.h"

int main()
{
    Rope rope("Hello, World! this is a test to try the multi node feature. this is of the rope data structure");
    rope.print(); // Should print the entire string
    std::cout << "String Count: " << rope.length() << "\nNode Count: " << rope.nodeCount() << std::endl;
    rope.replace("this ", "Universe ");
    rope.print(); // Should print the entire string with "World" replaced by "Universe"
    std::cout << "String Count: " << rope.length() << "\nNode Count: " << rope.nodeCount() << std::endl;

    return 0;
}