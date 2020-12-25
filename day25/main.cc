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
using namespace std;

//
void stuff(int subject_number) {
	int value = 1;
	int loop_size = 1;
	for (int i = 0; i <= loop_size; ++i) {
		value = value * subject_number;
		value = value % 20201227;
	}

	// loop size is secret, door/card have different loop sizes
}

// Handshake:
// card transforms 7 based on its loop size - result is card's public key
// door transforms 7 according to its loop size - result is door's public key
// I get both public keys.
// card transforms door's public key to get encryption key
// door transform's card's public key as encryption key.

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	
	int pk1 = 2959251;
	int pk2 = 4542595;

	// subject_number^(something)%20201227, where something is loop_size.
	// 20201227 is prime.

	int64_t s = 7;
	int64_t v = 1;
	int64_t loopsize_1 = 0;
	int64_t loopsize_2 = 0;

	int64_t v1 = 1;
	int64_t v2 = 1;

	for (int i = 0; i < 20201227; ++i) {
		v *= s;
		v %= 20201227;
		v1 *= pk1;
		v1 %= 20201227;
		v2 *= pk2;
		v2 %= 20201227;
		if (v == pk1) {
			loopsize_1 = i;
			cout << "answer: " << v2 << endl;
		}
	    if (v == pk2) {
			loopsize_2 = i;
			cout << "answer: " << v1 << endl;
		}
	}
}

int main(int argc, char** argv) {
	auto start = std::chrono::steady_clock::now();
	Part1();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
