#include <gtest/gtest.h>
#include "bplustree.h"

using namespace ds;
TEST(BPLUSTREEINSERTION, INSERTING) {
    BplusTree b(2);
    b.insert(1,1);
    Node* root = b.getNode();

    EXPECT_EQ(root->is_leaf(), true);
    EXPECT_EQ(root->keys, (vector<int>{1}));
    EXPECT_EQ(root->children.size(), 0);
    EXPECT_EQ(root->values, (vector<pair<int,int>>{{1,1}}));

    b.insert(2,2);
    root = b.getNode();

    EXPECT_EQ(root->is_leaf(), true);
    EXPECT_EQ(root->keys, (vector<int>{1, 2}));
    EXPECT_EQ(root->children.size(), 0);
    EXPECT_EQ(root->values, (vector<pair<int,int>>{{1,1}, {2,2}}));
    // EXPECT_EQ(false, true);

    b.insert(3,3);
    root = b.getNode();

    EXPECT_EQ(root->is_leaf(), true);
    EXPECT_EQ(root->keys, (vector<int>{1,2,3}));
    EXPECT_EQ(root->children.size(), 0);
    EXPECT_EQ(root->values, (vector<pair<int,int>>{{1,1}, {2,2}, {3,3}}));

    b.insert(4,4);
    root = b.getNode();

    EXPECT_EQ(root->is_leaf(), false);
    EXPECT_EQ(root->keys, (vector<int>{3}));
    EXPECT_EQ(root->children.size(), 2);
    EXPECT_EQ(root->children.at(0)->keys, (vector<int>{1,2}));
    EXPECT_EQ(root->children.at(1)->keys, (vector<int>{3,4}));
    EXPECT_EQ(root->children.at(0)->is_leaf(), true);
    EXPECT_EQ(root->children.at(1)->is_leaf(), true);

    EXPECT_EQ(root->children.at(0)->values, (vector<pair<int,int>>{{1,1}, {2,2}}));
    EXPECT_EQ(root->children.at(1)->values, (vector<pair<int,int>>{{3,3}, {4,4}}));

}