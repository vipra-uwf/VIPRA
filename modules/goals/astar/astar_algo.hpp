#pragma once

#include <algorithm>
#include <limits>
#include <optional>
#include <vector>

#include "goals/astar/douglas_peucker.hpp"
#include "goals/astar/pathing_graph.hpp"
#include "vipra/geometry/f3d.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"

#include "vipra/macros/performance.hpp"

namespace VIPRA {

struct Node {
  VIPRA::idx   self;
  VIPRA::idx   parent;
  VIPRA::f_pnt distanceFromStart;
  VIPRA::f_pnt distanceWithHeuristic;

  VIPRA_INLINE auto operator==(Node const& other) const noexcept -> bool
  {
    return self == other.self;
  }

  VIPRA_INLINE auto operator<(Node const& other) const noexcept -> bool
  {
    return distanceWithHeuristic > other.distanceWithHeuristic;
  }
};

[[nodiscard]] inline auto astar(VIPRA::f3d endPos, VIPRA::idx start, VIPRA::idx end,
                                Goals::PathingGraph const& graph) noexcept
    -> std::optional<std::vector<VIPRA::f3d>>
{
  auto const startPos = graph.pos(start);
  auto const endGridPos = graph.pos(end);

  std::vector<Node> nodes(graph.node_count());
  std::vector<Node> openQueue;
  std::vector<bool> openSet(graph.node_count());
  std::vector<bool> closedSet(graph.node_count());

  nodes[start] = Node{start, start, 0, startPos.distance_to(endGridPos)};

  Node curr{};
  openSet[start] = true;
  openQueue.push_back(nodes[start]);

  while ( ! openQueue.empty() ) {
    curr = openQueue.front();

    if ( curr.self == end ) break;

    std::pop_heap(openQueue.begin(), openQueue.end());
    openQueue.pop_back();

    openSet[curr.self] = false;
    closedSet[curr.self] = true;

    for ( VIPRA::idx const neighborIdx : graph.neighbors(curr.self) ) {
      if ( neighborIdx == std::numeric_limits<VIPRA::idx>::max() ) continue;
      if ( closedSet[neighborIdx] ) continue;

      auto const& currPos = graph.pos(curr.self);
      auto const& neighborPos = graph.pos(neighborIdx);
      Node const  neighbor{
          neighborIdx, curr.self,
          nodes[curr.self].distanceFromStart + currPos.distance_to(neighborPos),
          nodes[curr.self].distanceFromStart + currPos.distance_to(neighborPos) +
              neighborPos.distance_to(endGridPos)};

      if ( ! openSet[neighborIdx] ) {
        nodes[neighborIdx] = neighbor;
        openSet[neighborIdx] = true;
        openQueue.push_back(neighbor);
        std::push_heap(openQueue.begin(), openQueue.end());
        continue;
      }

      if ( neighbor.distanceFromStart < nodes[neighborIdx].distanceFromStart ) {
        nodes[neighborIdx] = neighbor;
      }
    }
  }

  if ( curr.self != end ) {
    // no path found
    return std::nullopt;
  }

  std::vector<VIPRA::f3d> path;
  VIPRA::f3d              dif;

  path.push_back(endPos);  // skip last grid and go straight to exact end position
  curr = nodes[curr.parent];

  while ( true ) {
    curr = nodes[curr.parent];

    if ( curr.parent == start ) break;  // skip first grid, since ped is already there

    auto currDif = graph.pos(curr.self) - graph.pos(curr.parent);
    if ( currDif != dif ) {
      path.push_back(graph.pos(curr.parent));
      dif = currDif;
    }
  }

  // TODO(rolland): parameterize epsilon here
  return VIPRA::douglas_peucker_algo(path, 0.1);
}
}  // namespace VIPRA
