#include "NodesContainer.h"

NodesContainer::NodePtr MemoryNodesContainer::AddNode() {
  nodes.emplace_back();
  return nodes.size() - 1;
}

NodesContainer::NodePtr MemoryNodesContainer::AddNode(
    NodesContainer::NodePtr node) {
  nodes.emplace_back(nodes[node]);
  return nodes.size() - 1;
}

std::size_t MemoryNodesContainer::Go(NodesContainer::NodePtr node,
                                     char c) const {
  auto it = nodes[node].to.find(c);

  if (it == nodes[node].to.end()) {
    throw std::runtime_error("Can't go by char " + std::to_string(c));
  }

  return it->second;
}

std::size_t MemoryNodesContainer::GetFirstOccurrence(
    NodesContainer::NodePtr node, std::size_t pattern_len) const {
  return GetFirstPosEnd(node) - pattern_len + 1;
}

std::optional<std::size_t> MemoryNodesContainer::GoByLink(
    NodesContainer::NodePtr node) const {
  return nodes[node].link;
}

bool MemoryNodesContainer::HasEdge(NodesContainer::NodePtr node, char c) const {
  return nodes[node].to.find(c) != nodes[node].to.end();
}

std::size_t MemoryNodesContainer::GetLen(NodesContainer::NodePtr node) const {
  return nodes[node].len;
}

std::size_t MemoryNodesContainer::GetFirstPosEnd(
    NodesContainer::NodePtr node) const {
  return nodes[node].first_pos_end;
}

void MemoryNodesContainer::SetEdge(NodesContainer::NodePtr node, char c,
                                   std::size_t v) {
  nodes[node].to[c] = v;
}

void MemoryNodesContainer::SetLink(NodesContainer::NodePtr node,
                                   std::size_t v) {
  nodes[node].link = v;
}

void MemoryNodesContainer::SetLen(NodesContainer::NodePtr node,
                                  std::size_t new_len) {
  nodes[node].len = new_len;
}

void MemoryNodesContainer::SetFirstPosEnd(NodesContainer::NodePtr node,
                                          std::size_t pos) {
  nodes[node].first_pos_end = pos;
}

void MemoryNodesContainer::Reserve(std::size_t size) { nodes.reserve(size); }

MemoryNodesContainer::MemoryNodesContainer(std::size_t size) {
  nodes.reserve(size);
}
FileNodesContainer::FileNodesContainer(std::size_t size) {
  nodes.reserve(size);
}

NodesContainer::NodePtr FileNodesContainer::AddNode() {
  nodes.emplace_back(allocator);
  return nodes.size() - 1;
}

NodesContainer::NodePtr FileNodesContainer::AddNode(
    NodesContainer::NodePtr node) {
  nodes.emplace_back(nodes[node]);
  return nodes.size() - 1;
}

std::size_t FileNodesContainer::Go(NodesContainer::NodePtr node,
                                     char c) const {
  auto it = nodes[node].to.find(c);

  if (it == nodes[node].to.end()) {
    throw std::runtime_error("Can't go by char " + std::to_string(c));
  }

  return it->second;
}

std::size_t FileNodesContainer::GetFirstOccurrence(
    NodesContainer::NodePtr node, std::size_t pattern_len) const {
  return GetFirstPosEnd(node) - pattern_len + 1;
}

std::optional<std::size_t> FileNodesContainer::GoByLink(
    NodesContainer::NodePtr node) const {
  return nodes[node].link;
}

bool FileNodesContainer::HasEdge(NodesContainer::NodePtr node, char c) const {
  return nodes[node].to.find(c) != nodes[node].to.end();
}

std::size_t FileNodesContainer::GetLen(NodesContainer::NodePtr node) const {
  return nodes[node].len;
}

std::size_t FileNodesContainer::GetFirstPosEnd(
    NodesContainer::NodePtr node) const {
  return nodes[node].first_pos_end;
}

void FileNodesContainer::SetEdge(NodesContainer::NodePtr node, char c,
                                   std::size_t v) {
  nodes[node].to[c] = v;
}

void FileNodesContainer::SetLink(NodesContainer::NodePtr node,
                                   std::size_t v) {
  nodes[node].link = v;
}

void FileNodesContainer::SetLen(NodesContainer::NodePtr node,
                                  std::size_t new_len) {
  nodes[node].len = new_len;
}

void FileNodesContainer::SetFirstPosEnd(NodesContainer::NodePtr node,
                                          std::size_t pos) {
  nodes[node].first_pos_end = pos;
}

void FileNodesContainer::Reserve(std::size_t size) { nodes.reserve(size); }