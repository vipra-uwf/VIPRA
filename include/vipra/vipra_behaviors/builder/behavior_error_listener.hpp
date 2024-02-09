#ifndef VIPRA_BEHAVIOR_ERROR_LISTENER_HPP
#define VIPRA_BEHAVIOR_ERROR_LISTENER_HPP

#include <antlr4-runtime.h>

namespace BHVR {
class BehaviorErrorListener : public antlr4::BaseErrorListener {
 public:
  void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                   size_t charPositionInLine, const std::string& msg, std::exception_ptr ex) override;

  void reportAmbiguity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex,
                       size_t stopIndex, bool exact, const antlrcpp::BitSet& ambigAlts,
                       antlr4::atn::ATNConfigSet* configs) override;

  void reportAttemptingFullContext(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex,
                                   size_t stopIndex, const antlrcpp::BitSet& conflictingAlts,
                                   antlr4::atn::ATNConfigSet* configs) override;

  void reportContextSensitivity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex,
                                size_t stopIndex, size_t prediction,
                                antlr4::atn::ATNConfigSet* configs) override;
};
}  // namespace BHVR

#endif