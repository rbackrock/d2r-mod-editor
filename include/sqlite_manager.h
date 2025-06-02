#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <variant>
#include <stdexcept>
#include "sqlite3.h"
#include "spdlog/spdlog.h"

class SQLiteDB
{
public:
  explicit SQLiteDB(const std::string &dbPath)
  {
    if (sqlite3_open(dbPath.c_str(), &db_) != SQLITE_OK)
    {
      spdlog::error(sqlite3_errmsg(db_));
      throw std::runtime_error(sqlite3_errmsg(db_));
    }
  }

  ~SQLiteDB()
  {
    if (db_)
    {
      sqlite3_close(db_);
    }
  }

  SQLiteDB(const SQLiteDB &) = delete;
  SQLiteDB &operator=(const SQLiteDB &) = delete;

  SQLiteDB(SQLiteDB &&other) noexcept : db_(other.db_)
  {
    other.db_ = nullptr;
  }

  SQLiteDB &operator=(SQLiteDB &&other) noexcept
  {
    if (this != &other)
    {
      if (db_)
      {
        sqlite3_close(db_);
      }
      db_ = other.db_;
      other.db_ = nullptr;
    }
    return *this;
  }

  // 执行非查询 SQL 语句（如 INSERT, UPDATE, DELETE）
  void execute(const std::string &sql)
  {
    char *errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, 0, &errMsg) != SQLITE_OK)
    {
      std::string error(errMsg);
      sqlite3_free(errMsg);
      spdlog::error(sqlite3_errmsg(db_));
      throw std::runtime_error(error);
    }
  }

  // 执行查询并返回结果
  template <typename T>
  std::vector<T> query(const std::string &sql, T (*rowParser)(sqlite3_stmt *))
  {
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
      throw std::runtime_error(sqlite3_errmsg(db_));
    }

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> statement(stmt, sqlite3_finalize);
    std::vector<T> results;

    int stepResult;
    while ((stepResult = sqlite3_step(statement.get())) == SQLITE_ROW)
    {
      results.push_back(rowParser(statement.get()));
    }

    if (stepResult != SQLITE_DONE)
    {
      throw std::runtime_error(sqlite3_errmsg(db_));
    }

    return results;
  }

private:
  sqlite3 *db_ = nullptr;
};