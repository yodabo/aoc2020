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
using namespace std;


void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day12/input.txt)");
	std::string line;

	// x pos east, y pos north
	int64_t x = 0;
	int64_t y = 0;

	int64_t dx = 1;
	int64_t dy = 0;

	while (std::getline(input, line)) {
		int amount = atoi(line.data()+1);
		switch (line[0]) {
			case 'F': x += (dx) * amount; y += (dy) * amount; break;
			case 'N': y += amount; break;
			case 'S': y -= amount; break;
			case 'E': x += amount; break;
			case 'W': x -= amount; break;
			case 'R': amount = 360 - amount % 360;
			case 'L':
			{
				if (amount % 90 != 0) {
					cout<<"error..."<<endl;
				}
				amount %= 360;
				if (amount == 90) {
					std::swap(dx, dy);
					dx *= -1;
				} else if (amount == 180) {
					dx = -dx;
					dy = -dy;
				}
				else if (amount == 270) {
					std::swap(dx, dy);
					dy *= -1;
				}
			}
		}
	}

	cout<<std::abs(x)+std::abs(y)<<endl;
}

void Part2() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day12/input.txt)");
	std::string line;

	// x pos east, y pos north
	int64_t x = 0;
	int64_t y = 0;

	int64_t dx = 10;
	int64_t dy = 1;

	while (std::getline(input, line)) {
		int amount = atoi(line.data()+1);
		switch (line[0]) {
			case 'F': x += (dx) * amount; y += (dy) * amount; break;
			case 'N': dy += amount; break;
			case 'S': dy -= amount; break;
			case 'E': dx += amount; break;
			case 'W': dx -= amount; break;
			case 'R': amount = 360 - amount % 360;
			case 'L':
			{
				if (amount % 90 != 0) {
					cout<<"error..."<<endl;
				}
				amount %= 360;
				if (amount == 90) {
					std::swap(dx, dy);
					dx *= -1;
				} else if (amount == 180) {
					dx = -dx;
					dy = -dy;
				}
				else if (amount == 270) {
					std::swap(dx, dy);
					dy *= -1;
				}
			}
		}
	}

	cout<<std::abs(x)+std::abs(y)<<endl;
}

int main(int argc, char** argv) {
	auto start = std::chrono::steady_clock::now();
	Part1();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	start = std::chrono::steady_clock::now();
	Part2();
	end = std::chrono::steady_clock::now();
	elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
