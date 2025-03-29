
#include "douglas_peucker.hpp"

#include "vipra/geometry/line.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"

namespace VIPRA {

inline auto perpendicular_distance(VIPRA::f3d const&     point,
                                   VIPRA::Geometry::Line line) -> VIPRA::f_pnt
{
  VIPRA::f_pnt slope = (line.end.y - line.start.y) / (line.end.x - line.start.x);
  VIPRA::f_pnt intercept = line.start.y - (slope * line.start.x);
  VIPRA::f_pnt result =
      std::abs(slope * point.x - point.y + intercept) / std::sqrt(std::pow(slope, 2) + 1);

  return result;
}

void douglas_peucker_algo(std::span<VIPRA::f3d> path, VIPRA::f_pnt epsilon,
                          std::vector<VIPRA::f3d>& result, size_t start, size_t end)
{
  if ( end <= start + 1 ) return;

  VIPRA::idx   maxIdx = start;
  VIPRA::f_pnt maxDist = 0;

  for ( size_t i = start + 1; i < end; ++i ) {
    auto dist = perpendicular_distance(path[i], {path[start], path[end]});
    if ( dist > maxDist ) {
      maxDist = dist;
      maxIdx = i;
    }
  }

  // If the maximum distance is greater than epsilon, recursively simplify the segments
  if ( maxDist > epsilon ) {
    douglas_peucker_algo(path, epsilon, result, start, maxIdx);
    result.push_back(path[maxIdx]);
    douglas_peucker_algo(path, epsilon, result, maxIdx, end);
  }
}

auto douglas_peucker_algo(std::span<VIPRA::f3d> path,
                          VIPRA::f_pnt          epsilon) -> std::vector<VIPRA::f3d>
{
  std::vector<VIPRA::f3d> result;
  if ( path.empty() ) return result;

  result.push_back(path[0]);
  douglas_peucker_algo(path, epsilon, result, 0, path.size() - 1);
  result.push_back(path.back());

  return result;
}
}  // namespace VIPRA