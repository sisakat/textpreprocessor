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

void parse_identifier(StatementsContainer& statements, TokenContainer& tokens,
                      int i) {
  unique_ptr<variable> var = make_unique<variable>(tokens[i].value);
  assignment_op aop(assignment_op_type::equal);
  if (tokens[i + 1].type != token_type::op &&
      tokens[i + 1].type != token_type::symbol) {
    std::cerr << "Expected operator at " << tokens[i + 1].position << " in "
              << tokens[i + 1].filename << std::endl;
    exit(1);
  } else {
    string val = tokens[i + 1].value;
    if (val == "+=") {
      aop = assignment_op(assignment_op_type::plus_equal);
    } else if (val == "-=") {
      aop = assignment_op(assignment_op_type::minus_equal);
    } else if (val == "*=") {
      aop = assignment_op(assignment_op_type::times_equal);
    } else if (val == "/=") {
      aop = assignment_op(assignment_op_type::divide_equal);
    } else if (val != "=") {
      parse_condition(statements, tokens, i);
    }
  }

  auto lval_var = make_unique<lval>(move(var));
  auto expr = make_unique<expression>();
  statements.push_back(
      std::make_unique<assignment>(aop, move(lval_var), move(expr)));
}

vector<std::unique_ptr<statement>> parse(vector<token> tokens) {
  vector<std::unique_ptr<statement>> statements;
  int i = 0;

  while (i < tokens.size()) {
    if (tokens[i].type == token_type::identifier) {
      parse_identifier(statements, tokens, i);
    }

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

std::ostream& operator<<(std::ostream& os, const assignment_op& aop) {
  os << aop.op;
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

std::ostream& operator<<(std::ostream& os, const unary_op& uop) {
  os << uop.op;
  return os;
}

std::ostream& operator<<(std::ostream& os, const binary_op& bop) {
  os << bop.op;
  return os;
}
