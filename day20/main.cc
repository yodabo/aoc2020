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

class Node : public std::enable_shared_from_this<Node> {
public:
	Node(int id, int x, int y, int rot) : x(x), y(y), id(id), rot(rot) {}
	int id;
	int x;
	int y;
	int rot;
	std::shared_ptr<Node> parent;

	std::set<std::shared_ptr<Node>> Expand(int count, map<int, vector<string>>& tiles) {
		std::set<std::shared_ptr<Node>> next;
		for (auto p : tiles) {
			for (int r = 0; r < 8; r++) {
				// rotate/flip/rearrange
				// can this tile be here?
				
				if (Check(p.first, (x + 1) % count, y + (x == count-1 ? 1 : 0), r, tiles)) {
					auto n = std::make_shared<Node>(p.first, (x + 1) % count, y + (x == count - 1 ? 1 : 0), r);
					n->parent = shared_from_this();
					next.insert(n);
				}
				//cout << endl;
			}
		}
		return next;
	}

	static bool GetChar(int x, int y, int rot, int id, map<int, vector<string>>& tiles) {
		if (rot >= 4) x = (9 - x);
		rot = rot % 4;

		// x > y, swap x/y
		if (rot >= 2) std::swap(x, y);
		rot = rot % 2;

		if (rot >= 1) { x = 9-x; y = 9-y; }
		//cout << tiles[id][x][y];
		return tiles[id][x][y] == '#';
	}

	static void Dump(int id1, int rot1, int id2, int rot2, map<int, vector<string>>& tiles) {
		cout << id1 << "\t" << id2 << endl;
		for (int y = 0; y < 10; ++y) {
			for (int x = 0; x < 10; ++x) {
				cout << (GetChar(x, y, rot1, id1, tiles) ? '#' : '.');
			}

			cout << " ";

			for (int x = 0; x < 10; ++x) {
				cout << (GetChar(x, y, rot2, id2, tiles) ? '#' : '.');
			}

			cout << endl;
		}
		cout << endl;
	}

	bool Check(int i, int x, int y, int rot, map<int, vector<string>>& tiles) {
		//cout << "check: " << i;
		//bool ret = true;
		for (auto* p = this; p; p = p->parent.get()) {
			//cout << " "<<p->id;
			if (p->id == i) {
				return false;
			}
			if (p->x == x - 1 && p->y == y && x >= 1) {
				//Dump(p->id, p->rot, i, rot, tiles);

				// check chars.
				for (int y = 0; y < 10; ++y) {
					if (GetChar(9, y, p->rot, p->id, tiles) != GetChar(0, y, rot, i, tiles))
						return false;
				}
			}
			else if (p->x == x && p->y == y - 1) {
				for (int x = 0; x < 10; ++x) {
					if (GetChar(x, 9, p->rot, p->id, tiles) != GetChar(x, 0, rot, i, tiles))
						return false;
				}
			}
		}
		return true;
	}

	uint64_t GetAnswer(int count) {
		uint64_t answer = 1;
		for (auto* p = this; p; p = p->parent.get()) {
			if (p->x == 0 || p->x == count-1) {
				if (p->y == 0 || p->y == count-1) {
					answer *= p->id;
				}
			}
		}
		return answer;
	}

	void AppendToImage(std::vector<string> &image, map<int, vector<string>>& tiles) {
		for (auto* p = this; p->parent; p = p->parent.get()) {
			for (int y = 1; y < 9; ++y) {
				string line = "";
				for (int x = 1; x < 9; ++x) {
					
			//for (int y = 1; y < 9; ++y) {
			//	for (int x = 1; x < 9; ++x) {
					char c = GetChar(x, y, p->rot, p->id, tiles) ? '#' : '.';
					// append to the right string.
					//auto cur = image[8 * p->y + y-1];
					//auto cur = image[8 * p->y + y-1];
					line = line + c;
					//image[10 * p->y + y] = std::string() + c + (cur.length() > 0 ? cur : "");
					//image[8 * p->y + y - 1] = std::string() + c + (cur.length() > 0 ? cur : "");
				}
				image[8 * p->y + y-1] = line + image[8 * p->y + y-1];
				//auto cur = image[10 * p->y + y];
				//image[10 * p->y + y] = std::string() + ' ' + (cur.length() > 0 ? cur : "");
			}
		}
	}
};


void Part2(std::vector<string> &image) {

	/*for (auto l : image) {
		cout << l << endl;
	}*/

	int sz = image.size();
	int sz2 = image[0].length();

	std::vector<string> monster = {
		"                  # ",
		"#    ##    ##    ###",
		" #  #  #  #  #  #   "
	};

	auto Rotx = [&](int x, int y, int rot) {
		if (rot >= 4) x = (sz - x - 1);
		rot = rot % 4;

		// x > y, swap x/y
		if (rot >= 2) std::swap(x, y);
		rot = rot % 2;

		if (rot >= 1) { x = sz - x - 1; y = sz - y - 1; }
		return x;
	};

	auto Roty = [&](int x, int y, int rot) {
		if (rot >= 4) x = (sz - x - 1);
		rot = rot % 4;

		// x > y, swap x/y
		if (rot >= 2) std::swap(x, y);
		rot = rot % 2;

		if (rot >= 1) { x = sz - x - 1; y = sz - y - 1; }
		return y;
	};


	auto GetChar = [&](int x, int y, int rot) {
		//if (rot >= 4) x = (sz - x - 1);
		//rot = rot % 4;

		//// x > y, swap x/y
		//if (rot >= 2) std::swap(x, y);
		//rot = rot % 2;

		//if (rot >= 1) { x = sz - x - 1; y = sz - y - 1; }
		
		return image[x][y];
	};

	auto IsMonster = [&](int x, int y, int rot) {
		if (GetChar(x, y, rot) != '#') return false;

		for (int x_in_monster = 0; x_in_monster < monster[0].length(); ++x_in_monster) {
			for (int y_in_monster = 0; y_in_monster < monster.size(); ++y_in_monster) {
				// monster can't go beyond image.
				int xx = x;// Rotx(x, y, rot);
				int yy = y;// Roty(x, y, rot);
				if (xx + monster[0].length() - x_in_monster >= sz) continue;
				if (yy + monster.size() - y_in_monster >= sz) continue;
				if (xx - x_in_monster < 0) continue;
				if (yy - y_in_monster < 0) continue;

				// '#' in the image should be part of monster still.
				if (monster[y_in_monster][x_in_monster] != '#') continue;
				
				bool valid = true;
				for (int dx = 0; dx < monster[0].length() && valid; ++dx) {
					for (int dy = 0; dy < monster.size() && valid; ++dy) {
						if (monster[dy][dx] == '#' && GetChar(x - x_in_monster + dx, y - y_in_monster + dy, rot) != '#')
							valid = false;
					}
				}
				if (valid) return true;
			}
		}
		return false;
	};

	int answer = 0;
	int most_monsters = 0;
	for (int rot = 0; rot < 1; ++rot) {
		int num_monsters = 0;
		int num_non = 0;
		for (int y = 0; y < sz; ++y) {
			for (int x = 0; x < sz; ++x) {
				if (IsMonster(x, y, rot))
					num_monsters++;
				else if (GetChar(x, y, rot) == '#')
					num_non++;
			}
		}

		//if (num_monsters > most_monsters) {
			most_monsters = num_monsters;
			answer = num_non;
		//}

			if (num_monsters > 0) {
				for (int y = 0; y < sz; ++y) {
					for (int x = 0; x < sz; ++x) {
						if (IsMonster(x, y, rot))
							cout << "O";
						else
							cout << GetChar(x, y, rot);// (GetChar(x, y, rot) == '#' ? '.' : ' ');
					}
					cout << endl;// << endl;
				}
				cout << endl << endl;
			}
	}
	cout << "most_monsters: " << most_monsters << endl;
	cout << "answer: "<<answer << endl;;
}

void Part1() {
	// When building on Windows replace this path with the absolute path of input.txt in the source folder.
	// On Windows, apparently bazel doesn't set up symlinks, so we can't access these automatically.
	// https://groups.google.com/g/bazel-discuss/c/Po8xN8dhWkI/m/sWPUYV9YBAAJ (I may have a bug in BUILD files myself).
	std::ifstream input(R"(day20/input.txt)");
	std::string line;

	std::map<int, std::vector<string>> tiles;
	// adjacent borders lign up after rotating, etc.

	std::vector<string> current_tile;

	std::vector<int> unused_tiles;
	int current_id = 0;
	while (std::getline(input, line)) {
		if (line[0] == 'T') {
			istringstream stream(line.data() + 4);
			stream >> current_id;
			tiles[current_id] = {};
			unused_tiles.push_back(current_id);
		}
		else if (line.length() != 0) {
			tiles[current_id].push_back(line);
		}
	}

	// place tiles in grid.
	int count = (int)sqrt(tiles.size());

	uint64_t answer = 0;

	int maxx = 0;
	int maxy = 0;

	std::vector<std::shared_ptr<Node>> toExpand;
	toExpand.push_back(std::make_shared<Node>(-1, -1, 0, 0));
	//toExpand.push_back(std::make_shared<Node>(1427, 0, 0, 0));

	std::vector<string> image(3 * 8);

	while (!toExpand.empty()) {
		std::shared_ptr<Node> next = toExpand.back();
		toExpand.pop_back();

		auto next_set = next->Expand(count, tiles);
		for (auto n : next_set) {
			toExpand.push_back(n);

			if (n->x == count-1 && n->y == count-1) {
				answer = n->GetAnswer(count);

				// remove borders of tiles
				// flatten to image.
				image = std::vector<string>(12 * 8);
				n->AppendToImage(image, tiles);
			
				Part2(image);
				//toExpand = {};

				break;
			}
			maxx = std::max(maxx, n->x);
			maxy = std::max(maxy, n->y);
		}
	}


	cout << answer << endl;
}


int main(int argc, char** argv) {
	auto start = std::chrono::steady_clock::now();
	Part1();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	start = std::chrono::steady_clock::now();
	//Part2();
	end = std::chrono::steady_clock::now();
	elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
