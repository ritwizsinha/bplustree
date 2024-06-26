#ifndef PAGE_H
#define PAGE_H
#include <cstddef>
#include <optional>
#include <cstring>
#include <cassert>
#include <iostream>
#include <concepts>

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
template<typename T>
concept HasGreaterThan = requires(T a, T b) {
    { a > b } -> std::convertible_to<bool>;
};

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
    
    auto getKeyForLinePointer(const LinePointer* lp) -> const Key*
    {
        if(!lp->filled) return nullptr;
        return reinterpret_cast<Key*>(base + lp->offset + 2 * sizeof(size_t));
    }

    auto make_space_line_pointer(const Key& k, const LinePointer& lp) -> void
    {
        auto line_pointer_storage_loc = base + header.pd_lower;
        memcpy(line_pointer_storage_loc, &lp, sizeof(LinePointer));

        if constexpr (HasGreaterThan<Key>)
        {
            for (auto offset = header.pd_lower - sizeof(LinePointer); offset >= header.lp_start; offset -= sizeof(LinePointer))
            {
                auto cur = reinterpret_cast<LinePointer*>(base + offset);
                const auto key = getKeyForLinePointer(cur);
                const auto nextKey = getKeyForLinePointer(cur + 1);
                if (*key < *nextKey) {
                    LinePointer tmp;
                    memcpy(&tmp, base + offset, sizeof(LinePointer));
                    memcpy(base + offset, base+offset + sizeof(LinePointer), sizeof(LinePointer));
                    memcpy(base + offset + sizeof(LinePointer), &tmp, sizeof(LinePointer));
                }
            }
        }
        
        header.pd_lower += sizeof(LinePointer);
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
        make_space_line_pointer(k, lp);
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

};


#endif