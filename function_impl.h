#pragma once
#include "token.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::string include(std::vector<token> parameters);
std::string include_file(std::string filename);
std::string include_dir(std::string path);
std::string include_file_regex(std::string path, std::string pattern);
std::string scope(std::vector<token> parameters);
std::string echo(std::vector<token> parameters);
std::string struppercase(std::vector<token> parameters);
std::string strlowercase(std::vector<token> parameters);
std::string equals(std::vector<token> parameters);
std::string nequals(std::vector<token> parameters);
std::string if_function(std::vector<token> parameters);

static std::map<std::string, std::function<std::string(std::vector<token>)>>
    function_impl{{"include", include},
                  {"scope", scope},
                  {"echo", echo},
                  {"upper-case", struppercase},
                  {"lower-case", strlowercase},
                  {"newline", [](std::vector<token>) { return "\n"; }},
                  {"eq", equals},
                  {"neq", nequals},
                  {"if", if_function}};
