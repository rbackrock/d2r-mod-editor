#pragma once
#include <iostream>
#include <string>

bool init_database();
bool exist_db_file(std::string const&);
bool read_official_strings_files();