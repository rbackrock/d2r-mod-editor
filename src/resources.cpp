#include <iostream>
#include <format>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <regex>
#include <nlohmann/json.hpp>
#include "constants.h"
#include "spdlog/spdlog.h"
#include "resources.h"

static std::string json_file_covert_to_sql(const std::string& file_path, const std::string& file_name) {
  std::string sql_str = "";
  std::ifstream f(file_path);
  // 确保迭代的时候，对象属性顺序和原始文件一致
  nlohmann::ordered_json item_names_json = nlohmann::ordered_json::parse(f);

  if (!item_names_json.empty() && item_names_json.is_array()) {
    std::string sql_table_name = std::format(
      "{}_{}",
      DiabloModEditor::Resources::Constants::STRINGS_TABLE_NAME_PREFIX,
      std::regex_replace(file_name, std::regex("-"), "_")
    );
    std::string sql_create_table_prefix = std::format("CREATE TABLE {} ( ", sql_table_name);
    std::string sql_create_table_content = "";
    std::string sql_create_table_suffix = " );";
    std::string sql_insert_prefix = std::format("INSERT INTO {} VALUES ( ", sql_table_name);
    std::string sql_insert_content = "";
    std::string sql_insert_suffix = " );";
    std::string sql_col_data_type = "";

    // 拿第一个元素用于建表 SQL 语句
    auto frist_element = item_names_json[0];
    for (auto frist_element_item = frist_element.begin(); frist_element_item != frist_element.end(); frist_element_item++) {
      const auto value_type = frist_element_item.value().type();
      if (value_type == nlohmann::json::value_t::number_integer || value_type == nlohmann::json::value_t::number_unsigned) {
        sql_col_data_type = "INTEGER";
      } else if (value_type == nlohmann::json::value_t::string) {
        sql_col_data_type = "TEXT";
      }

      sql_create_table_content.append(
        std::format(
          "{} {} ",
          frist_element_item.key(),
          sql_col_data_type
        )
      );

      if (std::next(frist_element_item) != frist_element.end()) {
        sql_create_table_content.append(", ");
      }
    }

    sql_str.append(sql_create_table_prefix);
    sql_str.append(sql_create_table_content);
    sql_str.append(sql_create_table_suffix);

    // 创建 sql 插入语句
    for (auto& element : item_names_json) {
      const auto items = element.items();

      for (auto it = items.begin(); it != items.end(); it++) {
        if (it.value().is_number()) {
          const int value = it.value();
          sql_insert_content.append(std::format(" {} ", value));
        } else if (it.value().is_string()) {
          const std::string value = it.value();
          // 处理如果有 ' 符号的情况
          const std::string process_value = std::regex_replace(value, std::regex("'"), "''");
          sql_insert_content.append(std::format("'{}'", process_value));
        }

        if (std::next(it) != items.end()) {
          sql_insert_content.append(", ");
        }
      }

      sql_str.append(sql_insert_prefix);
      sql_str.append(sql_insert_content);
      sql_str.append(sql_insert_suffix);
      sql_insert_content.clear();
    }
  }

  return sql_str;
}

std::string get_strings_directory_files_convert_to_sql_string() {
  const std::filesystem::path strings_directory = DiabloModEditor::Resources::Constants::RESOURCES_OFFICIAL_STRINGS_DIRECTORY_PATH;
  std::string sql_str = "";

  for (const auto& entry : std::filesystem::directory_iterator(strings_directory)) {
    if (entry.is_regular_file()) {
      const std::string full_file_path = entry.path().string();
      const std::string filename = entry.path().stem().string();

      sql_str.append(json_file_covert_to_sql(full_file_path, filename));
    }
  }

  return sql_str;
}
