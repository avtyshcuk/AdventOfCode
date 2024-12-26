#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <climits>
#include <queue>

using namespace std;
enum Cell {EMPTY = 0, WALL};
enum Direction {LEFT = 0, RIGHT, UP, DOWN};

vector<pair<int, int>> directions{
    {0, -1},
    {0, 1},
    {-1, 0},
    {1, 0}
};

int getDir(pair<int, int> p) {
    for (int i = 0; i < 4; ++i) {
        if (p == directions[i]) return i;
    }
    return {};
}

struct State {
    int cost, x, y, direction;
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};


int findPath(vector<vector<int>>& maze, pair<int, int> start, pair<int, int> end) {
    int m = maze.size();
    int n = maze[0].size();
    
    priority_queue<State, vector<State>, greater<State>> q;
    vector<vector<vector<int>>> dist(n, vector<vector<int>>(m, vector<int>(4, INT_MAX)));
    vector<vector<vector<int>>> path(n, vector<vector<int>>(m, vector<int>(4, 0)));
    int x = start.first;
    int y = start.second;
    for (int i = 0; i < 4; ++i) {
        dist[x][y][i] = 0;
        path[x][y][i] = 1;
        if (i == 1) {
            q.push({0, x, y, i});
        } else {
            q.push({1000, x, y, i}); 
        }
    }

    while (!q.empty()) {
        auto [currCost, x, y, d] = q.top();
        q.pop();

        int dCount{};
        for (auto dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            if (maze[newX][newY] == EMPTY) {
                int cost = currCost + (d == getDir(dir) ? 1 : 1001);
                if (cost < dist[newX][newY][dCount]) {
                    dist[newX][newY][dCount] = cost;
                    path[newX][newY][dCount] = cost;
                    q.push({cost, newX, newY, dCount});
                } else if (cost == dist[newX][newY][dCount]) {
                    path[newX][newY][dCount] += path[x][y][d];
                }
            }
            dCount++;
        }
    }
    int bestCost{INT_MAX};
    for (int i = 0; i < 4; ++i) {
        bestCost = min(bestCost, dist[end.first][end.second][i]);
    }
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    queue<pair<int, int>> qq;
    int totalCells{};
    for (int i = 0; i < 4; ++i) {
        if (dist[end.first][end.second][i] == bestCost) {
            qq.push({end.first, end.second});
            visited[end.first][end.second] = true;
        }
    }
    set<pair<int, int>> s;
    while (!qq.empty()) {
        auto [x, y] = qq.front();
        qq.pop();
        if (!s.count({x, y})) {
            s.insert({x, y});
            totalCells++;
            cout << totalCells << endl;
        }
        for (int i = 0; i < 4; ++i) {
            int prevX = x - directions[i].first;
            int prevY = y - directions[i].second;
            if (maze[prevX][prevY] == EMPTY && !visited[prevX][prevY]) {
                for (int j = 0; j < 4; ++j) {
                    if (dist[prevX][prevY][j] + (j == i ? 1 : 1001) == dist[x][y][i]) {
                        visited[prevX][prevY] = true;
                        qq.push({prevX, prevY});
                    }
                }
            }
            
        }
    }

    return s.size();
}

void parse(vector<vector<int>>& maze, pair<int, int>& s, pair<int, int>& e) {
    fstream in("input16");
    if (!in) {
        cerr << "Input file not found!\n";
        return;
    }
    int rowCount{};
    for (string line; getline(in, line);) {
        vector<int> row;
        int colCount{};
        for (auto c : line) {
            if (c == '#') {
                row.push_back(WALL);
            } else {
                row.push_back(EMPTY);
                if (c == 'S') {
                    s = {rowCount, colCount}; 
                } else if (c == 'E') {
                    e = {rowCount, colCount}; 
                }
            }
            colCount++;
        }
        maze.push_back(row);
        rowCount++;
    }
}

void draw(vector<vector<int>>& maze) {
    int m = maze.size();
    int n = maze[0].size();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (maze[i][j] == EMPTY) {
                cout << ". ";
            } else if (maze[i][j] == WALL) { 
                cout << "# ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    vector<vector<int>> maze;
    pair<int, int> start{}, end{};
    parse(maze, start, end);
    set<pair<int, int>> visited;
    auto cost = findPath(maze, start, end);
    cout << "Result: " << cost << endl;
    return 0;
}
