#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

// Функция для разделения строки по разделителю
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

// Проверка, является ли строка валидным IP-адресом
bool is_valid_ip(const std::string& str) {
   std::vector<std::string> parts = split(str, '.');
   if (parts.size() != 4) return false;

   for (const auto& part : parts) {
      try {
         int num = std::stoi(part);
         if (num < 0 || num > 255) return false;
      }
      catch (...) {
         return false;
      }
   }
   return true;
}

// Разделение IP на части (октеты)
std::vector<std::string> split_ip(const std::string& ip) {
   return split(ip, '.');
}

int main(int argc, char* argv[])
{
   #ifdef _WIN32
   SetConsoleOutputCP(CP_UTF8);
   SetConsoleCP(CP_UTF8);
   #endif

   try
   {
      std::ifstream file("D:/C_Plus_Pus/homework_data/ip_filter.tsv");

      if (!file.is_open()) {
         std::cerr << "Ошибка: не удалось открыть файл\n";
         return 1;
      }

      std::cout << "Файл открыт успешно!\n\n";

      // Читаем ВСЕ IP-адреса из файла
      std::vector<std::vector<std::string>> ip_pool;
      std::string line;

      while (std::getline(file, line)) {
         // Убираем \r если есть
         if (!line.empty() && line.back() == '\r') {
            line.pop_back();
         }

         // Разделяем строку по табуляции
         std::vector<std::string> tokens = split(line, '\t');

         // Обрабатываем каждый токен
         for (const auto& token : tokens) {
            // Проверяем, является ли токен валидным IP
            if (is_valid_ip(token)) {
               ip_pool.push_back(split_ip(token));
            }
            // Игнорируем числа вроде "1", "2", "9"
         }
      }
      file.close();

      std::cout << "Загружено IP-адресов: " << ip_pool.size() << "\n\n";

      // ==========================================================
      // 1. Сортировка по возрастанию (лексикографическая по числам)
      // ==========================================================
      std::sort(ip_pool.begin(), ip_pool.end(),
         [](const std::vector<std::string>& a, const std::vector<std::string>& b) {
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
      );

      std::cout << "--- Отсортированный список (по возрастанию) ---\n";
      for (size_t i = 0; i < std::min(size_t(10), ip_pool.size()); ++i) {
         for (size_t j = 0; j < ip_pool[i].size(); ++j) {
            if (j > 0) std::cout << ".";
            std::cout << ip_pool[i][j];
         }
         std::cout << "\n";
      }
      std::cout << "...\n\n";

      // ==========================================================
      // 2. Reverse lexicographically sort (По убыванию)
      // ==========================================================
      std::cout << "--- Отсортированный список (по убыванию) ---\n";
      std::sort(ip_pool.begin(), ip_pool.end(),
         [](const std::vector<std::string>& a, const std::vector<std::string>& b) {
            size_t min_size = std::min(a.size(), b.size());
            for (size_t i = 0; i < min_size; ++i) {
               int num_a = std::stoi(a[i]);
               int num_b = std::stoi(b[i]);
               if (num_a != num_b) {
                  return num_a > num_b; // Знак >
               }
            }
            return a.size() > b.size();
         }
      );

      for (size_t i = 0; i < std::min(size_t(10), ip_pool.size()); ++i) {
         for (size_t j = 0; j < ip_pool[i].size(); ++j) {
            if (j > 0) std::cout << ".";
            std::cout << ip_pool[i][j];
         }
         std::cout << "\n";
      }
      std::cout << "...\n\n";

      // ==========================================================
      // 3. Filter by first byte (Фильтр по первому байту = "1")
      // ==========================================================
      std::cout << "--- IP, начинающиеся с 1 ---\n";
      for (const auto& ip : ip_pool) {
         if (ip.size() >= 1 && ip[0] == "1") {
            std::cout << ip[0] << "." << ip[1] << "." << ip[2] << "." << ip[3] << "\n";
         }
      }
      std::cout << "\n";

      // ==========================================================
      // 4. Filter by first and second bytes (46.70)
      // ==========================================================
      std::cout << "--- IP, начинающиеся с 46.70 ---\n";
      for (const auto& ip : ip_pool) {
         if (ip.size() >= 2 && ip[0] == "46" && ip[1] == "70") {
            std::cout << ip[0] << "." << ip[1] << "." << ip[2] << "." << ip[3] << "\n";
         }
      }
      std::cout << "\n";

      // ==========================================================
      // 5. Filter by ANY byte (содержит "46" в любом месте)
      // ==========================================================
      std::cout << "--- IP, содержащие 46 в любом октете ---\n";
      int count = 0;
      for (const auto& ip : ip_pool) {
         bool found = false;
         for (const auto& part : ip) {
            if (part == "46") {
               found = true;
               break;
            }
         }
         if (found) {
            std::cout << ip[0] << "." << ip[1] << "." << ip[2] << "." << ip[3] << "\n";
            count++;
            if (count >= 30) {
               std::cout << "... (показано 30 из " << count << ")\n";
               break;
            }
         }
      }
   }
   catch (const std::exception& e)
   {
      std::cerr << "Исключение: " << e.what() << std::endl;
   }

   return 0;
}