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

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day14/input.txt)");
	std::string line;

	uint64_t and_mask = ~0;
	uint64_t or_mask = 0;

	auto SetMask = [&](string line) {
		and_mask = ~0;
		or_mask = 0;
		for (uint64_t i = 0; i < line.length(); ++i) {
			if (line[i] == '0') {
				and_mask &= ~(1ull<<(35ull-i));
			}
			if (line[i] == '1') {
				or_mask |= 1ull<<(35ull-i);
			}
		}
	};
	auto Parse = [](string line) -> uint64_t {
		auto res = strtoull(line.data(), nullptr, 10);
		return res;
	};

	std::map<uint64_t, uint64_t> memory;
	// 36 bit unsigned int.
	while (std::getline(input, line)) {
		if (line[1] == 'a') {
			// set mask.
			SetMask(line.data() + 7);
		} else {
			uint64_t address = atoi(line.data()+4);
			int i = 0;
			for (;line[i] != '='; i++){}

			memory[address] = (and_mask & Parse(line.data() + i+1)) | or_mask;
		}
	}


	uint64_t sum = 0;
	for (auto p : memory) {
		sum += p.second;
	}
	cout<<"answer: "<<sum<<endl;
}

template <typename TFunc>
void ForAllCombinations(std::vector<uint64_t> &bits, TFunc &&f) {
	for (uint64_t x = 0; x < 1ull<<bits.size(); ++x) {
		uint64_t mask = 0;
		for (uint64_t i = 0; i < 36; ++i) {
			if (x & (1ull<<i)) {
				mask |= bits[i];
			}
		}

		f(mask);
	}
}

void Part2() {
		// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day14/input.txt)");
	std::string line;

	uint64_t and_mask = ~0;
	uint64_t or_mask = 0;

	auto SetMask = [&](string line) {
		and_mask = ~0;
		or_mask = 0;
		for (uint64_t i = 0; i < line.length(); ++i) {
			if (line[i] == '0') {
				and_mask &= ~(1ull<<(35ull-i));
			}
			if (line[i] == '1') {
				or_mask |= 1ull<<(35ull-i);
			}
		}
	};
	auto Parse = [](string line) -> uint64_t {
		auto res = strtoull(line.data(), nullptr, 10);
		return res;
	};

	std::map<uint64_t, uint64_t> memory;
	// 36 bit unsigned int.
	while (std::getline(input, line)) {
		if (line[1] == 'a') {
			// set mask.
			SetMask(line.data() + 7);
		} else {
			uint64_t address = atoi(line.data()+4);
			int i = 0;
			for (;line[i] != '='; i++){}

			address |= or_mask;
			auto f_mask = and_mask & ~or_mask;
			address &= ~f_mask;
			
			// For each address that matches our pattern.
			std::vector<uint64_t> bits= {};
			for (uint64_t b = 0; b < 36; b++) {
				if (f_mask & (1ull<<b)) {
					bits.push_back(1ull<<b);
				}
			}
			uint64_t value = Parse(line.data() + i+1);
			ForAllCombinations(bits, [&](uint64_t bc) {
				memory[bc|address] = value;
			});

			uint64_t sum = 0;
			for (auto p : memory) {
				sum += p.second;
			}
		}
	}

	uint64_t sum = 0;
	for (auto p : memory) {
		sum += p.second;
	}
	cout<<"answer: "<<sum<<endl;
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
