/**
 * @file ast.hpp
 * @author Rolland Goodenough (goodenoughr@gmail.com)
 * @date 2023-12-19
 *
 * @copyright Copyright 2023 Rolland Goodenough
 *
 * This file is part of kuso which is released under the MIT License
 * See file LICENSE for the full License
 */

#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "token.hpp"
#include "vipra/geometry/f3d.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Abstract Syntax Tree class
 * 
 */
class AST {
 public:
  struct Statement;

  struct Selector;
  struct Location;
  struct Event;
  struct Action;

  struct Response;
  struct Target;
  struct Duration;
  struct Types;

  struct Condition;

  struct Expression;
  struct Equality;
  struct Comparison;
  struct Term;
  struct Factor;
  struct Unary;
  struct Primary;
  struct Terminal;
  struct String;
  struct Call;

  struct Value;

  /**
   * @brief Binary operations
   * 
   */
  enum class BinaryOp {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    POW,
    EQ,
    NEQ,
    LT,
    GT,
    LTE,
    GTE,
    NOT,
  };

  using iterator = std::vector<Statement>::iterator;
  using const_iterator = std::vector<Statement>::const_iterator;

  void               add_statement(Statement&&);
  [[nodiscard]] auto statements() const -> const std::vector<Statement>&;
  [[nodiscard]] auto to_string() const -> std::string;

  [[nodiscard]] auto begin() -> iterator;
  [[nodiscard]] auto end() -> iterator;
  [[nodiscard]] auto begin() const -> const_iterator;
  [[nodiscard]] auto end() const -> const_iterator;

 private:
  std::vector<Statement> _statements;

  [[nodiscard]] static auto op_to_string(BinaryOp) -> std::string;
};

/**
 * @brief AST node for holding pedestrian types
 * 
 */
struct AST::Types {
  std::vector<std::string> types;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for holding location information
 * 
 */
struct AST::Location {
  std::string  name;
  VIPRA::f3d   center;
  VIPRA::f3d   dimensions;
  VIPRA::f_pnt rotation;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for conditions
 * 
 */
struct AST::Condition {
  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for function calls
 * 
 */
struct AST::Call {
  enum class Type { ATOM, CONDITION, SELECT };

  std::string name;
  Type        type;
  // TODO(rolland): figure out if these need to be full expressions
  std::vector<std::string> args;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for holding numerical values information
 * 
 */
struct AST::Value {
  enum class Type { EXACT, RANDOM, RANGE };

  Type         type;
  VIPRA::f_pnt min;
  VIPRA::f_pnt max;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

struct AST::Duration {
  AST::Value value;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for holding action information
 * 
 */
struct AST::Action {
  std::string            type;
  std::string            target;
  AST::Condition         condition;
  std::vector<AST::Call> response;
  AST::Value             duration;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

struct AST::Selector {
  std::vector<std::string>   types;
  AST::Call                  select;
  std::optional<std::string> from;
  bool                       required;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

struct AST::Expression {
  std::unique_ptr<Equality> value;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for equality expressions
 * 
 */
struct AST::Equality {
  std::unique_ptr<Comparison> left;
  std::unique_ptr<Equality>   right;
  bool                        equal;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for comparison expressions
 * 
 */
struct AST::Comparison {
  std::unique_ptr<Term>       left;
  std::unique_ptr<Comparison> right;
  BinaryOp                    op;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for term expressions
 * 
 */
struct AST::Term {
  std::unique_ptr<Factor> left;
  std::unique_ptr<Term>   right;
  BinaryOp                op;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for factor expressions
 * 
 */
struct AST::Factor {
  std::unique_ptr<Unary>  left;
  std::unique_ptr<Factor> right;
  BinaryOp                op;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for unary expressions
 * 
 */
struct AST::Terminal {
  std::variant<Token, std::unique_ptr<String>> value;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for unary expressions
 * 
 */
struct AST::Unary {
  std::variant<std::unique_ptr<Unary>, std::unique_ptr<Primary>> value;
  BinaryOp                                                       op;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for primary expressions
 * 
 */
struct AST::Primary {
  std::variant<std::unique_ptr<Terminal>, std::unique_ptr<Expression>, std::unique_ptr<String>,
               std::unique_ptr<Call>>
      value;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for strings
 * 
 */
struct AST::String {
  std::string value;

  [[nodiscard]] auto to_string(int) const -> std::string;
};

/**
 * @brief AST node for statements
 * 
 */
struct AST::Statement {
  std::variant<std::nullptr_t, AST::Types, AST::Action, AST::Location, AST::Selector> statement;

  [[nodiscard]] auto to_string(int) const -> std::string;

  explicit Statement(std::nullptr_t) : statement(nullptr) {}
  explicit Statement(Types&& types) : statement(types) {}
};
}  // namespace VIPRA::Behaviors