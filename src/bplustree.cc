#include "bplustree.h"

#include <iostream>

namespace ds {

    using std::cout;
    using std::endl;
    BplusTree::BplusTree(const int &degree):
    max_keys(2*degree - 1),
    min_keys(degree - 1),
    min_children(degree),
    max_children(2*degree)  {
        if (degree < 2) {
            cout << "Invalid degree" << endl;
        }
        
        root = new Node(true);
    }

    auto BplusTree::getNode() const -> Node* {
        return root;
    }
    auto BplusTree::getIndexForKey(const int &key, Node* cur) const -> int {
        int index = 0;
        while(index < cur->keys.size() && key > cur->keys[index]) index++;
        if (index < cur->keys.size() && cur->keys[index] == key) index++;

        return index;

    }
    auto BplusTree::getLeafNodeForValue(const int &key, Node* cur) -> Node* {
        if (cur->is_leaf()) {
            return cur;
        }
    
        auto index = getIndexForKey(key, cur);
        return getLeafNodeForValue(key, cur->children.at(index));
    }

    auto BplusTree::get(const int &key, Node* cur) -> int {
        if (cur == nullptr) {
            cur = root;
        }

        cur = getLeafNodeForValue(key, cur);

        for(auto &[k, v] : cur->values) {
            if (k == key) return v;
        }

        return -1;
    }

    auto BplusTree::insert(const int &key, const int &value) -> void {
        if (root->is_leaf() and root->keys.size() == 0) {
            root = new Node(true, {key}, {}, {{key, value}});
            return;
        }

        if (root->is_leaf() and root->keys.size() < max_keys) {
            root->keys.push_back(key);
            root->values.push_back({ key, value });
            return;
        }

        if (root->is_leaf() and  root->keys.size() == max_keys) {
            root->keys.push_back(key);
            root->values.push_back({key, value});
            auto pseudoRoot = new Node(false, {}, {root}, {});
            pseudoRoot->splitLeafNode(0, max_keys);

            std::cout << "Max Keys "<< max_keys<< " For pseudoRoot keys ";
            for(auto &key : pseudoRoot->keys) cout << key <<" ";
            cout << endl;
            cout << "For root keys ";
            for(auto &key: root->keys) cout << key << " ";
            cout<<endl; 
            root = pseudoRoot;
            return;
        }

        bool split_needed = insertIntoLevel(key, value, root);

        if (split_needed) {
            auto pseudoRoot = new Node();
            pseudoRoot->children.push_back(root);
            pseudoRoot->splitInternalNode(0, max_keys);
            root = pseudoRoot;
        }

    }

    auto BplusTree::insertIntoLevel(const int &key, const int &value, Node* cur) -> bool {
       if (cur->is_leaf()) {
            cur->keys.push_back(key);
            cur->values.push_back({key, value});

            if (cur->keys.size() > max_keys) return true;
            return false;
       } 

       auto index = getIndexForKey(key, cur);

       bool split_needed = insertIntoLevel(key, value, cur->children[index]);

       if (split_needed) {
            cur->children[index]->is_leaf() ? cur->splitLeafNode(index, max_keys) : cur->splitInternalNode(index, max_keys);
       }

       return cur->keys.size() > max_keys;
    }
}