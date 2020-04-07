#pragma once
#include "keywords.h"
#include <algorithm>
#include <ostream>
#include <string>

enum token_type { constant, keyword, op, identifier, symbol, unknown };
enum token_symbol {
  lparen,
  rparen,
  lcurly,
  rcurly,
  lbracket,
  rbracket,
  semicolon,
  comma,
  plus,
  minus,
  divide,
  times,
  equal,
  plus_equal,
  minus_equal,
  divide_equal,
  times_equal,
  equal_equal,
  negate_equal,
  negate,
  greater,
  greater_equal,
  smaller,
  smaller_equal
};

inline std::ostream& operator<<(std::ostream& os, const token_type t) {
  switch (t) {
  case token_type::constant:
    os << "constant";
    break;
  case token_type::op:
    os << "operator";
    break;
  case token_type::keyword:
    os << "keyword";
    break;
  case token_type::identifier:
    os << "identifier";
    break;
  case token_type::symbol:
    os << "symbol";
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
  std::string filename;
  int position;
  token_symbol symbol;
  token() : type{token_type::unknown} {}
  token(std::string value, token_type type, std::string filename, int position)
      : value{value}, type{type}, filename{filename}, position{position} {}
  token(std::string value, token_type type, std::string filename, int position,
        token_symbol symbol)
      : token(value, type, filename, position) {
      this->symbol = symbol;
      }
};

inline std::ostream& operator<<(std::ostream& os, const token& t) {
  os << t.type;
  return os;
}
