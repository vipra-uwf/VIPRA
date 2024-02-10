
#include <stdexcept>

class DSLException : public std::runtime_error {
 public:
  explicit DSLException(const std::string& message) : std::runtime_error(message) {}
  [[noreturn]] static void error(const std::string& message) { throw DSLException(message); }
};

class BuilderException : public std::runtime_error {
 public:
  explicit BuilderException() : std::runtime_error("") {}
  [[noreturn]] static void error() { throw BuilderException(); }
};
