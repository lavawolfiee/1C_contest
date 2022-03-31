#include <chrono>
#include <fstream>
#include <iostream>

#include "SuffixAutomaton.h"
#include "mmap_allocator.h"

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
  std::cout << "Automaton was built (including reading of file) in "
            << duration<double>(end - start).count() << "s" << std::endl;

  std::string query;
  std::getline(std::cin, query);

  while (std::getline(std::cin, query)) {
    start = std::chrono::steady_clock::now();
    std::optional<std::size_t> index =
        suffix_automaton.FindFirstOccurrencePos(query);
    end = std::chrono::steady_clock::now();

    if (index) {
      std::cout << "Found match at position " << *index << ": \""
                << suffix_automaton.GetFirstOccurrenceContext(*index, query)
                << "\" in " << duration<double, std::nano>(end - start).count()
                << " nanoseconds" << std::endl;
    } else {
      std::cout << "Nothing found in "
                << duration<double, std::nano>(end - start).count()
                << " nanoseconds" << std::endl;
    }
  }

  return 0;
}