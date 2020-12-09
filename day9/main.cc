#include <ctime>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <map>
using namespace std;

bool isValid(unsigned long long num, std::vector<unsigned long long> &nums) {
	bool found = false;
	for (int i = 0; i < 25; ++i) {
		for (int j = i+1; j < 25; ++j) {
			if (nums[nums.size() - 25 + i] + nums[nums.size() - 25 + j] == num)
			return true;
		}
	}
	return false;
}

int main(int argc, char** argv) {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day9/input.txt)");
	std::string line;

	std::vector<unsigned long long> nums;

	for (int i = 0; i < 25; ++i) {
		unsigned long long num;
		input>>num;
		nums.push_back(num);
	}

	unsigned long long part1 = 0;
	while (!input.eof()) {
		unsigned long long num;
		input>>num;
		if (!isValid(num, nums)) {
			cout<<num<<endl;
			part1 = num;
			break;
		}
		
		nums.push_back(num);
	}

	for (int i = 0; i < nums.size() - 1; ++i) {
		unsigned long long sum = nums[i];
		for (int len = 1; i+len < nums.size() && sum < part1; ++len) {
			sum += nums[i+len];
			if (sum == part1) {
				unsigned long long min = part1;
				unsigned long long max = 0;
				for (int j = 0; j <= len; ++j) {
					max = std::max(max, nums[j+i]);
					min = std::min(min, nums[j+i]);
				}
				cout<<min<<" "<<max<<" "<<min+max<<endl; // was it already sorted?
				break;
			}
		}
	}

	return 0;
}
