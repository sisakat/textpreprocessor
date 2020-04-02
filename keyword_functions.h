#pragma once
#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include "token.h"

int include(std::vector<token> tokens, int index);
void include_file(std::string filename);
void include_dir(std::string path);

static std::map<std::string, std::function<int(std::vector<token>, int)>> keyword_functions {
    { "include", include }
};