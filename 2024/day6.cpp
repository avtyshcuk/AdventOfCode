#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <set>

using namespace std;

enum Direction {NORTH = 0, EAST, SOUTH, WEST};

Direction changeDirection(Direction current) {
    switch (current) {
        case Direction::NORTH: return Direction::EAST;
        case Direction::EAST: return Direction::SOUTH;
        case Direction::SOUTH: return Direction::WEST;
        case Direction::WEST: return Direction::NORTH;
    }
    return Direction::NORTH;
}

void printDirection(Direction current) {
    switch (current) {
        case Direction::NORTH: cout << "NORTH"; break;
        case Direction::EAST: cout << "EAST"; break; 
        case Direction::SOUTH: cout << "SOUTH"; break;
        case Direction::WEST: cout << "WEST"; break;
    }
}

pair<int, int> move(pair<int, int> curr, Direction dir) {
    switch (dir) {
        case Direction::NORTH: return {curr.first - 1, curr.second};
        case Direction::EAST: return {curr.first, curr.second + 1};
        case Direction::SOUTH: return {curr.first + 1, curr.second};
        case Direction::WEST: return {curr.first, curr.second - 1};
    }
    return curr;
}

int main() {
    fstream in("input6");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }

    vector<vector<char>> field;
    for (string line; getline(in, line);) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        field.push_back(row);
    }
    int m = field.size();
    int n = field[0].size();
    pair<int, int> start;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (field[i][j] == '^') {
                start = {i, j};
                break;
            }
        }
    }
    vector<vector<char>> origField{field};
    Direction dir{Direction::NORTH};
    pair<int, int> pos{start};
    while (true) {
        field[pos.first][pos.second] = '*';
        auto nextPos = move(pos, dir);
        int i = nextPos.first;
        int j = nextPos.second;
        if (i < 0 || i >= m || j < 0 || j >= n) break; 
        if (field[i][j] == '#') {
            dir = changeDirection(dir);
        } else {
            pos = nextPos;
        }
    }
    int result{};
    vector<pair<int, int>> path;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == start.first && j == start.second) continue;
            if (field[i][j] == '*') {
                path.push_back({i, j});
            }
        }
    }
    for (int i = 0; i < path.size(); ++i) {
        field = origField;
        int xObs = path[i].first;
        int yObs = path[i].second;
        field[xObs][yObs] = '#';

        Direction dir{Direction::NORTH};
        pair<int, int> pos{start};
        set<vector<int>> visited;
        while (true) {
            visited.insert({pos.first, pos.second, dir});
            auto nextPos = move(pos, dir);
            int i = nextPos.first;
            int j = nextPos.second;
            if (i < 0 || i >= m || j < 0 || j >= n) break; 
            if (field[i][j] == '#') {
                dir = changeDirection(dir);
            } else {
                pos = nextPos;
            }
            if (visited.count({i, j, dir})) {
                result++;
                break;
            }
        }
    }
    cout << "Result: " << result << endl;
    return 0;
}
