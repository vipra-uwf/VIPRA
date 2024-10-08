#pragma once

#include <algorithm>
#include <limits>
#include <optional>
#include <unordered_set>
#include <vector>

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

[[nodiscard]] inline auto astar(VIPRA::idx start, VIPRA::idx end,
                                Goals::PathingGraph const& graph) noexcept
    -> std::optional<std::vector<VIPRA::f3d>>
{
  const auto startPos = graph.pos(start);
  const auto endPos = graph.pos(end);

  std::vector<Node> nodes(graph.node_count());
  std::vector<Node> openQueue;
  std::vector<bool> openSet(graph.node_count());
  std::vector<bool> closedSet(graph.node_count());

  nodes[start] = Node{start, start, 0, startPos.distance_to(endPos)};

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

    for ( const VIPRA::idx neighborIdx : graph.neighbors(curr.self) ) {
      if ( neighborIdx == std::numeric_limits<VIPRA::idx>::max() ) continue;
      if ( closedSet[neighborIdx] ) continue;

      const auto currPos = graph.pos(curr.self);
      const auto neighborPos = graph.pos(neighborIdx);
      const Node neighbor{
          neighborIdx, curr.self,
          nodes[curr.self].distanceFromStart + currPos.distance_to(neighborPos),
          neighbor.distanceFromStart + neighborPos.distance_to(endPos)};

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

  while ( curr.self != start ) {
    path.push_back(graph.pos(curr.self));
    curr = nodes[curr.parent];
  }

  return path;
}
}  // namespace VIPRA
