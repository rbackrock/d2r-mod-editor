#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "spdlog/spdlog.h"
#include "constants.h"
#include "database.h"
#include "resources.h"

bool init_database() {
  if (std::filesystem::exists(DiabloModEditor::Resources::Constants::OFFICIAL_DB_FILENAME) == false) {
    sqlite3* official_db;

    const int official_data_open_result = sqlite3_open(DiabloModEditor::Resources::Constants::OFFICIAL_DB_FILENAME.c_str(), &official_db);
    if (official_data_open_result == 1) {
      spdlog::error("创建原始游戏数据数据库失败");
      sqlite3_close(official_db);
      return false;
    }
  }

  // json_file_covert_to_sql("");
  get_strings_directory_files_convert_to_sql_string();

  return true;
}
