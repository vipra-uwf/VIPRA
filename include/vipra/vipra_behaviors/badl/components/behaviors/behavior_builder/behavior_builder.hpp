
#include "badl/components/behaviors/behavior_builder/parsing/base_grammar.hpp"
#include "badl/components/behaviors/dsl_behavior.hpp"

#include "badl/components/component.hpp"
#include "badl/components/component_methods.hpp"

#include "parsing/peglib.h"

namespace BADL {
class BehaviorBuilder {
 public:
  void initialize(BADL::Agent const& agent);

  [[nodiscard]] auto build_behavior(std::filesystem::path const& path)
      -> DSLBehavior;
  [[nodiscard]] auto build_behavior(std::string const& behaviorStr)
      -> DSLBehavior;

 private:
  peg::parser _parser;

  std::string                           _grammar{BASE_DSL_GRAMMAR};
  std::map<std::string, ActivationFunc> _activationMap;
  std::map<std::string, QueryFunc>      _queryMap;

  DSLBehavior _currentBehavior;
  void        replace_full_grammar();

  void visit(const std::shared_ptr<peg::Ast>& ast);

  static auto parse_func_grammar(std::string      name,
                                 std::string_view grammar) -> std::string;

  void add_component(BADL::Component const& component);
  void build_grammar();
};
}  // namespace BADL