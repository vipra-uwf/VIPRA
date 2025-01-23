#pragma once

#include <filesystem>
#include <map>
#include <vector>

#include "token.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Lexer class
 * 
 */
class Lexer {
  using cstr_iter = std::string_view::const_iterator;

 public:
  std::string_view _source;
  cstr_iter        _iter{};
  int              _line{1};
  int              _col{1};

  [[nodiscard]] auto tokenize(std::filesystem::path const&) -> std::vector<Token>;
  [[nodiscard]] auto tokenize(std::string const&) -> std::vector<Token>;

 private:
  [[nodiscard]] static auto is_keyword(std::string const&) -> bool;
  [[nodiscard]] static auto keywords() -> std::map<std::string, Token::Type> const&;
  [[nodiscard]] static auto replace_keyword_type(std::string const&) -> Token::Type;

  [[nodiscard]] auto replace_bool_equal() -> Token;
  [[nodiscard]] auto replace_gt_lt(bool) -> Token;
  [[nodiscard]] auto replace_not_equal() -> Token;

  void               skip_comments(bool);
  [[nodiscard]] auto parse_identifier(bool) noexcept -> Token;
  [[nodiscard]] auto parse_number(bool) noexcept -> Token;
  [[nodiscard]] auto parse_string() noexcept -> Token;
  [[nodiscard]] auto parse_token() noexcept -> Token;
  [[nodiscard]] auto parse_asm() noexcept -> Token;

  inline constexpr auto next() -> bool {
    if ( *_iter == '\n' ) {
      _line++;
      _col = 1;
    } else {
      _col++;
    }
    _iter++;

    if ( _iter >= _source.end() ) {
      _iter = _source.end();
    }

    return _iter < _source.end();
  }
};
}  // namespace VIPRA::Behaviors