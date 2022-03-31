#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "SuffixAutomaton.h"

using std::chrono::duration;
using std::chrono::duration_cast;

int main() {
  std::cout << "Do you want to use [1] RAM memory (default) or [2] File "
               "(external memory)? ";
  int option;
  std::cin >> option;

  std::shared_ptr<NodesContainer> nodes_container;

  if (2 == option) {
    std::cout << "Building suffix automaton in File (external memory). ";
    nodes_container = std::make_shared<FileNodesContainer>(false);
  } else {
    std::cout << "Building suffix automaton in RAM memory. ";
    nodes_container = std::make_shared<MemoryNodesContainer>();
  }

  std::cout << "Wait a little bit, please..." << std::endl;

  std::ifstream file("../tests/War&Peace.txt");

  auto start = std::chrono::steady_clock::now();
  SuffixAutomaton suffix_automaton(file, std::move(nodes_container));
  auto end = std::chrono::steady_clock::now();

  file.close();

  std::cout.precision(4);
  std::cout << "Automaton was built in "
            << duration<double>(end - start).count() << "s" << std::endl;

  std::vector<std::pair<std::string, bool>> tests = {
      {"mind", true},
      {"of", true},
      {"conscious", true},
      {"conscios", false},
      {"As in the question of astronomy then, so in the question of "
       "history now,",
       true},
      {"привет", false},
      {"abacaba", false}};

  start = std::chrono::steady_clock::now();

  for (const auto& [query, right_answer] : tests) {
    std::optional<std::size_t> answer =
        suffix_automaton.FindFirstOccurrencePos(query);

    if (!answer && right_answer || answer && !right_answer) {
      throw std::runtime_error("Wrong answer!");
    }
  }

  end = std::chrono::steady_clock::now();

  std::cout << "All tests passed in "
            << duration<double, std::nano>(end - start).count()
            << " nanoseconds" << std::endl;

  return 0;
}