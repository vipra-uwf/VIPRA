
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>

#include <support/Any.h>

#include <generated/BehaviorLexer.h>
#include <generated/BehaviorParser.h>

#include "attributes/attributes.hpp"

#include "behavior/exceptions.hpp"
#include "behavior/human_behavior.hpp"

#include "builder/behavior_builder.hpp"
#include "builder/declaration_components.hpp"

#include "conditions/subconditions/subcondition_attribute.hpp"
#include "conditions/subconditions/subcondition_exists.hpp"
#include "definitions/dsl_types.hpp"
#include "definitions/pedestrian_types.hpp"
#include "vipra/geometry/f3d.hpp"

#include "selectors/selector_everyone.hpp"
#include "selectors/selector_exactly_N.hpp"
#include "selectors/selector_location.hpp"
#include "selectors/selector_percent.hpp"

#include "actions/atoms/atom_scale.hpp"
#include "actions/atoms/atom_set.hpp"

#include "conditions/subconditions/subcondition_elapsed_time.hpp"
#include "conditions/subconditions/subcondition_enter.hpp"
#include "conditions/subconditions/subcondition_event.hpp"
#include "conditions/subconditions/subcondition_event_occurring.hpp"
#include "conditions/subconditions/subcondition_in_location.hpp"
#include "conditions/subconditions/subcondition_leave.hpp"
#include "conditions/subconditions/subcondition_start.hpp"

#include "conditions/subconditions/subcondition_event_ending.hpp"
#include "conditions/subconditions/subcondition_event_starting.hpp"
#include "conditions/subconditions/subcondition_spatial.hpp"

#include "targets/target.hpp"
#include "targets/target_modifier.hpp"
#include "targets/target_modifiers/modifier_direction.hpp"
#include "targets/target_modifiers/modifier_distance.hpp"
#include "targets/target_modifiers/modifier_location.hpp"
#include "targets/target_selector.hpp"
#include "targets/target_selectors/target_nearest.hpp"
#include "targets/target_selectors/target_self.hpp"

#include "time/time.hpp"

#include "values/direction.hpp"
#include "values/numeric_value.hpp"
#include "values/values.hpp"

namespace VIPRA::Behaviors {

// NOLINTNEXTLINE
std::vector<CAttributeValue> AttributeHandling::valueStore{};

/**
 * @brief Parses the behavior file at filepath, returns the behavior it describes
 * 
 * @param behaviorName : name of the behavior
 * @param filepath : path to the behavior file
 * @param seedNum : seed for randomization
 * @return HumanBehavior&&
 */
auto BehaviorBuilder::build(std::string behaviorName, std::filesystem::path const& filepath,
                            Behaviors::seed seedNum) -> HumanBehavior {
  if (!std::filesystem::exists(filepath)) {
    // spdlog::error("Behavior \"{}\" Does NOT Exist at {}", behaviorName, filepath.c_str());
    BuilderException::error();
  }

  // spdlog::debug("Loading Behavior: {} at {}", behaviorName, std::filesystem::canonical(filepath).c_str());
  initial_behavior_setup(behaviorName, seedNum);

  std::ifstream dslFile(filepath);

  antlr4::ANTLRInputStream  input(dslFile);
  BehaviorLexer             lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  BehaviorParser            parser(&tokens);

  parser.removeErrorListeners();
  parser.addErrorListener(&_errorListener);

  BehaviorParser::ProgramContext* tree = parser.program();

  visitProgram(tree);

  end_behavior_check();

  return _currentBehavior;
}

// ---------------------------------------- INITIALIZATION -----------------------------------------------------------------------------------

/**
 * @brief Cleans and sets up the builder for a new behavior
 * 
 * @param behaviorName : name of new behavior
 * @param seedNum : randomization seed for behavior
 */
void BehaviorBuilder::initial_behavior_setup(std::string const& behaviorName, Behaviors::seed seedNum) {
  _currentBehavior = HumanBehavior(behaviorName);
  _currentBehavior.set_seed(seedNum);
  _currSeed = seedNum;
  initialize_states();
  initialize_events();
  initialize_types();
  initialize_locations();
}

/**
 * @brief cleans and sets up the events map
 * 
 */
void BehaviorBuilder::initialize_events() {
  _eventsMap.clear();

  _startEvent = Event("Start");
  _startCond = Condition();
  // TODO: this

  _startCond.add_subcondition(SubConditionStart{});
  _startEvent.set_start_condition(_startCond);

  _eventsMap["!Start"] = _currentBehavior.add_event(_startEvent);
}

/**
 * @brief cleans and sets up the states map
 * 
 */
void BehaviorBuilder::initialize_states() {
  _states.clear();
  _currState = Behaviors::stateUID{1};
}

/**
 * @brief cleans and sets up the types map
 * 
 */
void BehaviorBuilder::initialize_types() {
  _types.clear();
  _types["pedestrian"] = 0;
  _types["pedestrians"] = 0;
  _currType = 1;
}

/**
 * @brief cleans and sets up the locations map
 * 
 */
void BehaviorBuilder::initialize_locations() { _locations.clear(); }

// ------------------------------------ END INITIALIZATION --------------------------------------------------------------------------------------

/**
 * @brief Checks the built behavior for possible issues
 * 
 */
void BehaviorBuilder::end_behavior_check() {
  if (_currentBehavior.selector_count() == 0) {
    // spdlog::error("Behavior Error: No Pedestrian Selector Defined For Behavior: \"{}\"",
                  _currentBehavior.get_name());
                  BuilderException::error();
  }
  if (_currentBehavior.action_count() == 0) {
    // spdlog::warn("Behavior Error: No Actions Defined For Behavior: \"{}\"", _currentBehavior.get_name());
  }
}

/**
 * @brief Creates a new condition adding all sub_conditions and operations
 * 
 * @param cond : condition context
 * @return Condition
 */
auto BehaviorBuilder::build_condition(BehaviorParser::ConditionContext* cond) -> Condition {
  Condition condition;
  condition_tree_condition(cond, condition);
  return condition;
}

void BehaviorBuilder::add_sub_condition(Condition& condTree, BehaviorParser::Sub_conditionContext* subcond) {
  if (subcond->condition_Time_Elapsed_From_Event()) {
    condTree.add_subcondition(build_time_elapsed_subcond(subcond->condition_Time_Elapsed_From_Event()));
    return;
  }

  if (subcond->condition_Enter_Location()) {
    condTree.add_subcondition(build_enter_subcond(subcond->condition_Enter_Location()));
    return;
  }

  if (subcond->condition_Event_Occurred()) {
    condTree.add_subcondition(build_event_occurred_subcond(subcond->condition_Event_Occurred()));
    return;
  }

  if (subcond->condition_Event_Occurring()) {
    condTree.add_subcondition(build_event_occurring_subcond(subcond->condition_Event_Occurring()));
    return;
  }

  if (subcond->condition_Event_Starting()) {
    condTree.add_subcondition(build_event_starting_subcond(subcond->condition_Event_Starting()));
    return;
  }

  if (subcond->condition_Event_Ending()) {
    condTree.add_subcondition(build_event_ending_subcond(subcond->condition_Event_Ending()));
    return;
  }

  if (subcond->condition_Spatial()) {
    condTree.add_subcondition(build_spatial_subcond(subcond->condition_Spatial()));
    return;
  }

  if (subcond->condition_Exit_Location()) {
    condTree.add_subcondition(build_exit_subcond(subcond->condition_Exit_Location()));
    return;
  }

  if (subcond->condition_Inside_Location()) {
    condTree.add_subcondition(build_in_location_subcond(subcond->condition_Inside_Location()));
    return;
  }

  if (subcond->condition_Attribute()) {
    condTree.add_subcondition(build_attribute_subcond(subcond->condition_Attribute()));
    return;
  }

  if (subcond->condition_Exists()) {
    condTree.add_subcondition(build_exists_subcond(subcond->condition_Exists()));
    return;
  }

  // spdlog::error("Behavior Error: No Valid SubCondition For: \"{}\"", subcond->getText());
  BuilderException::error();
}

void BehaviorBuilder::condition_tree_condition(BehaviorParser::ConditionContext* condition, Condition& tree) {
  condition_tree_unary(condition->unary(), tree);

  if (condition->condition()) {
    if (condition->AND()) {
      tree.add_operation(BoolOp::AND);
    } else {
      tree.add_operation(BoolOp::OR);
    }

    condition_tree_condition(condition->condition(), tree);
  }
}

void BehaviorBuilder::condition_tree_unary(BehaviorParser::UnaryContext* unary, Condition& tree) {
  // if (unary->unary()) {
  //   condition_tree_unary(unary->unary(), tree);
  // }

  if (unary->primary()) {
    condition_tree_primary(unary->primary(), tree);
  }
}

void BehaviorBuilder::condition_tree_primary(BehaviorParser::PrimaryContext* primary, Condition& tree) {
  if (primary->condition()) {
    condition_tree_condition(primary->condition(), tree);
  }

  if (primary->sub_condition()) {
    add_sub_condition(tree, primary->sub_condition());
  }
}

/**
 * @brief Creates a subselector
 * 
 * @param ctx : subselector context
 * @return Selector 
 */
auto BehaviorBuilder::build_sub_selector(slType type, slSelector selector, std::optional<slGroup> group,
                                         bool required) -> SubSelector {
  if (selector->selector()->selector_Everyone()) {
    return build_everyone_selector(type, required);
  }

  if (selector->selector()->selector_Exactly_N_Random()) {
    return build_exactly_n_selector(type, selector, group, required);
  }

  if (selector->selector()->selector_Percent()) {
    return build_percent_selector(type, selector, group, required);
  }

  if (selector->selector()->selector_Location()) {
    return build_location_selector(type, selector, group, required);
  }

  // spdlog::error("Behavior Error: Unable To Create Selector For Behavior \"{}\"", _currentBehavior.get_name());
  BuilderException::error();
}

/**
 * @brief Adds the modifier described by the modifier context to a target modifier
 * 
 * @param targetModifier : target modifier to add to
 * @param modifier : modifier context
 */
void BehaviorBuilder::add_modifier(TargetModifier&                  targetModifier,
                                   BehaviorParser::ModifierContext* modifier) const {
  if (modifier->direction()) {
    add_direction_modifier(targetModifier, modifier->direction());
    return;
  }
  if (modifier->distance()) {
    add_distance_modifier(targetModifier, modifier->distance());
    return;
  }
  if (modifier->location_modifier()) {
    add_location_modifier(targetModifier, modifier->location_modifier());
    return;
  }

  error("Behavior Error: No Valid Target Modifier For: \"{}\"", modifier->getText());
}

/**
 * @brief Creates an action atom and adds it to the action
 * 
 * @param action : action to add the atom to
 * @param atom : atom context
 */
void BehaviorBuilder::add_atom_to_action(Action& action, BehaviorParser::Action_atomContext* atom) {
  if (atom->set_atom()) {
    add_set_atom(action, atom->set_atom());
    return;
  }

  if (atom->scale_atom()) {
    add_scale_atom(action, atom->scale_atom());
    return;
  }

  // spdlog::error("Behavior Error: No Atom");
  BuilderException::error();
}

/**
 * @brief Adds a target to an action
 * 
 */
void BehaviorBuilder::add_target_to_action(Action& action, BehaviorParser::TargetContext* ctx) {
  if (ctx->other()) {
    auto modList = ctx->modifier();
    auto modifiers = make_target_modifier(modList);
    if (ctx->other()->nearest_type()) {
      add_nearest_type_target(action, ctx->other()->nearest_type(), modifiers);
      return;
    }
  }

  action.add_target(TargetSelector(TargetSelf{}));
}

/**
 * @brief Creates a target modifier from a vector of modifier contexts
 * 
 * @param modifiers : modifiers to add to target modifier
 * @return std::optional<TargetModifier> 
 */
auto BehaviorBuilder::make_target_modifier(std::vector<BehaviorParser::ModifierContext*>& modifiers)
    -> std::optional<TargetModifier> {
  if (modifiers.empty()) return std::nullopt;

  TargetModifier mod;
  for (auto* modifier : modifiers) {
    add_modifier(mod, modifier);
  }

  return mod;
}

// --------------------------------------------------- END HELPERS ---------------------------------------------------------------------------------------------

// --------------------------------------------------- GETTERS -------------------------------------------------------------------------------------------------

/**
 * @brief Gets the id for a type from its name
 * 
 * @param type : type name
 * @return typeUID
 */
auto BehaviorBuilder::get_type(std::string const& type) const -> std::optional<typeUID> {
  auto const typeId = _types.find(type);
  if (typeId == _types.end()) return std::nullopt;
  return (*typeId).second;
}

/**
 * @brief Gets the typeUID of the group for a selector
 * 
 * @param ctx : 
 * @return Behaviors::typeUID 
 */
auto BehaviorBuilder::get_group(std::optional<slGroup> group) const
    -> std::pair<Behaviors::typeUID, std::string> {
  if (group) {
    auto* ctx = group.value()->group();
    if (!(ctx->PEDESTRIAN() || ctx->PEDESTRIANS())) {
      std::string name = ctx->ID()->toString();
      return {get_check_type(name), name};
    }
  }

  return {0, "Pedestrians"};
}

/**
 * @brief Returns the stateUID associated with a state string
 * 
 * @param state : name of state
 * @return stateUID 
 */
auto BehaviorBuilder::get_state(std::string const& state) const -> std::optional<Behaviors::stateUID> {
  if (_states.find(state) == _states.end()) return std::nullopt;
  return _states.at(state);
}

/**
 * @brief Combines a list of types into one type
 * 
 * @param types : id_list vector
 * @return Behaviors::Ptype 
 */
auto BehaviorBuilder::get_composite_type(std::vector<antlr4::tree::TerminalNode*> const& types) const
    -> Behaviors::Ptype {
  Ptype compType;
  for (auto* type : types) {
    const std::string tStr = type->toString();
    auto              tid = get_check_type(tStr);
    compType += tid;
  }

  return compType;
}

/**
 * @brief Gets a pointer to the Location with a name locName from LocationsMap
 * @exits if the location was not found
 * 
 * @param locName : Name of location to find
 * @return Location*
*/
auto BehaviorBuilder::get_location(std::string const& locName) const -> std::optional<VIPRA::idx> {
  auto loc = _locations.find(locName);
  if (loc == _locations.end()) return std::nullopt;

  return (*loc).second;
}

/**
 * @brief Gets a pointer to the event with name evName from _eventsMap
 * @exits if the event isn't found
 * 
 * @param evName : name of event to find
 * @return Event*
 */
auto BehaviorBuilder::get_event(std::string const& evName) const -> std::optional<VIPRA::idx> {
  auto event = _eventsMap.find(evName);
  if (event == _eventsMap.end()) return std::nullopt;
  return (*event).second;
}

/**
 * @brief Returns the attribute type for attr
 * 
 * @param attr : attribute string
 * @return Attribute 
 */
auto BehaviorBuilder::get_attribute(std::string attr) -> Attribute {
  static std::map<std::string, Attribute> attrMap{
      {"position", Attribute::POSITION}, {"end goal", Attribute::END_GOAL}, {"goal", Attribute::CURR_GOAL},
      {"state", Attribute::STATE},       {"velocity", Attribute::VELOCITY}, {"location", Attribute::LOCATION},
      {"status", Attribute::STATUS}};

  std::transform(attr.begin(), attr.end(), attr.begin(), [](char chr) { return std::tolower(chr); });

  auto iter = attrMap.find(attr);
  if (iter == attrMap.end()) {
    return Attribute::INVALID;
  }

  return iter->second;
}

/**
 * @brief Gets the location index and checks that it exists, throwing an error if not defined
 * 
 * @param locName 
 * @return VIPRA::idx 
 */
auto BehaviorBuilder::get_check_location(std::string const& locName) const -> VIPRA::idx {
  auto loc = get_location(locName);
  if (!loc) error("Behavior Error: Attempt To Use Undeclared Location: \"{}\"", locName);
  return loc.value();
}

/**
 * @brief Gets the state index and checks that it exists, throwing an error if not defined
 * 
 * @param stateName 
 * @return Behaviors::stateUID 
 */
auto BehaviorBuilder::get_check_state(std::string const& stateName) const -> Behaviors::stateUID {
  auto state = get_state(stateName);
  if (!state) error("Behavior Error: Attempt To Use Undeclared State: \"{}\"", stateName);
  return state.value();
}

/**
 * @brief Gets the event index and checks that it exists, throwing an error if not defined
 * 
 * @param eventName 
 * @return VIPRA::idx 
 */
auto BehaviorBuilder::get_check_event(std::string const& eventName) const -> VIPRA::idx {
  auto event = get_event(eventName);
  if (!event) error("Behavior Error: Attempt To Use Undeclared Event: \"{}\"", eventName);
  return event.value();
}

/**
 * @brief Gets the type index and checks that it exists, throwing an error if not defined
 * 
 * @param typeName 
 * @return Behaviors::typeUID 
 */
auto BehaviorBuilder::get_check_type(std::string const& typeName) const -> Behaviors::typeUID {
  auto type = get_type(typeName);
  if (!type) error("Behavior Error: Attempt To Use Undeclared Type: \"{}\"", typeName);
  return type.value();
}

// --------------------------------------------------- END GETTERS ---------------------------------------------------------------------------------------------

// --------------------------------------------------- MAKERS --------------------------------------------------------------------------------------------------

/**
 * @brief Returns the dimensions of a Location from a dimensions context
 * 
 * @param ctx 
 * @return std::tuple<VIPRA::f3d, VIPRA::f3d, float> 
 */
auto BehaviorBuilder::make_dimensions(BehaviorParser::Loc_dimensionsContext* ctx) const
    -> std::tuple<VIPRA::f3d, VIPRA::f3d, float> {
  auto rot = get_numeric(ctx->loc_rotation()->value_numeric(), _currSeed).value(0);
  auto center = get_coord(ctx->loc_center()->value_coord(), _currSeed);
  auto dims = get_coord(ctx->loc_lengths()->value_coord(), _currSeed);
  return {center, dims, rot};
}

/**
 * @brief Creates a new CAttributeValue from an attribute value context
 * 
 * @param ctx : attribute value context
 * @return Behaviors::CAttributeValue 
 */
auto BehaviorBuilder::make_attribute_value(BehaviorParser::Attr_valueContext* ctx)
    -> Behaviors::CAttributeValue {
  if (ctx->value_coord()) {
    return AttributeHandling::store_value<VIPRA::f3d>(Type::COORD, get_coord(ctx->value_coord(), _currSeed));
  }

  if (ctx->STATE_VAL()) {
    return AttributeHandling::store_value<Behaviors::stateUID>(Type::STATE,
                                                               get_check_state(ctx->STATE_VAL()->toString()));
  }

  if (ctx->value_numeric()) {
    return AttributeHandling::store_value<Behaviors::NumericValue>(
        Type::NUMBER, get_numeric(ctx->value_numeric(), _currSeed));
  }

  if (ctx->LOC_NAME()) {
    auto location = get_check_location(ctx->LOC_NAME()->toString());
    return AttributeHandling::store_value(Type::LOCATION, location);
  }

  if (ctx->towards()) {
    if (ctx->towards()->LOC_NAME()) {
      auto location = get_check_location(ctx->towards()->LOC_NAME()->toString());
      return AttributeHandling::store_value(Type::TOWARDS_LOC, location);
    }
    if (ctx->towards()->attribute()) {
      auto attrStr = make_attribute_str(ctx->towards()->attribute());
      auto attr = get_attribute(attrStr);
      if (attr != Attribute::INVALID) {
        return AttributeHandling::store_value(Type::TOWARDS_ATTR, attr);
      }
    }
  }

  error("Unable To Create Attribute Value");
}

/**
 * @brief Creates a vector with all of the strings for each type in a given id_list
 * 
 * @param types : id_list vector
 * @return std::vector<std::string>
 */
auto BehaviorBuilder::make_list_strs(std::vector<antlr4::tree::TerminalNode*> const& types)
    -> std::vector<std::string> {
  std::vector<std::string> strs(types.size());

  std::transform(types.begin(), types.end(), strs.begin(),
                 [](antlr4::tree::TerminalNode* type) { return type->toString(); });

  return strs;
}

/**
 * @brief Gets the string value of an attribute from a context
 * 
 */
auto BehaviorBuilder::make_attribute_str(BehaviorParser::AttributeContext* ctx) -> std::string {
  if (ctx->POSITION()) {
    return "position";
  }
  if (ctx->VELOCITY()) {
    return "velocity";
  }
  if (ctx->GOAL()) {
    return "goal";
  }
  if (ctx->end_goal()) {
    return "end goal";
  }
  if (ctx->STATE()) {
    return "state";
  }
  if (ctx->LOCATION()) {
    return "location";
  }
  if (ctx->STATUS()) {
    return "status";
  }

  return "";
}

/**
 * @brief Returns the Direction enum that matches the Direction context
 * 
 * @param ctx : direction context
 * @return Direction 
 */
auto BehaviorBuilder::make_direction(BehaviorParser::DirectionContext* ctx) -> Direction {
  if (ctx->front()) {
    return Direction::FRONT;
  }

  if (ctx->behind()) {
    return Direction::BEHIND;
  }

  error("Behavior Error: Unkown Direction {}", ctx->toString());
}

// --------------------------------------------------- END MAKERS --------------------------------------------------------------------------------------------------

/**
 * @brief Adds an event to the behavior
 * 
 * @param ctx : event context
 * @return VIPRA::idx 
 */
auto BehaviorBuilder::add_event(BehaviorParser::Event_nameContext* ctx) -> VIPRA::idx {
  if (!ctx) error("Behavior Error: Event Not Given a Name: \"{}\"", _currentBehavior.get_name());

  auto eventName = ctx->ID()->toString();
  // spdlog::debug(R"(Behavior "{}": Adding Lasting Event: "{}")", _currentBehavior.get_name(), eventName);

  VIPRA::idx eventIdx = _currentBehavior.event_count();
  _eventsMap[eventName] = eventIdx;

  return eventIdx;
}

// --------------------------------------------- ANTLR VISITOR METHODS -----------------------------------------------------------------------------------------

auto BehaviorBuilder::visitLocation(BehaviorParser::LocationContext* ctx) -> antlrcpp::Any {
  auto name = find_location_component<lcName>(ctx);
  if (!name) error(R"(Behavior "{}": Missing Location Name)", _currentBehavior.get_name());
  std::string locName = "@" + name.value()->ID()->toString();

  if (get_location(locName))
    error(R"(Behavior "{}": Attempt To Redefine Location: "{}")", _currentBehavior.get_name(), locName);

  auto dims = find_location_component<lcDimensions>(ctx);
  if (!dims)
    error(R"(Behavior "{}": Location Missing Dimensions: "{}")", _currentBehavior.get_name(), locName);

  auto [center, dimensions, rotation] = make_dimensions(dims.value());

  _locations[locName] = _currentBehavior.add_location(Location{center, dimensions, rotation});
  // spdlog::debug(R"(Behavior "{}": Adding Location "{}")", _currentBehavior.get_name(), locName);

  return BehaviorBaseVisitor::visitLocation(ctx);
}

auto BehaviorBuilder::visitEvent(BehaviorParser::EventContext* ctx) -> antlrcpp::Any {
  auto name = find_event_component<evName>(ctx);
  if (!name) error(R"(Behavior "{}": Missing Event Name)", _currentBehavior.get_name());
  std::string eventName = "!" + name.value();

  if (get_event(eventName))
    error(R"(Behavior "{}": Attempt To Redefine Event: "{}")", _currentBehavior.get_name(), eventName);

  auto startCond = find_event_component<evStart>(ctx);
  if (!startCond) error(R"(Behavior "{}": Missing Start Condition)", _currentBehavior.get_name());

  auto endCond = find_event_component<evEnd>(ctx);

  // NOTE(rolland): a bit odd but we need to ensure that the event is in the map, incase the conditions rely on it
  _eventsMap[eventName] = _currentBehavior.event_count();

  Event event(eventName);

  // spdlog::debug(R"(Behavior "{}": Event: "{}" Adding Start Condition)", _currentBehavior.get_name(),
                eventName);
                event.set_start_condition(build_condition(startCond.value()->condition()));

                if (endCond) {
                  // spdlog::debug(R"(Behavior "{}": Event: "{}" Adding End Condition)", _currentBehavior.get_name(),
                  eventName);
                  event.set_end_condition(build_condition(endCond.value()->condition()));
                }

                _eventsMap[eventName] = _currentBehavior.add_event(event);

                return BehaviorBaseVisitor::visitEvent(ctx);
}

auto BehaviorBuilder::visitAction(BehaviorParser::ActionContext* ctx) -> antlrcpp::Any {
  auto response = find_action_component<acResponse>(ctx);
  if (!response) error(R"(Behavior "{}": Action has no Response)", _currentBehavior.get_name());

  auto stimulus = find_action_component<acStimulus>(ctx);
  auto target = find_action_component<acTarget>(ctx);
  auto duration = find_action_component<acDuration>(ctx);

  Action action;

  auto const typeStr = ctx->ID()->toString();
  auto const type = get_check_type(typeStr);

  // spdlog::debug("Behavior \"{}\": Adding Action For {}", _currentBehavior.get_name(), typeStr);

  auto atoms = response.value()->sub_action()->action_atom();
  std::for_each(atoms.begin(), atoms.end(),
                [&](BehaviorParser::Action_atomContext* atom) { add_atom_to_action(action, atom); });

  if (stimulus) action.add_condition(build_condition(stimulus.value()->condition()));
  if (target) add_target_to_action(action, target.value()->target());
  if (duration) action.add_duration(get_numeric(duration.value()->duration()->value_numeric(), _currSeed));

  _currentBehavior.add_action(type, action);
  return BehaviorBaseVisitor::visitAction(ctx);
}

auto BehaviorBuilder::visitPed_Selector(BehaviorParser::Ped_SelectorContext* ctx) -> antlrcpp::Any {
  auto type = find_selector_component<slType>(ctx);
  if (!type) error(R"(Behavior "{}": Selector has no Type)", _currentBehavior.get_name());

  auto selector = find_selector_component<slSelector>(ctx);
  if (!selector) error(R"(Behavior "{}": Selector has no Selector version)", _currentBehavior.get_name());

  auto group = find_selector_component<slGroup>(ctx);
  auto required = find_selector_component<slRequired>(ctx);

  _currentBehavior.add_sub_selector(
      build_sub_selector(type.value(), selector.value(), group, required.has_value()));

  return BehaviorBaseVisitor::visitPed_Selector(ctx);
}

// ------------------------------- END ACTIONS -----------------------------------------------------------------------------------------

// ------------------------------- DECLARATIONS -----------------------------------------------------------------------------------------

auto BehaviorBuilder::visitDecl_Ped_State(BehaviorParser::Decl_Ped_StateContext* ctx) -> antlrcpp::Any {
  auto const stateNames = ctx->ID();

  for (auto* state : stateNames) {
    auto name = state->toString();
    // spdlog::debug("Behavior \"{}\": Adding Pedestrian State {}, id: {}", _currentBehavior.get_name(), name,
                  _currState);
                  _states[name] = _currState;
                  ++_currState;
  }

  return BehaviorBaseVisitor::visitDecl_Ped_State(ctx);
}

auto BehaviorBuilder::visitDecl_Ped(BehaviorParser::Decl_PedContext* ctx) -> antlrcpp::Any {
  auto const typeNames = ctx->ID();
  Ptype      allTypes;

  for (auto* type : typeNames) {
    auto name = type->toString();
    // spdlog::debug("Behavior \"{}\": Adding Pedestrian Type {}, id: {}", _currentBehavior.get_name(), name,
                  _currType);
                  _types[name] = _currType;
                  allTypes += _currType;
                  _currType = _currType << 1U;
  }

  // spdlog::debug("Behavior \"{}\": All Types: {}", _currentBehavior.get_name(), allTypes.fullType);
  _currentBehavior.set_all_ped_types(allTypes);
  return BehaviorBaseVisitor::visitDecl_Ped(ctx);
}

// ------------------------------- END DECLARATIONS -----------------------------------------------------------------------------------------

// ------------------------------- ATOMS -----------------------------------------------------------------------------------------

/**
 * @brief Adds a set atom to the action
 * 
 * @param action : action to add atom to
 * @param ctx : set atom context
 */
void BehaviorBuilder::add_set_atom(Action& action, BehaviorParser::Set_atomContext* ctx) {
  auto attrStr = make_attribute_str(ctx->attribute());
  auto attr = get_attribute(attrStr);
  auto attrValue = make_attribute_value(ctx->attr_value());
  action.add_atom(AtomSet{attr, attrValue});
}

/**
 * @brief Adds a scale atom to the action
 * 
 * @param action : action to add to
 * @param ctx : scale atom context
 */
void BehaviorBuilder::add_scale_atom(Action& action, BehaviorParser::Scale_atomContext* ctx) {
  auto attrStr = make_attribute_str(ctx->attribute());
  auto attr = get_attribute(attrStr);
  auto attrValue = make_attribute_value(ctx->attr_value());
  action.add_atom(AtomScale{attr, attrValue});
}

// ------------------------------- END ATOMS -----------------------------------------------------------------------------------------

// ------------------------------- SUBCONDITIONS -----------------------------------------------------------------------------------------

/**
 * @brief Adds an enter location subcondition to the condition
 * 
 * @param condition 
 * @param ctx 
 */
auto BehaviorBuilder::build_enter_subcond(BehaviorParser::Condition_Enter_LocationContext* ctx)
    -> SubConditionEnter {
  auto location = get_check_location(ctx->LOC_NAME()->toString());

  return SubConditionEnter{location};
}

/**
 * @brief Adds an exit location subcondition to the condition
 * 
 * @param condition 
 * @param ctx 
 */
auto BehaviorBuilder::build_exit_subcond(BehaviorParser::Condition_Exit_LocationContext* ctx)
    -> SubConditionLeave {
  auto location = get_check_location(ctx->LOC_NAME()->toString());

  return SubConditionLeave{location};
}
/**
 * @brief Adds a time elapsed subcondition to a condition
 * 
 * @param condition : condition to add to
 * @param ctx : subcondition context
 */
auto BehaviorBuilder::build_time_elapsed_subcond(
    BehaviorParser::Condition_Time_Elapsed_From_EventContext* ctx) -> SubConditionElapsedTimeFromEvent {
  Behaviors::NumericValue dur = get_numeric(ctx->value_numeric(), _currSeed);
  std::string             evName = ctx->EVNT()->toString();
  // spdlog::debug(R"(Behavior "{}": Adding SubCondition: Elapsed Time From "{}" Event)",
                _currentBehavior.get_name(), evName);
                auto event = get_check_event(evName);

                return {dur, event};
}

/**
 * @brief Adds an event occurring subcondition to the condition
 * 
 * @param condition : condition to add to
 * @param ctx : subcondition context
 */
auto BehaviorBuilder::build_event_occurred_subcond(BehaviorParser::Condition_Event_OccurredContext* ctx)
    -> SubConditionEventOccurred {
  std::string evName = ctx->EVNT()->toString();
  // spdlog::debug(R"(Behavior "{}": Adding SubCondition: Event "{}" Occurred)", _currentBehavior.get_name(),
                evName);
                auto event = get_check_event(evName);
                return SubConditionEventOccurred(event);
}

/**
 * @brief Adds an event occurrring subconditon to the condition
 * 
 * @param condition : condition to add to
 * @param ctx : subcondition context
 */
auto BehaviorBuilder::build_event_occurring_subcond(BehaviorParser::Condition_Event_OccurringContext* ctx)
    -> SubConditionEventOccurring {
  std::string evName = ctx->EVNT()->toString();
  // spdlog::debug(R"(Behavior "{}": Adding SubCondition: Event "{}" Occurring)", _currentBehavior.get_name(),
                evName);
                auto event = get_check_event(evName);
                return SubConditionEventOccurring(event);
}

/**
 * @brief Adds an event starting subconditon to a condition
 * 
 * @param condition : condition to add to
 * @param ctx : subcondition context
 */
auto BehaviorBuilder::build_event_starting_subcond(BehaviorParser::Condition_Event_StartingContext* ctx)
    -> SubConditionEventStarting {
  std::string evName = ctx->EVNT()->toString();
  // spdlog::debug(R"(Behavior "{}": Adding SubCondition: Event "{}" Occurring)", _currentBehavior.get_name(),
                evName);
                auto event = get_check_event(evName);

                return SubConditionEventStarting(event);
}

/**
 * @brief Adds an event ending subcondition to the condition
 * 
 * @param condition : condition to add to
 * @param ctx : subcondition context
 */
auto BehaviorBuilder::build_event_ending_subcond(BehaviorParser::Condition_Event_EndingContext* ctx)
    -> SubConditionEventEnding {
  std::string evName = ctx->EVNT()->toString();
  // spdlog::debug(R"(Behavior "{}": Adding SubCondition: Event "{}" Occurring)", _currentBehavior.get_name(),
                evName);
                auto event = get_check_event(evName);
                return SubConditionEventEnding(event);
}

/**
 * @brief Adds a spatial subcondition to the condition
 * 
 * @param condition : condition to add to
 * @param ctx : spatial condition context
 */
auto BehaviorBuilder::build_spatial_subcond(BehaviorParser::Condition_SpatialContext* ctx)
    -> SubConditionSpatial {
  auto distance = get_numeric(ctx->value_numeric(), _currSeed);
  // spdlog::debug(R"(Behavior "{}": Adding SubCondition: Spatial)", _currentBehavior.get_name());
  return SubConditionSpatial(distance);
}

auto BehaviorBuilder::build_in_location_subcond(BehaviorParser::Condition_Inside_LocationContext* ctx)
    -> SubConditionInLocation {
  auto location = get_check_location(ctx->LOC_NAME()->toString());

  // spdlog::debug(R"(Behavior "{}": Adding SubCondition: Inside Location "{}")", _currentBehavior.get_name(),
                ctx->LOC_NAME()->toString());
                return SubConditionInLocation{location};
}

/**
 * @brief Adds an attribute subcondition to the condition
 * 
 * @param condition 
 * @param ctx 
 */
auto BehaviorBuilder::build_attribute_subcond(BehaviorParser::Condition_AttributeContext* ctx)
    -> SubConditionAttribute {
  auto attrStr = make_attribute_str(ctx->attribute());
  auto attr = get_attribute(attrStr);
  auto attrValue = make_attribute_value(ctx->attr_value());
  bool negative = ctx->NOT() != nullptr;

  // spdlog::debug(R"(Behavior "{}": Adding SubCondition: Attribute "{}")", _currentBehavior.get_name(),
                attrStr);
                return SubConditionAttribute{attr, attrValue, negative};
}

/**
 * @brief Adds an exists subcondition to the condition
 * 
 * @param condition 
 * @param ctx 
 */
auto BehaviorBuilder::build_exists_subcond(BehaviorParser::Condition_ExistsContext* ctx)
    -> SubConditionExists {
  auto modifiers = ctx->modifier();
  auto targetModifier = make_target_modifier(modifiers);

  auto attrStr = make_attribute_str(ctx->attribute());
  auto attr = get_attribute(attrStr);
  auto attrValue = make_attribute_value(ctx->attr_value());
  bool negative = ctx->NOT() != nullptr;

  // spdlog::debug(R"(Behavior "{}": Adding SubCondition: Exists Attribute \"{}\")", _currentBehavior.get_name(),
                attrStr);
                return SubConditionExists{
                    targetModifier.has_value() ? targetModifier.value() : TargetModifier{},
                    SubConditionAttribute{attr, attrValue, negative}};
}

// ------------------------------- END SUBCONDITIONS -----------------------------------------------------------------------------------------

// ------------------------------- TARGET SELECTORS -----------------------------------------------------------------------------------------

/**
 * @brief Adds a target selector for nearest of a type to an action
 * 
 * @param action : action to add target to
 * @param ctx : target context
 */
void BehaviorBuilder::add_nearest_type_target(Action& action, BehaviorParser::Nearest_typeContext* ctx,
                                              std::optional<TargetModifier> modifier) {
  auto  types = ctx->id_list()->ID();
  Ptype comPtype = get_composite_type(types);
  auto  typeStrs = make_list_strs(types);
  bool  allPeds = !ctx->id_list()->PEDESTRIAN().empty();

  // spdlog::debug(R"(Behavior: "{}": Adding Target: Nearest "{}")", _currentBehavior.get_name(),
                fmt::join(typeStrs, " or "));
                action.add_target(TargetSelector(TargetNearest{comPtype, allPeds, std::move(modifier)}));
}

// ------------------------------- END TARGET SELECTORS -----------------------------------------------------------------------------------------

// --------------------------------- TARGET MODIFIERS ------------------------------------------------------------------------------------------------

/**
 * @brief Adds a distance modifier to a target modifier
 * 
 * @param modifier : 
 * @param ctx : 
 */
void BehaviorBuilder::add_distance_modifier(TargetModifier&                  modifier,
                                            BehaviorParser::DistanceContext* ctx) const {
  NumericValue value = get_numeric(ctx->value_numeric(), _currSeed);
  modifier.add_check(ModifierDistance{value});
}

/**
 * @brief Adds a direction modifier to a target modifier
 * 
 * @param modifier : 
 * @param ctx : 
 */
void BehaviorBuilder::add_direction_modifier(TargetModifier&                   modifier,
                                             BehaviorParser::DirectionContext* ctx) {
  auto direction = make_direction(ctx);
  modifier.add_check(ModifierDirection{direction});
}

/**
 * @brief Adds a location modifier to a target modifier
 * 
 * @param modifier : 
 * @param ctx : 
 */
void BehaviorBuilder::add_location_modifier(TargetModifier&                           modifier,
                                            BehaviorParser::Location_modifierContext* ctx) const {
  auto location = get_check_location(ctx->LOC_NAME()->toString());

  modifier.add_check(ModifierLocation{location});
}

// --------------------------------- END TARGET MODIFIERS ------------------------------------------------------------------------------------------------

// ------------------------------- SUBSELECTORS -----------------------------------------------------------------------------------------

/**
 * @brief Creates an Eveyone selector
 * 
 * @param type : type selector selects for
 * @param required : whether the selector needs to be satisfied
 * @return SubSelector 
 */
auto BehaviorBuilder::build_everyone_selector(slType type, bool required) -> SubSelector {
  auto  types = type->id_list()->ID();
  Ptype comPtype = get_composite_type(types);

  auto typeStrs = make_list_strs(types);

  // spdlog::debug(R"(Behavior "{}": Adding Selector: "Everyone" Is Ped Type: {})", _currentBehavior.get_name(),
                fmt::join(typeStrs, ", "));
                return SubSelector{0, comPtype, required, SelectorEveryone{}};
}

/**
 * @brief Creates an Exactly N selector
 * 
 * @param type : type selector selects for
 * @param selector : selector context
 * @param group : group selector pulls from
 * @param required : whether the selector needs to be satisfied
 * @return SubSelector 
 */
auto BehaviorBuilder::build_exactly_n_selector(slType type, slSelector selector, std::optional<slGroup> group,
                                               bool required) -> SubSelector {
  auto  types = type->id_list()->ID();
  Ptype comPtype = get_composite_type(types);
  auto  typeStrs = make_list_strs(types);
  auto [groupType, groupName] = get_group(group);

  NumericValue nPeds =
      get_numeric(selector->selector()->selector_Exactly_N_Random()->value_number(), _currSeed);
  // spdlog::debug(R"(Behavior "{}": Adding Selector: "Exactly {}" of {} Are Ped Type: {})",
                _currentBehavior.get_name(), nPeds.value(0), groupName, fmt::join(typeStrs, ", "));
                return SubSelector{groupType, comPtype, required, SelectorExactlyN{nPeds}};
}

/**
 * @brief Creates a percent selector
 * 
 * @param type : type selector selects for
 * @param selector : selector context
 * @param group : group selector pulls from
 * @param required : whether the selector needs to be satisfied
 * @return SubSelector 
 */
auto BehaviorBuilder::build_percent_selector(slType type, slSelector selector, std::optional<slGroup> group,
                                             bool required) -> SubSelector {
  auto  types = type->id_list()->ID();
  Ptype comPtype = get_composite_type(types);
  auto  typeStrs = make_list_strs(types);
  auto [groupType, groupName] = get_group(group);

  NumericValue percentage = get_numeric(selector->selector()->selector_Percent()->value_number(), _currSeed);
  // spdlog::debug(R"(Behavior "{}": Adding Selector: "{} Percent" of {} Are Ped Type: {})",
                _currentBehavior.get_name(), percentage.value(0), groupName, fmt::join(typeStrs, ", "));
                return SubSelector{groupType, comPtype, required,
                                   SelectorPercent{percentage.value(0) / 100.0F}};
}

/**
 * @brief Creates a location selector
 * 
 * @param type 
 * @param selector 
 * @param group 
 * @param required 
 * @return SubSelector 
 */
auto BehaviorBuilder::build_location_selector(slType type, slSelector selector, std::optional<slGroup> group,
                                              bool required) -> SubSelector {
  auto  types = type->id_list()->ID();
  Ptype comPtype = get_composite_type(types);
  auto  typeStrs = make_list_strs(types);
  auto [groupType, groupName] = get_group(group);
  auto locName = selector->selector()->selector_Location()->LOC_NAME()->toString();
  auto location = get_check_location(locName);

  // spdlog::debug(R"(Behavior "{}": Adding Selector: "In {}" Are Ped Type: {})", _currentBehavior.get_name(),
                locName, groupName, fmt::join(typeStrs, ", "));
                return SubSelector{groupType, comPtype, required, SelectorLocation{location}};
}

// ------------------------------- END SUBSELECTORS -----------------------------------------------------------------------------------------

}  // namespace VIPRA::Behaviors