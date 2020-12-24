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

struct Coord {
	int64_t row;
	int64_t col; // increases as you go across for same row
};

bool operator<(const Coord& lhs, const Coord& rhs) {
	if (lhs.row < rhs.row) return true;
	if (lhs.row > rhs.row) return false;

	if (lhs.col < rhs.col) return true;

	return false;
}

std::set<Coord> Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day24/input.txt)");
	std::string line;

	// hex tiles.
	// each row is steps to get to a single tile

	// six neighbors - e, se, sw, w, nw, ne

	// coordinate convention. going straight down always shifts 0.5 in col.
	// |0|1|2|   row 0
	//|0|1|2|3|
	// |1|2|3|   row 2

	auto step = [&](string s) {
		Coord c = {};
		for (int i = 0; i < s.length(); ++i) {
			if (s[i] == 'e') {
				c.col++;
			} if (s[i] == 'w') {
				c.col--;
			} if (s[i] == 's') {
				if (s[i + 1] == 'e') {
					c.col++;
				}
				c.row++;
				i++;
			} if (s[i] == 'n') {
				if (s[i + 1] == 'w') {
					c.col--;
				}
				c.row--;
				i++;
			}
		}
		return c;
	};

	std::set<Coord> flipped;
	while (std::getline(input, line)) {
		auto c = step(line);
		if (flipped.find(c) != flipped.end()) {
			flipped.erase(c);
		}
		else {
			flipped.insert(c);
		}
	}
	cout << flipped.size() << endl;
	return flipped;
}

void Part2(std::set<Coord> initial) {
	std::set<Coord> current = initial;
	std::set<Coord> next = {};

	// coordinate convention. going straight down always shifts 0.5 in col.
	// |0|1|2|   row 0
	//|0|1|2|3|
	// |1|2|3|   row 2

	std::vector<Coord> neighbors = { {-1,-1},{-1,0},{0,-1},{0,1},{1,0},{1,1} };
	
	for (int i = 0; i < 100; ++i) {
		for (auto c : current) {
			for (auto d : neighbors) {
				Coord cell = { c.row + d.row, c.col + d.col };
				int count = 0;
				for (auto e : neighbors) {
					Coord n = { cell.row + e.row, cell.col + e.col };
					if (current.find(n) != current.end()) {
						count++;
					}
				}

				if (current.find(cell) != current.end()) {
					// black stays black for 1 or 2
					if (count == 1 || count == 2) {
						next.insert(cell);
					}
				}
				else {
					// white turns black if 2 adjacent
					if (count == 2) {
						next.insert(cell);
					}
				}
			}
		}

		current = next;
		next = {};

		cout << "after " << i + 1 << " days " << current.size() << endl;
	}

	cout << "Part2: " << current.size();
}

int main(int argc, char** argv) {
	auto start = std::chrono::steady_clock::now();
	auto f = Part1();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	start = std::chrono::steady_clock::now();
	Part2(f);
	end = std::chrono::steady_clock::now();
	elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
