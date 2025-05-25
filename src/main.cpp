#include <iostream>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  for (int i = 0; i < argc; i++)
  {
    std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
  }
  std::cout << "--------" << std::endl;
  return 0;
}

int main()
{
  sqlite3 *db;
  char *errMsg = 0;

  if (sqlite3_open("test.db", &db))
  {
    std::cout << "error" << std::endl;
    return 1;
  }

  const char *select_sql = "SELECT * FROM users;";
  if (sqlite3_exec(db, select_sql, callback, 0, &errMsg) != SQLITE_OK)
  {
    std::cerr << "SQL error: " << errMsg << std::endl;
    sqlite3_free(errMsg);
  }

  sqlite3_close(db);

  return 0;
}