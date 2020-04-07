#include "tokenizer.h"
#include "token.h"
#include "util.h"
#include <iomanip>
#include <ostream>
#include <stack>

using namespace std;

vector<token> tokenize(std::string filename, std::string& input) {
  vector<token> tokens;
  int position = 0;
  bool quote = false;
  ostringstream oss;

  for (int i = 0; i < input.length(); i++) {
    char c = input[i];
    token tok;

    if (c == '\"') {
      // if quoted - then this means the quote now ends
      // push the string as constant
      if (quote) {
        tok = token(oss.str(), token_type::constant, filename, position);
        oss.str("");
        oss.clear();
      }
      quote = !quote;
    } else if (quote) {
      oss << c;
    } else if (c == ';') {
      tok = token(";", token_type::symbol, filename, position,
                  token_symbol::semicolon);
    } else if (c == '(') {
      tok = token("(", token_type::symbol, filename, position,
                  token_symbol::lparen);
    } else if (c == ')') {
      tok = token(")", token_type::symbol, filename, position,
                  token_symbol::rparen);
    } else if (c == '{') {
      tok = token("{", token_type::symbol, filename, position,
                  token_symbol::lcurly);
    } else if (c == '}') {
      tok = token("}", token_type::symbol, filename, position,
                  token_symbol::rcurly);
    } else if (c == ',') {
      tok = token(",", token_type::symbol, filename, position,
                  token_symbol::comma);
    } else if (c == '=') {
      if (i + 1 < input.length() && input[i + 1] == '=') {
        tok = token("==", token_type::op, filename, position,
                    token_symbol::equal_equal);
        i += 1;
      } else {
        tok = token("=", token_type::symbol, filename, position,
                    token_symbol::equal);
      }
    } else if (c == '>') {
      if (i + 1 < input.length() && input[i + 1] == '=') {
        tok = token(">=", token_type::op, filename, position,
                    token_symbol::greater_equal);
        i += 1;
      } else {
        tok = token(">", token_type::op, filename, position,
                    token_symbol::greater);
      }
    } else if (c == '<') {
      if (i + 1 < input.length() && input[i + 1] == '=') {
        tok = token("<=", token_type::op, filename, position,
                    token_symbol::smaller_equal);
        i += 1;
      } else {
        tok = token("<", token_type::op, filename, position,
                    token_symbol::smaller);
      }
    } else if (c == '!') {
      if (i + 1 < input.length() && input[i + 1] == '=') {
        tok = token("!=", token_type::op, filename, position,
                    token_symbol::negate_equal);
        i += 1;
      } else {
        tok = token("!", token_type::symbol, filename, position,
                    token_symbol::negate);
      }
    } else if (c == '+') {
      tok = token("+", token_type::op, filename, position, token_symbol::plus);
    } else if (c == '-') {
      tok = token("-", token_type::op, filename, position, token_symbol::minus);
    } else if (c == '*') {
      tok = token("*", token_type::op, filename, position, token_symbol::times);
    } else if (c == '/') {
      tok =
          token("/", token_type::op, filename, position, token_symbol::divide);
    } else if (c != ' ' && c != '\n') {
      oss << c;
    }

    // any operator or symbol interrupting a character sequence
    // is possibly a keyword or an identifier
    string tmp = oss.str();
    if (tmp.length() > 0 &&
        (tok.type == token_type::op || tok.type == token_type::symbol)) {
      if (digits_only(tmp)) {
        tokens.push_back(token(tmp, token_type::constant, filename, position));
      } else {
        if (std::find(keywords.begin(), keywords.end(), tmp) !=
            keywords.end()) {
          tokens.push_back(token(tmp, token_type::keyword, filename, position));
        } else {
          tokens.push_back(
              token(tmp, token_type::identifier, filename, position));
        }
      }
      oss.str("");
      oss.clear();
    }

    // only push token of valid types
    if (tok.type != token_type::unknown)
      tokens.push_back(tok);

    position++;
  }

  return tokens;
}

ostream& operator<<(ostream& os, const vector<token> tokens) {
  for (const token& t : tokens) {
    os << setw(15) << left << t.filename << " " << setw(5) << right
       << t.position << left << " " << setw(10) << t.type << " " << t.value
       << " " << setw(5) << t.symbol << endl;
  }
  return os;
}
