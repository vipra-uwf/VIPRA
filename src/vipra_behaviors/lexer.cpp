
#include "parsing/lexer.hpp"
#include "parsing/token.hpp"

#include <cwctype>
#include <fstream>
#include <iostream>

namespace VIPRA::Behaviors {

auto Lexer::tokenize(std::string const& str) -> std::vector<Token> {
  std::vector<Token> tokens;
  _source = str;
  _iter = _source.begin();
  _line = 1;
  _col = 1;
  Token token{Token::Type::ASTERISK};

  while ( token.type != Token::Type::END_OF_FILE && token.type != Token::Type::INVALID ) {
    token = parse_token();
    tokens.push_back(token);
  }

  return tokens;
}

auto Lexer::tokenize(std::filesystem::path const& path) -> std::vector<Token> {
  std::ifstream file(path);
  if ( ! file.is_open() ) {
    throw std::runtime_error("Failed to open file: " + path.string());
  }

  file.seekg(0, std::ios::end);
  std::string source(file.tellg(), ' ');
  file.seekg(0);
  file.read(source.data(), static_cast<int>(source.size()));
  file.close();

  // TODO(rolland): ?? not everything should be tolower, any identifiers should keep their case sensitivity
  std::transform(source.begin(), source.end(), source.begin(), [](char chr) { return std::tolower(chr); });

  return tokenize(source);
}

/**
 * @brief Parses a token from the source file
 * 
 * @return Token 
 */
auto Lexer::parse_token() noexcept -> Token {
  if ( _iter >= _source.end() ) {
    return Token(Token::Type::END_OF_FILE, _line, _col);
  }

  while ( *_iter == '/' || std::iswspace(*_iter) ) {
    if ( std::iswspace(*_iter) ) {
      next();
      continue;
    }
    if ( _iter + 1 < _source.end() ) {
      if ( *(_iter + 1) == '/' ) {
        skip_comments(false);
      } else if ( *(_iter + 1) == '*' ) {
        skip_comments(true);
      } else {
        break;
      }
    } else {
      break;
    }
  }

  if ( _iter >= _source.end() ) {
    return Token(Token::Type::END_OF_FILE, _line, _col);
  }

  switch ( *_iter ) {
    case ';':
      next();
      return Token(Token::Type::SEMI_COLON, _line, _col);
      break;
    case ',':
      next();
      return Token(Token::Type::COMMA, _line, _col);
      break;
    case '(':
      next();
      return Token(Token::Type::OPEN_PAREN, _line, _col);
      break;
    case ')':
      next();
      return Token(Token::Type::CLOSE_PAREN, _line, _col);
      break;
    case '{':
      next();
      return Token(Token::Type::OPEN_BRACE, _line, _col);
      break;
    case '}':
      next();
      return Token(Token::Type::CLOSE_BRACE, _line, _col);
      break;
    case '[':
      next();
      return Token(Token::Type::OPEN_BRACKET, _line, _col);
      break;
    case ']':
      next();
      return Token(Token::Type::CLOSE_BRACKET, _line, _col);
      break;
    case ':':
      next();
      return Token(Token::Type::COLON, _line, _col);
      break;
    case '.':
      next();

      if ( std::isdigit(*_iter) ) {
        --_iter;
        return parse_number(false);
      }

      return Token(Token::Type::DOT, _line, _col);
      break;
    case '#':
      next();
      return Token(Token::Type::HASH, _line, _col);
      break;
    case '@':
      next();
      return Token(Token::Type::AT, _line, _col);
      break;
    case '&':
      next();
      return Token(Token::Type::AMPERSAND, _line, _col);
      break;
    case '*':
      next();
      return Token(Token::Type::ASTERISK, _line, _col);
      break;
    case '+':
      next();
      return Token(Token::Type::PLUS, _line, _col);
      break;
    case '-':
      next();
      if ( *_iter == '>' ) {
        next();
        return Token(Token::Type::ARROW, _line, _col);
      }

      if ( std::isdigit(*_iter) ) {
        return parse_number(true);
      }

      return Token(Token::Type::MINUS, _line, _col);
      break;
    case '%':
      next();
      return Token(Token::Type::PERCENT, _line, _col);
      break;
    case '^':
      next();
      return Token(Token::Type::CARET, _line, _col);
      break;
    case '~':
      next();
      return Token(Token::Type::TILDE, _line, _col);
      break;
    case '!':
      next();
      return replace_not_equal();
      break;
    case '?':
      next();
      return Token(Token::Type::QUESTION, _line, _col);
      break;
    case '<':
      next();
      return replace_gt_lt(false);
      break;
    case '>':
      next();
      return replace_gt_lt(true);
      break;
    case '=':
      next();
      return replace_bool_equal();
      break;
    case '/':
      next();
      return Token(Token::Type::SLASH, _line, _col);
      break;
    case '|':
      next();
      return Token(Token::Type::PIPE, _line, _col);
      break;
    case '"':
      next();
      return parse_string();
      break;
    case '\'':
      next();
      return Token(Token::Type::SINGLE_QUOTE, _line, _col);
      break;
    case '\\':
      next();
      return Token(Token::Type::BACKSLASH, _line, _col);
      break;
    case '`':
      next();
      return Token(Token::Type::BACKTICK, _line, _col);
      break;
    case '$':
      next();
      return Token(Token::Type::DOLLAR, _line, _col);
      break;
    case '_':
      next();
      return Token(Token::Type::UNDERSCORE, _line, _col);
      break;
    default: {
      if ( std::isalpha(*_iter) ) {
        return parse_identifier(true);
      }
      if ( std::isdigit(*_iter) ) {
        return parse_number(false);
      }

      return Token(Token::Type::INVALID, _line, _col, std::string(1, *_iter));
    }
  }
}

/**
 * @brief Skips comments
 * 
 * @return char last character after the comment
 */
void Lexer::skip_comments(bool multiline) {
  if ( multiline ) {
    while ( _iter < _source.end() - 1 ) {
      next();
      if ( *_iter == '*' && *(_iter + 1) == '/' ) {
        next();
        next();
        return;
      }
    }
    return;
  }

  while ( _iter < _source.end() && *_iter != '\n' ) {
    next();
  }
}

/**
 * @brief Parses an identifier, returning it as a keyword if it is one
 * 
 * @param *_iter first character of the identifier
 * @return Token resulting token
 */
auto Lexer::parse_identifier(bool allowKeywordSubstitution) noexcept -> Token {
  std::string value;
  value += *_iter;

  next();
  while ( (_iter < _source.end()) && std::isalnum(*_iter) ) {
    value += *_iter;
    next();
  }

  if ( ! allowKeywordSubstitution ) {
    return Token(Token::Type::IDENTIFIER, _line, _col, value);
  }

  auto type = replace_keyword_type(value);
  if ( type == Token::Type::IDENTIFIER ) {
    return Token(type, _line, _col, value);
  }

  return Token(type, _line, _col);
}

/**
 * @brief Parses a number
 * 
 * @param *_iter first character of the number
 * @return Token resulting token
 */
auto Lexer::parse_number(bool negative) noexcept -> Token {
  std::string value = negative ? "-" : "";
  value += *_iter;

  bool flt = false;

  if ( *_iter == '.' ) {
    flt = true;
  }

  next();
  while ( (_iter < _source.end()) && (std::isdigit(*_iter) || *_iter == '.') ) {
    if ( *_iter == '.' ) {
      if ( flt ) {
        return Token(Token::Type::FLOAT, _line, _col, value);
      }
      flt = true;
    }

    value += *_iter;
    next();
  }

  return Token(flt ? Token::Type::FLOAT : Token::Type::INT, _line, _col, value);
}

/**
 * @brief Parses a string
 * 
 * @return Token resulting token
 */
auto Lexer::parse_string() noexcept -> Token {
  std::string value = "\"";

  next();
  while ( (_iter < _source.end()) && *_iter != '"' ) {
    value += *_iter;
    next();
  }

  if ( _iter == _source.end() ) {
    return Token(Token::Type::INVALID, _line, _col, value);
  }

  value += '"';
  return Token(Token::Type::STRING, _line, _col, value);
}

/**
 * @brief Replaces an assignment operator with a boolean equal operator, if it is one
 * 
 * @return Token resulting token
 */
auto Lexer::replace_bool_equal() -> Token {
  if ( _iter >= _source.end() ) return Token(Token::Type::EQUAL, _line, _col);

  if ( *_iter == '=' ) {
    next();
    return Token(Token::Type::BOOL_EQUAL, _line, _col);
  }

  return Token(Token::Type::EQUAL, _line, _col);
}

/**
 * @brief Replaces a greater than or less than operator with a greater than or less than equal operator, if it is one
 * 
 * @param greater whether the operator is a greater than or less than operator
 * @return Token resulting token
 */
auto Lexer::replace_gt_lt(bool greater) -> Token {
  if ( _iter >= _source.end() )
    return greater ? Token(Token::Type::GREATER_THAN, _line, _col)
                   : Token(Token::Type::LESS_THAN, _line, _col);

  if ( *_iter == '=' ) {
    next();
    if ( greater ) {
      return Token(Token::Type::GREATER_THAN_EQUAL, _line, _col);
    }
    return Token(Token::Type::LESS_THAN_EQUAL, _line, _col);
  }

  return greater ? Token(Token::Type::GREATER_THAN, _line, _col) : Token(Token::Type::LESS_THAN, _line, _col);
}

/**
 * @brief Replaces a not equal operator with a not equal operator, if it is one
 * 
 * @return Token resulting token
 */
auto Lexer::replace_not_equal() -> Token {
  if ( _iter >= _source.end() ) return Token(Token::Type::EXCLAMATION, _line, _col);

  if ( *_iter == '=' ) {
    next();
    return Token(Token::Type::NOT_EQUAL, _line, _col);
  }

  return Token(Token::Type::EXCLAMATION, _line, _col);
}

/**
 * @brief Checks if a string is a keyword
 * 
 * @param value string to check
 * @return true if the string is a keyword
 * @return false if the string is not a keyword
 */
auto Lexer::is_keyword(std::string const& value) -> bool {
  return keywords().find(value) != keywords().end();
}

/**
 * @brief Replaces a keyword with its corresponding token type
 * 
 * @param value keyword to replace
 * @return Token::Type resulting token type
 */
auto Lexer::replace_keyword_type(std::string const& value) -> Token::Type {
  auto iter = keywords().find(value);
  if ( iter != keywords().end() ) {
    return iter->second;
  }

  return Token::Type::IDENTIFIER;
}

/**
 * @brief Gets the keyword map
 * 
 * @return const std::map<std::string, Token::Type>& keyword map
 */
auto Lexer::keywords() -> std::map<std::string, Token::Type> const& {
  static const std::map<std::string, Token::Type> KEYWORDS{
      {"selector", Token::Type::SELECTOR}, {"action", Token::Type::ACTION},
      {"location", Token::Type::LOCATION}, {"stimulus", Token::Type::STIMULUS},
      {"response", Token::Type::RESPONSE}, {"target", Token::Type::TARGET},
      {"duration", Token::Type::DURATION}, {"event", Token::Type::EVENT},
      {"select", Token::Type::SELECT},     {"from", Token::Type::FROM},
      {"types", Token::Type::TYPES},       {"name", Token::Type::NAME},
      {"center", Token::Type::CENTER},     {"dimensions", Token::Type::DIMENSIONS},
      {"rotation", Token::Type::ROTATION},
  };
  return KEYWORDS;
}
}  // namespace VIPRA::Behaviors