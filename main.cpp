#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

class RopeNode {
public:
    std::string data; // Leaf node data
    int weight;       // Weight of the left subtree
    std::shared_ptr<RopeNode> left, right;

    // Constructor for leaf nodes
    RopeNode(const std::string &str) : data(str), weight(str.length()), left(nullptr), right(nullptr) {}

    // Constructor for internal nodes
    RopeNode(std::shared_ptr<RopeNode> leftChild, std::shared_ptr<RopeNode> rightChild)
        : data(""), weight(leftChild ? leftChild->weight + (leftChild->right ? leftChild->right->weight : 0) : 0), left(leftChild), right(rightChild) {}
};

class Rope {
private:
    std::shared_ptr<RopeNode> root;

    // Helper for splitting at a given index
    void split(std::shared_ptr<RopeNode> node, int index, std::shared_ptr<RopeNode> &leftPart, std::shared_ptr<RopeNode> &rightPart) {
        if (!node) {
            leftPart = rightPart = nullptr;
            return;
        }

        if (!node->left && !node->right) { // Leaf node
            if (index >= node->data.length()) {
                leftPart = node;
                rightPart = nullptr;
            } else if (index <= 0) {
                leftPart = nullptr;
                rightPart = node;
            } else {
                leftPart = std::make_shared<RopeNode>(node->data.substr(0, index));
                rightPart = std::make_shared<RopeNode>(node->data.substr(index));
            }
        } else {
            if (index < node->weight) { // Split in the left subtree
                std::shared_ptr<RopeNode> tempRight;
                split(node->left, index, leftPart, tempRight);
                rightPart = std::make_shared<RopeNode>(tempRight, node->right);
            } else { // Split in the right subtree
                std::shared_ptr<RopeNode> tempLeft;
                split(node->right, index - node->weight, tempLeft, rightPart);
                leftPart = std::make_shared<RopeNode>(node->left, tempLeft);
            }
        }
    }

    // Private helper for concatenating two RopeNode pointers
    std::shared_ptr<RopeNode> concatenate(std::shared_ptr<RopeNode> left, std::shared_ptr<RopeNode> right) {
        return std::make_shared<RopeNode>(left, right);
    }

    // Helper for extracting a substring
    void extract(std::shared_ptr<RopeNode> node, int start, int length, std::string &result) {
        if (!node || length <= 0) return;

        if (!node->left && !node->right) { // Leaf node
            result += node->data.substr(start, length);
        } else {
            if (start < node->weight) { // Part in the left subtree
                int leftLength = std::min(length, node->weight - start);
                extract(node->left, start, leftLength, result);
                extract(node->right, 0, length - leftLength, result);
            } else { // Entirely in the right subtree
                extract(node->right, start - node->weight, length, result);
            }
        }
    }

public:
    // Constructor
    Rope(const std::string &str) : root(std::make_shared<RopeNode>(str)) {}

    // Concatenate another rope
    void concatenate(Rope &other) {
        root = concatenate(root, other.root);
    }

    // Substring
    std::string substring(int start, int length) {
        if (start < 0 || length < 0) {
            throw std::invalid_argument("start and length must be non-negative");
        }

        std::string result;
        extract(root, start, length, result);
        return result;
    }

    // Insert
    void insert(int index, const std::string &str) {
        if (index < 0) {
            throw std::invalid_argument("index must be non-negative");
        }

        std::shared_ptr<RopeNode> leftPart, rightPart;
        split(root, index, leftPart, rightPart);
        root = concatenate(concatenate(leftPart, std::make_shared<RopeNode>(str)), rightPart);
    }

    // Delete the rope node
    void erase(int start, int length) {
        if (start < 0 || length < 0) {
            throw std::invalid_argument("start and length must be non-negative");
        }

        std::shared_ptr<RopeNode> leftPart, middlePart, rightPart;
        split(root, start, leftPart, middlePart);
        split(middlePart, length, middlePart, rightPart);
        root = concatenate(leftPart, rightPart);
    }

    void replace(int start, int length, const std::string &newStr) {
        if (start < 0 || length < 0) {
            throw std::invalid_argument("start and length must be non-negative");
        }

        std::shared_ptr<RopeNode> left, middle, right;
        split(root, start, left, middle);
        split(middle, length, middle, right);

        std::shared_ptr<RopeNode> replacement = std::make_shared<RopeNode>(newStr);
        root = concatenate(concatenate(left, replacement), right);
    }

    // Print the rope
    void print(std::shared_ptr<RopeNode> node) {
        if (!node) return;
        if (!node->left && !node->right) {
            std::cout << node->data;
        } else {
            print(node->left);
            print(node->right);
        }
    }

    void display() {
        print(root);
        std::cout << std::endl;
    }
};

int main() {
    Rope rope("Hello, Beautiful World!");
    rope.display(); // Output: Hello, Beautiful World!

    try {
        // Replace "Beautiful" with "Wonderful"
        rope.replace(7, 9, "Wonderful");
        rope.display(); // Output: Hello, Wonderful World!

        // Replace "Wonderful" with "Amazing"
        rope.replace(7, 9, "Amazing");
        rope.display(); // Output: Hello, Amazing World!

        // Replace "World" with "Universe"
        rope.replace(14, 5, "Universe");
        rope.display(); // Output: Hello, Amazing Universe!

        // Invalid replace (start out of range)
        rope.replace(50, 5, "Error");
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
