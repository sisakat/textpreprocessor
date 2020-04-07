#pragma once
#include "token.h"
#include <memory>
#include <string>
#include <vector>

enum assignment_op_type {
  equal,
  plus_equal,
  minus_equal,
  times_equal,
  divide_equal
};

enum binary_op_type { plus, minus, divide, times };

enum unary_op_type { negate };

struct ast_op {
  virtual ~ast_op() = 0;
};

struct assignment_op : public ast_op {
  assignment_op_type op = assignment_op_type::equal;
  assignment_op(assignment_op_type op) : op{op} {}
};

struct binary_op : public ast_op {
  binary_op_type op;
  binary_op(binary_op_type op) : op{op} {}
};

struct unary_op : public ast_op {
  unary_op_type op;
  unary_op(unary_op_type op) : op{op} {}
};

struct statement {
  virtual ~statement() = 0;
};

struct expression {
  virtual ~expression() = 0;
};

struct variable {
  std::string identifier;
  variable(std::string identifier) : identifier{identifier} {}
};

struct lval : public expression {
  variable var;
  lval(variable var) : var{var} {}
};

struct block : public statement {
  std::vector<statement> statements;
};

struct assignment : public statement {
  assignment_op op;
  lval lvalue;
  std::unique_ptr<expression> rvalue;
};

struct binary : public expression {
  binary_op op;
  std::unique_ptr<expression> left;
  std::unique_ptr<expression> right;
  binary(std::unique_ptr<expression> left, std::unique_ptr<expression> right,
         binary_op op)
      : op{op}, left{std::move(left)}, right{std::move(right)} {}
};

struct unary : public expression {
  unary_op op;
  std::unique_ptr<expression> expr;
  unary(std::unique_ptr<expression> expr, unary_op op)
      : op{op}, expr{std::move(expr)} {}
};

struct ite : public statement {
  std::unique_ptr<expression> condition;
  std::unique_ptr<statement> if_branch;
  std::unique_ptr<statement> else_branch;
  ite(std::unique_ptr<expression> condition,
      std::unique_ptr<statement> if_branch,
      std::unique_ptr<statement> else_branch)
      : condition{std::move(condition)}, if_branch{std::move(if_branch)},
        else_branch{std::move(else_branch)} {}
};
