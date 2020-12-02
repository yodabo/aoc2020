#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

bool isValid1(int min, int max, char cc, const string& str) {
  int num = 0;
  for (auto c : str) {
    if (c == cc) {
      num++;
    }
  }
  return num <= max && num >= min;
}

bool isValid2(int i, int j, char cc, const string& str) {
  bool first = i-1 < str.length() && str[i-1] == cc;
  bool second = j-1 < str.length() &&  str[j-1] == cc;
  return (first && !second) || (second && !first);
}

int main(int argc, char** argv) {
  // When building on Windows replace this path with the absolute path of input.txt in the source folder.
  // On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
  // https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
  std::ifstream input(R"(day2/input.txt)");
  std::string line;
  int num_part1 = 0;
  int num_part2 = 0;
  while (std::getline(input, line)) {
    std::istringstream stream(line);
    char c, throwaway;
    int min, max;
    std::string str;
    stream>>min>>throwaway>>max;
    stream>>c>>throwaway;
    stream>>str;

    if (isValid1(min, max, c, str)) num_part1++;
    if (isValid2(min, max, c, str)) num_part2++;
  }

  cout<<"Part 1: " << num_part1<<endl;
  cout<<"Part 2: " << num_part2<<endl;
  return 0;
}
