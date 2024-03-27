#pragma once

#include <vector>

#include "vipra/algorithms/astar.hpp"
#include "vipra/types/idx.hpp"

namespace VIPRA::DataStructures {
template <typename data_t>
class Graph {
 public:
  using value_t = data_t;

  struct Node {
    data_t                  data;
    std::vector<VIPRA::idx> neighbors;
  };

  void reserve(VIPRA::idx size) { _nodes.reserve(size); }

  void clear() { _nodes.clear(); }

  [[nodiscard]] constexpr auto node_count() const -> VIPRA::idx { return _nodes.size(); }

  [[nodiscard]] constexpr auto nodes() -> std::vector<Node>& { return _nodes; }
  [[nodiscard]] constexpr auto nodes() const -> std::vector<Node> const& { return _nodes; }

  [[nodiscard]] constexpr auto neighbors(VIPRA::idx nodeIdx) -> std::vector<VIPRA::idx>& {
    assert(nodeIdx < _nodes.size());
    return _nodes[nodeIdx].neighbors;
  }
  [[nodiscard]] constexpr auto neighbors(VIPRA::idx nodeIdx) const -> std::vector<VIPRA::idx> const& {
    assert(nodeIdx < _nodes.size());
    return _nodes[nodeIdx].neighbors;
  }

  [[nodiscard]] constexpr auto data(VIPRA::idx nodeIdx) -> data_t& {
    assert(nodeIdx < _nodes.size());
    return _nodes[nodeIdx].data;
  }
  [[nodiscard]] constexpr auto data(VIPRA::idx nodeIdx) const -> data_t const& {
    assert(nodeIdx < _nodes.size());
    return _nodes[nodeIdx].data;
  }

  void add_edge(VIPRA::idx nodeIdx1, VIPRA::idx nodeIdx2) {
    assert(nodeIdx1 < _nodes.size());
    assert(nodeIdx2 < _nodes.size());

    _nodes[nodeIdx1].neighbors.push_back(nodeIdx2);
    _nodes[nodeIdx2].neighbors.push_back(nodeIdx1);
  }

  [[nodiscard]] constexpr auto add_node(data_t data) -> VIPRA::idx {
    _nodes.push_back(Node{data, {}});
    return _nodes.size() - 1;
  }

  [[nodiscard]] auto add_node(data_t data, std::vector<VIPRA::idx> neighbors) -> VIPRA::idx {
    _nodes.emplace_back(Node{data, neighbors});
    for (auto neighbor : neighbors) {
      _nodes[neighbor].neighbors.push_back(_nodes.size() - 1);
    }
    return _nodes.size() - 1;
  }

 private:
  std::vector<Node> _nodes;
};

static_assert(Algo::AStar::Graph<Graph<VIPRA::f3d>>);
}  // namespace VIPRA::DataStructures