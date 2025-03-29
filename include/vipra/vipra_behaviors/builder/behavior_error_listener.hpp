#pragma once

#include <ostream>

#include "antlr4-runtime.h"

#include "vipra/vipra_behaviors/behavior/exceptions.hpp"

namespace VIPRA::Behaviors {
class BehaviorErrorListener : public antlr4::BaseErrorListener {
  // NOLINTBEGIN
 public:
  void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol,
                   size_t line, size_t charPositionInLine, std::string const& msg,
                   std::exception_ptr ex) override
  {
    if ( msg.find("no viable alternative") != std::string::npos ) {
      std::fprintf(stderr,
                   "Behavior Syntax Error: Line %zul:%zul, Possible Misspelling?\n", line,
                   charPositionInLine);
    }
    else {
      std::fprintf(stderr, "Behavior Syntax Error: Line %zul:%zul, %s\n", line,
                   charPositionInLine, msg.c_str());
    }
    std::flush(std::cerr);

    throw BuilderException();
  }

  void reportAmbiguity(antlr4::Parser* recognizer, antlr4::dfa::DFA const& dfa,
                       size_t startIndex, size_t stopIndex, bool exact,
                       antlrcpp::BitSet const&    ambigAlts,
                       antlr4::atn::ATNConfigSet* configs) override
  {
    std::fprintf(stderr, "Behavior Ambiguity Error: Line %zu:%zu\n", startIndex,
                 stopIndex);
    std::flush(std::cerr);

    throw BuilderException();
  }

  void reportAttemptingFullContext(antlr4::Parser*         recognizer,
                                   antlr4::dfa::DFA const& dfa, size_t startIndex,
                                   size_t                     stopIndex,
                                   antlrcpp::BitSet const&    conflictingAlts,
                                   antlr4::atn::ATNConfigSet* configs) override
  {
  }

  void reportContextSensitivity(antlr4::Parser* recognizer, antlr4::dfa::DFA const& dfa,
                                size_t startIndex, size_t stopIndex, size_t prediction,
                                antlr4::atn::ATNConfigSet* configs) override
  {
  }
  // NOLINTEND
};
}  // namespace VIPRA::Behaviors