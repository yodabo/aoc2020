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

struct Coord {
	int x; int y; int z; int w = 0;
	bool operator<(const Coord& other) {
		return x < other.x ? true :
			x > other.x ? false :
			y < other.y ? true :
			y > other.y ? false :
			z < other.z ? true :
			z > other.z ? false : false;
	}
};

bool operator<(const Coord& lhs, const Coord& rhs) {
	if (lhs.z < rhs.z) return true;
	if (lhs.z == rhs.z && lhs.y < rhs.y) return true;
	if (lhs.z == rhs.z && lhs.y == rhs.y && lhs.x < rhs.x) return true;
	if (lhs.z == rhs.z && lhs.y == rhs.y && lhs.x == rhs.x && lhs.w < rhs.w) return true;
	return false;
}

void Dump(std::set<Coord>& active) {
	for (int z = -2; z <= 2; ++z) {
		cout << endl << "z=" << z << endl;
		for (int y = -6; y <= 6; ++y) {
			for (int x = -6; x <= 6; ++x) {
				if (active.find({ x,y,z }) != active.end()) {
					cout << "#";
				}
				else {
					cout << ".";
				}
			}
			cout << endl;
		}
	}
}

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day17/input.txt)");
	std::string line;

	// game of life:
	// active and 2 or 3 are active, it remains active (otherwise changes)
	// inactive and 3 neighbors it becomes active otherwise inactive.

	std::set<Coord> active = {};
	int y = 0;
	while (std::getline(input, line)) {
		for (int x = 0; x < line.length(); ++x) {
			if (line[x] == '#') {
				active.insert(Coord{ x, y, 0 });
			}
		}
		y++;
	}

	// 6 rounds.
	std::set<Coord> next = {};
	// consider neighbors of active.
	for (int i = 0; i < 6; ++i) {
		//cout << endl<<"After " << i << " cycles:" << endl << endl;
		//Dump(active);
		for (auto coord : active) {
			for (int dx = -1; dx <= 1; ++dx) {
				for (int dy = -1; dy <= 1; ++dy) {
					for (int dz = -1; dz <= 1; ++dz) {

						int num = 0;
						for (int x = coord.x + dx - 1; x <= coord.x + dx + 1; ++x) {
							for (int y = coord.y + dy - 1; y <= coord.y + dy + 1; ++y) {
								for (int z = coord.z + dz - 1; z <= coord.z + dz + 1; ++z) {
									if (x == coord.x+dx && y == coord.y+dy && z == coord.z+dz) continue;
									if (active.find({ x,y,z }) != active.end()) {
										num++;
									}
								}
							}
						}
						if (active.find({ coord.x + dx, coord.y + dy, coord.z + dz }) != active.end()) {
							if (num == 2 || num == 3) next.insert({ coord.x + dx, coord.y + dy, coord.z + dz });
						}
						else if (num == 3) {
							next.insert({ coord.x + dx, coord.y + dy, coord.z + dz });
						}
					}
				}
			}
		}
		active = next;
		next = {};
	}
	cout << "active.size: " << active.size() << endl;
}

void Part2() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day17/input.txt)");
	std::string line;

	// game of life:
	// active and 2 or 3 are active, it remains active (otherwise changes)
	// inactive and 3 neighbors it becomes active otherwise inactive.

	std::set<Coord> active = {};
	int y = 0;
	while (std::getline(input, line)) {
		for (int x = 0; x < line.length(); ++x) {
			if (line[x] == '#') {
				active.insert(Coord{ x, y, 0, 0 });
			}
		}
		y++;
	}

	// 6 rounds.
	std::set<Coord> next = {};
	// consider neighbors of active.
	for (int i = 0; i < 6; ++i) {
		//cout << endl<<"After " << i << " cycles:" << endl << endl;
		//Dump(active);
		for (auto coord : active) {
			for (int dx = -1; dx <= 1; ++dx) {
				for (int dy = -1; dy <= 1; ++dy) {
					for (int dz = -1; dz <= 1; ++dz) {
						for (int dw = -1; dw <= 1; ++dw) {

							int num = 0;
							for (int x = coord.x + dx - 1; x <= coord.x + dx + 1; ++x) {
								for (int y = coord.y + dy - 1; y <= coord.y + dy + 1; ++y) {
									for (int z = coord.z + dz - 1; z <= coord.z + dz + 1; ++z) {
										for (int w = coord.w + dw - 1; w <= coord.w + dw + 1; ++w) {
											if (x == coord.x + dx && y == coord.y + dy && z == coord.z + dz && w == coord.w + dw) continue;
											if (active.find({ x,y,z,w }) != active.end()) {
												num++;
											}
										}
									}
								}
							}
							if (active.find({ coord.x + dx, coord.y + dy, coord.z + dz, coord.w + dw }) != active.end()) {
								if (num == 2 || num == 3) next.insert({ coord.x + dx, coord.y + dy, coord.z + dz, coord.w + dw});
							}
							else if (num == 3) {
								next.insert({ coord.x + dx, coord.y + dy, coord.z + dz, coord.w + dw });
							}
						}
					}
				}
			}
		}
		active = next;
		next = {};
	}
	cout << "active.size: " << active.size() << endl;
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
