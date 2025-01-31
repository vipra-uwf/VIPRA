

#include <memory>
#include <stdexcept>
#include <string>

#include "badl/components/behaviors/behavior_builder/behavior_builder.hpp"
#include "badl/components/behaviors/behavior_builder/parsing/base_grammar.hpp"
#include "badl/components/behaviors/behavior_builder/parsing/peglib.h"
#include "badl/components/behaviors/dsl_behavior.hpp"
#include "badl/components/component_methods.hpp"

namespace {
void replace(std::string& str, std::string_view fromStr, std::string_view toStr)
{
  auto start = str.find(fromStr);
  if ( start == std::string::npos ) return;

  str.replace(start, fromStr.size(), toStr);
};
}  // namespace

namespace BADL {
void BehaviorBuilder::add_query_grammar(std::string const& name,
                                        std::string const& grammar,
                                        QueryFunc const&   method)
{
  //TODO(rolland): this
}

void BehaviorBuilder::add_activation_grammar(std::string const&    name,
                                             std::string const&    grammar,
                                             ActivationFunc const& method)
{
  auto activationGrammar = parse_func_grammar(name, grammar);

  _grammar.append(activationGrammar);
  _grammar.append("\n");

  _activationMap[name] = method;
}

auto BehaviorBuilder::parse_func_grammar(
    std::string const& name, std::string const& grammar) -> std::string
{
  peg::parser parser{FUNC_CALL_GRAMMAR};

  std::string output{name + " <-"};

  parser["String"] = [&](const peg::SemanticValues& /*values*/) {
    output.append(" ID");
  };
  parser["Int"] = [&](const peg::SemanticValues& /*values*/) {
    output.append(" Int");
  };
  parser["Float"] = [&](const peg::SemanticValues& /*values*/) {
    output.append(" Flt");
  };
  parser["word"] = [&](const peg::SemanticValues& values) {
    output.append(" \'" + values.token_to_string(0) += "\'");
  };

  // TODO(rolland): provide better error message
  if ( ! parser.parse(grammar) )
    throw std::runtime_error("Unable to parse action grammar");

  return output;
}

void BehaviorBuilder::build_grammar()
{
  replace_full_grammar();
  _parser.load_grammar(_grammar);
  _parser.enable_ast();
}

void BehaviorBuilder::replace_full_grammar()
{
  std::string activationFuncs{"("};

  for ( auto const& [name, method] : _activationMap ) {
    activationFuncs.append(name);
    activationFuncs.append("/");
  }
  *(activationFuncs.end() - 1) = ')';

  std::string queryFuncs{"("};
  for ( auto const& [name, method] : _queryMap ) {
    queryFuncs.append(name);
    queryFuncs.append("/");
  }
  *(queryFuncs.end() - 1) = ')';

  replace(_grammar, "#activation_calls#", activationFuncs);
  replace(_grammar, "#query_calls#", queryFuncs);

  std::cout << "\nBEGIN GRAMMAR\n" << _grammar << "\nEND GRAMMAR\n";
}

auto BehaviorBuilder::build_behavior(std::filesystem::path const& path)
    -> DSLBehavior
{
  std::ifstream file(path);

  if ( ! file.is_open() )
    throw std::runtime_error("Unable to open behavior file: " + path.string());

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  return build_behavior(buffer.str());
}

auto BehaviorBuilder::build_behavior(std::string const& behaviorStr)
    -> DSLBehavior
{
  DSLBehavior behavior;

  _parser.enable_ast();
  std::shared_ptr<peg::Ast> ast;
  if ( ! _parser.parse(behaviorStr, ast) )
    throw std::runtime_error("Unable to parse behavior");

  // ast = _parser.optimize_ast(ast);
  visit(ast);

  return behavior;
}

void BehaviorBuilder::visit(const std::shared_ptr<peg::Ast>& ast)
{
  std::cout << ast->name << ": " << "\n";
  // ActivationCall call{_ActivationFuncMap[ast->name], {}};
  // _currentBehavior.add_actuator_call(std::move(call));

  for ( auto& child : ast->nodes ) {
    visit(child);
  }
}
}  // namespace BADL