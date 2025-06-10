#include "editor.h"
#include "logger.h"
#include "database.h"

namespace DiabloModEditor {
  void run() {
    DiabloModEditor::Logger::init_logger();
    DiabloModEditor::DataBase::init_database();
  }
}
