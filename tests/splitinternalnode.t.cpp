#include <gtest/gtest.h>
#include "node.h"

using namespace ds;
class SetupNodes: public ::testing::Test {
    protected:
    Node* curWithLeaf;
    Node* curWithEmptyRoot;
    Node* curInternal;
    void setupEmptyRoot() {
        auto Node_1 = new Node(true, {1,2,3, 4}, {}, {{1,1}, {2,2}, {3,3}, {4,4}});
        curWithEmptyRoot = new Node(false, {}, {Node_1}, {});
    }
    void setupleafNodeSplit() {
        auto Node_1 = new Node(true, {1,2}, {}, {{1,100}, {2, 200}});
        auto Node_2 = new Node(true, {3,4}, {}, {{3,300}, {4,400}});
        auto Node_3 = new Node(true, {5,6,7}, {}, {{5,500}, {6,600}, {7,700}});
        auto Node_4 = new Node(true, {11,12}, {}, {{11,1100}, {12,1200}});
        
        curWithLeaf = new Node(false, {3,5,11}, {Node_1, Node_2, Node_3, Node_4});
    }

    void setupInternalNodeSplit() {
        auto Node_1 = new Node(false, {1,2}, {}, {});
        auto Node_2 = new Node(false, {3,4}, {}, {});
        auto Node_3 = new Node(false, {5,6,7}, {}, {});
        auto Node_4 = new Node(false, {11,12}, {}, {});

        curInternal = new Node(false, {3,5,11}, {Node_1, Node_2, Node_3, Node_4});
    }

    void SetUp() override {
        setupleafNodeSplit();
        setupInternalNodeSplit();
        setupEmptyRoot();
    }

    void TearDown() override {
        for(auto &node: curWithLeaf->children) delete node;
        delete curWithLeaf;

        for(auto &node: curInternal->children) delete node;
        delete curInternal;

        for(auto &node : curWithEmptyRoot->children) delete node;
        delete curWithEmptyRoot;
    }
};

TEST_F(SetupNodes, SPLITLEAFNODES) {
    curWithLeaf->splitLeafNode(2, 2);
    EXPECT_EQ(curWithLeaf->keys.size(), 4);
    EXPECT_EQ(curWithLeaf->keys, (vector<int>{3,5,6,11}));
    EXPECT_EQ(curWithLeaf->children.size(), 5);
    EXPECT_EQ(curWithLeaf->children.at(0)->keys, (vector<int>{1,2}));
    EXPECT_EQ(curWithLeaf->children.at(1)->keys, (vector<int>{3,4}));
    EXPECT_EQ(curWithLeaf->children.at(2)->keys, (vector<int>{5}));
    EXPECT_EQ(curWithLeaf->children.at(3)->keys, (vector<int>{6,7}));
    EXPECT_EQ(curWithLeaf->children.at(4)->keys, (vector<int>{11,12}));

    EXPECT_EQ(curWithLeaf->children.at(0)->values, (vector<pair<int,int>>{{1,100}, {2,200}}));
    EXPECT_EQ(curWithLeaf->children.at(1)->values, (vector<pair<int,int>>{{3,300},{4,400}}));
    EXPECT_EQ(curWithLeaf->children.at(2)->values, (vector<pair<int,int>>{{5,500}}));
    EXPECT_EQ(curWithLeaf->children.at(3)->values, (vector<pair<int,int>>{{6,600}, {7, 700}}));
    EXPECT_EQ(curWithLeaf->children.at(4)->values, (vector<pair<int,int>>{{11,1100}, {12, 1200}}));
}

TEST_F(SetupNodes, SPLITINTERNAL) {
    curInternal->splitInternalNode(2, 2);

    EXPECT_EQ(curInternal->keys.size(), 4);
    EXPECT_EQ(curInternal->keys, (vector<int>{3,5,6,11}));
    EXPECT_EQ(curInternal->children.size(), 5);
    EXPECT_EQ(curInternal->children.at(0)->keys, (vector<int>{1,2}));
    EXPECT_EQ(curInternal->children.at(1)->keys, (vector<int>{3,4}));
    EXPECT_EQ(curInternal->children.at(2)->keys, (vector<int>{5}));
    EXPECT_EQ(curInternal->children.at(3)->keys, (vector<int>{7}));
    EXPECT_EQ(curInternal->children.at(4)->keys, (vector<int>{11,12}));
}

TEST_F(SetupNodes, SPLITEMPTYROOT) {
    curWithEmptyRoot->splitLeafNode(0, 3);

    EXPECT_EQ(curWithEmptyRoot->keys, (vector<int>{3}));
    EXPECT_EQ(curWithEmptyRoot->is_leaf(), false);
    EXPECT_EQ(curWithEmptyRoot->children.size(), 2);
    EXPECT_EQ(curWithEmptyRoot->children.at(0)->keys, (vector<int>{1,2}));
    EXPECT_EQ(curWithEmptyRoot->children.at(1)->keys, (vector<int>{3,4}));
}