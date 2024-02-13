#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

#include <vector>
#include "node.h"

namespace ds {

using std::vector;

struct BplusTree {
    
    Node *root;
    BplusTree(const int& degree);

    auto insert(const int &key, const int &value) -> void;
    auto get(const int &key, Node* cur = nullptr) -> int;
    auto remove(const int &key) -> void;
    auto update(const int &key, const int &value) -> void;
    auto range(const int &lower_bound_k, const int &upper_bound_k) const -> vector<int>;
    auto getNode() const -> Node*;

    private:
        const int max_keys, min_keys, min_children, max_children;
        auto getLeafNodeForValue(const int &key, Node* cur) -> Node*;
        auto insertIntoLevel(const int &key, const int &value, Node* cur) -> bool;
        auto getIndexForKey(const int &key, Node* cur) const -> int;
};

};

#endif