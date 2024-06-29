#include <gtest/gtest.h>
#include <page.h>

// TEST(PAGE, NEW_PAGE_INIT_FAIL_ASSERT)
// {
//     char memory_area[1000];
//     ASSERT_DEATH(
//         Page<int,int>(reinterpret_cast<void*>(&memory_area), 10, PageType::KEY_VALUE),
//         ""
//     );
// }

TEST(PAGE, NEW_PAGE_INIT_HP)
{
    char memory_area[1000];
    LOCATION ptr = reinterpret_cast<LOCATION>(&memory_area);
    Page<int,int> p(ptr, 100, PageType::KEY_VALUE);

    EXPECT_EQ(p.base, ptr);
    EXPECT_EQ(p.header.pageid, 0);
    EXPECT_EQ(p.header.pd_lower, sizeof(p.header));
    EXPECT_EQ(p.header.lp_start, sizeof(p.header));
    EXPECT_EQ(p.header.pd_upper, 100);
    EXPECT_EQ(p.header.type, PageType::KEY_VALUE);
    EXPECT_EQ(p.header.total, 0);
}

TEST(PAGE_UTILS_STORAGE_SIZE, PRIMITIVE)
{
    int a;
    double b;
    bool c;
    char d;
    float e;

    const auto x = sizeof(size_t);
    EXPECT_EQ(utils::getStorageSize(a, a), 2 * x + 2 * sizeof(int));
    EXPECT_EQ(utils::getStorageSize(b, e), 2 * x + sizeof(b) + sizeof(e));
}

TEST(PAGE_UTILS_STORAGE_SIZE, COMPLEX)
{
    struct key 
    {
        int a;
        bool b;
        int c;
    } k;

    struct value
    {
        int a;
        bool b;
        float c;
    } v;

    EXPECT_EQ(utils::getStorageSize(k, v), 2 * sizeof(size_t) + sizeof(k) + sizeof(v));
}

TEST(PAGE, INT_INSERT_FIND_VALUE)
{
    int key = 3, value = 4;
    char memory_area[1000];

    Page<int, int> p(reinterpret_cast<void*>(&memory_area), 1000, PageType::KEY_VALUE);

    auto success = p.insertData(key, value);
    EXPECT_EQ(success, true);
    success = p.insertData(value, key);
    EXPECT_EQ(success, true);
    EXPECT_EQ(p.find(key), value);
    EXPECT_EQ(p.find(value), key);
}

TEST(PAGE, DOUBLE_INSERT_FIND_VALUE)
{
    char memory_area[1000];
    Page<double, double> p(reinterpret_cast<void*>(&memory_area), 1000, PageType::KEY_VALUE);
    double k[3] = {1.43233, 1.52332, 323432.343243};
    double v[3] = {234234.234234, 234234123.123123, 323.2323};

    for (int i = 0; i < 3; i++)
    {
        auto success = p.insertData(k[i], v[i]);
        EXPECT_TRUE(success) << "False for " << k[i] <<" " << v[i];
    }

    for(int i = 0; i < 3; i++) 
    {
        EXPECT_EQ(p.find(k[i]), v[i]) << k[i] <<" " << v[i];
    }
}

TEST(PAGE, CHAR_ARR_INSERT_FIND_VALUE)
{
    typedef std::array<char, 4> char4;
    typedef std::array<char, 10> char10;
    char memory_area[1000];
    char4 x = {"abc"}, xx = {"ufi"};
    char10 y = {"dasdfsdf"}, yy = {"dsewr"};
    Page<char4, char10> p(reinterpret_cast<void*>(&memory_area), 1000, PageType::KEY_VALUE);
    auto success = p.insertData(x, y);
    EXPECT_TRUE(success);
    success = p.insertData(xx, yy);
    EXPECT_TRUE(success);

    auto result = p.find(x);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), y);

    result = p.find(xx);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), yy);
}


TEST(PAGE, DELETE_PAGE_ITEM)
{
    char memory_area[1000];
    int x = 4, y = 5;

    Page<int,int> p(reinterpret_cast<void*>(&memory_area), 1000, PageType::KEY_VALUE);

    auto success = p.insertData(x, y);
    EXPECT_TRUE(success);
    success = p.insertData(y, x);
    EXPECT_TRUE(success);

    p.remove(x);

    auto result = p.find(x);
    EXPECT_FALSE(result.has_value());

    result = p.find(y);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), x);
}