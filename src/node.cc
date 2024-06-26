#include "node.h"
#include <iostream>

namespace ds {

    Node::Node(): leaf(false), keys({}), children({}), values({}) {}

    auto Node::is_leaf() const -> bool {
        return leaf;
    }

    Node::Node(
        bool is_leaf,
        vector<int> keys,
        vector<Node*> children,
        vector<pair<int,int>> values
        ): leaf(is_leaf), keys(keys), children(children), values(values) {};

    auto Node::splitInternalNode(const int childNodeIndex, const int maxKeys) -> void {
        // std::cout <<"Splitting internal nodes" << std::endl;
        if (keys.size() <= maxKeys) return;
        // Check if the child node has more than required keys
        auto newNode = new Node();
        auto childNode = children.at(childNodeIndex);
        auto middleIdx = childNode->keys.size() / 2;
        // Split the child node into 2

        // Move the middle element to the current node
        keys.insert(begin(keys) + childNodeIndex, childNode->keys[middleIdx]);
        children.insert(begin(children) + childNodeIndex + 1, newNode); 

        // update the left pointer and right pointer for the inserted key
        for(int i = middleIdx + 1; i < childNode->keys.size(); i++) {
            // std::cout <<    childNode->keys[i] << std::endl;
            newNode->keys.push_back(childNode->keys[i]);
            if (childNode->children.size() > i)
                newNode->children.push_back(childNode->children[i]);
        }

        childNode->keys.erase(begin(childNode->keys) + middleIdx, end(childNode->keys));
        childNode->children.erase(begin(childNode->children) + middleIdx, end(childNode->children));
        // Add half elements into first node

        // Add other half apart from the first element to the other.
    }


    auto Node::splitLeafNode(const int childNodeIndex, const int maxKeys) -> void {

        auto newNode = new Node(true);
        auto childNode = children.at(childNodeIndex);

        if (childNode->keys.size() <= maxKeys) return;
        auto middleIdx = childNode->keys.size() / 2;

        keys.insert(begin(keys) + childNodeIndex, childNode->keys[middleIdx]);
        children.insert(begin(children) + childNodeIndex + 1, newNode); 

        for(int i = middleIdx; i < childNode->keys.size(); i++) {
            newNode->keys.push_back(childNode->keys[i]);
            newNode->values.push_back(childNode->values[i]);
        }

        childNode->keys.erase(begin(childNode->keys) + middleIdx, end(childNode->keys));
        childNode->values.erase(begin(childNode->values) + middleIdx, end(childNode->values));

    }
}