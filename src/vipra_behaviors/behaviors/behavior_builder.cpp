

#include <charconv>
#include <memory>
#include <stdexcept>
#include <string>

#include "badl/actuators/func_call.hpp"
#include "badl/agent.hpp"
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

void BehaviorBuilder::initialize(BADL::Agent const& agent)
{
  agent.for_component(
      [this](BADL::Component const& comp) { add_component(comp); });
  build_grammar();
}

void BehaviorBuilder::add_component(BADL::Component const& component)
{
  const auto activations = component.get_activation_grammar();
  const auto queries = component.get_query_grammar();

  for ( auto const& [name, grammar, method] : activations ) {
    auto activationGrammar = parse_func_grammar(name, grammar);
    _grammar.append(activationGrammar);
    _grammar.append("\n");
    _activationMap[name] = method;
  }

  for ( auto const& [name, grammar, method] : queries ) {
    auto queryGrammar = parse_func_grammar(name, grammar);
    _grammar.append(queryGrammar);
    _grammar.append("\n");
    _queryMap[name] = method;
  }
}

auto BehaviorBuilder::parse_func_grammar(
    std::string name, std::string_view grammar) -> std::string
{
  peg::parser parser{FUNC_CALL_GRAMMAR};

  name += " <-";

  parser["String"] = [&](const peg::SemanticValues& /*values*/) {
    name.append(" ID");
  };
  parser["Int"] = [&](const peg::SemanticValues& /*values*/) {
    name.append(" Int");
  };
  parser["Float"] = [&](const peg::SemanticValues& /*values*/) {
    name.append(" Flt");
  };
  parser["Word"] = [&](const peg::SemanticValues& values) {
    name.append(" \'" + values.token_to_string(0) += "\'i");
  };
  parser["Punc"] = [&](const peg::SemanticValues& values) {
    name.append(" \'" + values.token_to_string(0) += "\'");
  };

  parser.set_logger([](size_t line, size_t col, const std::string& msg,
                       const std::string& rule) {
    std::cerr << line << ":" << col << ": " << msg << "\n";
  });

  std::cout << "Adding Func Grammar: " << grammar << '\n';

  // TODO(rolland): provide better error message
  if ( ! parser.parse(grammar) )
    throw std::runtime_error("Unable to parse action grammar");

  return name;
}

void BehaviorBuilder::build_grammar()
{
  replace_full_grammar();

  if ( ! _parser.load_grammar(_grammar) )
    throw std::runtime_error("Unable to load behavior grammar");
  _parser.enable_ast();

  _grammarBuilt = true;
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
  if ( ! _grammarBuilt )
    throw std::runtime_error(
        "Attempting to build behavior without grammar being built");

  DSLBehavior behavior;

  std::shared_ptr<peg::Ast> ast;
  if ( ! _parser.parse(behaviorStr, ast) )
    throw std::runtime_error("Unable to parse behavior");

  visit(ast, behavior);

  // TODO(rolland): allow setting the utility in the behavior
  behavior.action().set_utility(1);
  return behavior;
}

auto BehaviorBuilder::get_parameters(const std::shared_ptr<peg::Ast>& ast)
    -> ComponentParams
{
  ComponentParams params;

  for ( auto& child : ast->nodes ) {
    if ( child->name == "ID" ) {
      std::cout << "ADDING ID parameter: " << child->token << "\n";
      params.parameters.emplace_back(std::string{child->token});
    }
    if ( child->name == "Int" ) {
      std::cout << "ADDING Int parameter: " << child->token << "\n";
      int val{};
      std::from_chars(child->token.begin(), child->token.end(), val);
      params.parameters.emplace_back(val);
    }
    if ( child->name == "Flt" ) {
      std::cout << "ADDING Float parameter: " << child->token << "\n";
      float val{};
      std::from_chars(child->token.begin(), child->token.end(), val);
      params.parameters.emplace_back(val);
    }
  }

  return params;
}

void BehaviorBuilder::visit(const std::shared_ptr<peg::Ast>& ast,
                            DSLBehavior&                     behavior)
{
  std::cout << ast->name << ": " << "\n";

  if ( _activationMap.contains(ast->name) ) {
    std::cout << "Adding Activation: " << ast->name << ", "
              << &_activationMap[ast->name] << '\n';

    ActivationCall call{_activationMap[ast->name], get_parameters(ast)};
    behavior.add_activation_call(std::move(call));
  }

  if ( _queryMap.contains(ast->name) ) {
    std::cout << "Adding Query: " << ast->name << ", " << &_queryMap[ast->name]
              << '\n';

    QueryCall call{_queryMap[ast->name], get_parameters(ast)};
    behavior.add_query_call(std::move(call));
  }

  for ( auto& child : ast->nodes ) {
    visit(child, behavior);
  }
}
}  // namespace BADL