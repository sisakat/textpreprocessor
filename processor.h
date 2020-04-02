#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "token.h"

void process(std::string scope, std::string& line);
std::string current_scope();
void invoke_command(std::string command);
void interpret(std::vector<token> tokens);
