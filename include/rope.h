#ifndef ROPE_H
#define ROPE_H

#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

/**
 * @class RopeNode
 * @brief A node in the Rope data structure.
 */
class RopeNode
{
public:
    std::string data;                      ///< Leaf node data
    int weight;                            ///< Weight of the left subtree
    std::shared_ptr<RopeNode> left, right; ///< Pointers to left and right children

    /**
     * @brief Constructor for leaf nodes.
     * @param str The string data for the leaf node.
     */
    RopeNode(const std::string &str);

    /**
     * @brief Constructor for internal nodes.
     * @param leftChild Pointer to the left child node.
     * @param rightChild Pointer to the right child node.
     */
    RopeNode(std::shared_ptr<RopeNode> leftChild, std::shared_ptr<RopeNode> rightChild);
};

/**
 * @class Rope
 * @brief A Rope data structure for efficient string manipulation.
 */
class Rope
{
private:
    std::shared_ptr<RopeNode> root; ///< Root node of the Rope

    /**
     * @brief Helper for counting nodes.
     * @param node The node to count.
     * @return The number of nodes in the subtree rooted at the given node.
     */
    int countNodes(std::shared_ptr<RopeNode> node);

    /**
     * @brief Helper for printing the Rope.
     * @param node The node to print.
     */
    void print(std::shared_ptr<RopeNode> node);

    /**
     * @brief Helper for concatenating strings from the Rope.
     * @param node The node to concatenate.
     * @param result The concatenated string result.
     */
    void concatenateStrings(std::shared_ptr<RopeNode> node, std::string &result);

    // Add these private member declarations
    void split(std::shared_ptr<RopeNode> node, int index,std::shared_ptr<RopeNode> &leftPart, std::shared_ptr<RopeNode> &rightPart); 

    std::shared_ptr<RopeNode> concatenate(std::shared_ptr<RopeNode> left, std::shared_ptr<RopeNode> right);

    void collectLeaves(std::shared_ptr<RopeNode> node, std::vector<std::string> &leaves);

    std::vector<int> findAllOccurrences(const std::string &oldStr);

public:
    /**
     * @brief Constructor.
     * @param str The initial string for the Rope.
     */
    Rope(const std::string &str);

    /**
     * @brief Print the Rope.
     */
    void print();

    /**
     * @brief Get the total length of characters in the Rope.
     * @return The total length of characters.
     */
    int length();

    /**
     * @brief Get the total number of nodes in the Rope.
     * @return The total number of nodes.
     */
    int nodeCount();

    /**
     * @brief Replace all occurrences of a substring with another substring.
     * @return bool True if the substring was found and replaced, false otherwise.
     */
    void replace(const std::string &oldStr, const std::string &newStr);
};

#endif // ROPE_H