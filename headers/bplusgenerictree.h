
#ifndef BPLUS_GENERIC_TREE_H
#define BPLUS_GENERIC_TREE_H
#include <map>

namespace ds {
    using namespace std;

    template<typename Key, typename Value>
    class BTree {
        map<Key, Value> mp;

        BTree();
        auto insert(const Key& k, const Value& v) -> void
        {
            if (mp.at(key) != mp.)
            mp[k] = v;
        }
        auto get(const Key& K) -> Value;
        {
            if (mp.)
        }
        auto remove(const Key& K) -> Value;
    };
    
}



#endif
