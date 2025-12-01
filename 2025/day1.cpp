#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int main() {
	ifstream in1{"input1"};
	if (!in1) {
		cerr << "File is missed!\n";
		return -1;
	}
	vector<pair<char, int>> seq;
	string line;
	while (getline(in1, line)) {
		char dir = line[0];
		int dist = stoi(line.substr(1));
		seq.push_back({dir, dist});
	}
	in1.close();

	int current{50};
	int password{};
	for (int i = 0; i < seq.size(); ++i) {
		if (seq[i].first == 'L') {
			current -= seq[i].second;
			current %= 100;
			if (current < 0) {
				current += 100;
			}
		} else {
			current += seq[i].second;
			current %= 100;
		}
		if (current == 0) {
			password++;
		}
	}
	cout << password << endl;

	current = 50;
	password = 0;
	for (int i = 0; i < seq.size(); ++i) {
		char direction = seq[i].first;
		int distance = seq[i].second;
		if (direction == 'R') {
            		for (int i = 0; i < distance; ++i) {
                		current = (current + 1) % 100;
                		if (current == 0) {
                    			password++;
                		}
            		}
        	} else {
            		for (int i = 0; i < distance; ++i) {
                		current = (current - 1 + 100) % 100;
                		if (current == 0) {
					password++;
                		}
            		}
        	}
	}
	cout << password << endl;
}


