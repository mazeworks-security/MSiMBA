#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace symbsat {

template <typename ZDDNode>
class ZDDCache {

  using node_ptr = std::unique_ptr<ZDDNode>;

  std::vector<node_ptr> nodes;

public:

  const ZDDNode* createNode(int var, const ZDDNode* mul, const ZDDNode* add) {
    nodes.push_back(std::make_unique<ZDDNode>(var, mul, add));
    return nodes.back().get();
  }

  int size() const {
    return nodes.size();
  }

};  // ZDDCache

};  // symbsat
