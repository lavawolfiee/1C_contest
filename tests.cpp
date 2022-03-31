#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "SuffixAutomaton.h"

using std::chrono::duration;
using std::chrono::duration_cast;

int main() {
  auto start = std::chrono::steady_clock::now();

  std::ifstream file("../tests/War&Peace.txt");
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  auto end = std::chrono::steady_clock::now();

  std::cout.precision(4);
  std::cout << "File was read in "
            << duration<double, std::milli>(end - start).count() << "ms"
            << std::endl;

  start = std::chrono::steady_clock::now();
  SuffixAutomaton suffix_automaton(buffer.str());
  end = std::chrono::steady_clock::now();

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