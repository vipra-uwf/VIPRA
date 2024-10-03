
#include "vipra/vipra_behaviors/actions/atoms/atom_set_obj.hpp"

#include "vipra/random/distributions.hpp"
#include "vipra/vipra_behaviors/attributes/attributes.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {
void AtomSetObjective::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                  std::vector<bool> const& conditionMet,
                                  std::vector<Target> const& /*targets*/) const
{
  auto const& objectives = pack.context.objectives[objName];
  auto        dist = Random::uniform_distribution<size_t>{0, objectives.size()};

  for ( VIPRA::idx idx = 0; idx < peds.size(); ++idx ) {
    if ( conditionMet[idx] ) {
      VIPRA::f3d newGoal;

      do {
        newGoal = objectives[dist(pack.context.engine)].random_point(
            pack.context.engine);
      } while ( pack.map.collision(Geometry::Circle{newGoal, 0.4}) );

      AttributeHandling::set_value(Target{TargetType::PEDESTRIAN, idx},
                                   Attribute::END_GOAL, pack, pack.state,
                                   CAttributeValue(Type::COORD, &newGoal));
    }
  }
}

}  // namespace VIPRA::Behaviors