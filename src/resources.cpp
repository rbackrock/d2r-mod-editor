#include <iostream>
#include <format>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <regex>
#include <nlohmann/json.hpp>
#include "constants.hpp"
#include "spdlog/spdlog.h"
#include "resources.hpp"

namespace DiabloModEditor::Resources {
  static std::string json_file_covert_to_sql(const std::string& file_path, const std::string& file_name) {
    std::string sql_str = "";
    std::ifstream f(file_path);
    // 确保迭代的时候，对象属性顺序和原始文件一致
    nlohmann::ordered_json strings_file_json = nlohmann::ordered_json::parse(f);

    if (!strings_file_json.empty() && strings_file_json.is_array()) {
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
      auto frist_element = strings_file_json[0];
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
      for (auto& element : strings_file_json) {
        const auto& items = element.items();

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

  /**
   * 删除字符串最后一个逗号，并且这个逗号要是字符串最后一个字符才可以
   */
  static void remove_last_comma(std::string& str) {
    if (!str.empty() && str.back() == ',') {
      str.pop_back();
    }
  }

  static std::string tsv_file_convert_to_sql(const std::filesystem::path& file_path, const std::string& file_name) {
    std::ifstream file(file_path);
    if (!file.good()) {
      throw std::runtime_error(std::format("{}文件无法读取", file_path.string()));
    }

    std::string sql_table_name = std::format(
      "{}_{}",
      DiabloModEditor::Resources::Constants::EXCEL_TABLE_NAME_PREFIX,
      std::regex_replace(file_name, std::regex("-"), "_")
    );
    std::string sql_str;
    std::string sql_create_table_prefix = std::format("CREATE TABLE {} ( ", sql_table_name);
    std::string sql_create_table_content = "";
    std::string sql_create_table_suffix = " );";
    std::string sql_insert_col_names;
    std::string sql_insert_content = "";
    std::string line;
    std::vector<std::string> table_col_name_list;
    int line_count = 0;

    while(std::getline(file, line)) {
      if (!line.empty()) {
        ++line_count;
        std::istringstream ss(line);
        std::string field;

        if (line_count == 1) {
          // 第一行对于 tsv 是表头
          while (std::getline(ss, field, '\t')) {
            sql_create_table_content.append(std::format(" '{}' TEXT,", field));
            table_col_name_list.push_back(field);
          }
          remove_last_comma(sql_create_table_content);

          sql_str.append(sql_create_table_prefix);
          sql_str.append(sql_create_table_content);
          sql_str.append(sql_create_table_suffix);
        } else {
          // 其他行就是数据了，全部数据当字符串处理
          int field_count = 0;
          while (std::getline(ss, field, '\t')) {
            ++field_count;
            std::string field_value = "NULL";

            if (!field.empty()) {
              field_value = std::format("'{}'", std::regex_replace(field, std::regex("'"), "''"));
            }
            sql_insert_content.append(std::format(" {},", field_value));
          }
          remove_last_comma(sql_insert_content);

          sql_str.append(std::format("INSERT INTO {} (", sql_table_name));
          for (size_t i = 0; i < field_count; ++i) {
            sql_insert_col_names.append(std::format(" '{}',", table_col_name_list[i]));
          }
          remove_last_comma(sql_insert_col_names);
          sql_str.append(sql_insert_col_names);
          sql_str.append(") VALUES (");
          sql_str.append(sql_insert_content);
          sql_str.append(");");
          sql_insert_col_names.clear();
          sql_insert_content.clear();
        }
      }
    }

    return sql_str;
  }

  std::string get_excel_directory_files_convert_to_sql_string() {
    const std::filesystem::path excel_directory = DiabloModEditor::Resources::Constants::RESOURCES_OFFICIAL_EXCEL_DIRECTORY_PATH;
    std::string sql_str;

    for (const auto& entry : std::filesystem::directory_iterator(excel_directory)) {
      std::filesystem::path path = entry.path();
      if (entry.is_regular_file()) {
        const std::string full_file_path = path.string();
        const std::string filename = path.stem().string();
        sql_str.append(tsv_file_convert_to_sql(path, filename));
      }
    }

    return sql_str;
  }
}
