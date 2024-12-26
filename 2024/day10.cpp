#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

vector<vector<int>> dirs{{1, 0},{-1, 0},{0, 1},{0, -1}};

int dfs(vector<vector<int>>& m, int i, int j) {
    if (m[i][j] == 9) return 1;
    int score{};
    for (const auto& dir : dirs) {
        int newX = i + dir[0];
        int newY = j + dir[1];
        if (newX >= 0 && newX < m.size() && newY >= 0 && newY < m[0].size() && m[newX][newY] - m[i][j] == 1) {
            score += dfs(m, newX, newY);
        }
    }
    return score;
}

int main() {
    fstream in("input10");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }

    vector<vector<int>> m;
    for (string line; getline(in, line);) {
        vector<int> row;
        for (char c : line) {
            row.push_back(c - '0');
        }
        m.push_back(row); 
    }
    int h = m.size();
    int w = m[0].size();

    int score{};
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (m[i][j] == 0) {
                score += dfs(m, i, j);
                cout << score << endl;
            }
        }
    }
    cout << "Result: " << score << endl;
    return 0;
}
