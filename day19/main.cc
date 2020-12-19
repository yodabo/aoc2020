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

struct AbstractRule {
	AbstractRule() {}

	virtual bool matches(string& line, std::map<int, AbstractRule*>& rules, int& next_index) = 0;

	virtual std::set<int> AllMatches(string& line, std::map<int, AbstractRule*>& rules, int next_index, int depth) = 0;
};

struct Rule : public AbstractRule
{
public:
	Rule(std::vector<int> rules) : rules(rules) {}

	bool matches(string& line, std::map<int, AbstractRule*>& all_rules, int& next_index) override {
		int index = next_index;
		bool match = true;
		for (auto p : rules) {
			match = match && all_rules[p]->matches(line, all_rules, index);
			if (!match) break;
		}

		// todo: could be multiple matches.
		if (match) {
			next_index = index;
			return true;
		}
		return false;
	}

	std::set<int> AllMatches(string& line, std::map<int, AbstractRule*>& all_rules, int next_index, int depth) {
		if (depth > 2 * line.length()) return {}; // only works because each match must remove at least 1 character (we could quit if depth > length)

		int index = next_index;
		std::set<int> current_index = { next_index };
		for (auto p : rules) {
			std::set<int> pnext_index;
			for (auto i : current_index) {
				std::set<int> pi_next_index = all_rules[p]->AllMatches(line, all_rules, i, depth + 1);
				for (auto next : pi_next_index) {
					pnext_index.insert(next);
				}
			}
			current_index = pnext_index;

			if (!current_index.size()) break;
		}

		return current_index;
	}

	std::vector<int> rules; // parts in order
};

struct StringRule : public AbstractRule {
	StringRule(std::string a) : result(a) {}

	bool matches(string& line, std::map<int, AbstractRule*>& rules, int& next_index) override {
		if (result.length() > line.length() + next_index) return false;

		for (int i = 0; i < result.length(); ++i) {
			if (line[i + next_index] != result[i]) return false;
		}

		next_index += result.length();
		return true;
	}

	std::set<int> AllMatches(string& line, std::map<int, AbstractRule*>& all_rules, int next_index, int depth) {
		if (depth > 2 * line.length()) return {};

		for (int i = 0; i < result.length(); ++i) {
			if (line[i + next_index] != result[i]) return {};
		}
		return { next_index + (int)result.length() };
	}

	std::string result;
};

struct OrRule : public AbstractRule {
	OrRule(std::vector<std::vector<int>> parts) : or_rules(parts) {}

	bool matches(string& line, std::map<int, AbstractRule*>& rules, int& next_index) override {
		for (auto& set : or_rules) {
			int index = next_index;
			bool match = true;
			for (auto p : set) {
				match = match && rules[p]->matches(line, rules, index);
				if (!match) break;
			}

			// todo: could be multiple matches.
			if (match) {
				next_index = index;
				return true;
			}
		}
		return false;
	}

	std::set<int> AllMatches(string& line, std::map<int, AbstractRule*>& all_rules, int next_index, int depth) {
		if (depth > 2 * line.length()) return {};  // only works because each match must remove at least 1 character (we could quit if depth > length)

		int index = next_index;

		std::set<int> all_next = {};

		for (auto rules : or_rules) {
			std::set<int> current_index = { next_index };
			for (auto p : rules) {
				std::set<int> pnext_index;
				for (auto i : current_index) {
					std::set<int> pi_next_index = all_rules[p]->AllMatches(line, all_rules, i, depth + 1);
					for (auto next : pi_next_index) {
						pnext_index.insert(next);
					}
				}
				current_index = pnext_index;

				if (!current_index.size()) break;
			}

			for (auto i : current_index) {
				all_next.insert(i);
			}
		}

		return all_next;
	}

	std::vector<std::vector<int>> or_rules;
};

void Part1(std::map<int, string> rule_replacements) {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day19/input.txt)");
	std::string line;

	std::map<int, AbstractRule*> rules;
	int num_pass = 0;
	while (std::getline(input, line)) {
		if (line.find(':') != string::npos) {
			int num = 0;
			int rule_num = 0;
			
			std::vector<int> nums;
			std::vector<std::vector<int>> or_rules;
			string match = "";

			for (int i = 0; i < line.length(); ++i) {
				if (line[i] == ':') {
					rule_num = num;

					if (rule_replacements.find(rule_num) != rule_replacements.end()) {
						line = rule_replacements[rule_num];
					}
					num = 0;
				}
				else if (line[i] >= '0' && line[i] <= '9') {
					num = num * 10 + (int)(line[i] - '0');
				}
				if (line[i] == '|') {
					or_rules.push_back(nums);
					nums = {};
				}
				else if (line[i] == ' ') {
					if (num != 0) nums.push_back(num);
					num = 0;
				}
				else if (line[i] == '"') {
					match = match + line[i + 1];
					break;
				}
			}

			if (num != 0) {
				nums.push_back(num);
			}

			if (or_rules.size() && nums.size()) {
				or_rules.push_back(nums);
				nums = {};
			}

			if (match != "") {
				rules[rule_num] = new StringRule(match);
			}
			else if (nums.size()) {
				rules[rule_num] = new Rule(nums);
			}
			else if (or_rules.size()) {
				rules[rule_num] = new OrRule(or_rules);
			}
		}
		else if (line.length()) {
			int num_chars_used = 0;
			//if (rules[0]->matches(line, rules, num_chars_used) && num_chars_used == line.length()) {
			//	num_pass++;
			//}

			auto matches = rules[0]->AllMatches(line, rules, 0, 0);
			for (auto m : matches) {
				if (m == line.length()) {
					num_pass++; // only count 1 match.
					break;
				}
			}
		}
	}

	cout << num_pass << endl;
}

int main(int argc, char** argv) {
	auto start = std::chrono::steady_clock::now();
	Part1({});
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	start = std::chrono::steady_clock::now();
	Part1({ { 8, "8: 42 | 42 8" }, {11, "11: 42 31 | 42 11 31"} });
	end = std::chrono::steady_clock::now();
	elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
