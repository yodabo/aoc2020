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
	std::ifstream input(R"(day10/input.txt)");
	std::string line;

	std::vector<unsigned long long> nums;

	nums.push_back(0);

	unsigned long long part1 = 0;
	while (std::getline(input, line)) {
		if (line.length() == 0) break;
		std::istringstream stream(line);
		unsigned long long num;
		stream>>num;
		nums.push_back(num);
	}

	std::sort(nums.begin(), nums.end());
	int num1 = 0;
	int num3 = 1;
	for (int i = 0; i < nums.size()-1; ++i) 
	{
		unsigned long long diff = nums[i+1] - nums[i];
		if (diff == 1) num1++;
		if (diff == 3) num3++;
	}
	cout<<num1 * num3<<endl;
	nums.push_back(nums[nums.size() - 1]+3);

	unsigned long long numWays = 1; // num ways to get to nums[i]
	unsigned long long numWaysP = 0; // num ways to get to nums[i-1]
	unsigned long long numWaysPP = 0;// num ways to get to nums[i-2]
	for (int i = 0; i < nums.size()-1; ++i) {
		unsigned long long numWaysCur = 0;

		// can we get to item i+1?
		if (i >= 2 && nums[i+1] - nums[i-2] <= 3) {
			numWaysCur += numWaysPP;
		}
		if (i >= 1 && nums[i+1] - nums[i-1] <= 3) {
			numWaysCur += numWaysP;
		}
		if (nums[i+1] - nums[i] <= 3) {
			numWaysCur += numWays;
		}
		numWaysPP = numWaysP;
		numWaysP = numWays;
		numWays = numWaysCur;
	}
	cout<<numWays<<endl;

	return 0;
}
