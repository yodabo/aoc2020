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
	Node* next; // next in list
	//int next_index;
};

void Part2() {
	std::map<int, Node*> nodes;
	std::vector<int> initial = { 3, 6, 8, 1, 9, 5, 7, 4, 2 };
	Node* node_storage = new Node[1000000];
	Node* previous = nullptr;
	Node* first = nullptr;
	for (auto i : initial) {
		Node* node = &node_storage[i - 1];
		if (!first) { first = node; }
		if (previous) {
			previous->next = node;
		}
		nodes[i] = node;
		previous = node;
	}

	// // structure created except for node[1]->lower, and previous->next
	for (int i = 10; i <= 1000000; ++i) {
		auto* node = &node_storage[i - 1];
		previous->next = node;
		
		previous = node;

		// nodes[i] = node; // shouldn't be needed.
	}
	
	previous->next = first; // make it cycle.

	// now structure build up.
	Node* current = first;
	for (int i = 0; i < 10000000; ++i) {
		Node* node = current;

		Node* m = current->next;
		Node* n = m->next;
		Node* o = n->next;

		// remove these 3
		current->next = o->next;

		// insert after the one less than current.
		Node* insert = (current == node_storage) ? &node_storage[1000000-1] : current - 1;
		while (insert == m || insert == n || insert == o) {
			insert = (insert == node_storage) ? &node_storage[1000000 - 1] : insert - 1;
		}
		o->next = insert->next;
		insert->next = m;

		current = current->next;
	}

	int64_t answer = (int64_t)(nodes[1]->next - node_storage + 1) * (int64_t)(nodes[1]->next->next - node_storage + 1);
	cout << "Part2: " << answer << endl;
}

void Part1() {
	std::map<int, Node*> nodes;
	std::vector<int> initial = { 3, 6, 8, 1, 9, 5, 7, 4, 2 };
	Node* previous = nullptr;
	Node* first = nullptr;
	Node* node_storage = new Node[9];
	for (auto i : initial) {
		Node* node = &node_storage[i - 1];
		if (!first) { first = node; }
		if (previous) {
			previous->next = node;
		}
		nodes[i] = node;
		previous = node;
	}
	
	previous->next = first; // make it cycle.

	// now structure build up.
	Node* current = first;
	for (int i = 0; i < 100; ++i) {
		Node* node = current;

		Node* m = current->next;
		Node* n = m->next;
		Node* o = n->next;

		// remove these 3
		current->next = o->next;

		// insert after the one less than current.
		Node* insert = (current == node_storage) ? &node_storage[9 - 1] : current - 1;
		while (insert == m || insert == n || insert == o) {
			insert = (insert == node_storage) ? &node_storage[9 - 1] : insert - 1;
		}
		o->next = insert->next;
		insert->next = m;

		current = current->next;
	}

	{
		Node* node = nodes[1];
		for (int i = 0; i < 8; ++i) {
			node = node->next;
			cout<<(int)(node - node_storage + 1);
		}
		cout << endl;
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
