#pragma once

#include <initializer_list>
#include <optional>

#include "ast.hpp"
#include "lexer.hpp"

namespace VIPRA::Behaviors {
/**
  * @brief Parser class
  * 
  */
class Parser {
  class Tokens {
    // NOLINTNEXTLINE
    std::vector<Token> const& _tokens;
    size_t                    _index{0};

   public:
    explicit Tokens(std::vector<Token> const& tokens) : _tokens(tokens) {}

    [[nodiscard]] auto current() const -> Token { return _tokens[_index]; }
    [[nodiscard]] auto next() -> Token { return _tokens[_index++]; }
    [[nodiscard]] auto peek() const -> Token { return _tokens[_index + 1]; }
    [[nodiscard]] auto has_next() const -> bool { return _index >= _tokens.size(); }
  };

 public:
  [[nodiscard]] auto parse(const std::filesystem::path&) -> std::optional<AST>;
  [[nodiscard]] auto parse(const std::vector<Token>&) -> std::optional<AST>;

  struct ParseError : public std::runtime_error {
    explicit ParseError(const std::string& what) : std::runtime_error(what) {}
  };

 private:
  Lexer _lexer;
  Token _lookahead;

  auto try_match(std::initializer_list<Token::Type>, Token&, Tokens&) -> bool;
  void match(std::initializer_list<Token::Type>, Token&, Tokens&);
  auto consume(Tokens&) -> Token;

  [[noreturn]] static void syntax_error(Token const&, Token const&);
  [[noreturn]] static void semantic_error(Token const&, std::string const&);

  [[nodiscard]] auto parse_statement(Token&, Tokens&) -> AST::Statement;
  [[nodiscard]] auto parse_types(Token&, Tokens&) -> AST::Types;
  [[nodiscard]] auto parse_location(Token&, Tokens&) -> AST::Location;
  [[nodiscard]] auto parse_selector(Token&, Tokens&) -> AST::Selector;

  [[nodiscard]] auto parse_f3d(Token&, Tokens&) -> VIPRA::f3d;
  [[nodiscard]] auto parse_select(Token&, Tokens&) -> AST::Call;
  [[nodiscard]] auto parse_condition(Token&, Tokens&) -> AST::Condition;

  [[nodiscard]] auto parse_id_list(Token&, Tokens&) -> std::vector<std::string>;
};
}  // namespace VIPRA::Behaviors