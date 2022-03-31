#include "SuffixAutomaton.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::duration;

int main() {
  auto start = std::chrono::steady_clock::now();

  std::ifstream file("../tests/book-war-and-peace.txt");
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  auto end = std::chrono::steady_clock::now();

  std::cout.precision(4);
  std::cout << "File was read in " << duration<double, std::milli>(end - start).count() << "ms" << std::endl;

  start = std::chrono::steady_clock::now();
  SuffixAutomaton suffix_automaton(buffer.str());
  end = std::chrono::steady_clock::now();

  std::cout << "Automaton was built in " << duration<double>(end - start).count() << "s" << std::endl;

  std::vector<std::pair<std::string, bool>> tests = {{"mind", true},
                                                     {"of", true},
                                                     {"conscious", true},
                                                     {"conscios", false},
                                                     {"As in the question of astronomy then, so in the question of "
                                                      "history now,", true},
                                                     {"привет", false},
                                                     {"abacaba", false}};

  start = std::chrono::steady_clock::now();

  for (const auto&[query, right_answer] : tests) {
    std::optional<std::string> answer = suffix_automaton.FindFirstOccurrenceContext(query);

    if (!answer && right_answer || answer && !right_answer) {
      throw std::runtime_error("Wrong answer!");
    }
  }

  end = std::chrono::steady_clock::now();

  std::cout << "All tests passed in " << duration<double, std::nano>(end - start).count() << " nanoseconds"
            << std::endl;

  return 0;
}