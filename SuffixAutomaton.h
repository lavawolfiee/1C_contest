#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "NodesContainer.h"

class SuffixAutomaton {
  using NodePtr = typename NodesContainer::NodePtr;

 public:
  explicit SuffixAutomaton(const std::string& s = "",
                           std::shared_ptr<NodesContainer> nodes =
                               std::make_shared<MemoryNodesContainer>());

  explicit SuffixAutomaton(std::ifstream& file,
                           std::shared_ptr<NodesContainer> nodes =
                               std::make_shared<MemoryNodesContainer>());

  SuffixAutomaton& operator+=(char c_);
  SuffixAutomaton& operator+=(const std::string& to_add);

  [[nodiscard]] std::optional<std::size_t> FindFirstOccurrencePos(
      const std::string& pattern) const;
  [[nodiscard]] std::string GetFirstOccurrenceContext(
      std::size_t index, const std::string& pattern, std::size_t before = 10,
      std::size_t after = 10) const;

 private:
  std::string s;
  std::shared_ptr<NodesContainer> nodes;
  typename NodesContainer::NodePtr last = 0;  // [S]
};