#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <map>
using namespace std;

int CountBagsIn(const std::string& bag, std::map<std::string, std::map<std::string, int>>& contains) {
	auto it = contains.find(bag);
	int sum = 0;
	if (it != contains.end()) {
		const std::map<std::string, int>& my_map = it->second;
		for (auto &p : my_map) {
			sum += p.second * (1 + CountBagsIn(p.first, contains));
		}
	}
	return sum;
}

int main(int argc, char** argv) {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day7/input.txt)");
	std::string line;

	std::map<std::string, std::map<std::string, int>> contains;

	std::vector<std::string> group;
	std::map<string, std::set<string>> reachable;
	while (std::getline(input, line)) {
		string first, second;
		std::istringstream stream(line);
		stream >> first >> second;
		string name = first + second;

		string temp;
		stream >> temp >> temp; // bag contain
		std::map<std::string, int> contains_inner;
		while (!stream.eof()) {
			string s;
			stream >> s;
			if (s == "no") break;
			int count = atoi(s.data());
			stream >> first >> second >> temp; // # name name bag
			if (count) {
				contains_inner[first + second] = count;
				reachable[first + second].insert(name);
			}
		}
		contains[name] = contains_inner;
	}

	std::set<string> toExpand = { "shinygold" };
	std::set<string> fromRoot = {};
	while (!toExpand.empty()) {
		auto expanding = toExpand.begin();

		// already expanded this.
		if (fromRoot.find(*expanding) == fromRoot.end()) {
			fromRoot.insert(*expanding);
			if (reachable.find(*expanding) != reachable.end()) {
				for (auto r : reachable.find(*expanding)->second) {
					if (fromRoot.find(r) == fromRoot.end())
						toExpand.insert(r);
				}
			}
		}
		toExpand.erase(expanding);
	}

	cout << fromRoot.size() - 1<<endl; // shiny gold by itself...

	cout << CountBagsIn("shinygold", contains) << endl;

	return 0;
}
