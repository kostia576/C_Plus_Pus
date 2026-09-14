#include "ip_utils.h"
#include <sstream>
#include <algorithm>

namespace ip_utils {

   std::vector<std::string> split(const std::string& str, char delimiter) {
      std::vector<std::string> tokens;
      std::stringstream ss(str);
      std::string token;
      while (std::getline(ss, token, delimiter)) {
         if (!token.empty()) {
            tokens.push_back(token);
         }
      }
      return tokens;
   }

   bool is_valid_ip(const std::string& str) {
      std::vector<std::string> parts = split(str, '.');
      if (parts.size() != 4) return false;

      for (const auto& part : parts) {
         try {
            int num = std::stoi(part);
            if (num < 0 || num > 255) return false;
            // Проверка на ведущие нули (опционально)
            if (part.length() > 1 && part[0] == '0') return false;
         }
         catch (...) {
            return false;
         }
      }
      return true;
   }

   std::vector<std::string> split_ip(const std::string& ip) {
      return split(ip, '.');
   }

   long long ip_to_number(const std::vector<std::string>& ip) {
      if (ip.size() != 4) return -1;
      long long result = 0;
      for (int i = 0; i < 4; ++i) {
         result = (result << 8) + std::stoi(ip[i]);
      }
      return result;
   }

   bool compare_ip_ascending(const std::vector<std::string>& a, const std::vector<std::string>& b) {
      size_t min_size = std::min(a.size(), b.size());
      for (size_t i = 0; i < min_size; ++i) {
         int num_a = std::stoi(a[i]);
         int num_b = std::stoi(b[i]);
         if (num_a != num_b) {
            return num_a < num_b;
         }
      }
      return a.size() < b.size();
   }

   bool compare_ip_descending(const std::vector<std::string>& a, const std::vector<std::string>& b) {
      size_t min_size = std::min(a.size(), b.size());
      for (size_t i = 0; i < min_size; ++i) {
         int num_a = std::stoi(a[i]);
         int num_b = std::stoi(b[i]);
         if (num_a != num_b) {
            return num_a > num_b;
         }
      }
      return a.size() > b.size();
   }

   std::vector<std::vector<std::string>> filter_by_first_byte(
      const std::vector<std::vector<std::string>>& ip_pool,
      const std::string& byte) {

      std::vector<std::vector<std::string>> result;
      for (const auto& ip : ip_pool) {
         if (!ip.empty() && ip[0] == byte) {
            result.push_back(ip);
         }
      }
      return result;
   }

   std::vector<std::vector<std::string>> filter_by_first_two_bytes(
      const std::vector<std::vector<std::string>>& ip_pool,
      const std::string& byte1,
      const std::string& byte2) {

      std::vector<std::vector<std::string>> result;
      for (const auto& ip : ip_pool) {
         if (ip.size() >= 2 && ip[0] == byte1 && ip[1] == byte2) {
            result.push_back(ip);
         }
      }
      return result;
   }

   std::vector<std::vector<std::string>> filter_by_any_byte(
      const std::vector<std::vector<std::string>>& ip_pool,
      const std::string& byte) {

      std::vector<std::vector<std::string>> result;
      for (const auto& ip : ip_pool) {
         for (const auto& part : ip) {
            if (part == byte) {
               result.push_back(ip);
               break;
            }
         }
      }
      return result;
   }

} // namespace ip_utils