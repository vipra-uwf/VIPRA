#pragma once

#include <optional>
#include <queue>

#include "vipra/types/f3d.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"

namespace VIPRA::Algo {

namespace AStar {
template <typename graph_t>
concept Graph = requires(const graph_t graph, VIPRA::idx idx) {
  { graph.neighbors(idx) } -> std::same_as<const std::vector<VIPRA::idx>&>;
  { graph.distance(idx, idx) } -> std::same_as<VIPRA::f_pnt>;
};
}  // namespace AStar

template <AStar::Graph graph_t>
[[nodiscard]] auto astar(VIPRA::idx start, VIPRA::idx end, const graph_t& graph) -> std::queue<VIPRA::idx> {
  // TODO(rolland): implement, and actually check that it's correct

  struct Node {
    VIPRA::idx   parent;
    VIPRA::f_pnt distanceFromStart;
    VIPRA::f_pnt distanceWithHeuristic;
  };

  std::vector<Node>       nodes{graph.nodes().size()};
  std::vector<VIPRA::idx> openset{start};
  std::vector<VIPRA::idx> closedset;

  VIPRA::idx current = start;

  while (true) {
    auto node = std::min(openset.begin(), openset.end(), [&](VIPRA::idx left, VIPRA::idx right) {
      return nodes[left].distanceWithHeuristic < nodes[right].distanceWithHeuristic;
    });

    if (node == end) {
      break;
    }

    closedset.push_back(current);
    for (auto neighbor : graph.neighbors(current)) {
      if (std::find(closedset.begin(), closedset.end(), neighbor) != closedset.end()) {
        continue;
      }

      auto neighborNode = nodes[neighbor];
      auto currentNode = nodes[current];

      auto distanceFromStart = currentNode.distanceFromStart + graph.distance(current, neighbor);
      auto distanceWithHeuristic = distanceFromStart + graph.distance(neighbor, end);

      if (std::find(openset.begin(), openset.end(), neighbor) == openset.end() ||
          distanceWithHeuristic < neighborNode.distanceWithHeuristic) {
        neighborNode.parent = current;
        neighborNode.distanceFromStart = distanceFromStart;
        neighborNode.distanceWithHeuristic = distanceWithHeuristic;
        nodes[neighbor] = neighborNode;
        openset.push_back(neighbor);
      }
    }
  }

  std::queue<VIPRA::idx> path;
  while (current != start) {
    path.push(current);
    current = nodes[current].parent;
  }

  return path;
}
}  // namespace VIPRA::Algo
