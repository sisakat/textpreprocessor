#pragma once
#include "token.h"
#include <sstream>
#include <string>
#include <vector>

extern bool command_mode;

void process(std::string scope, std::string& line);
std::string current_scope();
void invoke_command(std::string command);
void interpret(std::vector<token>& tokens, int start_index);

void set_debug(bool state);
