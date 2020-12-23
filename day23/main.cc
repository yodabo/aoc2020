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

struct Node {
	std::shared_ptr<Node> lower; // lower in value
	std::shared_ptr<Node> next; // next in list
	int value;
};

void Part2() {
	std::map<int, shared_ptr<Node>> nodes;
	std::vector<int> initial = { 3, 6, 8, 1, 9, 5, 7, 4, 2 };
	shared_ptr<Node> previous;
	shared_ptr<Node> first;
	for (auto i : initial) {
		auto node = make_shared<Node>();
		if (!first) { first = node;}
		node->value = i;
		if (previous) {
			previous->next = node;
		}
		nodes[i] = node;
		previous = node;
	}

	// all nodes created, fix up lower
	for (int i = 2; i <= 9; ++i) {
		nodes[i]->lower = nodes[i-1];
	}

	nodes[1]->lower = nodes[9];

	// // structure created except for node[1]->lower, and previous->next
	for (int i = 10; i <= 1000000; ++i) {
		auto node = make_shared<Node>();
		node->value = i;
		previous->next = node;
		if (i > 10)
			node->lower = previous;
		else
			node->lower = nodes[9];

		previous = node;
		
		// nodes[i] = node; // shouldn't be needed.
	}
	nodes[1]->lower = previous; // wrap when finding lower.

	previous->next = first; // make it cycle.
	
	// now structure build up.
	shared_ptr<Node> current = first;
	for (int i = 0; i < 10000000; ++i) {
		shared_ptr<Node> node = current;

		shared_ptr<Node> m = current->next;
		shared_ptr<Node> n = m->next;
		shared_ptr<Node> o = n->next;

		// remove these 3
		current->next = o->next;

		// insert after the one less than current.
		shared_ptr<Node> insert = current->lower;
		while (insert == m || insert == n || insert == o) { insert = insert->lower; }
		o->next = insert->next;
		insert->next = m;

		current = current->next;
	}

	int64_t answer = (int64_t)nodes[1]->next->value * (int64_t)nodes[1]->next->next->value;
	cout<<"Part2: "<<answer<<endl;
}

void Part1() {
	std::map<int, shared_ptr<Node>> nodes;
	std::vector<int> initial = { 3, 6, 8, 1, 9, 5, 7, 4, 2 };
	shared_ptr<Node> previous;
	shared_ptr<Node> first;
	for (auto i : initial) {
		auto node = make_shared<Node>();
		if (!first) { first = node;}
		node->value = i;
		if (previous) {
			previous->next = node;
		}
		nodes[i] = node;
		previous = node;
	}

	// all nodes created, fix up lower
	for (int i = 2; i <= 9; ++i) {
		nodes[i]->lower = nodes[i-1];
	}

	nodes[1]->lower = nodes[9]; // lower wraps
	previous->next = first; // make it cycle.
	
	// now structure build up.
	shared_ptr<Node> current = first;
	for (int i = 0; i < 100; ++i) {
		shared_ptr<Node> node = current;

		shared_ptr<Node> m = current->next;
		shared_ptr<Node> n = m->next;
		shared_ptr<Node> o = n->next;

		// remove these 3
		current->next = o->next;

		// insert after the one less than current.
		shared_ptr<Node> insert = current->lower;
		while (insert == m || insert == n || insert == o) { insert = insert->lower; }
		o->next = insert->next;
		insert->next = m;

		current = current->next;
	}

	{
		shared_ptr<Node> node = nodes[1];
		for (int i = 0; i < 8; ++i) {
			node = node->next;
			cout<<node->value;
		}
		cout<<endl;
	}
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
