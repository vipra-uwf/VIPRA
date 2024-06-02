
#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "parsing/ast.hpp"
#include "parsing/parser.hpp"
#include "parsing/token.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief Prints a syntax error and exits the program
 * 
 * @param token token found
 * @param expected expected token
 */
void Parser::syntax_error(const Token& token, const Token& expected) {
  throw ParseError("Syntax Error: Line " + std::to_string(token.line) + " Column " +
                   std::to_string(token.column) + "\nExpected: " + to_string(expected) +
                   "\nFound: " + to_string(token));
}

void Parser::semantic_error(Token const& token, std::string const& message) {
  throw ParseError("Semantic Error: Line " + std::to_string(token.line) + " Column " +
                   std::to_string(token.column) + "\n" + message);
}

/**
 * @brief Constructs an AST from a list of tokens
 * 
 * @return AST 
 */
auto Parser::parse(const std::filesystem::path& sourcepath) -> std::optional<AST> {
  // std::cout << "parse\n";
  AST    ast;
  Tokens tokens = Tokens(_lexer.tokenize(sourcepath));

  consume(tokens);
  auto token = consume(tokens);

  try {
    while ( tokens.has_next() ) {
      if ( _lookahead.type == Token::Type::END_OF_FILE ) {
        break;
      }
      ast.add_statement(parse_statement(token, tokens));
    }
  } catch ( const ParseError& e ) {
    // Logging::error(e.what());
    return std::nullopt;
  }

  return ast;
}

auto Parser::parse(std::vector<Token> const& tokens) -> std::optional<AST> {
  // std::cout << "parse\n";
  AST ast;

  auto tokenGen = Tokens(tokens);

  consume(tokenGen);
  auto token = consume(tokenGen);

  try {
    while ( tokenGen.has_next() ) {
      if ( _lookahead.type == Token::Type::END_OF_FILE ) {
        break;
      }
      ast.add_statement(parse_statement(token, tokenGen));
    }
  } catch ( const ParseError& e ) {
    // Logging::error(e.what());
    return std::nullopt;
  }

  return ast;
}

/**
 * @brief Matches a token, if it doesn't match it prints a syntax error and exits the program
 * 
 * @param types list of types to match
 * @param token token found
 * @param tokens list of tokens
 */
void Parser::match(std::initializer_list<Token::Type> types, Token& token, Tokens& tokens) {
  for ( auto type : types ) {
    if ( _lookahead.type == type ) {
      token = consume(tokens);
      return;
    }
  }

  syntax_error(_lookahead, Token(*types.begin()));
}

/**
 * @brief Tries to match a token, if it matches it consumes the token and returns true, otherwise it
 * returns false
 * 
 * @param types list of types to match
 * @param token token found
 * @param tokens list of tokens
 * @return true 
 * @return false 
 */
auto Parser::try_match(std::initializer_list<Token::Type> types, Token& token, Tokens& tokens) -> bool {
  for ( auto type : types ) {
    if ( _lookahead.type == type ) {
      token = consume(tokens);
      return true;
    }
  }

  return false;
}

/**
 * @brief Consumes a token from the list of tokens
 * 
 * @param tokens list of tokens
 * @return Token 
 */
auto Parser::consume(Tokens& tokens) -> Token {
  Token temp = _lookahead;
  if ( tokens.has_next() ) _lookahead = tokens.next();
  return temp;
}

/**
 * @brief Parses a statement
 *
 * @param token token found
 * @param tokens list of tokens
 * @return AST::Statement
 */
auto Parser::parse_statement(Token& token, Tokens& tokens) -> AST::Statement {
  AST::Statement statement{nullptr};

  switch ( token.type ) {
    case Token::Type::TYPES:
      statement.statement = parse_types(token, tokens);
      break;
    case Token::Type::LOCATION:
      statement.statement = parse_location(token, tokens);
      break;
    case Token::Type::SELECTOR:
      statement.statement = parse_selector(token, tokens);
      break;
    // case Token::Type::ACTION:
    //   // statement.statement = parse_action(token, tokens);
    //   break;
    // case Token::Type::EVENT:
    //   // statement.statement = parse_event(token, tokens);
    //   break;
    default:
      syntax_error(token, Token(Token::Type::ACTION));
  }

  match({Token::Type::DOT}, token, tokens);
  token = consume(tokens);
  return statement;
}

auto Parser::parse_types(Token& token, Tokens& tokens) -> AST::Types {
  /* Types Rule
  Types:
    <ID> (, <ID>)*
  .
  */

  AST::Types types;

  match({Token::Type::COLON}, token, tokens);
  match({Token::Type::IDENTIFIER}, token, tokens);

  types.types.push_back(token.value);

  if ( try_match({Token::Type::COMMA}, token, tokens) ) {
    match({Token::Type::IDENTIFIER}, token, tokens);
    types.types.push_back(token.value);
  }

  return types;
}

auto Parser::parse_location(Token& token, Tokens& tokens) -> AST::Location {
  /* Location Rule
    Location:
      Name: <ID>
      Center: <F3D>
      Dimensions: <F3D>
      Rotation: <FLOAT>
    .
  */

  AST::Location location;

  match({Token::Type::COLON}, token, tokens);

  while ( ! try_match({Token::Type::DOT}, token, tokens) ) {
    match({Token::Type::NAME, Token::Type::CENTER, Token::Type::DIMENSIONS, Token::Type::ROTATION}, token,
          tokens);
    switch ( token.type ) {
      case Token::Type::NAME:
        match({Token::Type::COLON}, token, tokens);
        match({Token::Type::IDENTIFIER}, token, tokens);
        location.name = token.value;
        break;
      case Token::Type::CENTER:
        match({Token::Type::COLON}, token, tokens);
        location.center = parse_f3d(token, tokens);
        break;
      case Token::Type::DIMENSIONS:
        match({Token::Type::COLON}, token, tokens);
        location.dimensions = parse_f3d(token, tokens);
        break;
      case Token::Type::ROTATION:
        match({Token::Type::COLON}, token, tokens);
        match({Token::Type::FLOAT, Token::Type::INT}, token, tokens);
        location.rotation = static_cast<VIPRA::f_pnt>(std::stod(token.value));
        break;
      default:
        // Should never reach here
        syntax_error(token, Token(Token::Type::NAME));
    }
  }

  return location;
}

auto Parser::parse_selector(Token& token, Tokens& tokens) -> AST::Selector {
  /* Selector Rule
    Selector:
      Required
      Types: <ID_LIST>
      Condition: <Condition>
      From: <ID>
    .
  */

  AST::Selector selector;

  std::array<bool, 3> found{false, false, false};

  match({Token::Type::COLON}, token, tokens);
  while ( _lookahead.type != Token::Type::DOT ) {
    match({Token::Type::NAME, Token::Type::SELECT, Token::Type::FROM, Token::Type::IDENTIFIER}, token,
          tokens);
    switch ( token.type ) {
      case Token::Type::TYPES:
        // SELECTOR Types
        if ( found[0] ) semantic_error(token, "Duplicate Selector Types");

        match({Token::Type::COLON}, token, tokens);
        selector.types = parse_id_list(token, tokens);

        found[0] = true;
        break;
      case Token::Type::SELECT:
        // SELECTOR Select
        if ( found[1] ) semantic_error(token, "Duplicate Selector Select");

        match({Token::Type::COLON}, token, tokens);
        selector.select = parse_select(token, tokens);

        found[1] = true;
        break;
      case Token::Type::FROM:
        // SELECTOR From
        if ( found[2] ) semantic_error(token, "Duplicate Selector From");

        match({Token::Type::COLON}, token, tokens);
        match({Token::Type::IDENTIFIER}, token, tokens);
        selector.from = token.value;

        found[2] = true;
        break;
      case Token::Type::IDENTIFIER:
        // SELECTOR Required
        if ( token.value == "required" ) {
          if ( selector.required ) semantic_error(token, "Duplicate Selector Required");

          selector.required = true;

        } else {
          syntax_error(token, Token(Token::Type::IDENTIFIER, token.line, token.column, "required"));
        }

      default:
        // Should never reach here
        syntax_error(token, Token(Token::Type::NAME));
    }
  }

  return selector;
}

auto Parser::parse_select(Token& token, Tokens& tokens) -> AST::Call {
  /* Select Rule
    select: [ <FLOAT> % | everyone | exactly <INT> | in <LOC> ]
  */

  AST::Call call;

  match({Token::Type::OPEN_BRACKET}, token, tokens);
  match({Token::Type::FLOAT, Token::Type::INT, Token::Type::IDENTIFIER}, token, tokens);

  if ( token.type == Token::Type::FLOAT || token.type == Token::Type::INT ) {
    // Percent
    call.name = token.value;
    call.type = AST::Call::Type::SELECT;
  } else {
    if ( token.value == "everyone" ) {
      // Everyone
      call.name = "everyone";
      call.type = AST::Call::Type::SELECT;
    } else if ( token.value == "exactly" ) {
      // Exactly N
      match({Token::Type::INT}, token, tokens);
      call.name = "exactly_n";
      call.args.push_back(token.value);
      call.type = AST::Call::Type::SELECT;
    } else if ( token.value == "in" ) {
      // In Location
      match({Token::Type::IDENTIFIER}, token, tokens);
      call.name = "location";
      call.type = AST::Call::Type::SELECT;
    } else {
      syntax_error(token, Token(Token::Type::FLOAT));
    }
  }

  return call;
}

auto Parser::parse_f3d(Token& token, Tokens& tokens) -> VIPRA::f3d {
  /* F3D Rule
    { <FLOAT> (, <FLOAT> (, <FLOAT>)) }
  */

  VIPRA::f3d value;

  match({Token::Type::OPEN_BRACE}, token, tokens);

  match({Token::Type::FLOAT, Token::Type::INT}, token, tokens);
  value.x = static_cast<VIPRA::f_pnt>(std::stod(token.value));

  if ( try_match({Token::Type::COMMA}, token, tokens) ) {
    match({Token::Type::FLOAT, Token::Type::INT}, token, tokens);
    value.y = static_cast<VIPRA::f_pnt>(std::stod(token.value));

    if ( try_match({Token::Type::COMMA}, token, tokens) ) {
      match({Token::Type::FLOAT, Token::Type::INT}, token, tokens);
      value.z = static_cast<VIPRA::f_pnt>(std::stod(token.value));
    }
  }

  return value;
}

auto Parser::parse_id_list(Token& token, Tokens& tokens) -> std::vector<std::string> {
  /* ID List Rule
    <ID> (, <ID>)*
  */

  std::vector<std::string> ids;

  match({Token::Type::IDENTIFIER}, token, tokens);
  ids.push_back(token.value);

  while ( try_match({Token::Type::COMMA}, token, tokens) ) {
    match({Token::Type::IDENTIFIER}, token, tokens);
    ids.push_back(token.value);
  }

  return ids;
}

}  // namespace VIPRA::Behaviors