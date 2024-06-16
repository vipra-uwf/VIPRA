#pragma once

#include <string>
#include <unordered_map>

namespace VIPRA::Behaviors {
/**
 * @brief Token class
 * 
 */
struct Token {
  /**
   * @brief Token types
   * 
   */
  enum class Type {
    INVALID,

    // ----------------- KEYWORDS ---------------------
    SELECTOR,
    ACTION,
    LOCATION,
    EVENT,
    TYPES,

    // ------------------ ATTRIBUTES -----------------------
    STIMULUS,
    RESPONSE,
    TARGET,
    DURATION,
    NAME,
    CENTER,
    DIMENSIONS,
    ROTATION,
    SELECT,
    FROM,

    // ------------------ SPECIAL ----------------------
    ATOM,
    LOC,
    EVNT,

    // ------------------ COMMON ----------------------
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    INT,
    FLOAT,
    STRING,
    END_OF_FILE,
    SEMI_COLON,
    COMMA,
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    COLON,
    DOT,
    ARROW,
    HASH,
    AT,
    AMPERSAND,
    ASTERISK,
    PLUS,
    MINUS,
    SLASH,
    PERCENT,
    CARET,
    TILDE,
    EXCLAMATION,
    QUESTION,
    LESS_THAN,
    GREATER_THAN,
    EQUAL,
    BOOL_EQUAL,
    NOT_EQUAL,
    LESS_THAN_EQUAL,
    GREATER_THAN_EQUAL,
    PIPE,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    BACKSLASH,
    BACKTICK,
    DOLLAR,
    UNDERSCORE,
    COUNT,
  };

  Type        type{Type::END_OF_FILE};
  int         line{0};
  int         column{0};
  std::string value;

  explicit Token(Type type, int lineNum = 0, int colNum = 0, std::string value = "") noexcept
      : type(type), line(lineNum), column(colNum), value(std::move(value)) {}
  Token() noexcept = default;
  Token(Token const&) noexcept = default;
  Token(Token&&) noexcept = default;
  auto operator=(Token const&) noexcept -> Token& = default;
  auto operator=(Token&&) noexcept -> Token& = default;
  ~Token() noexcept = default;
};

/**
 * @brief Convert token to string
 * 
 * @param token Token to convert
 * @return std::string String representation of token
 */
inline auto to_string(Token const& token) -> std::string {
  static const std::unordered_map<Token::Type, std::string> TYPE_MAP{
      {Token::Type::INVALID, "INVALID"},
      {Token::Type::SELECTOR, "SELECTOR"},
      {Token::Type::ACTION, "ACTION"},
      {Token::Type::LOCATION, "LOCATION"},
      {Token::Type::EVENT, "EVENT"},
      {Token::Type::TYPES, "TYPES"},
      {Token::Type::STIMULUS, "STIMULUS"},
      {Token::Type::RESPONSE, "RESPONSE"},
      {Token::Type::TARGET, "TARGET"},
      {Token::Type::DURATION, "DURATION"},
      {Token::Type::NAME, "NAME"},
      {Token::Type::CENTER, "CENTER"},
      {Token::Type::DIMENSIONS, "DIMENSIONS"},
      {Token::Type::ROTATION, "ROTATION"},
      {Token::Type::SELECT, "SELECT"},
      {Token::Type::FROM, "FROM"},
      {Token::Type::ATOM, "ATOM"},
      {Token::Type::IDENTIFIER, "IDENTIFIER"},
      {Token::Type::KEYWORD, "KEYWORD"},
      {Token::Type::OPERATOR, "OPERATOR"},
      {Token::Type::INT, "INT"},
      {Token::Type::FLOAT, "FLOAT"},
      {Token::Type::STRING, "STRING"},
      {Token::Type::END_OF_FILE, "END_OF_FILE"},
      {Token::Type::SEMI_COLON, "SEMI_COLON"},
      {Token::Type::COMMA, "COMMA"},
      {Token::Type::OPEN_PAREN, "OPEN_PAREN"},
      {Token::Type::CLOSE_PAREN, "CLOSE_PAREN"},
      {Token::Type::OPEN_BRACE, "OPEN_BRACE"},
      {Token::Type::CLOSE_BRACE, "CLOSE_BRACE"},
      {Token::Type::OPEN_BRACKET, "OPEN_BRACKET"},
      {Token::Type::CLOSE_BRACKET, "CLOSE_BRACKET"},
      {Token::Type::COLON, "COLON"},
      {Token::Type::DOT, "DOT"},
      {Token::Type::ARROW, "ARROW"},
      {Token::Type::HASH, "HASH"},
      {Token::Type::AT, "AT"},
      {Token::Type::AMPERSAND, "AMPERSAND"},
      {Token::Type::ASTERISK, "ASTERISK"},
      {Token::Type::PLUS, "PLUS"},
      {Token::Type::MINUS, "MINUS"},
      {Token::Type::SLASH, "SLASH"},
      {Token::Type::PERCENT, "PERCENT"},
      {Token::Type::CARET, "CARET"},
      {Token::Type::TILDE, "TILDE"},
      {Token::Type::EXCLAMATION, "EXCLAMATION"},
      {Token::Type::QUESTION, "QUESTION"},
      {Token::Type::LESS_THAN, "LESS_THAN"},
      {Token::Type::GREATER_THAN, "GREATER_THAN"},
      {Token::Type::EQUAL, "EQUAL"},
      {Token::Type::BOOL_EQUAL, "BOOL_EQUAL"},
      {Token::Type::NOT_EQUAL, "NOT_EQUAL"},
      {Token::Type::LESS_THAN_EQUAL, "LESS_THAN_EQUAL"},
      {Token::Type::GREATER_THAN_EQUAL, "GREATER_THAN_EQUAL"},
      {Token::Type::PIPE, "PIPE"},
      {Token::Type::DOUBLE_QUOTE, "DOUBLE_QUOTE"},
      {Token::Type::SINGLE_QUOTE, "SINGLE_QUOTE"},
      {Token::Type::BACKSLASH, "BACKSLASH"},
      {Token::Type::BACKTICK, "BACKTICK"},
      {Token::Type::DOLLAR, "DOLLAR"},
      {Token::Type::UNDERSCORE, "UNDERSCORE"},
  };
  auto type = TYPE_MAP.find(token.type);

  if ( type == TYPE_MAP.end() ) {
    return "Token { type: UNKNOWN value: " + token.value + "}";
  }

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  return "Token { type: " + type->second + " value: " + token.value + "}";
}
}  // namespace VIPRA::Behaviors