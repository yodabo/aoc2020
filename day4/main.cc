#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>
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

bool CheckYear(string value, int min, int max) {
  int year = atoi(value.data());
   return value.length() == 4 && year >= min && year <= max;
}

void VerifyValue(string field, string value, Passport* p) {
  if (field == "byr") {
    p->byr = CheckYear(value, 1920, 2002);
  } else if (field == "iyr") {
    p->iyr = CheckYear(value, 1920, 2020);
  } else if (field == "eyr") {
    p->eyr = CheckYear(value, 2020, 2030);
  } else if (field == "hgt") {
    std::istringstream stream(value);
    int num;
    string unit;
    stream>>num>>unit;
    if (unit == "cm") {
      p->hgt = num <= 193 && num >= 150;
    }
    if (unit == "in") {
      p->hgt = num <= 76 && num >= 59;
    }
  } else if (field == "hcl") {
    p->hcl = regex_match(value, regex("^#[0-9a-f]{6}$"));
  } else if (field == "ecl") {
    p->ecl = (value == "brn" || value=="amb" || value=="blu" || value=="gry" || value=="grn" || value=="hzl" || value=="oth");
  } else if (field == "pid") {
    // 9 digits, including leading zeros
    p->pid = regex_match(value, regex("^[0-9]{9}$"));
  } else if (field == "cid") {
  }
}

int main(int argc, char** argv) {
  // When building on Windows replace this path with the absolute path of input.txt in the source folder.
  // On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
  // https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
  std::ifstream input(R"(day4/input.txt)");
  std::string line;

  int numCurrent = 0;
  Passport current = {};

  while (std::getline(input, line)) {
    if (line.length() == 0) {
      if (CheckCurrent(current)) numCurrent++;
      current = {};
      cout<<endl;
    }

    istringstream stream(line);
    string component;
    while (stream >> component) {
      // This is expected to be something like "foo:bar".  Split to get each part.
      string field;
      string value;
      istringstream f(component);
      if (getline(f, field, ':') && getline(f, value)) {
        cout<<"field: " << field<<" value: "<<value<<endl;
        VerifyValue(field, value, &current);
      }
    }
  }
  if (CheckCurrent(current)) numCurrent++;

cout<<numCurrent;
  return 0;
}
