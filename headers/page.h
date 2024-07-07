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

template<typename T>
concept HasOStreamOperator = requires(std::ostream& os, T value) {
    { os << value } -> std::convertible_to<std::ostream&>;
};

// Function to output a value if it has an ostream operator
template<HasOStreamOperator T>
void output_if_possible(const T& value) {
    std::cout << value << std::endl;
}

template<typename T>
concept EqualityComparable = requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>;
};

template<typename T>
concept FullyComparable = EqualityComparable<T> && HasGreaterThan<T> && HasOStreamOperator<T>;
// Overload for types without an ostream operator (optional)
void output_if_possible(...) {
    std::cout << "Type does not have an ostream operator." << std::endl;
}

template<FullyComparable Key, typename Value>
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

    auto printKeyList()
    {
        std::cout << "Starting Print Key List" << std::endl;
        for(auto i = header.lp_start; i < header.pd_lower; i+=sizeof(LinePointer)){
            auto lp = findLinePointerFromOffset(i);
            output_if_possible(*getActiveKeyForLinePointer(lp));
        }
    }
    auto isFull() -> bool
    {
        return header.pd_upper - header.pd_lower < 2 * sizeof(size_t) + sizeof(Key) + sizeof(Value);
    }

    auto getKeyForLinePointer(const LinePointer* lp) -> const Key*
    {
        return reinterpret_cast<Key*>(base + lp->offset + 2 * sizeof(size_t));
    }
    auto getActiveKeyForLinePointer(const LinePointer* lp) -> const Key*
    {
        if(!lp->filled) return nullptr;
        return getKeyForLinePointer(lp);
    }

    auto make_space_line_pointer(const Key& k, const LinePointer& lp) -> void
    {
        auto line_pointer_storage_loc = base + header.pd_lower;
        memcpy(line_pointer_storage_loc, &lp, sizeof(LinePointer));
        header.pd_lower += sizeof(LinePointer);

        if constexpr (HasGreaterThan<Key>)
        {
            for (auto offset = header.pd_lower - 2 * sizeof(LinePointer); offset >= header.lp_start; offset -= sizeof(LinePointer))
            {
                if (offset < header.lp_start) continue;
                auto cur = reinterpret_cast<LinePointer*>(base + offset);
                auto next = reinterpret_cast<LinePointer*>(base + offset + sizeof(LinePointer));
                const auto key = getKeyForLinePointer(cur);
                const auto nextKey = getKeyForLinePointer(next);
                if (*key < *nextKey) break;
                LinePointer tmp;
                memcpy(&tmp, base + offset, sizeof(LinePointer));
                memcpy(base + offset, base+offset + sizeof(LinePointer), sizeof(LinePointer));
                memcpy(base + offset + sizeof(LinePointer), &tmp, sizeof(LinePointer));
            }
        }
        
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
        // Start storing the data in the cell data
        memcpy(base + offset, &KeySize, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(base + offset, &ValueSize, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(base + offset, &k, KeySize);
        offset += KeySize;
        memcpy(base + offset, &v, ValueSize);
        header.pd_upper -= payload_size;

        make_space_line_pointer(k, lp);
        
        return true;
    }

    auto find(const Key& k) -> std::optional<Value>
    {
        auto cellOffset = findCellOffset(k);
        if (cellOffset <= 0)
            return std::nullopt;
        
        auto lp = reinterpret_cast<LinePointer*>(base + cellOffset);
        auto cur_key = reinterpret_cast<Key*>(base + lp->offset + 2 * sizeof(size_t));

        const auto keySize = *reinterpret_cast<size_t*>(base + lp->offset);
        auto value = reinterpret_cast<Value*>(base + lp->offset + 2 * sizeof(size_t) + keySize);
        return *value;
    }

    auto remove(const Key& k) -> void
    {
        auto cellOffset = findCellOffset(k);
        if (cellOffset <= 0) return;
        auto lp = reinterpret_cast<LinePointer*>(base+cellOffset);
        lp->filled = false;
    }

    auto findLinePointerFromOffset(const size_t& offset) -> LinePointer*
    {
        return reinterpret_cast<LinePointer*>(base + offset);
    }

    auto findCellOffset(const Key& k) -> size_t
    {
        auto jumpSize = sizeof(LinePointer);
        auto totalCells = (header.pd_lower - header.lp_start) / jumpSize;
        int low = 0, hi = totalCells - 1;
        auto check = [&](const int index) -> const Key*
        {
            size_t ptr = header.lp_start + index * jumpSize;
            auto lp = findLinePointerFromOffset(ptr);
            return getKeyForLinePointer(lp);
        };
        while(low <= hi)
        {
            auto mid = low + (hi - low) / 2;
            auto checkResult = check(mid);
            if (*checkResult == k) {
                auto lp = reinterpret_cast<LinePointer*>(base + header.lp_start + mid * jumpSize);
                if (!lp->filled)
                    return 0;
                return header.lp_start + mid * jumpSize;
            } 
            else if (*checkResult < k) low = mid + 1;
            else if (*checkResult > k) hi = mid - 1;
        }

        return 0;
        
    }


};


#endif