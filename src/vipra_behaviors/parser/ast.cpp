
#include "vipra/vipra_behaviors/parsing/ast.hpp"

#include "vipra/vipra_behaviors/util/overloaded_visit.hpp"

#include <fmt/format.h>

namespace VIPRA::Behaviors {
/**
 * @brief Converts the AST to a string
 * 
 * @return std::string
 */
auto AST::to_string() const -> std::string {
  std::string result;
  for ( const auto& statement : _statements ) {
    overloaded_visit(
        statement.statement, [&result](const AST::Types& types) { result += types.to_string(0); },
        [&result](const AST::Location& location) { result += location.to_string(0); },
        [&result](const AST::Selector& selector) { result += selector.to_string(0); },
        [&result](const AST::Action& action) { result += action.to_string(0); },
        [&result](std::nullptr_t) { result += "null\n"; });
  }
  return result;
}

/**
 * @brief Returns a reference to the ast statements vector
 * 
 * @return const std::vector<Statement>& 
 */
auto AST::statements() const -> const std::vector<Statement>& { return _statements; }

/**
 * @brief Adds a statement to the AST
 * 
 * @param statement 
 */
void AST::add_statement(Statement&& statement) { _statements.emplace_back(statement); }

/**
 * @brief Converts a binary operator to a string
 * 
 * @param type binary operator
 */
auto AST::op_to_string(BinaryOp type) -> std::string {
  static const std::unordered_map<BinaryOp, std::string> OPERATORS{
      {BinaryOp::ADD, "+"}, {BinaryOp::SUB, "-"}, {BinaryOp::MUL, "*"},  {BinaryOp::DIV, "/"},
      {BinaryOp::MOD, "%"}, {BinaryOp::POW, "^"}, {BinaryOp::GTE, ">="}, {BinaryOp::LTE, "<="},
      {BinaryOp::GT, ">"},  {BinaryOp::LT, "<"},  {BinaryOp::EQ, "=="},  {BinaryOp::NEQ, "!="},
  };
  auto oper = OPERATORS.find(type);
  if ( oper == OPERATORS.end() ) {
    // TODO(rolland): handle this error
    return "!";
  }

  return oper->second;
}

auto AST::begin() -> iterator { return _statements.begin(); }
auto AST::end() -> iterator { return _statements.end(); }
auto AST::begin() const -> const_iterator { return _statements.begin(); }
auto AST::end() const -> const_iterator { return _statements.end(); }

// ----------------------------------------- NODE TYPES ------------------------------------------------------

/**
 * @brief returns the string representation of the expression
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Expression::to_string(int indent) const -> std::string {
  return fmt::format("\n{: >{}}Expression:", "", indent) + (value ? value->to_string(indent + 1) : "");
}

/**
 * @brief returns the string representation of the statement
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Statement::to_string(int indent) const -> std::string {
  return overloaded_visit<std::string>(
      statement, [&](const Types& types) { return types.to_string(indent); },
      [&](const Location& location) { return location.to_string(indent); },
      [&](const Selector& selector) { return selector.to_string(indent); },
      [&](const Action& action) { return action.to_string(indent); },
      [&](std::nullptr_t) { return std::string("null\n"); });
}

/**
 * @brief returns the string representation of the comparison
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Comparison::to_string(int indent) const -> std::string {
  return fmt::format("\n{: >{}}Comparison", "", indent) + left->to_string(indent + 1) + " " +
         (right ? op_to_string(op) + " " + right->to_string(indent + 1) : "");
}

/**
 * @brief returns the string representation of the equality
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Equality::to_string(int indent) const -> std::string {
  return fmt::format("\n{: >{}}Equality:", "", indent) + left->to_string(indent + 1) + " " +
         (right ? (equal ? "== " : "!= ") + right->to_string(indent + 1) : "");
}

/**
 * @brief returns the string representation of the term
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Term::to_string(int indent) const -> std::string {
  return fmt::format("\n{: >{}}Term:", "", indent) + left->to_string(indent + 1) + " " +
         (right ? op_to_string(op) + " " + right->to_string(indent + 1) : "");
}

/**
 * @brief returns the string representation of the factor
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Factor::to_string(int indent) const -> std::string {
  return fmt::format("\n{: >{}}Factor:", "", indent) + left->to_string(indent + 1) + " " +
         (right ? op_to_string(op) + " " + right->to_string(indent + 1) : "");
}

/**
 * @brief returns the string representation of the unary
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Unary::to_string(int indent) const -> std::string {
  return overloaded_visit<std::string>(
      value,
      [&](const std::unique_ptr<Unary>& unary) {
        return fmt::format("\n{: >{}}Unary:", "", indent) + op_to_string(op) + unary->to_string(indent + 1);
      },
      [&](const std::unique_ptr<Primary>& value) {
        return fmt::format("\n{: >{}}Unary:", "", indent) + (value ? value->to_string(indent + 1) : "");
      });
}

/**
 * @brief returns the string representation of the terminal
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Terminal::to_string(int indent) const -> std::string {
  return overloaded_visit<std::string>(
      value, [&](const Token& token) { return fmt::format("\n{: >{}}Terminal:", "", indent) + token.value; },
      [&](const std::unique_ptr<String>& str) {
        return fmt::format("\n{: >{}}Terminal:", "", indent) + str->value;
      });
}

/**
 * @brief returns the string representation of the type
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Types::to_string(int indent) const -> std::string {
  return fmt::format("\n{: >{}}Types: {}", "", indent, fmt::join(types, ", "));
}

/**
 * @brief returns the string representation of the location
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Location::to_string(int indent) const -> std::string {
  // TODO(rolland): Add more to the string
  return fmt::format("\n{: >{}}Location: {}", "", indent, name);
}

/**
 * @brief returns the string representation of the action
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Action::to_string(int indent) const -> std::string {
  // TODO(rolland): Add more to the string
  return fmt::format("\n{: >{}}Action: {}", "", indent, type);
}

/**
 * @brief returns the string representation of the selector
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Selector::to_string(int indent) const -> std::string {
  // TODO(rolland): Add more to the string
  return fmt::format("\n{: >{}}Selector: {}", "", indent, fmt::join(types, ", "));
}

/**
 * @brief returns the string representation of the call
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Call::to_string(int indent) const -> std::string {
  return fmt::format("\n{: >{}}Call: {}", "", indent, name);
}

/**
 * @brief returns the string representation of the primary
 * 
 * @param indent spaces to indent
 * @return std::string string representation
 */
auto AST::Primary::to_string(int indent) const -> std::string {
  return overloaded_visit<std::string>(
      value,
      [&](const std::unique_ptr<Expression>& expression) {
        return fmt::format("\n{: >{}}Primary:", "", indent) + expression->to_string(indent + 1);
      },
      [&](const std::unique_ptr<Terminal>& terminal) {
        return fmt::format("\n{: >{}}Primary:", "", indent) + terminal->to_string(indent + 1);
      },
      [&](const std::unique_ptr<String>& str) {
        return fmt::format("\n{: >{}}Primary:", "", indent) + str->value;
      },
      [&](const std::unique_ptr<Call>& call) {
        return fmt::format("\n{: >{}}Primary:", "", indent) + call->to_string(indent + 1);
      });
}
}  // namespace VIPRA::Behaviors
