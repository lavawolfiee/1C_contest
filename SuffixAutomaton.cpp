#include "SuffixAutomaton.h"

SuffixAutomaton::SuffixAutomaton(const std::string &s) {
  nodes.emplace_back();
  *this += s;
}

SuffixAutomaton &SuffixAutomaton::operator+=(const std::string &to_add) {
  for (char c : to_add) {
    *this += c;
  }

  return *this;
}

SuffixAutomaton &SuffixAutomaton::operator+=(char c) {
  s += c;

  nodes.emplace_back();
  std::size_t curr = static_cast<std::size_t>(nodes.size()) - 1; // [Sc]
  nodes[curr].SetLen(nodes[last].GetLen() + 1);
  nodes[curr].SetFirstPosEnd(nodes[curr].GetLen() - 1);
  std::optional<std::size_t> p = last;
  last = curr;

  while (p && !nodes[*p].HasEdge(c)) {
    nodes[*p].SetEdge(c, curr);
    p = nodes[*p].GoByLink();
  }

  if (!p) {
    nodes[curr].SetLink(0);
  } else {
    std::size_t q = nodes[*p].Go(c);

    if (nodes[q].GetLen() == nodes[*p].GetLen() + 1) {
      nodes[curr].SetLink(q);
    } else {
      nodes.push_back(nodes[q]);
      std::size_t clone = static_cast<std::size_t>(nodes.size()) - 1;
      nodes[clone].SetLen(nodes[*p].GetLen() + 1);
      nodes[clone].SetFirstPosEnd(nodes[q].GetFirstPosEnd());

      while (p && nodes[*p].Go(c) == q) {
        nodes[*p].SetEdge(c, clone);
        p = nodes[*p].GoByLink();
      }

      nodes[q].SetLink(clone);
      nodes[curr].SetLink(clone);
    }
  }

  return *this;
}

std::optional<std::size_t> SuffixAutomaton::FindFirstOccurrencePos(const std::string &pattern) const {
  std::size_t v = 0;

  for (char c : pattern) {
    if (!nodes[v].HasEdge(c))
      return std::nullopt;

    v = nodes[v].Go(c);
  }

  return nodes[v].GetFirstOccurrence(pattern.size());
}

std::optional<std::string> SuffixAutomaton::FindFirstOccurrenceContext(const std::string &pattern,
                                                                       std::size_t before,
                                                                       std::size_t after) const {
  std::optional<std::size_t> pos = FindFirstOccurrencePos(pattern);

  if (!pos) {
    return std::nullopt;
  }

  ssize_t start_pos = std::max<ssize_t>(0, static_cast<ssize_t>(*pos) - before);
  return s.substr(start_pos, pattern.size() + before + after);
}

std::size_t SuffixAutomaton::Node::Go(char c) const {
  auto it = to.find(c);

  if (it == to.end()) {
    throw std::runtime_error("Can't go by char " + std::to_string(c));
  }

  return it->second;
}
