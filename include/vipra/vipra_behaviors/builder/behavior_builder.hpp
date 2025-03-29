#pragma once

#include <filesystem>

#include "vipra/vipra_behaviors/attributes/attributes.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_enter_objective.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_event_occurring.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_event_starting.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_exists.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_in_location.hpp"

#include "vipra/vipra_behaviors/behavior/human_behavior.hpp"
#include "vipra/vipra_behaviors/builder/behavior_error_listener.hpp"
#include "vipra/vipra_behaviors/builder/builder_maps.hpp"
#include "vipra/vipra_behaviors/builder/declaration_components.hpp"

#include "vipra/vipra_behaviors/conditions/condition.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_attribute.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_elapsed_time.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_enter.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_event.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_event_ending.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_leave.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_spatial.hpp"

#include "vipra/vipra_behaviors/targets/target_modifier.hpp"

#include "vipra/vipra_behaviors/values/direction.hpp"

#include "vipra/vipra_behaviors/_grammar/generated/BehaviorBaseVisitor.h"
#include "vipra/vipra_behaviors/_grammar/generated/BehaviorLexer.h"
#include "vipra/vipra_behaviors/_grammar/generated/BehaviorParser.h"

#include "vipra/geometry/f3d.hpp"
#include "vipra/logging/logging.hpp"

// TODO(rolland): give proper errors and logging

namespace VIPRA::Behaviors {
/**
 * @brief Parses Behavior Files and Creates the Runtime Functionality they describe
 * 
 */

class BehaviorBuilder : public BehaviorBaseVisitor {
  DEFAULT_CONSTRUCTIBLE(BehaviorBuilder)
  COPYABLE(BehaviorBuilder)
  MOVEABLE(BehaviorBuilder)

 public:
  [[nodiscard]] auto build(std::string name, std::filesystem::path const& filepath,
                           Modules::Map const& map, VIPRA::seed seed) -> HumanBehavior;

 private:
  BehaviorErrorListener _errorListener;

  StateMap    _states;
  TypeMap     _types;
  EventMap    _eventsMap;
  LocationMap _locations;

  Condition     _startCond;
  Event         _startEvent;
  HumanBehavior _currentBehavior;

  Behaviors::stateUID _currState{};
  Behaviors::typeUID  _currType{};
  VIPRA::seed         _currSeed{};

  void initial_behavior_setup(std::string const& behaviorName, Modules::Map const& map,
                              VIPRA::seed seedNum);
  void initialize_types();
  void initialize_events();
  void initialize_states();
  void initialize_locations(Modules::Map const& map);
  void end_behavior_check();

  // ------------------------------- UTIL -----------------------------------------------------------------------------------------

  void add_atom_to_action(Action& action, BehaviorParser::Action_atomContext* atom);
  void add_target_to_action(Action& action, BehaviorParser::TargetContext* ctx);

  [[nodiscard]] auto add_event(BehaviorParser::Event_nameContext* ctx) -> VIPRA::idx;

  [[nodiscard]] auto build_condition(BehaviorParser::ConditionContext* cond) -> Condition;
  void               condition_tree_condition(BehaviorParser::ConditionContext* condition,
                                              Condition&                        tree);
  void condition_tree_unary(BehaviorParser::UnaryContext* unary, Condition& tree);
  void condition_tree_primary(BehaviorParser::PrimaryContext* primary, Condition& tree);
  void add_sub_condition(Condition&                            condTree,
                         BehaviorParser::Sub_conditionContext* subcond);

  [[nodiscard]] auto build_sub_selector(slType type, slSelector selector,
                                        std::optional<slGroup> group,
                                        bool                   required) -> SubSelector;

  [[nodiscard]] auto get_location(std::string const& name) const
      -> std::optional<VIPRA::idx>;
  [[nodiscard]] auto get_state(std::string const& state) const
      -> std::optional<Behaviors::stateUID>;
  [[nodiscard]] auto get_event(std::string const& name) const
      -> std::optional<VIPRA::idx>;
  [[nodiscard]] auto get_range(BehaviorParser::Value_numberContext* ctx) const
      -> VIPRA::time_range_s;
  [[nodiscard]] auto get_type(std::string const& type) const
      -> std::optional<Behaviors::typeUID>;
  [[nodiscard]] auto get_group(std::optional<slGroup> group) const
      -> std::pair<Behaviors::typeUID, std::string>;
  [[nodiscard]] auto get_composite_type(
      std::vector<antlr4::tree::TerminalNode*> const& types) const -> Behaviors::Ptype;
  [[nodiscard]] static auto get_attribute(std::string attr) -> Behaviors::Attribute;

  [[nodiscard]] auto get_check_location(std::string const& name) const -> VIPRA::idx;
  [[nodiscard]] auto get_check_state(std::string const& name) const
      -> Behaviors::stateUID;
  [[nodiscard]] auto get_check_event(std::string const& name) const -> VIPRA::idx;
  [[nodiscard]] auto get_check_type(std::string const& name) const -> Behaviors::typeUID;

  [[nodiscard]] auto make_attribute_value(BehaviorParser::Attr_valueContext* ctx)
      -> Behaviors::CAttributeValue;
  [[nodiscard]] static auto make_attribute_str(BehaviorParser::AttributeContext* ctx)
      -> std::string;
  [[nodiscard]] static auto make_list_strs(
      std::vector<antlr4::tree::TerminalNode*> const& types) -> std::vector<std::string>;
  [[nodiscard]] auto make_target_modifier(std::vector<BehaviorParser::ModifierContext*>&
                                              modifiers) -> std::optional<TargetModifier>;
  [[nodiscard]] static auto make_direction(BehaviorParser::DirectionContext* ctx)
      -> Direction;
  [[nodiscard]] auto make_dimensions(BehaviorParser::Loc_dimensionsContext* ctx) const
      -> std::tuple<VIPRA::f3d, VIPRA::f3d, VIPRA::f_pnt>;

  /**
   * @brief Logs an error to the console and throws an exception
   * 
   * @tparam T : 
   * @param message : message to log to console
   * @param values : values to format message with
   */
  template <typename... args_t>
  [[noreturn]] static void error(fmt::format_string<args_t...> message,
                                 args_t&&... values)
  {
    VIPRA::Log::error(message, std::forward<args_t>(values)...);
    BuilderException::error();
  }

  // ------------------------------- ANTLR FUNCTIONS -----------------------------------------------------------------------------------------

  auto visitEvent(BehaviorParser::EventContext* /*ctx*/) -> antlrcpp::Any override;
  auto visitPed_Selector(BehaviorParser::Ped_SelectorContext* /*ctx*/)
      -> antlrcpp::Any override;
  auto visitAction(BehaviorParser::ActionContext* /*ctx*/) -> antlrcpp::Any override;
  auto visitDecl_Ped_State(BehaviorParser::Decl_Ped_StateContext* /*ctx*/)
      -> antlrcpp::Any override;
  auto visitDecl_Ped(BehaviorParser::Decl_PedContext* /*ctx*/) -> antlrcpp::Any override;
  auto visitLocation(BehaviorParser::LocationContext* /*ctx*/) -> antlrcpp::Any override;

  // --------------------------------- ATOMS ------------------------------------------------------------------------------------------------

  void add_set_atom(Action& /*action*/, BehaviorParser::Set_atomContext* /*ctx*/);
  void add_scale_atom(Action& /*action*/, BehaviorParser::Scale_atomContext* /*ctx*/);
  static void add_set_obj_atom(Action& /*action*/,
                               BehaviorParser::Set_objective_atomContext* /*ctx*/);

  // --------------------------------- TARGET SELECTORS ------------------------------------------------------------------------------------------------

  void add_nearest_type_target(Action& /*action*/,
                               BehaviorParser::Nearest_typeContext* /*ctx*/,
                               std::optional<TargetModifier>& /*modifier*/);

  // --------------------------------- TARGET MODIFIERS ------------------------------------------------------------------------------------------------

  void add_modifier(TargetModifier& /*targetModifier*/,
                    BehaviorParser::ModifierContext* /*modifier*/) const;

  void        add_distance_modifier(TargetModifier& /*modifier*/,
                                    BehaviorParser::DistanceContext* /*ctx*/) const;
  static void add_direction_modifier(TargetModifier& /*modifier*/,
                                     BehaviorParser::DirectionContext* /*ctx*/);
  void        add_location_modifier(TargetModifier& /*modifier*/,
                                    BehaviorParser::Location_modifierContext* /*ctx*/) const;

  // --------------------------------- SUBCONDITIONS ------------------------------------------------------------------------------------------------

  [[nodiscard]] auto build_enter_obj_subcond(
      BehaviorParser::Condition_Enter_LocationContext* /*ctx*/) -> SubConditionEnterObj;
  [[nodiscard]] auto build_enter_subcond(
      BehaviorParser::Condition_Enter_LocationContext* /*ctx*/) -> SubConditionEnter;
  [[nodiscard]] auto build_exit_subcond(
      BehaviorParser::Condition_Exit_LocationContext* /*ctx*/) -> SubConditionLeave;
  [[nodiscard]] auto build_time_elapsed_subcond(
      BehaviorParser::Condition_Time_Elapsed_From_EventContext* /*ctx*/)
      -> SubConditionElapsedTimeFromEvent;
  [[nodiscard]] auto build_event_occurred_subcond(
      BehaviorParser::Condition_Event_OccurredContext* /*ctx*/)
      -> SubConditionEventOccurred;
  [[nodiscard]] auto build_event_occurring_subcond(
      BehaviorParser::Condition_Event_OccurringContext* /*ctx*/)
      -> SubConditionEventOccurring;
  [[nodiscard]] auto build_event_starting_subcond(
      BehaviorParser::Condition_Event_StartingContext* /*ctx*/)
      -> SubConditionEventStarting;
  [[nodiscard]] auto build_event_ending_subcond(
      BehaviorParser::Condition_Event_EndingContext* /*ctx*/) -> SubConditionEventEnding;
  [[nodiscard]] auto build_spatial_subcond(
      BehaviorParser::Condition_SpatialContext* /*ctx*/) -> SubConditionSpatial;
  [[nodiscard]] auto build_in_location_subcond(
      BehaviorParser::Condition_Inside_LocationContext* /*ctx*/)
      -> SubConditionInLocation;
  [[nodiscard]] auto build_attribute_subcond(
      BehaviorParser::Condition_AttributeContext* /*ctx*/) -> SubConditionAttribute;
  [[nodiscard]] auto build_exists_subcond(
      BehaviorParser::Condition_ExistsContext* /*ctx*/) -> SubConditionExists;

  // --------------------------------- SUBSELECTORS ------------------------------------------------------------------------------------------------

  auto build_everyone_selector(slType /*type*/, bool /*required*/) -> SubSelector;
  auto build_exactly_n_selector(slType /*type*/, slSelector /*selector*/,
                                std::optional<slGroup> /*group*/,
                                bool /*required*/) -> SubSelector;
  auto build_percent_selector(slType /*type*/, slSelector /*selector*/,
                              std::optional<slGroup> /*group*/,
                              bool /*required*/) -> SubSelector;
  auto build_location_selector(slType /*type*/, slSelector /*selector*/,
                               std::optional<slGroup> /*group*/,
                               bool /*required*/) -> SubSelector;

  // --------------------------------- LOCATION ------------------------------------------------------------------------------------------------

  // -------------------------------- FINDERS ------------------------------------------------------------------------------------------------

  /**
   * @brief Gets components from an event declaration context
   * 
   * @tparam T : type of component
   * @param ctx : event context
   * @return std::optional<T> 
   */
  template <typename comp_t>
  [[nodiscard]] static auto find_event_component(BehaviorParser::EventContext* ctx)
      -> std::optional<comp_t>
  {
    for ( auto const& attr : ctx->event_attribute() ) {
      // Name
      if constexpr ( std::is_same_v<comp_t, std::string> )
        if ( attr->event_name() ) return attr->event_name()->ID()->toString();

      // Start Condition
      if constexpr ( std::is_same_v<comp_t, evStart> )
        if ( attr->event_start() ) return attr->event_start();

      // End Condition
      if constexpr ( std::is_same_v<comp_t, evEnd> )
        if ( attr->event_end() ) return attr->event_end();
    }

    return std::nullopt;
  }

  /**
   * @brief Gets components from an action declaration context
   * 
   * @tparam T : type of component
   * @param ctx : action context
   * @return std::optional<T> 
   */
  template <typename comp_t>
  [[nodiscard]] static auto find_action_component(BehaviorParser::ActionContext* ctx)
      -> std::optional<comp_t>
  {
    for ( auto const& attr : ctx->action_attribute() ) {
      // Stimulus
      if constexpr ( std::is_same_v<comp_t, acStimulus> )
        if ( attr->action_stimulus() ) return attr->action_stimulus();

      // Response
      if constexpr ( std::is_same_v<comp_t, acResponse> )
        if ( attr->action_response() ) return attr->action_response();

      // Target
      if constexpr ( std::is_same_v<comp_t, acTarget> )
        if ( attr->action_target() ) return attr->action_target();

      // Duration
      if constexpr ( std::is_same_v<comp_t, acDuration> )
        if ( attr->action_duration() ) return attr->action_duration();
    }

    return std::nullopt;
  }

  /**
   * @brief Gets components from an selector declaration context
   * 
   * @tparam T : type of component
   * @param ctx : selector context
   * @return std::optional<T> 
   */
  template <typename comp_t>
  [[nodiscard]] static auto find_selector_component(
      BehaviorParser::Ped_SelectorContext* ctx) -> std::optional<comp_t>
  {
    for ( auto const& attr : ctx->selector_attribute() ) {
      // Type
      if constexpr ( std::is_same_v<comp_t, slType> )
        if ( attr->selector_type() ) return attr->selector_type();

      // Selector
      if constexpr ( std::is_same_v<comp_t, slSelector> )
        if ( attr->selector_selector() ) return attr->selector_selector();

      // Group
      if constexpr ( std::is_same_v<comp_t, slGroup> )
        if ( attr->selector_from() ) return attr->selector_from();

      // Required
      if constexpr ( std::is_same_v<comp_t, slRequired> )
        if ( attr->selector_required() ) return attr->selector_required();
    }

    return std::nullopt;
  }

  /**
   * @brief Gets components from an selector declaration context
   * 
   * @tparam T : type of component
   * @param ctx : selector context
   * @return std::optional<T> 
   */
  template <typename comp_t>
  [[nodiscard]] static auto find_location_component(BehaviorParser::LocationContext* ctx)
      -> std::optional<comp_t>
  {
    for ( auto const& attr : ctx->location_attribute() ) {
      // Name
      if constexpr ( std::is_same_v<comp_t, lcName> )
        if ( attr->loc_name() ) return attr->loc_name();

      // Dimensions
      if constexpr ( std::is_same_v<comp_t, lcDimensions> )
        if ( attr->loc_dimensions() ) return attr->loc_dimensions();
    }

    return std::nullopt;
  }
};

}  // namespace VIPRA::Behaviors
