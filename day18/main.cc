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

int64_t GetNum(string s, int& index) {
	int64_t num = 0;
	for (int i = index; s[i] >= '0' && s[i] <= '9'; ++i) {
		num = 10 * num + (int64_t)(s[i] - '0');
		index = i;
	}
	return num;
}

template <typename TEval>
int64_t Do(string s, int& index, TEval&& evaluate) {
	int start = index;

	std::vector<int64_t> nums;
	std::vector<char> operators;

	char op = 0;
	for (int i = index; i < s.length(); ++i) {
		if (s[i] == '(') {
			i += 1; // skip '('
			nums.push_back(Do(s.data(), i, evaluate)); // line[i] == ')'
		}
		else if (s[i] == '*') {
			operators.push_back('*');
		}
		else if (s[i] == '+') {
			operators.push_back('+');
		}
		else if (s[i] >= '0' && s[i] <= '9') {
			nums.push_back(GetNum(s, i));
		}
		else if (s[i] == ')') {
			index = i; // outer caller continues.
			break;
		}
	}
	return evaluate(nums, operators);
}

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day18/input.txt)");
	std::string line;

	int64_t sum = 0;
	while (std::getline(input, line)) {
		int index = 0;
		int64_t num = Do(line, index, [](std::vector<int64_t> nums, std::vector<char> operators) {
			int64_t result = nums[0];
			for (int i = 0; i < operators.size(); ++i) {
				if (operators[i] == '*')
					result *= nums[i + 1];
				else if (operators[i] == '+')
					result += nums[i + 1];
			}
			return result;
		});

		sum += num;
	}
	cout << sum<<endl;
}

void Part2() {
	std::ifstream input(R"(day18/input.txt)");
	std::string line;

	int64_t sum = 0;
	while (std::getline(input, line)) {
		int index = 0;
		int64_t num = Do(line, index, [](std::vector<int64_t> nums, std::vector<char> operators) {
			std::vector<int64_t> to_multiply = {};

			int64_t sum = nums[0];
			for (int i = 0; i < operators.size(); ++i) {
				if (operators[i] == '*') {
					to_multiply.push_back(sum);
					sum = nums[i+1];
				}
				else if (operators[i] == '+') {
					sum += nums[i + 1];
				}
			}
			if (sum == 0) sum = 1;
			for (auto p : to_multiply) {
				sum *= p;
			}
			return sum;
		});
		sum += num;
	}
	cout << sum<<endl;
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
