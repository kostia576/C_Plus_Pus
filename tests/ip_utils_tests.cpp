#include <gtest/gtest.h>
#include "../ip_utils.h"
#include <algorithm>
using namespace ip_utils;

// ==========================================================
// Тесты для функции split
// ==========================================================
TEST(SplitTest, BasicSplit) {
   auto result = split("a.b.c", '.');
   ASSERT_EQ(result.size(), 3);
   EXPECT_EQ(result[0], "a");
   EXPECT_EQ(result[1], "b");
   EXPECT_EQ(result[2], "c");
}

TEST(SplitTest, EmptyString) {
   auto result = split("", '.');
   EXPECT_EQ(result.size(), 0);
}

TEST(SplitTest, NoDelimiter) {
   auto result = split("hello", '.');
   ASSERT_EQ(result.size(), 1);
   EXPECT_EQ(result[0], "hello");
}

TEST(SplitTest, MultipleDelimiters) {
   auto result = split("a..b...c", '.');
   ASSERT_EQ(result.size(), 3); // Пустые токены игнорируются
}

// ==========================================================
// Тесты для функции is_valid_ip
// ==========================================================
TEST(IsValidIpTest, ValidIPs) {
   EXPECT_TRUE(is_valid_ip("192.168.1.1"));
   EXPECT_TRUE(is_valid_ip("0.0.0.0"));
   EXPECT_TRUE(is_valid_ip("255.255.255.255"));
   EXPECT_TRUE(is_valid_ip("10.0.0.1"));
}

TEST(IsValidIpTest, InvalidIPs) {
   EXPECT_FALSE(is_valid_ip("256.1.1.1"));      // Число > 255
   EXPECT_FALSE(is_valid_ip("192.168.1"));      // Мало частей
   EXPECT_FALSE(is_valid_ip("192.168.1.1.1"));  // Много частей
   EXPECT_FALSE(is_valid_ip("abc.def.ghi.jkl")); // Не числа
   EXPECT_FALSE(is_valid_ip("192.168.1.-1"));   // Отрицательное число
   EXPECT_FALSE(is_valid_ip(""));               // Пустая строка
   EXPECT_FALSE(is_valid_ip("01.02.03.04"));    // Ведущие нули
}

// ==========================================================
// Тесты для функции split_ip
// ==========================================================
TEST(SplitIpTest, BasicIP) {
   auto result = split_ip("192.168.1.1");
   ASSERT_EQ(result.size(), 4);
   EXPECT_EQ(result[0], "192");
   EXPECT_EQ(result[1], "168");
   EXPECT_EQ(result[2], "1");
   EXPECT_EQ(result[3], "1");
}

// ==========================================================
// Тесты для функции ip_to_number
// ==========================================================
TEST(IpToNumberTest, Conversion) {
   auto ip1 = split_ip("192.168.1.1");
   auto ip2 = split_ip("10.0.0.1");

   EXPECT_GT(ip_to_number(ip1), ip_to_number(ip2));

   // Проверка конкретного значения
   auto ip = split_ip("1.0.0.0");
   EXPECT_EQ(ip_to_number(ip), 16777216); // 1 * 256^3
}

// ==========================================================
// Тесты для компараторов сортировки
// ==========================================================
TEST(CompareIpTest, Ascending) {
   auto ip1 = split_ip("10.0.0.1");
   auto ip2 = split_ip("192.168.1.1");

   EXPECT_TRUE(compare_ip_ascending(ip1, ip2));
   EXPECT_FALSE(compare_ip_ascending(ip2, ip1));
}

TEST(CompareIpTest, Descending) {
   auto ip1 = split_ip("10.0.0.1");
   auto ip2 = split_ip("192.168.1.1");

   EXPECT_TRUE(compare_ip_descending(ip2, ip1));
   EXPECT_FALSE(compare_ip_descending(ip1, ip2));
}

TEST(CompareIpTest, SamePrefix) {
   auto ip1 = split_ip("192.168.1.1");
   auto ip2 = split_ip("192.168.1.2");

   EXPECT_TRUE(compare_ip_ascending(ip1, ip2));
}

// ==========================================================
// Тесты для функций фильтрации
// ==========================================================
class FilterTest : public ::testing::Test {
protected:
   std::vector<std::vector<std::string>> ip_pool;

   void SetUp() override {
      ip_pool = {
          split_ip("1.231.69.33"),
          split_ip("1.87.203.225"),
          split_ip("46.70.225.39"),
          split_ip("46.70.147.26"),
          split_ip("185.46.87.231"),
          split_ip("5.189.203.46")
      };
   }
};

TEST_F(FilterTest, FilterByFirstByte) {
   auto result = filter_by_first_byte(ip_pool, "1");
   EXPECT_EQ(result.size(), 2);
   EXPECT_EQ(result[0][0], "1");
   EXPECT_EQ(result[1][0], "1");
}

TEST_F(FilterTest, FilterByFirstTwoBytes) {
   auto result = filter_by_first_two_bytes(ip_pool, "46", "70");
   EXPECT_EQ(result.size(), 2);
   EXPECT_EQ(result[0][1], "70");
   EXPECT_EQ(result[1][1], "70");
}

TEST_F(FilterTest, FilterByAnyByte) {
   auto result = filter_by_any_byte(ip_pool, "46");
   EXPECT_EQ(result.size(), 4); // 46.70.225.39, 46.70.147.26, 185.46.87.231, 5.189.203.46
}

TEST_F(FilterTest, FilterNoMatch) {
   auto result = filter_by_first_byte(ip_pool, "999");
   EXPECT_EQ(result.size(), 0);
}

// ==========================================================
// Интеграционный тест: сортировка + фильтрация
// ==========================================================
TEST(IntegrationTest, SortAndFilter) {
   std::vector<std::vector<std::string>> pool = {
       split_ip("192.168.1.1"),
       split_ip("10.0.0.1"),
       split_ip("172.16.0.1"),
       split_ip("1.1.1.1")
   };

   // Сортируем по возрастанию
   std::sort(pool.begin(), pool.end(), compare_ip_ascending);

   EXPECT_EQ(pool[0][0], "1");
   EXPECT_EQ(pool[1][0], "10");
   EXPECT_EQ(pool[2][0], "172");
   EXPECT_EQ(pool[3][0], "192");

   // Фильтруем по первому байту "1"
   auto filtered = filter_by_first_byte(pool, "1");
   EXPECT_EQ(filtered.size(), 1);
   EXPECT_EQ(filtered[0][0], "1");
}