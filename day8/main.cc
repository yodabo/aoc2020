#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <map>
using namespace std;

int main(int argc, char** argv) {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day8/input.txt)");
	std::string line;

	std::vector<string> lines;

	while (std::getline(input, line)) {
		lines.push_back(line);
	}

	int part1 = 0;
	for (int noop_ins = 0; noop_ins <= lines.size(); ++noop_ins) {
		std::set<int> insts_run;

		int acc = 0;
		int ip = 0;
		while(true) {
			insts_run.insert(ip);
			auto& inst = lines[ip];
			std::istringstream stream(inst);
			string op;
			int num;
			stream>>op>>num;
			if (ip == noop_ins) {
				op = (op == "jmp") ? string("nop") :
					 (op == "nop") ? string("jmp") : op;
				ip++; continue;
			}
			if (op == "jmp") {
				ip += num;
			} else {
				ip += 1;
			}

			if (op == "acc") {
				acc += num;
			}

			if (insts_run.find(ip) != insts_run.end()) {
				part1 = acc;
				break;
			}
			if (ip == lines.size()) {
				cout<<"part2: "<<acc<<endl;
				break;
			}
		}
	}

	cout << "part1: "<<part1 <<endl;
	return 0;
}
