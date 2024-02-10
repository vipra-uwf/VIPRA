#pragma once

#include <antlr4-runtime.h>

namespace VIPRA::Behaviors {
class BehaviorErrorListener : public antlr4::BaseErrorListener {
 public:
  void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                   size_t charPositionInLine, std::string const& msg, std::exception_ptr ex) override;

  void reportAmbiguity(antlr4::Parser* recognizer, antlr4::dfa::DFA const& dfa, size_t startIndex,
                       size_t stopIndex, bool exact, antlrcpp::BitSet const& ambigAlts,
                       antlr4::atn::ATNConfigSet* configs) override;

  void reportAttemptingFullContext(antlr4::Parser* recognizer, antlr4::dfa::DFA const& dfa, size_t startIndex,
                                   size_t stopIndex, antlrcpp::BitSet const& conflictingAlts,
                                   antlr4::atn::ATNConfigSet* configs) override;

  void reportContextSensitivity(antlr4::Parser* recognizer, antlr4::dfa::DFA const& dfa, size_t startIndex,
                                size_t stopIndex, size_t prediction,
                                antlr4::atn::ATNConfigSet* configs) override;
};
}  // namespace VIPRA::Behaviors