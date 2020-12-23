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
#include <queue>
#include <list>
using namespace std;

uint64_t GetScore(std::queue<int> cards) {
	uint64_t ret = 0;
	uint64_t m = cards.size();
	while (m > 0) {
		ret += m * (uint64_t)cards.front();
		cards.pop();
		m--;
	}
	return ret;
}

uint64_t GetScore(std::list<int> cards) {
	uint64_t ret = 0;
	uint64_t m = cards.size();
	for (auto c : cards) {
		ret += m * (uint64_t)c;
		m--;
	}
	return ret;
}

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day22/input.txt)");
	std::string line;
	
	std::queue<int> player1;
	std::queue<int> player2;
	while (std::getline(input, line)) {
		if (line[0] >= '0' && line[0] <= '9')
			player1.push(atoi(line.data()));
		if (line == "") break;
	}
	while (std::getline(input, line)) {
		if (line[0] >= '0' && line[0] <= '9')
			player2.push(atoi(line.data()));
	}

	while (player1.size() != 0 && player2.size() != 0) {
		int card1 = player1.front();
		player1.pop();

		int card2 = player2.front();
		player2.pop();

		if (card1 > card2) {
			player1.push(card1);
			player1.push(card2);
		}
		else {
			player2.push(card2);
			player2.push(card1);
		}
	}

	uint64_t score = (player1.size() > 0) ? GetScore(player1) : GetScore(player2);
	cout << score << endl;
}

std::pair<std::list<int>, list<int>> GetStateHash(std::list<int> &p1, std::list<int> &p2) {
	/*string s = "";
	while (p1.size()) {
		char buffer[10];
		_itoa_s(p1.front(), buffer, 10);
		s = s + buffer + ",";
		p1.pop();
	}
	s = s + ';';
	while (p2.size()) {
		char buffer[10];
		_itoa_s(p2.front(), buffer, 10);
		s = s + buffer + ",";
		p2.pop();
	}
	return s;*/
	return { p1, p2 };
}

uint64_t last_score = 0;

//std::map < std::pair<std::queue<int>, queue<int>> , bool > Winner;
//uint64_t num_rounds = 0;

bool ResursiveCombat(std::list<int> p1, std::list<int> p2, bool update_score = false, int depth = 0) { // copy instead of reference.
	std::pair<std::list<int>, list<int>> s_orig = GetStateHash(p1, p2);

	int local_num_iterations = 0;

	/*if (Winner.find(s_orig) != Winner.end()) {
		return Winner[s_orig];
	}*/
	
	if (!p1.size()) return false;
	if (!p2.size()) return true;

	std::vector<std::pair<std::list<int>, list<int>>> hashed_states = {}; // should be local

	bool done = false;
	while (!done) {
		local_num_iterations++;
		/*if (update_score) {
			cout<<local_num_iterations << endl;
		}*/
		/*num_rounds++;
		if (num_rounds % 10000 == 0)
			cout << num_rounds << endl;*/
		std::pair<std::list<int>, list<int>> s = GetStateHash(p1, p2);
		if (std::find(hashed_states.begin(), hashed_states.end(), s) != hashed_states.end()) {// find(s) != hashed_states.end()) {
			if (update_score) {
				uint64_t score = (p1.size() > 0) ? GetScore(p1) : GetScore(p2);
				cout << score << endl;
			}
			//Winner[s_orig] = true;
			return true;
		}

		int c1 = p1.front();
		p1.pop_front();

		int c2 = p2.front();
		p2.pop_front();

		hashed_states.push_back(s);

		bool winner;
		if (c1 <= p1.size() && c2 <= p2.size()) {
			std::list<int> n1, n2;
			int i = 0;
			for (auto n : p1) {
				n1.push_back(n);
				if (n1.size() >= c1) break;
			}
			for (auto n : p2) {
				n2.push_back(n);
				if (n2.size() >= c2) break;
			}

			winner = ResursiveCombat(n1, n2, false, depth + 1);
			if (depth < 4) {
				/*for (int i = 0; i < depth; ++i) {
					cout << " ";
				}
				cout << "recursive solved:\t" << local_num_iterations << "\t" << winner << " " << depth << endl;*/
			}
		}
		else {
			winner = (c1 > c2);
		}

		if (winner) {
			p1.push_back(c1);
			p1.push_back(c2);
		}
		else {
			p2.push_back(c2);
			p2.push_back(c1);
		}

		if (!p1.size()) {
			if (update_score) {
				uint64_t score = (p1.size() > 0) ? GetScore(p1) : GetScore(p2);
				cout << score << endl;
			}
			//Winner[s_orig] = false;
			return false;
		}
		if (!p2.size()) {
			if (update_score) {
				uint64_t score = (p1.size() > 0) ? GetScore(p1) : GetScore(p2);
				cout << score << endl;
			}
			//Winner[s_orig] = true;
			return true;
		}
	}
}

void Part2() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day22/input.txt)");
	std::string line;

	std::list<int> player1;
	std::list<int> player2;
	while (std::getline(input, line)) {
		if (line[0] >= '0' && line[0] <= '9')
			player1.push_back(atoi(line.data()));
		if (line == "") break;
	}
	while (std::getline(input, line)) {
		if (line[0] >= '0' && line[0] <= '9')
			player2.push_back(atoi(line.data()));
	}

	// Get to same state, game over.
	// otherwise, draw top card as normal.
	// if both players have as at least as many cards remaining as value, winner is decided by playing another game recursively
	// if can't recurse (not enough cards), winner is higher-value card.

	ResursiveCombat(player1, player2, true);

	uint64_t score = (player1.size() > 0) ? GetScore(player1) : GetScore(player2);
	cout << score << endl;
}

int main(int argc, char** argv) {
	auto start = std::chrono::steady_clock::now();
	//Part1();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	start = std::chrono::steady_clock::now();
	Part2();
	end = std::chrono::steady_clock::now();
	elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
