#include <iostream>
#include "spdlog/spdlog.h"
#include "logger_manager.h"
#include "sqlite3.h"

int main()
{
  init_logger();
  spdlog::info("这是一条信息日志");
  return 0;
}