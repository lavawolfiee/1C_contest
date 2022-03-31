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

  std::string query;

  while (std::getline(std::cin, query)) {
    start = std::chrono::steady_clock::now();
    std::optional<std::string> answer = suffix_automaton.FindFirstOccurrenceContext(query);
    end = std::chrono::steady_clock::now();

    if (answer) {
      std::cout << "Found: \"" << *answer << "\" in " << duration<double, std::nano>(end - start).count()
                << " nanoseconds"
                << std::endl;
    } else {
      std::cout << "Nothing found in " << duration<double, std::nano>(end - start).count() << " nanoseconds"
                << std::endl;
    }
  }

  return 0;
}