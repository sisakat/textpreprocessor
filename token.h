#pragma once
#include "functions.h"
#include <algorithm>
#include <ostream>
#include <string>

enum token_type { literal, op, function, variable };

inline std::ostream& operator<<(std::ostream& os, const token_type t) {
  switch (t) {
  case token_type::literal:
    os << "literal";
    break;
  case token_type::op:
    os << "operator";
    break;
  case token_type::function:
    os << "function";
    break;
  case token_type::variable:
    os << "variable";
    break;
  default:
    os << "generic";
    break;
  }
  return os;
}

struct token {
  token_type type;
  std::string value;
  token(std::string value) : value{value} { tokenize(); }

  token(std::string value, token_type type) : value{value}, type{type} {}

  void tokenize() {
    if (value[0] == '\"') {
      value = value.substr(1, value.length() - 2);
      type = token_type::literal;
    } else if (std::find(functions.begin(), functions.end(), value) !=
               functions.end()) {
      type = token_type::function;
    }
  }
};
