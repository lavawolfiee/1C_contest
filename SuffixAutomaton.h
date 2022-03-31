#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class SuffixAutomaton {
 public:
  explicit SuffixAutomaton(const std::string &s = "");

  SuffixAutomaton &operator+=(char c_);
  SuffixAutomaton &operator+=(const std::string &to_add);

  [[nodiscard]] std::optional<std::size_t> FindFirstOccurrencePos(const std::string &pattern) const;
  [[nodiscard]] std::optional<std::string> FindFirstOccurrenceContext(const std::string &pattern,
                                                                      std::size_t before = 10,
                                                                      std::size_t after = 10) const;

 private:
  class Node {
   public:
    Node() = default;

    std::size_t Go(char c) const;
    std::optional<std::size_t> GoByLink() const { return link; }
    bool HasEdge(char c) const { return to.find(c) != to.end(); }
    std::size_t GetLen() const { return len; }
    std::size_t GetFirstPosEnd() const { return first_pos_end; }
    std::size_t GetFirstOccurrence(std::size_t pattern_len) const { return GetFirstPosEnd() - pattern_len + 1; }

    void SetEdge(char c, std::size_t v) { to[c] = v; }
    void SetLink(std::size_t v) { link = v; }
    void SetLen(std::size_t new_len) { len = new_len; }
    void SetFirstPosEnd(std::size_t pos) { first_pos_end = pos; }

   private:
    std::unordered_map<char, std::size_t> to;
    std::size_t len = 0;
    std::optional<std::size_t> link{std::nullopt};
    std::size_t first_pos_end = -1; // the position of the end of the first occurrence
  };

 private:
  std::string s;
  std::vector<Node> nodes;
  std::size_t last = 0; // [S]
};