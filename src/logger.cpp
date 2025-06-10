#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "logger.h"

namespace DiabloModEditor::Logger {
  void init_logger() {
    #ifdef DEBUG_MODE
      spdlog::set_level(spdlog::level::debug);
    #else
      // 生产模式：输出到文件
      auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/app.log", true);

      std::vector<spdlog::sink_ptr> sinks{file_sink};
      auto logger = std::make_shared<spdlog::logger>("prod_logger", sinks.begin(), sinks.end());
      logger->set_level(spdlog::level::info);

      spdlog::set_default_logger(logger);
    #endif
  }
}
