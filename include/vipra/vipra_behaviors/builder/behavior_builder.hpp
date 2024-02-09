#ifndef VIPRA_BEHAVIORS_BEHAVIOR_BUILDER_HPP
#define VIPRA_BEHAVIORS_BEHAVIOR_BUILDER_HPP

#include <filesystem>
#include <stdexcept>

#include "vipra/vipra_behaviors/generated/BehaviorBaseVisitor.h"
#include "vipra/vipra_behaviors/generated/BehaviorParser.h"

#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_attribute.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_elapsed_time.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_enter.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_event.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_event_ending.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_event_occurring.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_event_starting.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_exists.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_in_location.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_leave.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_spatial.hpp"

#include "selectors/subselector.hpp"

#include "behavior/human_behavior.hpp"
#include "builder/behavior_error_listener.hpp"

#include "events/event.hpp"
#include "locations/location.hpp"

#include "attributes/attributes.hpp"
#include "conditions/condition.hpp"
#include "definitions/dsl_types.hpp"
#include "time/time.hpp"
#include "values/values.hpp"

#include "builder/builder_maps.hpp"
#include "builder/declaration_components.hpp"
#include "targets/target_modifier.hpp"
#include "values/direction.hpp"

namespace BHVR {
/**
 * @brief Parses Behavior Files and Creates the Runtime Functionality they describe
 * 
 */
class BehaviorBuilder : public BehaviorBaseVisitor {
  DEFAULT_CONSTRUCTIBLE(BehaviorBuilder)
  COPYABLE(BehaviorBuilder)
  MOVEABLE(BehaviorBuilder)

 public:
  [[nodiscard]] auto build(std::string, std::filesystem::path const&, BHVR::seed) -> HumanBehavior;

 private:
  BehaviorErrorListener _errorListener;

  StateMap    _states;
  TypeMap     _types;
  EventMap    _eventsMap;
  LocationMap _locations;

  Condition     _startCond;
  Event         _startEvent;
  HumanBehavior _currentBehavior;

  BHVR::stateUID _currState{};
  BHVR::typeUID  _currType{};
  BHVR::seed     _currSeed{};

  void initial_behavior_setup(std::string const&, BHVR::seed);
  void initialize_types();
  void initialize_events();
  void initialize_states();
  void initialize_locations();
  void end_behavior_check();

  // ------------------------------- UTIL -----------------------------------------------------------------------------------------

  void add_atom_to_action(Action&, BehaviorParser::Action_atomContext*);
  void add_target_to_action(Action&, BehaviorParser::TargetContext*);

  [[nodiscard]] auto add_event(BehaviorParser::Event_nameContext*) -> VIPRA::idx;

  [[nodiscard]] auto build_condition(BehaviorParser::ConditionContext*) -> Condition;
  void               condition_tree_condition(BehaviorParser::ConditionContext*, Condition&);
  void               condition_tree_unary(BehaviorParser::UnaryContext*, Condition&);
  void               condition_tree_primary(BehaviorParser::PrimaryContext*, Condition&);
  void               add_sub_condition(Condition&, BehaviorParser::Sub_conditionContext*);

  [[nodiscard]] auto build_sub_selector(slType, slSelector, std::optional<slGroup>, bool) -> SubSelector;

  [[nodiscard]] auto get_location(std::string const&) const -> std::optional<VIPRA::idx>;
  [[nodiscard]] auto get_state(std::string const&) const -> std::optional<BHVR::stateUID>;
  [[nodiscard]] auto get_event(std::string const&) const -> std::optional<VIPRA::idx>;
  [[nodiscard]] auto get_range(BehaviorParser::Value_numberContext*) const -> VIPRA::time_range_s;
  [[nodiscard]] auto get_type(std::string const&) const -> std::optional<BHVR::typeUID>;
  [[nodiscard]] auto get_group(std::optional<slGroup>) const -> std::pair<BHVR::typeUID, std::string>;
  [[nodiscard]] auto get_composite_type(std::vector<antlr4::tree::TerminalNode*> const&) const -> BHVR::Ptype;
  [[nodiscard]] static auto get_attribute(std::string) -> BHVR::Attribute;

  [[nodiscard]] auto get_check_location(std::string const&) const -> VIPRA::idx;
  [[nodiscard]] auto get_check_state(std::string const&) const -> BHVR::stateUID;
  [[nodiscard]] auto get_check_event(std::string const&) const -> VIPRA::idx;
  [[nodiscard]] auto get_check_type(std::string const&) const -> BHVR::typeUID;

  [[nodiscard]] auto        make_attribute_value(BehaviorParser::Attr_valueContext*) -> BHVR::CAttributeValue;
  [[nodiscard]] static auto make_attribute_str(BehaviorParser::AttributeContext*) -> std::string;
  [[nodiscard]] static auto make_list_strs(std::vector<antlr4::tree::TerminalNode*> const&)
      -> std::vector<std::string>;
  [[nodiscard]] auto make_target_modifier(std::vector<BehaviorParser::ModifierContext*>&)
      -> std::optional<TargetModifier>;
  [[nodiscard]] static auto make_direction(BehaviorParser::DirectionContext*) -> Direction;
  [[nodiscard]] auto        make_dimensions(BehaviorParser::Loc_dimensionsContext*) const
      -> std::tuple<VIPRA::f3d, VIPRA::f3d, float>;

  /**
   * @brief Logs an error to the console and throws an exception
   * 
   * @tparam T : 
   * @param message : message to log to console
   * @param values : values to format message with
   */
  template <typename... args_t>
  [[noreturn]] static void error(std::string const& message, args_t&&... values) {
    // spdlog::error(message, std::forward<args_t>(values)...);
    BuilderException::error();
  }

  // ------------------------------- ANTLR FUNCTIONS -----------------------------------------------------------------------------------------

  auto visitEvent(BehaviorParser::EventContext* /*ctx*/) -> antlrcpp::Any override;
  auto visitPed_Selector(BehaviorParser::Ped_SelectorContext* /*ctx*/) -> antlrcpp::Any override;
  auto visitAction(BehaviorParser::ActionContext* /*ctx*/) -> antlrcpp::Any override;
  auto visitDecl_Ped_State(BehaviorParser::Decl_Ped_StateContext* /*ctx*/) -> antlrcpp::Any override;
  auto visitDecl_Ped(BehaviorParser::Decl_PedContext* /*ctx*/) -> antlrcpp::Any override;
  auto visitLocation(BehaviorParser::LocationContext* /*ctx*/) -> antlrcpp::Any override;

  // --------------------------------- ATOMS ------------------------------------------------------------------------------------------------

  void add_set_atom(Action&, BehaviorParser::Set_atomContext*);
  void add_scale_atom(Action&, BehaviorParser::Scale_atomContext*);

  // --------------------------------- TARGET SELECTORS ------------------------------------------------------------------------------------------------

  void add_nearest_type_target(Action&, BehaviorParser::Nearest_typeContext*, std::optional<TargetModifier>);

  // --------------------------------- TARGET MODIFIERS ------------------------------------------------------------------------------------------------

  void add_modifier(TargetModifier&, BehaviorParser::ModifierContext*) const;

  void        add_distance_modifier(TargetModifier&, BehaviorParser::DistanceContext*) const;
  static void add_direction_modifier(TargetModifier&, BehaviorParser::DirectionContext*);
  void        add_location_modifier(TargetModifier&, BehaviorParser::Location_modifierContext*) const;

  // --------------------------------- SUBCONDITIONS ------------------------------------------------------------------------------------------------

  [[nodiscard]] auto build_enter_subcond(BehaviorParser::Condition_Enter_LocationContext*)
      -> SubConditionEnter;
  [[nodiscard]] auto build_exit_subcond(BehaviorParser::Condition_Exit_LocationContext*) -> SubConditionLeave;
  [[nodiscard]] auto build_time_elapsed_subcond(BehaviorParser::Condition_Time_Elapsed_From_EventContext*)
      -> SubConditionElapsedTimeFromEvent;
  [[nodiscard]] auto build_event_occurred_subcond(BehaviorParser::Condition_Event_OccurredContext*)
      -> SubConditionEventOccurred;
  [[nodiscard]] auto build_event_occurring_subcond(BehaviorParser::Condition_Event_OccurringContext*)
      -> SubConditionEventOccurring;
  [[nodiscard]] auto build_event_starting_subcond(BehaviorParser::Condition_Event_StartingContext*)
      -> SubConditionEventStarting;
  [[nodiscard]] auto build_event_ending_subcond(BehaviorParser::Condition_Event_EndingContext*)
      -> SubConditionEventEnding;
  [[nodiscard]] auto build_spatial_subcond(BehaviorParser::Condition_SpatialContext*) -> SubConditionSpatial;
  [[nodiscard]] auto build_in_location_subcond(BehaviorParser::Condition_Inside_LocationContext*)
      -> SubConditionInLocation;
  [[nodiscard]] auto build_attribute_subcond(BehaviorParser::Condition_AttributeContext*)
      -> SubConditionAttribute;
  [[nodiscard]] auto build_exists_subcond(BehaviorParser::Condition_ExistsContext*) -> SubConditionExists;

  // --------------------------------- SUBSELECTORS ------------------------------------------------------------------------------------------------

  auto build_everyone_selector(slType, bool) -> SubSelector;
  auto build_exactly_n_selector(slType, slSelector, std::optional<slGroup>, bool) -> SubSelector;
  auto build_percent_selector(slType, slSelector, std::optional<slGroup>, bool) -> SubSelector;
  auto build_location_selector(slType, slSelector, std::optional<slGroup>, bool) -> SubSelector;

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
  [[nodiscard]] static auto find_event_component(BehaviorParser::EventContext* ctx) -> std::optional<comp_t> {
    for (auto const& attr : ctx->event_attribute()) {
      // Name
      if constexpr (std::is_same_v<comp_t, std::string>)
        if (attr->event_name()) return attr->event_name()->ID()->toString();

      // Start Condition
      if constexpr (std::is_same_v<comp_t, evStart>)
        if (attr->event_start()) return attr->event_start();

      // End Condition
      if constexpr (std::is_same_v<comp_t, evEnd>)
        if (attr->event_end()) return attr->event_end();
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
      -> std::optional<comp_t> {
    for (auto const& attr : ctx->action_attribute()) {
      // Stimulus
      if constexpr (std::is_same_v<comp_t, acStimulus>)
        if (attr->action_stimulus()) return attr->action_stimulus();

      // Response
      if constexpr (std::is_same_v<comp_t, acResponse>)
        if (attr->action_response()) return attr->action_response();

      // Target
      if constexpr (std::is_same_v<comp_t, acTarget>)
        if (attr->action_target()) return attr->action_target();

      // Duration
      if constexpr (std::is_same_v<comp_t, acDuration>)
        if (attr->action_duration()) return attr->action_duration();
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
  [[nodiscard]] static auto find_selector_component(BehaviorParser::Ped_SelectorContext* ctx)
      -> std::optional<comp_t> {
    for (auto const& attr : ctx->selector_attribute()) {
      // Type
      if constexpr (std::is_same_v<comp_t, slType>)
        if (attr->selector_type()) return attr->selector_type();

      // Selector
      if constexpr (std::is_same_v<comp_t, slSelector>)
        if (attr->selector_selector()) return attr->selector_selector();

      // Group
      if constexpr (std::is_same_v<comp_t, slGroup>)
        if (attr->selector_from()) return attr->selector_from();

      // Required
      if constexpr (std::is_same_v<comp_t, slRequired>)
        if (attr->selector_required()) return attr->selector_required();
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
      -> std::optional<comp_t> {
    for (auto const& attr : ctx->location_attribute()) {
      // Name
      if constexpr (std::is_same_v<comp_t, lcName>)
        if (attr->loc_name()) return attr->loc_name();

      // Dimensions
      if constexpr (std::is_same_v<comp_t, lcDimensions>)
        if (attr->loc_dimensions()) return attr->loc_dimensions();
    }

    return std::nullopt;
  }
};
}  // namespace BHVR

#endif