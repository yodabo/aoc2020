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
#include <memory>
#include <unordered_map>
using namespace std;

struct Recipe {
	std::set<string> ingredients;
	std::set<string> allergens;
};

std::set<string> intersect(std::set<string>& first, std::set<string>& second) {
	std::set<string> ret;
	for (auto& s : first) {
		if (second.find(s) != second.end()) {
			ret.insert(s);
		}
	}
	return ret;
}

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day21/input.txt)");
	std::string line;

	std::set<string> all_allergens;
	std::set<string> all_ingredients;
	std::vector<Recipe> recipes;

	while (std::getline(input, line)) {
		istringstream stream(line);
		std::set<string> ingredients;
		while (getline(stream, line, ' ')) {
			if (line[0] != '(') {
				ingredients.insert(line);
				all_ingredients.insert(line);
			}
			else break;
		}
		std::set<string> allergens;
		if (line[0] == '(') { // skip line '(contains'
			while (getline(stream, line, ' ')) {
				while (line[line.length() - 1] == ',' || line[line.length() - 1] == ')') {
					line = line.substr(0, line.length() - 1);
				}
				allergens.insert(line);
				all_allergens.insert(line);
			}
		}

		recipes.push_back({ ingredients, allergens });
	}

	
	// each allergen is found in exactly one ingredient, but some allergens may not be marked.
	// each ingredient contains exactly zero or one allergen.
	// each allergen is contained in exactly one ingredient.
	// 200 ingredients, 8 allergens, 42 recipes.

	std::map<string, std::set<string>> maybe_contains; // allergen->ingredient mapping.
	for (auto &allergen : all_allergens) {
		maybe_contains[allergen] = all_ingredients;
		for (auto &r : recipes) {
			if (r.allergens.find(allergen) != r.allergens.end()) {
				maybe_contains[allergen] = intersect(maybe_contains[allergen], r.ingredients);
			}
		}
	}
	for (int i = 0; i < 10; ++i) {
		for (auto& p : maybe_contains) {
			if (p.second.size() == 1) {
				auto& ingredient = *p.second.begin();
				for (auto& p2 : maybe_contains) {
					if (p.first == p2.first) continue;

					if (p2.second.find(ingredient) != p2.second.end()) {
						p2.second.erase(ingredient);
					}
				}
			}
		}
	}

	std::set<string> non_allergenic = all_ingredients;
	for (auto &p : maybe_contains) {
		for (auto& ingredient : p.second) {
			if (non_allergenic.find(ingredient) != non_allergenic.end()) {
				non_allergenic.erase(ingredient);
			}
		}
	}

	int answer = 0;
	for (auto &r : recipes) {
		for (auto &ingredient : r.ingredients) {
			if (non_allergenic.find(ingredient) != non_allergenic.end()) {
				answer++;
			}
		}
	}

	cout << "part1: " << answer<<endl;

	string part2 = "";
	for (auto &p : maybe_contains) {
		part2 = part2 + "," + *p.second.begin();
	}
	part2 = part2.substr(1);
	cout << "part2: " << part2 << endl;

}

void Part2() {

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
