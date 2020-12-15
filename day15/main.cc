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

void Part1(int num) {
	std::vector<int> last(30000000);
	std::fill(last.begin(), last.end(), -1);
	last[9] = 0;
	last[3] = 1;
	last[1] = 2;
	last[0] = 3;
	last[8] = 4;

	int last_number = 4;
	for (int i = 6; i<num; ++i) {
		int next = 0;
		if (last[last_number] != -1) {
			next = i-1 - last[last_number];
		}
		last[last_number] = i-1;
		last_number = next;
	}
	cout<<last_number<<endl;
}

int main(int argc, char** argv) {
	auto start = std::chrono::steady_clock::now();
	Part1(2020);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	start = std::chrono::steady_clock::now();
	Part1(30000000);
	end = std::chrono::steady_clock::now();
	elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
