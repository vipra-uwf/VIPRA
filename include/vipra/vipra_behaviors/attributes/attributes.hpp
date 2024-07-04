#pragma once

#include <stdexcept>
#include <type_traits>

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/behavior/exceptions.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/events/event_status.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {
/**
  * @brief Attribute Type
  * 
  */
enum class Attribute {
  INVALID = 0,
  POSITION,
  DIMENSIONS,
  VELOCITY,
  END_GOAL,
  CURR_GOAL,
  STATE,
  LOCATION,
  STATUS,
};

/**
 * @brief Attribute Value Type Information
 * 
 */
enum class Type {
  INVALID = 0,
  NUMBER,
  COORD,
  STATE,
  STATUS,
  LOCATION,
  TOWARDS_LOC,
  TOWARDS_ATTR,
};

/**
 * @brief Holds an immutable attribute value and its type
 * 
 */
struct CAttributeValue {
  Type        type;
  void const* value;

  CAttributeValue(Type aType, void const* aVal) : type(aType), value(aVal) {}

  inline void type_check(Type check) const {
    if ( type != check ) DSLException::error("Invalid Type");
  }

  /**
   * @brief Returns a refernce to the value as a given type
   * 
   * @tparam value_t 
   * @return const value_t& 
   */
  template <typename value_t>
  [[nodiscard]] inline auto as() const -> value_t const& {
    return *static_cast<value_t const*>(value);
  }

  /**
   * @brief Returns a pointer to the value as a given type
   * 
   * @tparam value_t 
   * @return const value_t* 
   */
  template <typename value_t>
  [[nodiscard]] inline auto as_ptr() const -> value_t const* {
    return static_cast<value_t const*>(value);
  }
};

/**
 * @brief Holds a mutable attribute value and its type
 * 
 */
struct AttributeValue {
  Type  type;
  void* value;

  /**
   * @brief Checks that the value is a given type, throws exception if not correct type
   * 
   * @param check : type value should be
   */
  inline void type_check(Type check) const {
    if ( type != check ) DSLException::error("Invalid Type");
  }

  /**
   * @brief Returns a refernce to the value as a given type
   * 
   * @tparam value_t 
   * @return value_t& 
   */
  template <typename value_t>
  [[nodiscard]] inline auto as() -> value_t& {
    return *static_cast<value_t*>(value);
  }

  /**
   * @brief Returns a pointer to the value as a given type
   * 
   * @tparam value_t 
   * @return value_t* 
   */
  template <typename value_t>
  [[nodiscard]] inline auto as_ptr() const -> value_t* {
    return static_cast<value_t*>(value);
  }
};

/**
 * @brief Handles all getting/setting of attributes in Behaviors
 * 
 */
class AttributeHandling {
 public:
  /**
   * @brief Returns the specified attribute value of the target
   * 
   * @param target : target to get attribute from
   * @param attr : attribute type to get
   * @param pack : simulation pack
   * @return CAttributeValue 
   */
  [[nodiscard]] inline static auto get_value(Target target, Attribute attr, auto pack) -> CAttributeValue {
    switch ( target.type ) {
      case TargetType::PEDESTRIAN:
        return get_ped_value(target, attr, pack);
      case TargetType::EVENT:
        return get_event_value(target, attr, pack);
      case TargetType::LOCATION:
        return get_location_value(target, attr, pack);
      case TargetType::INVALID:
        return {Type::INVALID, nullptr};
    }

    return {Type::INVALID, nullptr};
  }

  /**
   * @brief Sets a targets attribute value
   * 
   * @param target : target to set the attribute of
   * @param attr : attribute to set
   * @param pack : simulation pack
   * @param state : timestep state
   * @param value : value to set attribute to
   */
  inline static void set_value(Target target, Attribute attr, auto pack, VIPRA::State& state,
                               CAttributeValue value) {
    switch ( target.type ) {
      case TargetType::PEDESTRIAN:
        set_ped_value(target, attr, pack, state, value);
        return;
      case TargetType::EVENT:
        set_event_value(target, attr, pack, value);
        return;
      case TargetType::LOCATION:
        set_location_value(target, attr, pack, value);
        return;
      case TargetType::INVALID:
        return;
    }
  }

  /**
   * @brief Scales a targets attribute value
   * 
   * @param target : target to scale the attribute of
   * @param attr : attribute to scale
   * @param pack : simulation pack
   * @param state : timestep state
   * @param value : value to scale attribute by
   */
  inline static void scale_value(Target target, Attribute attr, auto pack, VIPRA::State& state,
                                 CAttributeValue value) {
    switch ( target.type ) {
      case TargetType::PEDESTRIAN:
        scale_ped_value(target, attr, pack, state, value);
        return;
      case TargetType::EVENT:
        DSLException::error("Cannot Scale Event Value");
        return;
      case TargetType::LOCATION:
        DSLException::error("Cannot Scale Location Value");
        return;
      case TargetType::INVALID:
        return;
    }
  }

  /**
   * @brief Checks if the values are a coordinate and a value, returns true if so
   * 
   * @param value1 : first value
   * @param value2 : second value
   * @return true : if one is a coordinate and the other is a location
   * @return false : if not
   */
  [[nodiscard]] inline static auto is_coord_loc_compare(CAttributeValue value1, CAttributeValue value2)
      -> bool {
    return (value1.type == Type::COORD && value2.type == Type::LOCATION) ||
           (value1.type == Type::LOCATION && value2.type == Type::COORD);
  }

  /**
   * @brief Compares a coordinate and location, returns true if the coordinate is in the location
   * 
   * @param value1 : first value
   * @param value2 : second value
   * @param pack : simulation pack
   * @return true : if coordinate is in location
   * @return false : if not
   */
  [[nodiscard]] inline static auto coord_loc_compare(CAttributeValue value1, CAttributeValue value2,
                                                     auto pack) -> bool {
    if ( value1.type == Type::COORD && value2.type == Type::LOCATION ) {
      return pack.context.locations[value2.as<VIPRA::idx>()].contains(value1.as<VIPRA::f3d>());
    }

    if ( value2.type == Type::COORD && value1.type == Type::LOCATION ) {
      return pack.context.locations[value1.as<VIPRA::idx>()].contains(value2.as<VIPRA::f3d>());
    }

    return false;
  }

  /**
   * @brief Checks if two AttributeValues are equal
   * 
   * @param value1 : first value
   * @param value2 : second value
   * @return true : if equal
   * @return false : if not equal or not same type
   */
  [[nodiscard]] inline static auto is_equal(CAttributeValue value1, CAttributeValue value2, auto pack)
      -> bool {
    if ( is_coord_loc_compare(value1, value2) ) {
      return coord_loc_compare(value1, value2, pack);
    }

    if ( value1.type != value2.type ) return false;

    switch ( value1.type ) {
      case Type::INVALID:
        throw std::runtime_error("Invalid Type");
        return false;
      case Type::NUMBER:
        return value1.as<VIPRA::f_pnt>() == value2.as<VIPRA::f_pnt>();
      case Type::COORD:
        return value1.as<VIPRA::f3d>() == value2.as<VIPRA::f3d>();
      case Type::STATE:
        return value1.as<stateUID>() == value2.as<stateUID>();
      case Type::STATUS:
        return value1.as<EventStatus>() == value2.as<EventStatus>();
      case Type::LOCATION:
        return value1.as<VIPRA::idx>() == value2.as<VIPRA::idx>();
        break;
      default:
        break;
    }

    return false;
  }

  /**
   * @brief Checks if two AttributeValues are not equal
   * 
   * @param value1 : first value
   * @param value2 : second value
   * @return true : if not equal
   * @return false : if equal or not same type
   */
  [[nodiscard]] inline static auto is_not_equal(CAttributeValue value1, CAttributeValue value2, auto pack)
      -> bool {
    return ! is_equal(value1, value2, pack);
  }

  /**
   * @brief Adds an attribute value to the value store, for later use in atoms
   * 
   * @tparam T : value type
   * @param type : attribute type
   * @param value : attribute value
   * @return CAttributeValue
   */
  template <typename value_t>
  [[nodiscard]] inline static auto store_value(Type type, value_t&& value) -> CAttributeValue {
    auto& valueStore = get_value_store();
    valueStore.emplace_back(type, new std::remove_cvref_t<value_t>(std::forward<value_t>(value)));
    return valueStore.back();
  }

  inline static void cleanup() {
    for ( auto data : get_value_store() ) {
      // NOLINTBEGIN(cppcoreguidelines-owning-memory)
      switch ( data.type ) {
        case Type::INVALID:
          // throw std::runtime_error("Invalid Type");
          break;
        case Type::NUMBER:
          delete data.as_ptr<VIPRA::f_pnt>();
          break;
        case Type::COORD:
          delete data.as_ptr<VIPRA::f3d>();
          break;
        case Type::STATE:
          delete data.as_ptr<stateUID>();
          break;
        case Type::STATUS:
          delete data.as_ptr<EventStatus>();
          break;
        case Type::LOCATION:
          [[fallthrough]];
        case Type::TOWARDS_LOC:
          delete data.as_ptr<VIPRA::idx>();
          break;
        case Type::TOWARDS_ATTR:
          delete data.as_ptr<Attribute>();
          break;
        default:
          // throw std::runtime_error("Cleanup Error: Unknown Type");
          break;
      }
      // NOLINTEND(cppcoreguidelines-owning-memory)
    }

    get_value_store().clear();
  }

  struct DeferedCleanup {
    DeferedCleanup(DeferedCleanup const&) = default;
    DeferedCleanup(DeferedCleanup&&) = default;
    auto operator=(DeferedCleanup const&) -> DeferedCleanup& = default;
    auto operator=(DeferedCleanup&&) -> DeferedCleanup& = default;
    DeferedCleanup() = default;
    ~DeferedCleanup() { AttributeHandling::cleanup(); }
  };

 private:
  static const DeferedCleanup CLEANUP;

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables) Bug in clang-tidy (https://bugs.llvm.org/show_bug.cgi?id=48040)
  static inline auto get_value_store() -> std::vector<CAttributeValue>& {
    static std::vector<CAttributeValue> valueStore;
    return valueStore;
  }

  /**
   * @brief Gets a pedestrians attribute value
   * 
   * @param target : pedestrian to get the attribute from
   * @param attr : attribute type
   * @param pack : simulation pack
   * @return CAttributeValue 
   */
  [[nodiscard]] inline static auto get_ped_value(Target target, Attribute attr, auto pack)
      -> CAttributeValue {
    switch ( attr ) {
      case Attribute::POSITION:
        return {Type::COORD, &pack.pedset.ped_coords(target.targetIdx)};
      case Attribute::VELOCITY:
        return {Type::COORD, &pack.state.velocities[target.targetIdx]};
      case Attribute::END_GOAL:
        return {Type::COORD, &pack.goals.end_goal(target.targetIdx)};
      case Attribute::CURR_GOAL:
        return {Type::COORD, &pack.goals.current_goal(target.targetIdx)};
      case Attribute::STATE:
        return {Type::STATE, &pack.context.pedStates[target.targetIdx]};
      default:
        DSLException::error("Invalid Pedestrian Attribute");
    }
  }

  /**
   * @brief Gets an events attribute value
   * 
   * @param target : event to get attribute of
   * @param attr : attribute type
   * @param pack : simulation pack
   * @return CAttributeValue 
   */
  [[nodiscard]] inline static auto get_event_value(Target target, Attribute attr, auto pack)
      -> CAttributeValue {
    switch ( attr ) {
      case Attribute::LOCATION:
        // TODO (rolland) : get this added in when event locations are done
        DSLException::error("Event Locations Not Implmented");
      case Attribute::STATUS:
        return {Type::STATUS, &pack.context.events[target.targetIdx].get_status()};
      default:
        DSLException::error("Invalid Event Attribute");
    }
  }

  /**
   * @brief Gets a locations attribute value
   * 
   * @param target : location to get attribute of
   * @param attr : attribute type
   * @param pack : simulation pack
   * @return CAttributeValue 
   */
  [[nodiscard]] inline static auto get_location_value(Target target, Attribute attr, auto pack)
      -> CAttributeValue {
    switch ( attr ) {
      case Attribute::POSITION:
        return {Type::COORD, &pack.context.locations[target.targetIdx].center()};
      case Attribute::DIMENSIONS:
        return {Type::COORD, &pack.context.locations[target.targetIdx].dims()};
      default:
        DSLException::error("Invalid Location Attribute");
    }
  }

  /**
   * @brief Sets a pedestrians attribute
   * 
   * @param target : pedestrian to set the attribute of
   * @param attr : attribute type
   * @param pack : simulation pack
   * @param state : next timestep state
   * @param value : value to set attribute to
   */
  inline static void set_ped_value(Target target, Attribute attr, auto pack, VIPRA::State& state,
                                   CAttributeValue value) {
    switch ( attr ) {
      case Attribute::POSITION:
        set_position(target, state, value);
        return;
      case Attribute::VELOCITY:
        set_velocity(target, pack, state, value);
        return;
      case Attribute::END_GOAL:
        set_goal(target, pack, value);
        return;
      case Attribute::CURR_GOAL:
        DSLException::error("Unable to set Pedestrian Current Goal");
        return;
      case Attribute::STATE:
        set_state(target, pack, value);
        return;
      default:
        DSLException::error("Invalid Pedestrian Attribute");
    }
  }

  /**
   * @brief Sets a locations attribute
   * 
   * @param target : location to set the attribute of
   * @param attr : attribute type
   * @param pack : simulation pack
   * @param value : value to set attribute to
   */
  inline static void set_location_value(Target target, Attribute attr, auto pack, CAttributeValue value) {
    switch ( attr ) {
      case Attribute::POSITION:
        set_location_position(target, pack, value);
      case Attribute::DIMENSIONS:
        set_location_dims(target, pack, value);
      default:
        DSLException::error("Invalid Location Attribute");
    }
  }

  /**
   * @brief Scales a pedestrians attribute by a given value
   * 
   * @param target : pedestrian to scale the attribute of
   * @param attr : attribute type
   * @param pack : simulation pack
   * @param state : next timestep state
   * @param value : value to set attribute to
   */
  inline static void scale_ped_value(Target target, Attribute attr, auto pack, VIPRA::State& state,
                                     CAttributeValue value) {
    switch ( attr ) {
      case Attribute::POSITION:
        DSLException::error("Cannot Scale Pedestrian Position");
        return;
      case Attribute::VELOCITY:
        scale_velocity(target, pack, state, value);
        return;
      case Attribute::CURR_GOAL:
      case Attribute::END_GOAL:
        DSLException::error("Cannot Scale Pedestrian Position");
        return;
      case Attribute::STATE:
        set_state(target, pack, value);
        return;
      case Attribute::INVALID:
      case Attribute::LOCATION:
      case Attribute::STATUS:
        DSLException::error("Invalid Pedestrian Attribute");
        break;
      case Attribute::DIMENSIONS:
        DSLException::error("Pedestrians Do Not Have Dimensions");
        break;
    }
  }

  /**
   * @brief Sets an events attribute
   * 
   * @param target : event to set the attribute of
   * @param attr : attribute type
   * @param pack : simulation pack
   * @param value : value to set attribute to
   */
  inline static void set_event_value(Target target, Attribute attr, auto pack, CAttributeValue value) {
    switch ( attr ) {
      case Attribute::LOCATION:
        // TODO (rolland) : get this added in when event locations are done
        DSLException::error("Event Locations Not Implmented");
      case Attribute::STATUS:
        pack.context.events[target.targetIdx].set_status(value.as<Behaviors::EventStatus>());
        return;
      default:
        DSLException::error("Invalid Event Attribute");
    }
  }

  // ------------------------------------------- SETTERS ------------------------------------------------------------------------

  /**
   * @brief Sets a pedesetrians position
   * 
   * @param target : pedestrian to set the position of
   * @param state : next time step state
   * @param value : value to set position to
   */
  static inline void set_position(Target target, VIPRA::State& state, CAttributeValue value) {
    value.type_check(Type::COORD);
    state.positions[target.targetIdx] = value.as<VIPRA::f3d>();
  }

  /**
   * @brief Sets a pedestrians state attribute
   * 
   * @param target : target to set state for
   * @param pack : simulation pack
   * @param value : value to set attribute to
   */
  static inline void set_state(Target target, auto pack, CAttributeValue value) {
    value.type_check(Type::STATE);
    pack.context.pedStates[target.targetIdx] = value.as<stateUID>();
  }

  /**
   * @brief Sets a pedestrians goal, calling the Goals module to recalculate pathing
   * 
   * @param target : target to change velocity of
   * @param pack : simulation pack
   * @param value : value to set velocity to
   */
  static inline void set_goal(Target target, auto pack, CAttributeValue value) {
    auto&       goals = pack.goals;
    auto&       context = pack.context;
    auto const& pedset = pack.pedset;

    if ( value.type == Type::COORD ) {
      goals.change_end_goal(target.targetIdx, value.as<VIPRA::f3d>());
    } else if ( value.type == Type::LOCATION ) {
      // TODO(rolland): this doesn't take into account two pedestrains going to the same location
      goals.change_end_goal(target.targetIdx,
                            context.locations[value.as<VIPRA::idx>()].random_point(context.engine));
    }

    value.type_check(Type::COORD);
  }

  /**
   * @brief Sets a pedestrians velocity, changes next timestep state to reflect that
   * 
   * @param target : target to change velocity of
   * @param pack : simulation pack
   * @param state : next timestep state
   * @param value : value to set velocity to
   */
  static inline void set_velocity(Target target, auto pack, VIPRA::State& state, CAttributeValue value) {
    value.type_check(Type::COORD);

    state.velocities[target.targetIdx] = value.as<VIPRA::f3d>();

    VIPRA::f3d originalPos = pack.pedset.ped_coords(target.targetIdx);
    state.positions[target.targetIdx] = originalPos + (state.velocities[target.targetIdx] * pack.dT);
  }

  /**
     * @brief Set the location position object
     * 
     * @param target 
     * @param pack 
     * @param value 
     */
  static inline void set_location_position(Target target, auto pack, CAttributeValue value) {
    value.type_check(Type::COORD);
    pack.context.locations[target.targetIdx].set_center(value.as<VIPRA::f3d>());
  }

  /**
     * @brief Set the location dims object
     * 
     * @param target 
     * @param pack 
     * @param value 
     */
  static inline void set_location_dims(Target target, auto pack, CAttributeValue value) {
    value.type_check(Type::COORD);
    pack.context.locations[target.targetIdx].set_dims(value.as<VIPRA::f3d>());
  }

  // ------------------------------------------- END SETTERS ------------------------------------------------------------------------

  // --------------------------------------------- SCALING ------------------------------------------------------------------------

  /**
   * @brief Scales a pedestrians velocity, changes next timestep state to reflect that
   * 
   * @param target : target to change velocity of
   * @param pack : simulation pack
   * @param state : next timestep state
   * @param value : value to set velocity to
   */
  static inline void scale_velocity(Target target, auto pack, VIPRA::State& state, CAttributeValue value) {
    value.type_check(Type::NUMBER);

    NumericValue const& scale = *static_cast<NumericValue const*>(value.value);
    // TODO (rolland): get magnitude, scale that then apply to the unit vector
    VIPRA::f_pnt scaleVal = scale.value(target.targetIdx);
    auto         tempvel = state.velocities[target.targetIdx];
    tempvel *= scaleVal;
    state.positions[target.targetIdx] = pack.pedset.ped_coords(target.targetIdx) + (tempvel * pack.dT);
  }

  // --------------------------------------------- END SCALING ------------------------------------------------------------------------
};
}  // namespace VIPRA::Behaviors
