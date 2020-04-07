#pragma once
#include "token.h"
#include <string>
#include <vector>

void debug_token(const token& t);
void debug_output(std::string output);
void debug_function_call(std::string function_name,
                         std::vector<token> parameters,
                         std::string return_value);