#include <chrono>
#include <fstream>
#include <iostream>

#include "SuffixAutomaton.h"

using std::chrono::duration;
using std::chrono::duration_cast;

int main() {
  std::ifstream file("../tests/War&Peace.txt");

  auto start = std::chrono::steady_clock::now();
  SuffixAutomaton suffix_automaton(file);
  auto end = std::chrono::steady_clock::now();

  file.close();

  std::cout.precision(4);
  std::cout << "Automaton was built (including reading of file) in "
            << duration<double>(end - start).count() << "s" << std::endl;

  std::string query;

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