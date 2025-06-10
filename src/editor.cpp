#include "editor.hpp"
#include "logger.hpp"
#include "database.hpp"

namespace DiabloModEditor {
  void run() {
    DiabloModEditor::Logger::init_logger();
    DiabloModEditor::DataBase::init_database();
  }
}
