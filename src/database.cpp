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
namespace DiabloModEditor::DataBase {
  bool init_database() {
    const std::string db_filename = DiabloModEditor::Resources::Constants::OFFICIAL_DB_FILENAME;
    try {
      if (std::filesystem::exists(db_filename) == false) {
        std::string strings_sql_str;
        std::string excel_sql_str;
        DiabloModEditor::SqliteManager::SQLiteDB db(db_filename.c_str());

        /**
         * 解析 resources/officialData/local/lng/strings 下的所有 json 文件将内容转换成 SQL 语句执行保存在数据库中
         * 表名称以 strings_ 开头
         */
        strings_sql_str.append("BEGIN TRANSACTION;");
        strings_sql_str.append(DiabloModEditor:: Resources::get_strings_directory_files_convert_to_sql_string());
        strings_sql_str.append("COMMIT;");
        db.execute(strings_sql_str);

        /**
         * 解析 resources/officialData/global/excel 下的所有 tsv 文件将内容转换成 SQL 语句执行保存在数据库中
         * 表名称以 excel_ 开头
         */
        excel_sql_str.append("BEGIN TRANSACTION;");
        excel_sql_str.append(DiabloModEditor:: Resources::get_excel_directory_files_convert_to_sql_string());
        excel_sql_str.append("COMMIT;");
        db.execute(excel_sql_str);
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
}
