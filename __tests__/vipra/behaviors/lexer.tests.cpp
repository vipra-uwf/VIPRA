
#include <random>

#include <gtest/gtest.h>

#include "vipra/vipra_behaviors/parsing/lexer.hpp"

namespace vb = VIPRA::Behaviors;

TEST(Lexer, EmptyFile) {
  std::string            input;
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 1);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, FuzzTest) {
  constexpr int ITERATIONS = 5000;
  constexpr int LENGTH = 100;

  std::random_device              rnd;
  std::default_random_engine      gen(rnd());
  std::uniform_int_distribution<> dis(1, LENGTH);

  vb::Lexer lexer;

  for ( int i = 0; i < ITERATIONS; i++ ) {
    std::string input;
    int         length = dis(gen);

    for ( int j = 0; j < length; j++ ) {
      input += static_cast<char>(dis(gen));
    }

    ASSERT_NO_THROW(std::vector<vb::Token> tokens = lexer.tokenize(input));
  }
}

TEST(Lexer, SingleIdentifier) {
  std::string            input("x");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "x");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, MultipleIdentifiers) {
  std::string            input("x y z");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 4);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "x");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[1].value, "y");
  ASSERT_EQ(tokens[2].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[2].value, "z");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, AssignmentStatement) {
  std::string            input("x = 10;");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 5);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "x");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[2].value, "10");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, ArithmeticExpression) {
  std::string            input("x = 5 + 3 * 2;");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 9);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "x");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[2].value, "5");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::PLUS);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[4].value, "3");
  ASSERT_EQ(tokens[5].type, vb::Token::Type::ASTERISK);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[6].value, "2");
  ASSERT_EQ(tokens[7].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[8].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, SingleLineComment) {
  std::string            input("// This is a comment");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 1);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, MultiLineComment) {
  std::string            input("/* This is a comment\nThis is the second line */");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 1);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, SingleLineCommentWithNewLine) {
  std::string            input("// This is a comment\n");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 1);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, MultiLineCommentWithNewLine) {
  std::string            input("/* This is a comment\nThis is the second line */\n");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 1);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, SingleLineCommentWithNewLineAndCode) {
  std::string            input("// This is a comment\na : int = 0;");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 7);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "a");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::COLON);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[2].value, "int");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[4].value, "0");
  ASSERT_EQ(tokens[5].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, ExpressionWithSlash) {
  std::string            input("x = 5 / 3;");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 7);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "x");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[2].value, "5");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::SLASH);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[4].value, "3");
  ASSERT_EQ(tokens[5].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, EmptyCommentAtEnd) {
  std::string            input("// This is a comment\n//");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 1);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, WhiteSpaceAtEnd) {
  std::string            input("x = 5 + 3 * 2;   ");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 9);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "x");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[2].value, "5");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::PLUS);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[4].value, "3");
  ASSERT_EQ(tokens[5].type, vb::Token::Type::ASTERISK);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[6].value, "2");
  ASSERT_EQ(tokens[7].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[8].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, NewLineAtEnd) {
  std::string            input("x = 5 + 3 * 2;\n");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 9);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "x");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[2].value, "5");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::PLUS);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[4].value, "3");
  ASSERT_EQ(tokens[5].type, vb::Token::Type::ASTERISK);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[6].value, "2");
  ASSERT_EQ(tokens[7].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[8].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, MultiLineCommentWithNewLineAndCode) {
  std::string            input("/* This is a comment\nThis is the second line */\na : int = 0;");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 7);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "a");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::COLON);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[2].value, "int");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[4].value, "0");
  ASSERT_EQ(tokens[5].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, SingleLineCommentWithNewLineAndCodeAndNewLine) {
  std::string            input("// This is a comment\na : int = 0;\n");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 7);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "a");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::COLON);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[2].value, "int");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[4].value, "0");
  ASSERT_EQ(tokens[5].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, MultiLineCommentWithNewLineAndCodeAndNewLine) {
  std::string            input("/* This is a comment\nThis is the second line */\na : int = 0;\n");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 7);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "a");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::COLON);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[2].value, "int");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[4].value, "0");
  ASSERT_EQ(tokens[5].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, SingleLineCommentWithNewLineAndCodeAndNewLineAndComment) {
  std::string            input("// This is a comment\na : int = 0;\n// This is a comment");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 7);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "a");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::COLON);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[2].value, "int");
  ASSERT_EQ(tokens[3].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[4].value, "0");
  ASSERT_EQ(tokens[5].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, Keywords) {
  std::string input(
      "selector action location event types stimulus response target duration name center dimensions "
      "rotation select from");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 16);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::SELECTOR);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::ACTION);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::LOCATION);
  ASSERT_EQ(tokens[3].type, vb::Token::Type::EVENT);
  ASSERT_EQ(tokens[4].type, vb::Token::Type::TYPES);
  ASSERT_EQ(tokens[5].type, vb::Token::Type::STIMULUS);
  ASSERT_EQ(tokens[6].type, vb::Token::Type::RESPONSE);
  ASSERT_EQ(tokens[7].type, vb::Token::Type::TARGET);
  ASSERT_EQ(tokens[8].type, vb::Token::Type::DURATION);
  ASSERT_EQ(tokens[9].type, vb::Token::Type::NAME);
  ASSERT_EQ(tokens[10].type, vb::Token::Type::CENTER);
  ASSERT_EQ(tokens[11].type, vb::Token::Type::DIMENSIONS);
  ASSERT_EQ(tokens[12].type, vb::Token::Type::ROTATION);
  ASSERT_EQ(tokens[13].type, vb::Token::Type::SELECT);
  ASSERT_EQ(tokens[14].type, vb::Token::Type::FROM);
  ASSERT_EQ(tokens[15].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, INTToken) {
  std::string            input("12345");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::INT);
  ASSERT_EQ(tokens[0].value, "12345");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, FLOAT) {
  std::string            input("123.45");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::FLOAT);
  ASSERT_EQ(tokens[0].value, "123.45");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);

  input = "123.";
  tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::FLOAT);
  ASSERT_EQ(tokens[0].value, "123.");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);

  input = ".45";
  tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::FLOAT);
  ASSERT_EQ(tokens[0].value, ".45");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);

  input = "123.45.";
  tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 3);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::FLOAT);
  ASSERT_EQ(tokens[0].value, "123.45");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::DOT);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, MultiCharIdentifier) {
  std::string            input("abc123");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::IDENTIFIER);
  ASSERT_EQ(tokens[0].value, "abc123");
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, ColonToken) {
  std::string            input(":");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::COLON);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, SemiColonToken) {
  std::string            input(";");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::SEMI_COLON);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, PlusToken) {
  std::string            input("+");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::PLUS);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, AsteriskToken) {
  std::string            input("*");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::ASTERISK);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, EqualToken) {
  std::string            input("=");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, NotEqualToken) {
  std::string            input("!=");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::NOT_EQUAL);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, GreaterThanToken) {
  std::string            input(">");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::GREATER_THAN);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, GreaterThanOrEqualToken) {
  std::string            input(">=");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::GREATER_THAN_EQUAL);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, LessThanToken) {
  std::string            input("<");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::LESS_THAN);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, LessThanOrEqualToken) {
  std::string            input("<=");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::LESS_THAN_EQUAL);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, BoolEqualToken) {
  std::string            input("==");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::BOOL_EQUAL);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, BoolEqualTokenWithSpaces) {
  std::string            input("= =");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 3);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::EQUAL);
  ASSERT_EQ(tokens[2].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, DollarToken) {
  std::string            input("$");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::DOLLAR);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, ExlamationToken) {
  std::string            input("!");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::EXCLAMATION);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, AmpersandToken) {
  std::string            input("&");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::AMPERSAND);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, PipeToken) {
  std::string            input("|");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::PIPE);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, CaretToken) {
  std::string            input("^");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::CARET);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}

TEST(Lexer, ArrowToken) {
  std::string            input("->");
  vb::Lexer              lexer;
  std::vector<vb::Token> tokens = lexer.tokenize(input);
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, vb::Token::Type::ARROW);
  ASSERT_EQ(tokens[1].type, vb::Token::Type::END_OF_FILE);
}