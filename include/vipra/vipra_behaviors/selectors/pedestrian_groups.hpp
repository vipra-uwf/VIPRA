#ifndef VIPRA_BHVR_PEDESTRIAN_GROUPS_HPP
#define VIPRA_BHVR_PEDESTRIAN_GROUPS_HPP

#include <vector>

#include "definitions/pedestrian_types.hpp"
#include "vipra/geometry/f3d.hpp"

namespace BHVR {

/**
 * @brief Holds the indexes for pedestrians in each type group
 * 
 */
class GroupsContainer {
  DEFAULT_CONSTRUCTIBLE(GroupsContainer)
  COPYABLE(GroupsContainer)
  MOVEABLE(GroupsContainer)

 public:
  /**
  * @brief Gets the index into the container for a given type
  * 
  * @param type : 
  * @return constexpr VIPRA::idx 
  */
  static constexpr auto index(typeUID type) -> VIPRA::idx {
    if (type == 0) {
      return 0;
    }

    VIPRA::idx check = 1;
    VIPRA::idx index = 1;
    while ((check & type) == 0U) {
      check = check << 1U;
      ++index;
    }

    return index;
  }

  void initialize(Ptype, VIPRA::size);
  void clean_used();

  [[nodiscard]] auto size() const -> VIPRA::size;

  [[nodiscard]] auto at(VIPRA::idx) const -> const VIPRA::idxVec&;
  [[nodiscard]] auto operator[](VIPRA::idx) -> VIPRA::idxVec&;

  [[nodiscard]] auto get_group(typeUID) const -> const VIPRA::idxVec&;

  void add_ped(VIPRA::idx, typeUID);
  void move_ped(VIPRA::idx, typeUID, typeUID);
  auto remove_ped(VIPRA::idx, typeUID) -> bool;

  auto               set_used(VIPRA::idx, typeUID) -> bool;
  [[nodiscard]] auto get_used(typeUID) const -> std::vector<bool> const&;
  [[nodiscard]] auto is_used(VIPRA::idx, typeUID) const -> bool;

 private:
  std::vector<VIPRA::idxVec>     _groups;
  std::vector<std::vector<bool>> _used;
};
}  // namespace BHVR

#endif