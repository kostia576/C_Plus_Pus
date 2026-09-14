#pragma once
#include <string>
#include <vector>

namespace ip_utils {

   // Разделение строки по разделителю
   std::vector<std::string> split(const std::string& str, char delimiter);

   // Проверка, является ли строка валидным IP-адресом
   bool is_valid_ip(const std::string& str);

   // Разделение IP на октеты
   std::vector<std::string> split_ip(const std::string& ip);

   // Преобразование IP в число (для сортировки)
   long long ip_to_number(const std::vector<std::string>& ip);

   // Компаратор для сортировки IP по возрастанию
   bool compare_ip_ascending(const std::vector<std::string>& a, const std::vector<std::string>& b);

   // Компаратор для сортировки IP по убыванию
   bool compare_ip_descending(const std::vector<std::string>& a, const std::vector<std::string>& b);

   // Фильтр по первому байту
   std::vector<std::vector<std::string>> filter_by_first_byte(
      const std::vector<std::vector<std::string>>& ip_pool,
      const std::string& byte);

   // Фильтр по первому и второму байтам
   std::vector<std::vector<std::string>> filter_by_first_two_bytes(
      const std::vector<std::vector<std::string>>& ip_pool,
      const std::string& byte1,
      const std::string& byte2);

   // Фильтр по любому байту
   std::vector<std::vector<std::string>> filter_by_any_byte(
      const std::vector<std::vector<std::string>>& ip_pool,
      const std::string& byte);

} // namespace ip_utils