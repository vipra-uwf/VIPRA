#pragma once

#include <stdexcept>

namespace VIPRA::Behaviors {
class DSLException : public std::runtime_error {
 public:
  explicit DSLException(std::string const& message) : std::runtime_error(message) {}
  [[noreturn]] static void error(std::string const& message) { throw DSLException(message); }
};

class BuilderException : public std::runtime_error {
 public:
  explicit BuilderException() : std::runtime_error("") {}
  [[noreturn]] static void error() { throw BuilderException(); }
};
}  // namespace VIPRA::Behaviors
