#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <map>
#include <algorithm>
#include <chrono>
#include <limits>
#include <unordered_map>
using namespace std;

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day16/input.txt)");
	std::string line;

	// get rules:
	struct RuleSet {
		int min1, max1, min2, max2; // 2 ranges
	};
	std::map<std::string, RuleSet> rules;
	while (std::getline(input, line)) {
		if (line == "") break;
		string name = "";
		for (int i = 0; i < line.length(); ++i) {
			if (line[i] != ':') {
				name = name + line[i];
			}
			else break;
		}
		RuleSet r;
		char c;
		istringstream stream(line.data() + name.length() + 1);
		stream >> r.min1 >> c >> r.max1 >> c>>c>>r.min2 >> c>>r.max2;
		rules[name] = r;
	}

	std::getline(input, line);//ignored - your ticket
	std::getline(input, line); // numbers.
	istringstream stream(line);
	std::vector<int> my_numbers;
	while (getline(stream, line, ',')) {
		my_numbers.push_back(atoi(line.data()));
	}

	std::getline(input, line);//blank
	std::getline(input, line);//nearby
	std::vector<std::vector<int>> nearby;
	while (std::getline(input, line)) {
		istringstream stream(line);
		std::vector<int> next;
		while (std::getline(stream, line, ',')) {
			next.push_back(atoi(line.data()));
		}
		nearby.push_back(next);
	}

	// some tickets invalid.. find those.
	int sum = 0;
	std::set<int> invalid;
	for (int i = 0; i < nearby.size(); ++i) {
		auto t = nearby[i];
		for (auto field : t) {
			bool valid = false;
			for (auto rule : rules) {
				if (field > rule.second.max2 || field < rule.second.min1 || (field > rule.second.max1 && field < rule.second.min2)) {
					// invalid.
				}
				else {
					valid = true;
					break;
				}
			}
			if (!valid) {
				sum += field;
				invalid.insert(i);
			}
		}
	}
	cout << sum << endl;
}

void Part2() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day16/input.txt)");
	std::string line;

	// get rules:
	struct RuleSet {
		int min1, max1, min2, max2; // 2 ranges
	};
	std::map<std::string, RuleSet> rules;
	while (std::getline(input, line)) {
		if (line == "") break;
		string name = "";
		for (int i = 0; i < line.length(); ++i) {
			if (line[i] != ':') {
				name = name + line[i];
			}
			else break;
		}
		RuleSet r;
		char c;
		istringstream stream(line.data() + name.length() + 1);
		stream >> r.min1 >> c >> r.max1 >> c >> c >> r.min2 >> c >> r.max2;
		rules[name] = r;
	}

	std::getline(input, line);//ignored - your ticket
	std::getline(input, line); // numbers.
	istringstream stream(line);
	std::vector<int> my_numbers;
	while (getline(stream, line, ',')) {
		my_numbers.push_back(atoi(line.data()));
	}

	std::getline(input, line);//blank
	std::getline(input, line);//nearby
	std::vector<std::vector<int>> nearby;
	while (std::getline(input, line)) {
		istringstream stream(line);
		std::vector<int> next;
		while (std::getline(stream, line, ',')) {
			next.push_back(atoi(line.data()));
		}
		nearby.push_back(next);
	}

	// some tickets invalid.. find those.
	int sum = 0;
	std::set<int> invalid;
	for (int i = 0; i < nearby.size(); ++i) {
		auto t = nearby[i];
		for (auto field : t) {
			bool valid = false;
			for (auto rule : rules) {
				if (field > rule.second.max2 || field < rule.second.min1 || (field > rule.second.max1 && field < rule.second.min2)) {
					// invalid.
				}
				else {
					valid = true;
					break;
				}
			}
			if (!valid) {
				sum += field;
				invalid.insert(i);
			}
		}
	}
	std::vector<std::vector<int>> valid;
	for (int i = 0; i < nearby.size(); ++i) {
		if (invalid.find(i) == invalid.end()) {
			valid.push_back(nearby[i]);
		}
	}

	// which field goes to which rule?
	std::map<std::string, std::set<int>> valid_rules;
	for (auto rule : rules) {
		valid_rules[rule.first] = {};
		for (int i = 0; i < valid[0].size(); ++i) {
			bool all_valid = true;
			for (auto &p : valid) {
				auto field = p[i];
				if (field > rule.second.max2 || field < rule.second.min1 || (field > rule.second.max1 && field < rule.second.min2)) {
					// invalid.
					all_valid = false;
					break;
				}
			}
			if (all_valid) {
				//cout << "rule: " << rule.first << ": " << i << endl;
				valid_rules[rule.first].insert(i);
			}
		}
	}

	// filter it down... are there fields that could go to only one rule?
	// (not general, but works for this input)
	std::map<int, string> field_mapping;
	for (int i = 0; i < 100; ++i) {
		for (auto &r : valid_rules) {
			if (r.second.size() == 1) {
				// nothing else could be this rule.
				for (auto &r2 : valid_rules) {
					if (&r == &r2) continue;
					r2.second.erase(*r.second.begin());
				}
				field_mapping[*r.second.begin()] = r.first;
			}

			for (auto field : r.second) {
				auto it = field_mapping.find(field);
				if (it != field_mapping.end() && it->second != r.first) {
					r.second.erase(it->first);
					break;
				}
			}
		}
	}

	uint64_t prod = 1;
	for (auto r : valid_rules) {
		if (r.first[0] == 'd' && r.first[1] == 'e') {
			for (auto i : r.second) {
				prod *= (uint64_t)(my_numbers[i]);
			}
		}
	}

	cout << prod << endl;
}

int main(int argc, char** argv) {
	auto start = std::chrono::steady_clock::now();
	Part1();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	start = std::chrono::steady_clock::now();
	Part2();
	end = std::chrono::steady_clock::now();
	elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
