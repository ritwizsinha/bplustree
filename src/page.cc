#include <page.h>
#include <cstring>
#include <cassert>

// auto getPageHeader(Page page) -> PageHeaderData*
// {
//     return reinterpret_cast<PageHeaderData*>(page);
// }

// auto setPageHeader(Page page, const PageHeaderData* p) -> void
// {
//     auto currentPageHeader = getPageHeader(page);
//     memcpy(currentPageHeader, p, sizeof(PageHeaderData));
// }

// auto initializePage(Page page, const int page_size) -> void
// {
//     assert(page_size > sizeof(PageHeaderData)); 
//     memset(page, 0, page_size);
//     auto ph = getPageHeader(page);
//     ph->pd_lower = PAGE_HEADER_SIZE;
//     ph->pd_upper = page_size + 1;
// }


// auto CellPointers::getCellPointers(Page page) -> CELL_OFFSET_TYPE*
// {
//     return reinterpret_cast<CELL_OFFSET_TYPE*>(page + PAGE_HEADER_SIZE);
// }

// auto CellPointers::insertCell(Page page, const CELL_OFFSET_TYPE& cell) -> void
// {
//     auto cell_ptrs_start = CellPointers::getCellPointers(page);
//     auto pageHeader = getPageHeader(page);
//     for(auto itr = cell_ptrs_start; itr <= pageHeader->pd_lower; itr += reinterpret_cast<int>(sizeof(CELL_OFFSET_TYPE)))
//     {
//         auto 
//     }
// }
