#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <queue>

using namespace std;

enum Cell {EMPTY, WALL};

vector<pair<int, int>> pairs;
const int mapSize = 71; 
const int initSize = 1024;

vector<vector<int>> parse() {
    vector<vector<int>> map(mapSize, vector<int>(mapSize, EMPTY));
    int count{};
    ifstream in("input18");
    if (!in) {
        cerr << "Input file not found!\n";
        return {};
    }
    for (string line; getline(in, line);) {
        stringstream ss(line);
        string pos;
        getline(ss, pos, ',');
        int x = stoi(pos);
        getline(ss, pos);
        int y = stoi(pos);
        if (count < initSize) {
            map[y][x] = WALL;
        }
        count++;
        pairs.push_back({y, x});
    }
    return map;
}

void draw(vector<vector<int>>& map) {
    int m = map.size();
    int n = map[0].size();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

vector<vector<int>> directions {
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};

void drawVisited(vector<vector<bool>>& v) {
    int m = v.size();
    int n = v[0].size();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int shortest(vector<vector<int>>& map) {
    int m = map.size();
    int n = map[0].size();

    vector<int> start{0, 0, 0};
    queue<vector<int>> q;
    q.push(start);
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    int count{};
    while (!q.empty()) {
        auto cell = q.front();
        q.pop();

        int x = cell[0];
        int y = cell[1];
        int d = cell[2];
        if (visited[x][y]) continue;
        visited[x][y] = true;

        if (x == m - 1 && y == n - 1) {
            return d;
        }
        for (auto& dir : directions) {
            int newX = x + dir[0];
            int newY = y + dir[1];
            if (newX >= 0 && newX < m && newY >= 0 && newY < n && map[newX][newY] == EMPTY && !visited[newX][newY]) {
                q.push({newX, newY, d + 1});
            }
        }
    }
    return -1;
}

int main() {
    auto map = parse();
    int total = pairs.size();
    for (int i = initSize; i < total; ++i) {
        int x = pairs[i].first;
        int y = pairs[i].second;
        map[x][y] = WALL;
        
        draw(map);
        if (shortest(map) == -1) {
            cout << "Result: " << y << "  " << x << endl;
            break;
        }
    }
    return 0;
}
