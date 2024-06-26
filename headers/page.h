#ifndef PAGE_H
#define PAGE_H
#include <cstddef>
#include <optional>
#include <cstring>
#include <cassert>
#include <iostream>

typedef void * LOCATION;
enum class PageType
{
    KEY_VALUE = 0,
    KEY_ONLY = 1,
    NONE = 2
};

struct PageHeader
{
    int pageid;
    PageType type;
    size_t pd_upper;
    size_t pd_lower;
    size_t lp_start;
    int total;
};
struct LinePointer
{
    size_t offset;
    bool filled;
};

namespace utils
{

    template<typename Key, typename Value>
    auto getStorageSize(const Key& k, const Value& v) -> const size_t
    {
        return 2 * sizeof(size_t) + sizeof(Key) + sizeof(Value);
    }
}

template<typename Key, typename Value>
struct Page
{
    LOCATION base;
    PageHeader header;
    Page(LOCATION l, const size_t& size, const PageType type)
    {
        assert(size > sizeof(header));
        header.pageid = 0;
        header.pd_lower = sizeof(header);
        header.lp_start = sizeof(header);
        header.pd_upper = size;
        header.type = type;
        header.total = 0;
        base = l;
    }
    auto insertData(const Key& k, const Value& v) -> bool
    {
        // Calculate payload size
        auto payload_size = utils::getStorageSize(k, v);
        const auto KeySize = sizeof(Key);
        const auto ValueSize = sizeof(Value);
        // Check if it can be stored.
        if (header.pd_lower + payload_size + sizeof(size_t) > header.pd_upper) return false;

        // Store the offset in a new cell pointer
        auto offset = header.pd_upper - payload_size;
        LinePointer lp{offset, true};
        memcpy(base + header.pd_lower, &lp, sizeof(LinePointer));
        header.pd_lower += sizeof(LinePointer);
        // Start storing the data in the cell data
        memcpy(base + offset, &KeySize, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(base + offset, &ValueSize, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(base + offset, &k, KeySize);
        offset += KeySize;
        memcpy(base + offset, &v, ValueSize);
        header.pd_upper -= payload_size;
        return true;
    }

    auto find(const Key& k) -> std::optional<Value>
    {
        for (auto itr = header.lp_start; itr < header.pd_lower; itr += sizeof(LinePointer))
        {
            auto lp = reinterpret_cast<LinePointer*>(base + itr);
            if (!lp->filled) continue;
            auto cur_key = reinterpret_cast<Key*>(base + lp->offset + 2 * sizeof(size_t));
            if (*cur_key == k) {
                const auto keySize = *reinterpret_cast<size_t*>(base + lp->offset);
                auto value = reinterpret_cast<Value*>(base + lp->offset + 2 * sizeof(size_t) + keySize);
                return *value;
            }
        }

        return std::nullopt;
    }

    // auto insertLinePointer(const size_t &offset) {
    //     LinePointer lp{offset, true};
    //     memcpy(cellLocations, &lp, sizeof(lp));
    //     cellLocations += 1;
    // }

    // template<typename Key>
    // auto insert(const Key& k) -> bool
    // {
    //     const auto KeySize = sizeof(Key);
    //     const auto data_chunk_size = sizeof(size_t) + sizeof(Key);
    //     if (header.pd_lower + data_chunk_size > header.pd_upper) return false;
    //     const auto copy_to = data - data_chunk_size;
    //     const auto offset = header.pd_upper - data_chunk_size;
    //     memcpy(data, &KeySize, sizeof(KeySize));
    //     memcpy(data + (sizeof(KeySize)), &Key, KeySize);
    // }
    // template<typename Key>
    // auto find(const Key& key) -> size_t
    // {
    //     auto runningPtr = cellLocations;
    //     while(reinterpret_cast<void*>(runningPtr) - base < header.pd_lower)
    //     {
    //         auto currentKey = getKeyFromOffset(runningPtr->offset);
    //         if (*currentKey == key) return cellLocations - runningPtr;
    //         runningPtr++;
    //     }

    //     return -1;
    // }

    // template<typename Key>
    // auto getKeyFromOffset(const size_t& offset) -> Key*
    // {
    //     return reinterpret_cast<Key*>(base + offset + 2 * sizeof(size_t));
    // }

};


#endif