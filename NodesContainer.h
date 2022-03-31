#pragma once

#include <optional>
#include <unordered_map>
#include <vector>

#include "mmap_allocator.h"

class NodesContainer {
 public:
  using NodePtr = std::size_t;

  virtual std::size_t Go(NodePtr node, char c) const = 0;
  virtual std::optional<std::size_t> GoByLink(NodePtr node) const = 0;
  virtual bool HasEdge(NodePtr node, char c) const = 0;
  virtual std::size_t GetLen(NodePtr node) const = 0;
  virtual std::size_t GetFirstPosEnd(NodePtr node) const = 0;
  virtual std::size_t GetFirstOccurrence(NodePtr node,
                                         std::size_t pattern_len) const = 0;

  virtual NodePtr AddNode() = 0;
  virtual NodePtr AddNode(NodePtr node) = 0;
  virtual void Reserve(std::size_t size) = 0;
  virtual void SetEdge(NodePtr node, char c, std::size_t v) = 0;
  virtual void SetLink(NodePtr node, std::size_t v) = 0;
  virtual void SetLen(NodePtr node, std::size_t new_len) = 0;
  virtual void SetFirstPosEnd(NodePtr node, std::size_t pos) = 0;
};

class MemoryNodesContainer : public NodesContainer {
 private:
  struct Node {
   public:
    Node() = default;

    std::unordered_map<char, std::size_t> to;
    std::size_t len = 0;
    std::size_t first_pos_end = -1;  // the position of the end of
                                     // the first occurrence
    std::optional<std::size_t> link{std::nullopt};
  };

 public:
  explicit MemoryNodesContainer(std::size_t size = 0);

  std::size_t Go(NodePtr node, char c) const override;
  std::optional<std::size_t> GoByLink(NodePtr node) const override;
  bool HasEdge(NodePtr node, char c) const override;
  std::size_t GetLen(NodePtr node) const override;
  std::size_t GetFirstPosEnd(NodePtr node) const override;
  std::size_t GetFirstOccurrence(NodePtr node,
                                 std::size_t pattern_len) const override;

  NodePtr AddNode() override;
  NodePtr AddNode(NodePtr) override;
  void Reserve(std::size_t size) override;
  void SetEdge(NodePtr node, char c, std::size_t v) override;
  void SetLink(NodePtr node, std::size_t v) override;
  void SetLen(NodePtr node, std::size_t new_len) override;
  void SetFirstPosEnd(NodePtr node, std::size_t pos) override;

 private:
  std::vector<Node> nodes;
};

// uses MmapAllocator to allocate everything in file
class FileNodesContainer : public NodesContainer {
 private:
  struct Node {
   public:
    Node(const MmapAllocator<Node>& alloc) : to(alloc) {};

    std::unordered_map<char, std::size_t, std::hash<char>, std::equal_to<>,
                       MmapAllocator<std::pair<char, std::size_t>>>
        to;
    std::size_t len = 0;
    std::size_t first_pos_end = -1;  // the position of the end of
                                     // the first occurrence
    std::optional<std::size_t> link{std::nullopt};
  };

 public:
  explicit FileNodesContainer(std::size_t size = 0);

  std::size_t Go(NodePtr node, char c) const override;
  std::optional<std::size_t> GoByLink(NodePtr node) const override;
  bool HasEdge(NodePtr node, char c) const override;
  std::size_t GetLen(NodePtr node) const override;
  std::size_t GetFirstPosEnd(NodePtr node) const override;
  std::size_t GetFirstOccurrence(NodePtr node,
                                 std::size_t pattern_len) const override;

  NodePtr AddNode() override;
  NodePtr AddNode(NodePtr) override;
  void Reserve(std::size_t size) override;
  void SetEdge(NodePtr node, char c, std::size_t v) override;
  void SetLink(NodePtr node, std::size_t v) override;
  void SetLen(NodePtr node, std::size_t new_len) override;
  void SetFirstPosEnd(NodePtr node, std::size_t pos) override;

 private:
  MmapAllocator<Node> allocator;
  std::vector<Node, MmapAllocator<Node>> nodes;
};