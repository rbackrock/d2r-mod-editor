#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "spdlog/spdlog.h"
#include "constants.h"
#include "database.h"
#include "resources.h"
#include "sqlite_manager.h"

bool init_database() {
  const std::string db_filename = DiabloModEditor::Resources::Constants::OFFICIAL_DB_FILENAME;
  try {
    if (std::filesystem::exists(db_filename) == false) {
      std::string strings_sql_str;
      SQLiteDB db(db_filename.c_str());

      strings_sql_str.append("BEGIN TRANSACTION;");
      strings_sql_str.append(get_strings_directory_files_convert_to_sql_string());
      strings_sql_str.append("COMMIT;");

      db.execute(strings_sql_str);
    }
  } catch (const std::exception& e) {
    try {
      std::filesystem::path remove_db_file_path = db_filename;
      std::filesystem::remove(remove_db_file_path);
    } catch (const std::filesystem::filesystem_error& e) {
      spdlog::error(e.what());
      std::cerr << "Error: " << e.what() << std::endl;
    }

    spdlog::error(e.what());
    std::cerr << "Error: " << e.what() << std::endl;

    return false;
  }

  return true;
}
