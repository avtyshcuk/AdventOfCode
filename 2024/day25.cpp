#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <sstream>

using namespace std;

enum Type {EMPTY = 0, PIN};

int main() {
    fstream in("input25");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    vector<vector<int>> keys;
    vector<vector<int>> locks;
    vector<vector<Type>> schemas;
    int counter{};
    for (string line; getline(in, line);) {
        if (line.empty()) continue;
        vector<Type> row;
        for (auto c : line) {
            if (c == '.') {
                row.push_back(EMPTY);
            } else if (c == '#') {
                row.push_back(PIN);
            }
        }
        schemas.push_back(row);
        counter++;
        if (counter == 7) {
            int size = schemas.size();
            if (all_of(schemas[0].begin(), schemas[0].end(), [](int c) { return c == PIN; })) {
                vector<int> heights(schemas[0].size());
                for (int i = 0; i < schemas[0].size(); ++i) {
                    int height{};
                    for (int j = 1; j < size - 1; ++j) {
                        if (schemas[j][i] == PIN) {
                            height++;
                        } else {
                            break;
                        }
                    }
                    heights[i] = height;
                }
                locks.push_back(heights);
            } else if (all_of(schemas[size - 1].begin(), schemas[size - 1].end(), [](int c) { return c == PIN; })) {
                vector<int> heights(schemas[size - 1].size());
                for (int i = 0; i < schemas[size - 1].size(); ++i) {
                    int height{};
                    for (int j = size - 2; j >= 1; --j) {
                        if (schemas[j][i] == PIN) {
                            height++;
                        } else {
                            break;
                        }
                    }
                    heights[i] = height;
                }
                keys.push_back(heights);
            }
            schemas.clear();
            counter = 0;
        }
    }

    int result{};
    for (auto& key : keys) {
        for (auto& lock : locks) {
            bool ok{true};
            for (int i = 0; ok && i < 5; ++i) {
                if (key[i] + lock[i] > 5) ok = false;
            }
            result += ok;
        }
    }
    cout << " RESULT " << result << endl;
    
    return 0;
}
