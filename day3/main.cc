#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


int main(int argc, char** argv) {
  // When building on Windows replace this path with the absolute path of input.txt in the source folder.
  // On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
  // https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
  std::ifstream input(R"(day3/input.txt)");
  std::string line;
  int x[5] = {0};
  int y = 0;
  
  int numTrees[5] = {0};
  int dx[5] = {1,3,5,7,1};

  while (std::getline(input, line)) {
    for (int i = 0; i < 5; ++i) {
        if (y % 2 == 0 || i < 4) {
            if (line[x[i]] == '#') numTrees[i]++;
            x[i] += dx[i];
            x[i] %= line.length();
        }
    }
    y++;
  }
  unsigned long long part2 = 1;
  for (int i = 0; i < 5; ++i) {
      part2 = part2 * numTrees[i];
  }

  // part1:
  cout<<numTrees[1]<<endl;
  // part2:
  cout<<part2<<endl;
  return 0;
}
