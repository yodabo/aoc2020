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
using namespace std;

int roundUp(int start, int m) {
	int mod = start % m;
	if (mod != 0) {
		start += m - mod;
	}
	return start;
}

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day13/input.txt)");
	std::string line;

	// timestamp 0 - all buses leave port
	// some time - busses get to airport
	// some other time - bus gets to sea port, then repeates forever.
	// loop takes time (id number)
	// timestamp - first time you can depart on a bus
	// busses in service at that time

	// earliest bus that can get to airport
	int64_t result = std::numeric_limits<int64_t>::max();

	std::map<int64_t, int64_t> nums;
	int64_t best_id = 0;

	std::getline(input, line);
	int64_t timestamp = atoi(line.data());
	while (std::getline(input, line, ',')) {
		if (line == "x") continue;
		int64_t id = atoi(line.data());
		nums[id] = roundUp(timestamp, id);
		result = std::min(result, nums[id]);
		if (result == nums[id])
			best_id = id;
	}
	cout << (result - timestamp) * best_id << endl;
}

bool isPrime(int64_t num) {
	for (int p = 2; p*p <= num; ++p) {
		if (num % p == 0) return false;
	}
	return true;
}

void Part2() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day13/input.txt)");
	std::string line;

	std::getline(input, line); // skip first line

	int64_t time = 0;
	int64_t prod_constraints = 1;

	int i = -1;
	while (std::getline(input, line, ',')) {
		i++;
		if (line == "x") continue;
		int64_t id = atoi(line.data());

		// add prod_constraints to time untill we satisfy new constraint
		while ((time+i) % id) {
			time += prod_constraints;
		}
		prod_constraints *= id;
	}

	cout<<time<<endl;
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
