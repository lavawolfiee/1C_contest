#include "SuffixAutomaton.h"
#include <fstream>

SuffixAutomaton::SuffixAutomaton(const std::string& s,
                                 std::shared_ptr<NodesContainer> nodes_)
    : nodes(std::move(nodes_)) {
  nodes->Reserve(s.size());
  nodes->AddNode();
  *this += s;
}

SuffixAutomaton::SuffixAutomaton(std::ifstream& file,
                                 std::shared_ptr<NodesContainer> nodes_)
    : nodes(std::move(nodes_)) {
  nodes->AddNode();

  char c;
  std::size_t i = 0;

  while (file.get(c)) {
    *this += c;
    ++i;
  }
}

SuffixAutomaton& SuffixAutomaton::operator+=(const std::string& to_add) {
  for (char c : to_add) {
    *this += c;
  }

  return *this;
}

SuffixAutomaton& SuffixAutomaton::operator+=(char c) {
  s += c;

  NodePtr curr = nodes->AddNode();  // [Sc]
  nodes->SetLen(curr, nodes->GetLen(last) + 1);
  nodes->SetFirstPosEnd(curr, nodes->GetLen(curr) - 1);
  std::optional<NodePtr> p = last;
  last = curr;

  while (p && !nodes->HasEdge(*p, c)) {
    nodes->SetEdge(*p, c, curr);
    p = nodes->GoByLink(*p);
  }

  if (!p) {
    nodes->SetLink(curr, 0);
  } else {
    NodePtr q = nodes->Go(*p, c);

    if (nodes->GetLen(q) == nodes->GetLen(*p) + 1) {
      nodes->SetLink(curr, q);
    } else {
      NodePtr clone = nodes->AddNode(q);
      nodes->SetLen(clone, nodes->GetLen(*p) + 1);
      nodes->SetFirstPosEnd(clone, nodes->GetFirstPosEnd(q));

      while (p && nodes->Go(*p, c) == q) {
        nodes->SetEdge(*p, c, clone);
        p = nodes->GoByLink(*p);
      }

      nodes->SetLink(q, clone);
      nodes->SetLink(curr, clone);
    }
  }

  return *this;
}

std::optional<std::size_t> SuffixAutomaton::FindFirstOccurrencePos(
    const std::string& pattern) const {
  NodePtr v = 0;

  for (char c : pattern) {
    if (!nodes->HasEdge(v, c)) return std::nullopt;

    v = nodes->Go(v, c);
  }

  return nodes->GetFirstOccurrence(v, pattern.size());
}

std::string SuffixAutomaton::GetFirstOccurrenceContext(
    std::size_t index, const std::string& pattern, std::size_t before,
    std::size_t after) const {
  ssize_t start_pos =
      std::max<ssize_t>(0, static_cast<ssize_t>(index) - before);
  return s.substr(start_pos, pattern.size() + before + after);
}
