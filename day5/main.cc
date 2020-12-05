#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
using namespace std;

unsigned long long GetId(string pass) {
  int minx = 0;
  int maxx = 128;
  // [min, max)
  for (int i = 0; i < 7; i++) {
    int mid = (minx + maxx) / 2;
    if (pass[i] == 'F') {
      maxx = mid;
    } else if (pass[i] == 'B') {
      minx = mid;
    }
  }

  int miny = 0;
  int maxy = 8;
  for (int i = 0; i < 3; i++) {
    int mid = (miny + maxy) / 2;
    if (pass[7+i] == 'L') {
      maxy = mid;
    } else if (pass[7+i] == 'R') {
      miny = mid;
    }
  }

  return minx * 8 + miny;
}

int main(int argc, char** argv) {
  // When building on Windows replace this path with the absolute path of input.txt in the source folder.
  // On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
  // https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
  std::ifstream input(R"(day5/input.txt)");
  std::string line;

  unsigned long long best = 0;

  std::set<unsigned long long> seets;

  while (std::getline(input, line)) {
    auto seet = GetId(line);
    best = max(best, seet);
    seets.insert(seet);
  }

  unsigned long long mySeet = 0;
  for (auto seet : seets) {
    if (seets.find(seet + 2) != seets.end() && seets.find(seet+1) == seets.end()) {
      mySeet = seet + 1;
      break;
    }
  }

  cout<<"part1: "<<best<<endl;
  cout<<"part2: "<<mySeet<<endl;
  return 0;
}
