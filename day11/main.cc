#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <map>
#include <algorithm>
using namespace std;


int numOccupiedNeighborsPart1(int row, int col, vector<string>& map) {
	int ret = 0;
	for (int dr = -1; dr <= 1; ++dr) {
		for (int dc = -1; dc <= 1; ++dc) {
			if (dr == 0 && dc == 0) continue;
			if (row + dr < 0 || row + dr >= map.size()) continue;
			if (col + dc < 0 || col + dc >= map[0].length()) continue;

			if (map[row+dr][col+dc] == '#')
				ret++;
		}
	}
	return ret;
}

int numOccupiedNeighborsPart2(int row, int col, vector<string>& map) {
	int ret = 0;
	for (int dr = -1; dr <= 1; ++dr) {
		for (int dc = -1; dc <= 1; ++dc) {
			if (dr == 0 && dc == 0) continue;
			if (row + dr < 0 || row + dr >= map.size()) continue;
			if (col + dc < 0 || col + dc >= map[0].length()) continue;

			// now we have a direction... walk it till we reach edge or find seet.
			for (int num = 1; row + dr*num >= 0 && row + dr*num < map.size() && col + dc * num >= 0 && col + dc*num < map[0].length(); ++num) {
				if (map[row+dr*num][col+dc*num] == '#') {
					ret++; break;
				} else if (map[row+dr*num][col+dc*num] == 'L') {
					break;
				}
			}

			//if (map[row+dr][col+dc] == '#')
			//ret++;
		}
	}
	return ret;
}

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day11/input.txt)");
	std::string line;

	std::vector<string> page1, page2;
	unsigned long long part1 = 0;
	while (std::getline(input, line)) {
		page1.push_back(line);
		page2.push_back(line);
	}

	int num_rounds = 0;
	int num_changes = 0;
	int occupied = 0;
	
	do {
		auto* current = num_rounds % 2 == 0 ? &page1 : &page2;
		auto* next = num_rounds % 2 == 0 ? &page2 : &page1;
		num_changes = 0;
		num_rounds++;
		occupied = 0;

		for (int row = 0; row < page1.size(); ++row) {
			for (int col = 0; col < page1[0].length(); ++col) {
				if ((*current)[row][col] == 'L' && (numOccupiedNeighborsPart1(row, col, *current) == 0)) {
					(*next)[row][col] = '#';
					num_changes++;
				}
				else if ((*current)[row][col] == '#' && numOccupiedNeighborsPart1(row, col, *current) >= 4) {
					(*next)[row][col] = 'L';
					num_changes++;
				} else {
					(*next)[row][col] = (*current)[row][col];
				}

				if ((*next)[row][col] == '#')
					occupied++;
			}
		}
	} while (num_changes > 0);

	cout<<occupied<<endl;
}

void Part2() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day11/input.txt)");
	std::string line;

	std::vector<string> page1, page2;
	unsigned long long part1 = 0;
	while (std::getline(input, line)) {
		page1.push_back(line);
		page2.push_back(line);
	}

	int num_rounds = 0;
	int num_changes = 0;
	int occupied = 0;
	
	do {
		auto* current = num_rounds % 2 == 0 ? &page1 : &page2;
		auto* next = num_rounds % 2 == 0 ? &page2 : &page1;
		num_changes = 0;
		num_rounds++;
		occupied = 0;

		for (int row = 0; row < page1.size(); ++row) {
			for (int col = 0; col < page1[0].length(); ++col) {
				if ((*current)[row][col] == 'L' && (numOccupiedNeighborsPart2(row, col, *current) == 0)) {
					(*next)[row][col] = '#';
					num_changes++;
				}
				else if ((*current)[row][col] == '#' && numOccupiedNeighborsPart2(row, col, *current) >= 5) {
					(*next)[row][col] = 'L';
					num_changes++;
				} else {
					(*next)[row][col] = (*current)[row][col];
				}

				if ((*next)[row][col] == '#')
					occupied++;
			}
		}
	} while (num_changes > 0);

	cout<<occupied<<endl;
}

int main(int argc, char** argv) {
	Part1();
	Part2();
}
