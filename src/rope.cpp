// rope.cpp
#include "rope.h"
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <vector>

RopeNode::RopeNode(const std::string &str) : data(str), weight(str.length()), left(nullptr), right(nullptr) {}

RopeNode::RopeNode(std::shared_ptr<RopeNode> leftChild, std::shared_ptr<RopeNode> rightChild)
    : data(""), weight(0), left(leftChild), right(rightChild) {
    weight += leftChild ? leftChild->weight : 0;
    weight += rightChild ? rightChild->weight : 0;
}

// Constructor for the Rope class
Rope::Rope(const std::string &str) {
    const int chunkSize = 10; // Define the size of each chunk
    std::vector<std::shared_ptr<RopeNode>> nodes;
    for (size_t i = 0; i < str.length(); i += chunkSize) {
        nodes.push_back(std::make_shared<RopeNode>(str.substr(i, chunkSize)));
    }
    while (nodes.size() > 1) {
        std::vector<std::shared_ptr<RopeNode>> newNodes;
        for (size_t i = 0; i < nodes.size(); i += 2) {
            if (i + 1 < nodes.size()) {
                newNodes.push_back(std::make_shared<RopeNode>(nodes[i], nodes[i + 1]));
            } else {
                newNodes.push_back(nodes[i]);
            }
        }
        nodes = newNodes;
    }
    root = nodes[0];
}

//Return the length of the rope
int Rope::length() {
    return root ? root->weight : 0;
}

int Rope::countNodes(std::shared_ptr<RopeNode> node) {
    if (!node) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

int Rope::nodeCount() {
    return countNodes(root);
}

void Rope::print(std::shared_ptr<RopeNode> node) {
    if (!node) return;
    if (!node->left && !node->right) {
        std::cout << node->data;
    } else {
        print(node->left);
        print(node->right);
    }
}

// Print the rope
void Rope::print() {
    if (!root){
        std::cout << "Empty Rope" << std::endl;
        return;
    } 
    print(root);
    std::cout << std::endl;
}

// Helper function to concatenate strings from the Rope
void Rope::concatenateStrings(std::shared_ptr<RopeNode> node, std::string &result) {
    if (!node) return;
    if (!node->left && !node->right) {
        result += node->data;
    } else {
        concatenateStrings(node->left, result);
        concatenateStrings(node->right, result);
    }
}

// Add these implementations before replace()
void Rope::split(std::shared_ptr<RopeNode> node, int index, 
                std::shared_ptr<RopeNode>& leftPart, std::shared_ptr<RopeNode>& rightPart) {
    if (!node) {
        leftPart = rightPart = nullptr;
        return;
    }

    if (!node->left && !node->right) { // Leaf node
        if (index >= node->data.length()) {
            leftPart = node;
            rightPart = std::make_shared<RopeNode>("");
        } else if (index <= 0) {
            leftPart = std::make_shared<RopeNode>("");
            rightPart = node;
        } else {
            leftPart = std::make_shared<RopeNode>(node->data.substr(0, index));
            rightPart = std::make_shared<RopeNode>(node->data.substr(index));
        }
    } else {
        int leftWeight = node->left ? node->left->weight : 0;
        if (index < leftWeight) { // Split left subtree
            std::shared_ptr<RopeNode> tempRight;
            split(node->left, index, leftPart, tempRight);
            rightPart = concatenate(tempRight, node->right);
        } else { // Split right subtree
            std::shared_ptr<RopeNode> tempLeft;
            split(node->right, index - leftWeight, tempLeft, rightPart);
            leftPart = concatenate(node->left, tempLeft);
        }
    }
}

std::shared_ptr<RopeNode> Rope::concatenate(std::shared_ptr<RopeNode> left, std::shared_ptr<RopeNode> right) {
    if (!left || left->weight == 0) return right;
    if (!right || right->weight == 0) return left;
    return std::make_shared<RopeNode>(left, right);
}

void Rope::collectLeaves(std::shared_ptr<RopeNode> node, std::vector<std::string>& leaves) {
    if (!node) return;
    if (!node->left && !node->right) {
        leaves.push_back(node->data);
    } else {
        collectLeaves(node->left, leaves);
        collectLeaves(node->right, leaves);
    }
}

std::vector<int> Rope::findAllOccurrences(const std::string& oldStr) {
    std::vector<int> occurrences;
    std::vector<std::string> leaves;
    collectLeaves(root, leaves);
    
    int currentPos = 0;
    std::string buffer;
    
    for (const auto& leaf : leaves) {
        for (char c : leaf) {
            buffer += c;
            currentPos++;
            
            // Maintain sliding window
            if (buffer.size() > oldStr.size()) {
                buffer.erase(buffer.begin());
            }
            
            if (buffer.size() == oldStr.size() && buffer == oldStr) {
                int start = currentPos - oldStr.size();
                occurrences.push_back(start);
            }
        }
    }
    
    return occurrences;
}

// Replace this existing implementation
void Rope::replace(const std::string &oldStr, const std::string &newStr) {
    if (oldStr.empty() || oldStr == newStr || !root) return;

    std::vector<int> occurrences = findAllOccurrences(oldStr);
    
    // Process from last to first to maintain correct indices
    for (auto it = occurrences.rbegin(); it != occurrences.rend(); ++it) {
        int start = *it;
        int end = start + oldStr.length();

        std::shared_ptr<RopeNode> leftPart, rightPart, middlePart;
        
        // Split at the end of the old string
        split(root, end, middlePart, rightPart);
        // Split at the start of the old string
        split(middlePart, start, leftPart, middlePart);
        
        // Create new node with replacement
        auto newNode = std::make_shared<RopeNode>(newStr);
        
        // Rebuild the rope
        root = concatenate(concatenate(leftPart, newNode), rightPart);
    }
}