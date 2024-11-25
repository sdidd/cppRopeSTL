#include <iostream>
#include <memory>
#include <string>

class RopeNode
{
public:
    std::string data; // Leaf node data
    int weight;       // Weight of the left subtree
    std::shared_ptr<RopeNode> left, right;

    // Constructor for leaf nodes
    RopeNode(const std::string &str) : data(str), weight(str.length()), left(nullptr), right(nullptr) {}

    // Constructor for internal nodes
    RopeNode(std::shared_ptr<RopeNode> leftChild, std::shared_ptr<RopeNode> rightChild)
        : data(""), weight(leftChild ? leftChild->weight : 0), left(leftChild), right(rightChild) {}
};

class Rope
{
private:
    std::shared_ptr<RopeNode> root;

    // Helper for splitting at a given index
    void split(std::shared_ptr<RopeNode> node, int index, std::shared_ptr<RopeNode> &leftPart, std::shared_ptr<RopeNode> &rightPart)
    {
        if (!node)
        {
            leftPart = rightPart = nullptr;
            return;
        }

        if (!node->left && !node->right)
        { // Leaf node
            // Ensure the index is within bounds
            int length = node->data.length();
            if (index >= length)
            {
                leftPart = std::make_shared<RopeNode>(node->data); // All data goes to left
                rightPart = nullptr;                               // Right is null
            }
            else if (index <= 0)
            {
                leftPart = nullptr;                                 // Left is null
                rightPart = std::make_shared<RopeNode>(node->data); // All data goes to right
            }
            else
            {
                leftPart = std::make_shared<RopeNode>(node->data.substr(0, index));
                rightPart = std::make_shared<RopeNode>(node->data.substr(index));
            }
        }
        else
        {
            if (index < node->weight)
            { // Split in the left subtree
                std::shared_ptr<RopeNode> tempRight;
                split(node->left, index, leftPart, tempRight);
                rightPart = std::make_shared<RopeNode>(tempRight, node->right);
            }
            else
            { // Split in the right subtree
                std::shared_ptr<RopeNode> tempLeft;
                split(node->right, index - node->weight, tempLeft, rightPart);
                leftPart = std::make_shared<RopeNode>(node->left, tempLeft);
            }
        }
    }

    // Private helper for concatenating two RopeNode pointers
    std::shared_ptr<RopeNode> concatenate(std::shared_ptr<RopeNode> left, std::shared_ptr<RopeNode> right)
    {
        return std::make_shared<RopeNode>(left, right);
    }

    // Helper for extracting a substring
    void extract(std::shared_ptr<RopeNode> node, int start, int length, std::string &result)
    {
        if (!node || length <= 0)
            return;

        if (!node->left && !node->right)
        { // Leaf node
            result += node->data.substr(start, length);
        }
        else
        {
            if (start < node->weight)
            { // Part in the left subtree
                int leftLength = std::min(length, node->weight - start);
                extract(node->left, start, leftLength, result);
                extract(node->right, 0, length - leftLength, result);
            }
            else
            { // Entirely in the right subtree
                extract(node->right, start - node->weight, length, result);
            }
        }
    }

public:
    // Constructor
    Rope(const std::string &str) : root(std::make_shared<RopeNode>(str)) {}

    // Concatenate another rope
    void concatenate(Rope &other)
    {
        root = concatenate(root, other.root); // Pass the roots of the Rope objects
    }

    // Substring
    std::string substring(int start, int length)
    {
        std::string result;
        extract(root, start, length, result);
        return result;
    }

    // Insert
    void insert(int index, const std::string &str)
    {
        std::shared_ptr<RopeNode> leftPart, rightPart;
        split(root, index, leftPart, rightPart);
        root = concatenate(concatenate(leftPart, std::make_shared<RopeNode>(str)), rightPart);
    }

    // Delete the rope node
    void erase(int start, int length)
    {
        std::shared_ptr<RopeNode> leftpart, middlepart, rightpart;
        split(root, start, leftpart, middlepart);

        // Splitting again on other side
        std::shared_ptr<RopeNode> tempRightPart;
        split(middlepart, length, middlepart, rightpart);

        // Step 3: Concatenate the left and right parts, excluding the middle part
        root = concatenate(leftpart, rightpart);
    }
    void replace(int start, int length, const std::string &newStr)
    {
        // Validate inputs
        if (start < 0 || length < 0)
        {
            throw std::invalid_argument("start and length must be non-negative");
        }

        std::shared_ptr<RopeNode> left, middle, right;

        // Step 1: Split at start index
        split(root, start, left, middle);

        // Step 2: Split at (start + length) to isolate the substring to replace
        if (middle)
        {
            std::shared_ptr<RopeNode> tempRight;
            split(middle, length, middle, right);
        }
        else
        {
            right = nullptr; // If no middle part, right remains null
        }

        // Step 3: Create a new node for the replacement string
        std::shared_ptr<RopeNode> replacement = std::make_shared<RopeNode>(newStr);

        // Step 4: Concatenate the parts: left + replacement + right
        if (!left && !right)
        {
            root = replacement;
        }
        else if (!left)
        {
            root = concatenate(replacement, right);
        }
        else if (!right)
        {
            root = concatenate(left, replacement);
        }
        else
        {
            root = concatenate(concatenate(left, replacement), right);
        }
    }

    // Print the rope
    void print(std::shared_ptr<RopeNode> node)
    {
        if (!node)
            return;
        if (!node->left && !node->right)
        {
            std::cout << node->data;
        }
        else
        {
            print(node->left);
            print(node->right);
        }
    }

    void display()
    {
        print(root);
        std::cout << std::endl;
    }
};

int main()
{
    Rope rope("Hello, Beautiful World!");
    rope.display(); // Output: Hello, Beautiful World!

    try
    {
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
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
