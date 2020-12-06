#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <map>
using namespace std;

std::pair<int, int> CheckGroup(std::vector<std::string> g) {
  std::map<char, int> yes;
  for (auto &line : g) {
    for (auto c : line) {
      if (yes.find(c) == yes.end()) yes[c] = 0;
      yes[c]++;
    }
  }

  int num = 0;
  for (auto p : yes) {
    if (p.second == g.size()) {
      num++;
    }
  }
  return {static_cast<int>(yes.size()), num};
}

int main(int argc, char** argv) {
  // When building on Windows replace this path with the absolute path of input.txt in the source folder.
  // On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
  // https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
  std::ifstream input(R"(day6/input.txt)");
  std::string line;

  int part1 = 0;
  int part2 = 0;
  std::vector<std::string> group;
  while (std::getline(input, line)) {
    if (line.length() == 0) {
      auto parts = CheckGroup(group);
      part1 += parts.first;
      part2 += parts.second;
      group = {};
    }
    else {
      group.push_back(line);
    }
  }
  auto parts = CheckGroup(group);
  part1 += parts.first;
  part2 += parts.second;
  cout<<part1<<endl<<part2<<endl;
  return 0;
}
