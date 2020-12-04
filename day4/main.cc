#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;



struct Passport {
  bool byr;
  bool iyr;
  bool eyr;
  bool hgt;
  bool hcl;
  bool ecl;
  bool pid;
  bool cid;
};

bool CheckCurrent(Passport current) {
  cout<<current.byr<<current.iyr<<current.eyr<<current.hgt<<current.hcl<<current.ecl<<current.pid<<current.cid<<endl;
  return current.byr&&current.iyr&&current.eyr&&current.hgt&&current.hcl&&current.ecl&&current.pid;
}

int main(int argc, char** argv) {
  // When building on Windows replace this path with the absolute path of input.txt in the source folder.
  // On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
  // https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
  std::ifstream input(R"(day3/input.txt)");
  std::string line;

  int numCurrent = 0;
  Passport current = {};

  while (std::getline(input, line)) {
    if (line.length() == 0) {
      if (CheckCurrent(current)) numCurrent++;
      current = {};
      cout<<endl;
    }

    string part;
    std::istringstream stream(line);
    while (stream>>part) {
      cout<<part<<endl;
      // part is prefix:suffix
      if (part[0] == 'b') {
        std::istringstream stream(part.data()+4);
        int num;
        stream>>num;
        cout<<"byr: "<<num<<endl;
        current.byr = num <= 2002 && num >= 1920;
      }
      if (part[0] == 'i') {
        std::istringstream stream(part.data() + 4);
        int num;
        stream>>num;
        current.iyr = num <= 2020 && num >= 2010;
      }
      if (part[0] == 'e' && part[1] == 'y') {
        std::istringstream stream(part.data() + 4);
        int num;
        stream>>num;
        current.eyr = num <= 2030 && num >= 2020;
      }
      if (part[1] == 'g') {
        std::istringstream stream(part.data()+4);
        int num;
        string unit;
        stream>>num>>unit;
        if (unit == "cm") {
          current.hgt = num <= 193 && num >= 150;
        }
        if (unit == "in") {
          current.hgt = num <= 76 && num >= 59;
        }
      }
      if (part[0] == 'h' && part[1] == 'c') {
        if (part.length() == 4+1+6 && part[4] == '#') {
          bool good = true;
          for (int i = 0; i < 6; ++i) {
            good = (part[5+i] <= '9' && part[5+1] >= '0') || (part[5+i] <= 'f' && part[5+i] >= 'a');
          }
          current.hcl = good;
        }
      }
      if (part[0] == 'e' && part[1] == 'c') {
        current.ecl = (part == "ecl:brn" || part=="ecl:amb" || part=="ecl:blu" || part=="ecl:gry" || part=="ecl:grn" || part=="ecl:hzl" || part=="ecl:oth");
      }
      if (part[0] == 'p') { 
        if (part.length() == 4+9) {
          bool good = true;
          for (int i = 0; i < 6; ++i) {
            good = (part[4+i] <= '9' && part[4+1] >= '0');
          }
          current.pid = good;
        }
      }
      if (part[0] == 'c') current.cid = true;
    }
  }
  if (CheckCurrent(current)) numCurrent++;

cout<<numCurrent;
  return 0;
}
