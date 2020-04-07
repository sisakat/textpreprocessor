#pragma once
#include "token.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/* ************************************ */
/* Operator Types                       */
/* ************************************ */
enum assignment_op_type {
  equal,
  plus_equal,
  minus_equal,
  times_equal,
  divide_equal
};

enum binary_op_type { plus, minus, divide, times };

enum unary_op_type { negate };

/* ************************************ */
/* Operator classes                     */
/* ************************************ */
struct ast_op {
  virtual ~ast_op() = default;
};

struct assignment_op : public ast_op {
  assignment_op_type op = assignment_op_type::equal;
  assignment_op() {}
  assignment_op(assignment_op_type op) : op{op} {}

  friend std::ostream& operator<<(std::ostream& os, const assignment_op& aop);
};

struct binary_op : public ast_op {
  binary_op_type op;
  binary_op() {}
  binary_op(binary_op_type op) : op{op} {}

  friend std::ostream& operator<<(std::ostream& os, const binary_op& bop);
};

struct unary_op : public ast_op {
  unary_op_type op;
  unary_op() {}
  unary_op(unary_op_type op) : op{op} {}

  friend std::ostream& operator<<(std::ostream& os, const unary_op& uop);
};

/* ************************************ */
/* Base constructs                      */
/* ************************************ */
struct statement {
  virtual ~statement() = default;
  virtual void print() const { std::cout << "statement"; }
};

struct expression {
  virtual ~expression() = default;
  virtual void print() const { std::cout << "expression"; }
};

struct variable {
  std::string identifier;
  variable() {}
  variable(std::string identifier) : identifier{identifier} {}

  friend std::ostream& operator<<(std::ostream& os, const variable& var);
};

struct lval : public expression {
  std::unique_ptr<variable> var;
  lval() {}
  lval(std::unique_ptr<variable> var) : var{std::move(var)} {}
  void print() const override;
};

struct block : public statement {
  std::vector<std::unique_ptr<statement>> statements;
  void print() const override;
};

/* ************************************ */
/* Statements                           */
/* ************************************ */
struct ite : public statement {
  std::unique_ptr<expression> condition;
  std::unique_ptr<statement> if_branch;
  std::unique_ptr<statement> else_branch;
  ite(std::unique_ptr<expression> condition,
      std::unique_ptr<statement> if_branch,
      std::unique_ptr<statement> else_branch)
      : condition{std::move(condition)}, if_branch{std::move(if_branch)},
        else_branch{std::move(else_branch)} {}

  void print() const override;
};

struct assignment : public statement {
  assignment_op op;
  std::unique_ptr<lval> lvalue;
  std::unique_ptr<expression> rvalue;

  assignment() {}
  assignment(assignment_op op, std::unique_ptr<lval> lvalue,
             std::unique_ptr<expression> rvalue)
      : op{op}, lvalue{std::move(lvalue)}, rvalue{std::move(rvalue)} {}
  void print() const override;
};

/* ************************************ */
/* Expressions                          */
/* ************************************ */
struct binary : public expression {
  binary_op op;
  std::unique_ptr<expression> left;
  std::unique_ptr<expression> right;
  binary(std::unique_ptr<expression> left, std::unique_ptr<expression> right,
         binary_op op)
      : op{op}, left{std::move(left)}, right{std::move(right)} {}

  void print() const override;
};

struct unary : public expression {
  unary_op op;
  std::unique_ptr<expression> expr;
  unary(std::unique_ptr<expression> expr, unary_op op)
      : op{op}, expr{std::move(expr)} {}

  void print() const override;
};

/* ************************************ */
/* Functions                            */
/* ************************************ */
std::vector<std::unique_ptr<statement>> parse(std::vector<token> tokens);

std::ostream&
operator<<(std::ostream& os,
           const std::vector<std::unique_ptr<statement>>& statements);
