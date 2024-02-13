#ifndef BPLUS_NODE_H
#define BPLUS_NODE_H

#include <vector>
#include <utility>

namespace ds {
    using std::vector;
    using std::pair;

    struct Node {
        bool leaf;
        vector<int> keys;
        vector<Node*> children;
        vector<pair<int,int>> values;
        auto is_leaf() const -> bool;
        auto splitInternalNode(const int childNodeIndex, const int maxKeys) -> void;
        auto splitLeafNode(const int childNodeIndex, const int maxKeys) -> void;
        Node(bool is_leaf, vector<int> keys = {}, vector<Node*> children = {}, vector<pair<int,int>> values = {});
        Node();
    };


    auto splitNode(Node *n) -> vector<Node*>;

}

#endif