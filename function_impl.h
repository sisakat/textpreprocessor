#pragma once
#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include "token.h"

std::string include(std::vector<token> parameters);
std::string include_file(std::string filename);
std::string include_dir(std::string path);
std::string include_file_regex(std::string path, std::string pattern);
std::string scope(std::vector<token> parameters);
std::string echo(std::vector<token> parameters);
std::string struppercase(std::vector<token> parameters);
std::string strlowercase(std::vector<token> parameters);

static std::map<std::string, std::function<std::string(std::vector<token>)>> function_impl {
    { "include", include },
    { "scope", scope },
    { "echo", echo },
    { "upper-case", struppercase },
    { "lower-case", strlowercase },
    { "newline", [](std::vector<token>) { return "\n"; }}
};