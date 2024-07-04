
#include <algorithm>
#include <numeric>

#include "vipra/vipra_behaviors/selectors/pedestrian_groups.hpp"

namespace {
/**
 * @brief Returns the number of individual types in a Ptype
 * 
 * @param allTypes : Ptype to get count of
 * @return VIPRA::size 
 */
inline auto get_type_count(VIPRA::Behaviors::Ptype allTypes) -> VIPRA::size {
  VIPRA::size typeCnt = 1;
  while ( allTypes.fullType != 0 ) {
    VIPRA::Behaviors::typeUID currType = allTypes.fullType & (allTypes.fullType - 1);
    allTypes.fullType = currType;
    ++typeCnt;
  }
  return typeCnt;
}
}  // namespace

namespace VIPRA::Behaviors {

/**
 * @brief Initializes the _groups, starting with all pedestrians in group 0
 * 
 * @param allTypes : Ptype with all possible types
 * @param pedCnt : number of pedestrians
 */
void GroupsContainer::initialize(Ptype allTypes, VIPRA::size pedCnt) {
  VIPRA::size typeCnt = get_type_count(allTypes);
  _groups.resize(typeCnt);
  _groups[0] = VIPRA::idxVec(pedCnt);
  std::iota(_groups[0].begin(), _groups[0].end(), 0);

  _used.resize(typeCnt);
  _used[0] = std::vector<bool>(pedCnt, false);
}

/**
 * @brief Returns the group with a given type
 * 
 * @param type : type group to get
 * @return const VIPRA::idxVec& 
 */
auto GroupsContainer::get_group(typeUID type) const -> const VIPRA::idxVec& { return _groups[index(type)]; }

/**
 * @brief Adds a pedestrians index to the group with type
 * 
 * @param pedIdx : pedestrian index to add to group
 * @param type : group to add to
 */
void GroupsContainer::add_ped(VIPRA::idx pedIdx, typeUID type) {
  const VIPRA::size ndx = index(type);
  _groups[ndx].push_back(pedIdx);
  _used[ndx].push_back(false);
}

/**
 * @brief Sets a pedestrian as _used in a particular group, this pedestrian can then no longer be selected from this group
 * 
 * @param pedIdx : pedestrian index
 * @param type : group pedestrian has been added to
 * @return true 
 * @return false 
 */
auto GroupsContainer::set_used(VIPRA::idx pedIdx, typeUID type) -> bool {
  const VIPRA::size ndx = index(type);
  auto&             group = _groups[ndx];
  auto&             usedGroup = _used[ndx];

  for ( VIPRA::idx i = 0; i < group.size(); ++i ) {
    if ( group[i] == pedIdx ) {
      usedGroup[i] = true;
      return true;
    }
  }
  return false;
}

/**
 * @brief Returns vector of _used pedestrians from a group
 * 
 * @param type : group to get _used pedestrians from
 * @return const std::vector<bool>& 
 */
auto GroupsContainer::get_used(typeUID type) const -> std::vector<bool> const& { return _used[index(type)]; }

/**
 * @brief Resets _used status of all pedestrians
 * 
 */
void GroupsContainer::clean_used() { _used.clear(); }

/**
 * @brief Removes the pedestrian from the group with type
 * 
 * @param pedIdx : pedestrian index to remove
 * @param type : group to remove the pedestrian from
 */
auto GroupsContainer::remove_ped(VIPRA::idx pedIdx, typeUID type) -> bool {
  auto& group = _groups[index(type)];
  for ( auto iter = group.begin(); iter != group.end(); ++iter ) {
    if ( *iter == pedIdx ) {
      group.erase(iter);
      return true;
    }
  }
  return false;
}

/**
 * @brief Gets the group at index
 * 
 * @param index : group index
 * @return VIPRA::idxVec& 
 */
auto GroupsContainer::operator[](VIPRA::idx index) -> VIPRA::idxVec& { return _groups[index]; }

/**
 * @brief Gets the group at index
 * 
 * @param index : group index
 * @return VIPRA::idxVec& 
 */
auto GroupsContainer::at(VIPRA::idx index) const -> const VIPRA::idxVec& { return _groups.at(index); }

/**
 * @brief Returns the number of _groups
 * 
 * @return VIPRA::size 
 */
auto GroupsContainer::size() const -> VIPRA::size { return _groups.size(); }
}  // namespace VIPRA::Behaviors
