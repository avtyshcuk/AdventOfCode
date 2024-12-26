#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>
#include <map>
#include <cmath>

using namespace std;

enum Cell {WALL = 0, EMPTY};

using P = pair<int, int>;
vector<vector<int>> dirs {
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};

void draw(vector<vector<int>>& maze, P s = {}, P e = {}) {
    int m = maze.size();
    int n = maze[0].size();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (s.first == i && s.second == j) {
                cout << "S ";
            } else if (e.first == i && e.second == j) {
                cout << "E ";
            } else if (maze[i][j] == WALL) {
                cout << "# ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void path(vector<vector<int>>& maze, P start, P end, vector<vector<pair<int, int>>>& prev) {
    int n = maze.size();
    int m = maze[0].size();
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    queue<P> q;
    q.push(start);

    while (!q.empty()) {
        auto point = q.front();
        q.pop();
        if (point == end) {
            return;
        }
        for (const auto& dir : dirs) {
            int x = dir[0];
            int y = dir[1];
            int newX = point.first + x;
            int newY = point.second + y;
            if (maze[newX][newY] != WALL && !visited[newX][newY]) {
                visited[newX][newY] = true;
                prev[newX][newY] = {point.first, point.second};
                q.push({newX, newY});
            }
        }
    }
}

int main() {
    fstream in("input20");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    vector<vector<int>> maze;
    P start, end;
    int rowCount{};
    for (string line; getline(in, line);) {
        int colCount{};
        vector<int> row;
        for (auto c : line) {
            row.push_back(c == '#' ? WALL : EMPTY);
            if (c == 'S') {
                start = {rowCount, colCount};
            } else if (c == 'E') {
                end = {rowCount, colCount};
            }
            colCount++;
        }
        rowCount++;
        maze.push_back(row);
    }
    draw(maze, start, end);

    int n = maze.size();
    int m = maze[0].size();
    vector<vector<P>> prev(n, vector<P>(m, pair{-1, -1}));
    path(maze, start, end, prev);

    vector<P> shortest{end};
    while (shortest.back() != start) {
        int i = shortest.back().first;
        int j = shortest.back().second;
        shortest.push_back(prev[i][j]);
    }

    int profit{};
    for (int i = 0; i < shortest.size(); ++i) {
        P x = shortest[i];
        for (int j = i + 100; j < shortest.size(); ++j) {
            P y = shortest[j];
            int manhattan = abs(x.first - y.first) + abs(x.second - y.second);
            if (j - i - manhattan < 100) continue;
            if (manhattan <= 20) profit++;
        }
    }
    cout << "Result: " << profit << endl;
    return 0;
}
