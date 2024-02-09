#include <stdexcept>

#include <builder/behavior_error_listener.hpp>

namespace BHVR {
void BehaviorErrorListener::syntaxError(antlr4::Recognizer*, antlr4::Token*, size_t line,
                                        size_t charPositionInLine, std::string const& msg,
                                        std::exception_ptr) {
  // spdlog::error("Behavior Syntax Error: Line {}:{}, {}", line, charPositionInLine, msg);
  throw std::runtime_error("");
}

void BehaviorErrorListener::reportAmbiguity(antlr4::Parser*, antlr4::dfa::DFA const&, size_t startIndex,
                                            size_t stopIndex, bool, antlrcpp::BitSet const&,
                                            antlr4::atn::ATNConfigSet*) {
  // spdlog::error("Behavior Ambiguity Error: Line {}:{}", startIndex, stopIndex);
  throw std::runtime_error("");
}

void BehaviorErrorListener::reportAttemptingFullContext(antlr4::Parser*, antlr4::dfa::DFA const&, size_t,
                                                        size_t, antlrcpp::BitSet const&,
                                                        antlr4::atn::ATNConfigSet*) {}

void BehaviorErrorListener::reportContextSensitivity(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t,
                                                     size_t, antlr4::atn::ATNConfigSet*) {}
}  // namespace BHVR