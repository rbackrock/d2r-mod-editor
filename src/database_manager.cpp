#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "spdlog/spdlog.h"
#include "constants.h"
#include "database_manager.h"

bool init_database() {
  if (exist_db_file(OFFICIAL_DB_FILENAME) == false) {
    sqlite3* official_db;

    const int official_data_open_result = sqlite3_open(OFFICIAL_DB_FILENAME.c_str(), &official_db);
    if (official_data_open_result == 1) {
      spdlog::error("创建官方数据库失败");
      sqlite3_close(official_db);
      return false;
    }
  }

  read_official_strings_files();

  return true;
}

bool exist_db_file(const std::string& path) {
  return std::filesystem::exists(path);
}

bool read_official_strings_files() {
  std::vector<std::string> filenames;
  const std::string strings_directory = RESOURCES_OFFICIAL_DATA_ROOT_PATH + "/local/lng/strings";

  for (const auto& entry : std::filesystem::directory_iterator(strings_directory)) {
    if (entry.is_regular_file()) {
      const std::string filename = entry.path().filename().string();
      filenames.push_back(filename);
      spdlog::info(filename);
    }
  }

  return true;
}
