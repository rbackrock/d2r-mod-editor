#include <format>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "constants.h"
#include "spdlog/spdlog.h"
#include "resources.h"

std::vector<std::string> get_official_strings_files() {
  std::vector<std::string> filenames;
  const std::string strings_directory = DiabloModEditor::Resources::Constants::RESOURCES_OFFICIAL_STRINGS_DIRECTORY_PATH;

  for (const auto& entry : std::filesystem::directory_iterator(strings_directory)) {
    if (entry.is_regular_file()) {
      const std::string filename = entry.path().filename().string();
      filenames.push_back(filename);
      spdlog::info(filename);
    }
  }

  return filenames;
}

std::vector<std::string> get_json_covert_to_sql() {
  std::vector<std::string> sql_list;

  std::ifstream f(DiabloModEditor::Resources::Constants::RESOURCES_OFFICIAL_STRINGS_DIRECTORY_PATH + "/item-names.json");
  nlohmann::json item_names_json = nlohmann::json::parse(f);

  // if (!item_names_json.empty()) {
  //   const auto begin_item = item_names_json.begin();
  // }

  // if (item_names_json.size() > 0) {
  //   const auto row = item_names_json[0];
  //   for (const auto& item : row.items()) {
  //     spdlog::info(item.key());
  //   }
  // }

  // for (const auto& row : item_names_json) {
  // }

  std::string sql_str;
  for (int i = 0; i < item_names_json.size(); i++) {
    const auto& row = item_names_json[i];

    if (i == 0) {
      // 创建建表 SQL 语句
      sql_str.append("CREATE TABLE table_name(");
      sql_str.append("id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, ");
      for (const auto& it : row.items()) {
        const std::string key = it.key();
        const std::string val = it.value();
        // const auto val_type = it.value().type();
        std::string val_sql_type = "TEXT";

        // if (val_type == nlohmann::json::value_t::number_integer) {
        //   val_sql_type = "INTEGER";
        // }

        // sql_str.append(std::format("{} {},", key, ""));
        // if (it == row.items().end()) {
        //   sql_str.append(std::format("{} {}", key, ""));
        // }

        // spdlog::info(std::format("{}-{}", key, val));
      }
      sql_str.append(");");

      spdlog::info(sql_str);
    }
    // spdlog::info(row["zhCN"]);
  }

  return sql_list;
}

std::vector<std::string> get_strings_directory_files_convert_to_sql_string() {
  std::vector<std::string> sql_strs;
  const std::string strings_directory = DiabloModEditor::Resources::Constants::RESOURCES_OFFICIAL_STRINGS_DIRECTORY_PATH;
  
  for (const auto& entry : std::filesystem::directory_iterator(strings_directory)) {
    if (entry.is_regular_file()) {
      const std::string filename = entry.path().stem().string();
      spdlog::info(filename);
    }
  }

  return sql_strs;
}
