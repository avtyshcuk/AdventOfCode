#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>

using namespace std;

int main() {
    ifstream in("input8");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1; 
    }

    unordered_map<char, vector<pair<int, int>>> m;
    int rowCount{}, w{};
    for (string line; getline(in, line);) {
        int l = line.length();
        w = l;
        for (int i = 0; i < l; ++i) {
            if (line[i] != '.') {
                m[line[i]].push_back({rowCount, i});
            }
        }
        rowCount++;
    }

    int h = rowCount; 
    set<pair<int, int>> antipodes;
    for (const auto& [freq, ant] : m) {
        int n = ant.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int x1 = ant[i].first; 
                int x2 = ant[j].first;
                int y1 = ant[i].second;
                int y2 = ant[j].second;
                int dX = abs(x1 - x2);
                int dY = abs(y1 - y2);

                int ax1{x1}, ay1{y1}, ax2{x2}, ay2{y2};
                auto valid = [&](int x, int y) {
                    return (x >= 0 && x < h && y >= 0 && y < w); 
                };
                if (x1 < x2 && y1 < y2) {
                    while (valid(ax1, ay1)) {
                        antipodes.insert({ax1, ay1});
                        ax1 -= dX;
                        ay1 -= dY;
                    }
                    while (valid(ax2, ay2)) {
                        antipodes.insert({ax2, ay2});
                        ax2 += dX;
                        ay2 += dY;
                    }
                } else if (x1 >= x2 && y1 <= y2) {
                    while (valid(ax1, ay1)) {
                        antipodes.insert({ax1, ay1});
                        ax1 += dX;
                        ay1 -= dY;
                    }
                    while (valid(ax2, ay2)) {
                        antipodes.insert({ax2, ay2});
                        ax2 -= dX;
                        ay2 += dY;
                    }
                } else if (x1 < x2 && y1 > y2) {
                    while (valid(ax1, ay1)) {
                        antipodes.insert({ax1, ay1});
                        ax1 -= dX;
                        ay1 += dY;
                    }
                    while (valid(ax2, ay2)) {
                        antipodes.insert({ax2, ay2});
                        ax2 += dX;
                        ay2 -= dY;
                    }
                } else if (x1 >= x2 && y1 >= y2) {
                    while (valid(ax1, ay1)) {
                        antipodes.insert({ax1, ay1});
                        ax1 += dX;
                        ay1 += dY;
                    }
                    while (valid(ax2, ay2)) {
                        antipodes.insert({ax2, ay2});
                        ax2 -= dX;
                        ay2 -= dY;
                    }
                }
            }
        }
    }
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) { 
            if (antipodes.count({i, j})) {
                cout << "# ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
    cout << "Result: " << antipodes.size() << endl;
    return 0;
}
