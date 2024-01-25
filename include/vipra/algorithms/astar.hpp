#pragma once

#include <algorithm>
#include <concepts>
#include <functional>
#include <numeric>
#include <optional>
#include <queue>
#include <type_traits>

#include <iostream>
#include <unordered_set>

#include "vipra/concepts/not_void.hpp"
#include "vipra/types/f3d.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"

#include "vipra/types/util/result_or_void.hpp"

#include "vipra/util/debug_do.hpp"
#include "vipra/util/invoke_result_or.hpp"

namespace VIPRA::Algo {

namespace AStar {
/**
 * @brief Concept for type that can be used as a graph for the A* algorithm
 * 
 * @tparam graph_t 
 */
template <typename graph_t>
concept Graph = requires(const graph_t graph, VIPRA::idx idx) {
  { graph.neighbors(idx) } -> std::same_as<const std::vector<VIPRA::idx>&>;
};

/**
 * @brief Concept for a function that returns the distance between two nodes
 * 
 * @tparam func_t 
 */
template <typename func_t>
concept distance_func = requires(func_t func, VIPRA::idx idx1, VIPRA::idx idx2) {
  { func(idx1, idx2) } -> std::same_as<VIPRA::f_pnt>;
};

/**
 * @brief Concept for a function that converts a node index to a desired type
 * 
 * @tparam func_t 
 */
template <typename func_t>
concept conversion_func = std::is_same_v<func_t, VOID> || requires(func_t func, VIPRA::idx idx1) {
  {func(idx1)};
};
}  // namespace AStar

/**
 * @brief Creates a path from start to end using the A* algorithm, and returns it as a queue of indices. Or a queue of results of the conversion function, if one is provided. 
 * @tparam graph_t Graph type that satisfies the AStar::Graph concept
 * @tparam distance_f_t Function for getting the distance between two nodes, that satisfies the AStar::distance_func concept
 * @tparam conversion_f_t (optional) Function for converting the node index to the desired type, that satisfies the AStar::conversion_func concept
 * @param start starting node index
 * @param end ending node index
 * @param graph graph to search
 * @param distance_func node distance function
 * @param conversion_func (optional) node conversion function
 * @return std::queue<VIPRA::idx> OR std::queue<std::invoke_result_t<conversion_func, VIPRA::idx>> 
 */
template <AStar::Graph graph_t, AStar::distance_func distance_f_t,
          AStar::conversion_func conversion_f_t = VOID>
[[nodiscard]] auto astar(VIPRA::idx start, VIPRA::idx end, const graph_t& graph, distance_f_t&& distance_func,
                         conversion_f_t&& conversion_func = VOID{})
    -> std::queue<std::remove_reference_t<Util::invoke_result_or_t<VIPRA::idx, conversion_f_t, VIPRA::idx>>> {
  // TODO(rolland): implement, and actually check that it's correct

  using ret_t =
      std::queue<std::remove_reference_t<Util::invoke_result_or_t<VIPRA::idx, conversion_f_t, VIPRA::idx>>>;
  struct Node {
    VIPRA::idx   self;
    VIPRA::idx   parent;
    VIPRA::f_pnt distanceFromStart;
    VIPRA::f_pnt distanceWithHeuristic;

    struct Compare {
      auto operator()(const Node& left, const Node& right) const -> bool {
        return left.distanceWithHeuristic > right.distanceWithHeuristic;
      }
    };

    struct Hash {
      auto operator()(VIPRA::idx node) const -> std::size_t { return std::hash<VIPRA::idx>{}(node); }
      auto operator()(const Node& node) const -> std::size_t { return std::hash<VIPRA::idx>{}(node.self); }
    };

    struct Equal {
      auto operator()(const Node& left, const Node& right) const -> bool { return left.self == right.self; }
    };
  };

  struct PQueue : public std::priority_queue<Node, std::vector<Node>, typename Node::Compare> {
    auto search(VIPRA::idx nodeIdx) -> std::optional<Node> {
      auto container = this->c;
      auto gridPoint =
          std::find_if(container.begin(), container.end(), [&](Node node) { return node.self == nodeIdx; });
      if (gridPoint == container.end()) {
        return std::nullopt;
      }
      return std::optional<Node>{*gridPoint};
    }
  };

  std::vector<Node> nodes{graph.nodes().size()};

  VIPRA::idx idx = 0;
  for (auto& node : nodes) {
    node.self = idx;
    ++idx;
  }

  PQueue                                                              openset;
  std::unordered_set<Node, typename Node::Hash, typename Node::Equal> closedset;

  Node current = Node{start, start, 0, distance_func(start, end)};
  openset.push(current);

  while (!openset.empty()) {
    current = openset.top();

    if (current.self == end) {
      break;
    }

    openset.pop();
    closedset.insert(current);
    for (VIPRA::idx neighborIdx : graph.neighbors(current.self)) {
      if (closedset.find(Node{neighborIdx}) == closedset.end()) {
        Node& neighbor = nodes[neighborIdx];
        neighbor.parent = current.self;
        neighbor.distanceFromStart = current.distanceFromStart + distance_func(current.self, neighborIdx);
        neighbor.distanceWithHeuristic = neighbor.distanceFromStart + distance_func(neighborIdx, end);

        auto found = openset.search(neighborIdx);
        if (!found.has_value()) {
          openset.push(neighbor);
        } else {
          if (neighbor.distanceFromStart < found.value().distanceFromStart) {
            found.value().distanceFromStart = neighbor.distanceFromStart;
            found.value().parent = neighbor.parent;
          }
        }
      }
    }
  }

  ret_t path;
  while (current.self != start) {
    if constexpr (std::is_same_v<conversion_f_t, VOID>) {
      path.push(current.self);
    } else {
      const auto converted = conversion_func(current.self);
      path.push(converted);
    }
    current = nodes[current.parent];
  }

  return path;
}
}  // namespace VIPRA::Algo
