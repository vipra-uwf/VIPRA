#pragma once

#include <optional>
#include <queue>
#include <unordered_set>
#include <vector>

#include "goals/astar/pathing_graph.hpp"
#include "vipra/data_structures/graph.hpp"
#include "vipra/geometry/f3d.hpp"
#include "vipra/types/idx.hpp"

#include "vipra/macros/performance.hpp"

namespace VIPRA::Algo {

struct Node {
  VIPRA::idx   self;
  VIPRA::idx   parent;
  VIPRA::f_pnt distanceFromStart;
  VIPRA::f_pnt distanceWithHeuristic;

  auto operator==(Node const& other) const -> bool
  {
    return self == other.self;
  }

  struct Compare {
    auto operator()(Node* left, Node* right) const -> bool
    {
      return left->distanceWithHeuristic > right->distanceWithHeuristic;
    }
  };
};

VIPRA_PERF_FUNC auto astar(VIPRA::idx start, VIPRA::idx end,
                           Goals::PathingGraph const& graph) noexcept
    -> std::optional<std::vector<VIPRA::f3d>>
{
  std::priority_queue<Node, std::vector<Node>, Node::Compare> openSetNodes;
  std::unordered_set<VIPRA::idx>                              openSet;
  std::unordered_set<VIPRA::idx>                              closedSet;

  while ( ! openSet.empty() ) {
    Node curr = openSetNodes.top();
    openSet.erase(curr.self);
  }
}
}  // namespace VIPRA::Algo
