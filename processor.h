#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "token.h"

extern bool command_mode;

void process(std::string scope, std::string& line);
std::string current_scope();
void invoke_command(std::string command);
void interpret(std::vector<token> tokens);

void set_debug(bool state);
