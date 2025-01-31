
#include "badl/components/behaviors/behavior_builder/parsing/base_grammar.hpp"
#include "badl/components/behaviors/dsl_behavior.hpp"

#include "badl/components/component_methods.hpp"
#include "parsing/peglib.h"

namespace BADL {
class BehaviorBuilder {
 public:
  [[nodiscard]] auto build_behavior(std::filesystem::path const& path)
      -> DSLBehavior;
  [[nodiscard]] auto build_behavior(std::string const& behaviorStr)
      -> DSLBehavior;

  void add_activation_grammar(std::string const&    name,
                              std::string const&    grammar,
                              ActivationFunc const& method);
  void add_query_grammar(std::string const& name, std::string const& grammar,
                         QueryFunc const& method);

  void build_grammar();

 private:
  peg::parser _parser;

  std::string                           _grammar{BASE_DSL_GRAMMAR};
  std::map<std::string, ActivationFunc> _activationMap;
  std::map<std::string, QueryFunc>      _queryMap;

  DSLBehavior _currentBehavior;
  void        replace_full_grammar();

  void visit(const std::shared_ptr<peg::Ast>& ast);

  static auto parse_func_grammar(std::string const& name,
                                 std::string const& grammar) -> std::string;
};
}  // namespace BADL