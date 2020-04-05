#pragma once
#include "token.h"
#include <sstream>
#include <string>
#include <vector>
#include <ostream>

std::vector<token> tokenize(std::string filename, std::string& input);
std::ostream& operator<<(std::ostream& os, const std::vector<token> tokens);
void set_debug(bool state);
