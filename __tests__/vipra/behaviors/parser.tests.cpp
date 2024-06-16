
#include <gtest/gtest.h>
#include <filesystem>
#include <random>

#include "vipra/vipra_behaviors/parsing/parser.hpp"

namespace vb = VIPRA::Behaviors;

// NOLINTNEXTLINE
const std::filesystem::path TESTS_PATH = std::filesystem::path(__FILE__).parent_path() / "parser_tests";

TEST(Parser, Parse) {
  vb::Parser parser;
  auto       ast = parser.parse(TESTS_PATH / "disembark.bhvr");
  ASSERT_TRUE(ast);
  if ( ast ) ASSERT_EQ(ast->statements().size(), 4);

  vb::Lexer  lexer;
  vb::Parser parser2;
  auto       tokens = lexer.tokenize(TESTS_PATH / "luggage.bhvr");
  auto       ast2 = parser2.parse(tokens);
  ASSERT_TRUE(ast2);
  if ( ast2 ) ASSERT_EQ(ast2->statements().size(), 4);
}

TEST(Parser, Selectors) {
  vb::Parser parser;
  auto       ast = parser.parse(TESTS_PATH / "selectors/selectors.bhvr");
  std::cout << ast->to_string() << std::endl;
  ASSERT_TRUE(ast);
  if ( ast ) ASSERT_EQ(ast->statements().size(), 5);

  // EXPECT_THROW(auto err = parser.parse(TESTS_PATH / "selectors/multi_attr.bhvr"), vb::Parser::ParseError);
}

TEST(Parser, Actions) {
  vb::Parser parser;
  auto       ast = parser.parse(TESTS_PATH / "actions/actions.bhvr");
  std::cout << ast->to_string() << std::endl;
  ASSERT_TRUE(ast);
  if ( ast ) ASSERT_EQ(ast->statements().size(), 5);
}

TEST(Parser, FuzzTest) {
  constexpr int ITERATIONS = 5000;
  constexpr int LENGTH = 100;

  std::random_device              rnd;
  std::default_random_engine      gen(rnd());
  std::uniform_int_distribution<> dis(1, LENGTH);
  std::uniform_int_distribution<> typeDis(0, static_cast<int>(vb::Token::Type::COUNT) - 1);

  vb::Parser parser;

  for ( int i = 0; i < ITERATIONS; ++i ) {
    // generate random tokens
    std::vector<vb::Token> tokens;

    for ( int j = 0; j < dis(gen); ++j ) {
      // gnerate random token type
      auto type = static_cast<vb::Token::Type>(typeDis(gen));
      tokens.emplace_back(type, 0, 0, "test");
    }

    ASSERT_NO_THROW(auto ast = parser.parse(tokens));
  }
}