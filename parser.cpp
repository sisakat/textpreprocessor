#include "parser.h"
#include "token.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::cout;
using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;

using StatementsContainer = vector<std::unique_ptr<statement>>;
using TokenContainer = vector<token>;

void parse_expression(StatementsContainer& statements, TokenContainer& tokens,
                      int i) {}

void parse_condition(StatementsContainer& statements, TokenContainer& tokens,
                     int i) {}

int parse_block(StatementsContainer& statements, TokenContainer& tokens, int i) {
  while (i < token.size()) {
    i = parse_statement(statements, tokens, i); 
    i++;
  }
}

int parse_statement(StatementsContainer& statements, TokenContainer& tokens, int i) {
  if (tokens[i].symbol == token_symbol::lcurly) {
    parse_block(statements, tokens, i+1);
  }
}

vector<std::unique_ptr<statement>> parse(vector<token> tokens) {
  vector<std::unique_ptr<statement>> statements;
  int i = 0;
  while (i < token.size()) {
    i = parse_statement(statements, tokens, i); 
    i++;
  }
  return statements;
}

std::ostream&
operator<<(std::ostream& os,
           const std::vector<std::unique_ptr<statement>>& statements) {
  for (const auto& stmt : statements) {
    stmt->print();
  }
  return os;
}

void unary::print() const {
  cout << "unary: ";
  expr->print();
}

void binary::print() const {
  cout << "binary: " << op << " ";
  left->print();
  cout << " ";
  right->print();
}

void assignment::print() const {
  cout << "assignment: " << op << " ";
  lvalue->print();
  cout << " ";
  rvalue->print();
}

void ite::print() const {
  cout << "ite: ";
  condition->print();
  cout << " ";
  if_branch->print();
}

void block::print() const {
  for (const auto& stmt : statements) {
    stmt->print();
  }
}

void lval::print() const { cout << *var; }

std::ostream& operator<<(std::ostream& os, const variable& var) {
  os << var.identifier;
  return os;
}
