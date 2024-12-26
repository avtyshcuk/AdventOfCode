#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>

using namespace std;
void draw(const vector<vector<int>>& map);
pair<int, int> findRobot(vector<vector<int>>& map);

enum Cell {EMPTY = 0, WALL, LEFT_BOX, RIGHT_BOX, ROBOT};
enum Command {LEFT = 0, RIGHT, UP, DOWN};

bool isWall(vector<vector<int>>& map, pair<int, int> p) { int i = p.first; int j = p.second; return map[i][j] == WALL; }
bool isLeftBox(vector<vector<int>>& map, pair<int, int> p) { int i = p.first; int j = p.second; return map[i][j] == LEFT_BOX; }
bool isRightBox(vector<vector<int>>& map, pair<int, int> p) { int i = p.first; int j = p.second; return map[i][j] == RIGHT_BOX; }
bool isEmpty(vector<vector<int>>& map, pair<int, int> p) { int i = p.first; int j = p.second; return map[i][j] == EMPTY; }

pair<int, int> next(pair<int, int> p, int c) {
    switch (c) {
        case LEFT: p.second--; break;
        case RIGHT: p.second++; break;
        case UP: p.first--; break;
        case DOWN: p.first++; break;
    }
    return p;
}

bool canMove(vector<vector<int>>& map, pair<int, int> p, int c, vector<pair<int, int>>& toMove) {
    int i = p.first;
    int j = p.second;
    auto nextPos = next(p, c);
    if (isWall(map, nextPos)) return false;

    toMove.push_back(p);
    if (isEmpty(map, nextPos)) {
        return true;
    }
    if (c == LEFT || c == RIGHT) {
        return canMove(map, nextPos, c, toMove);
    } else {
        pair<int, int> secondPos;
        if (isLeftBox(map, nextPos)) {
            secondPos = {nextPos.first, nextPos.second + 1};
        } else {
            secondPos = {nextPos.first, nextPos.second - 1};
        }
        return canMove(map, nextPos, c, toMove) && canMove(map, secondPos, c, toMove);
    }
    return false;
}

void removeDuplicates(vector<pair<int, int>>& toMove) {
    set<pair<int, int>> s;
    vector<pair<int, int>> noDups;
    for (auto it = toMove.rbegin(); it != toMove.rend(); ++it) {
        auto p = *it;
        if (!s.count(p)) {
            noDups.push_back(p);
        }
        s.insert(p);
    }
    toMove = noDups;
}

void shift(vector<vector<int>>& map, vector<pair<int, int>>& toMove, int c) {
    removeDuplicates(toMove);
    for (auto it = toMove.begin(); it != toMove.end(); ++it) {
        auto p = *it;
        int i = p.first;
        int j = p.second;
        switch (c) {
            case LEFT: swap(map[i][j - 1], map[i][j]); break;
            case RIGHT: swap(map[i][j + 1], map[i][j]); break;
            case UP: swap(map[i][j], map[i - 1][j]); break;
            case DOWN: swap(map[i][j], map[i + 1][j]); break;
        }
    }
}

void move(vector<vector<int>>&map, pair<int, int> &p, int c) {
    vector<pair<int, int>> toMove;
    if (canMove(map, p, c, toMove)) {
        shift(map, toMove, c);
        p = findRobot(map);
    }
}

void simulate(vector<vector<int>>& map, pair<int, int> pos, vector<int>& commands) {
    for (int i = 0; i < commands.size(); ++i) {
        move(map, pos, commands[i]);
        draw(map);
    }
}

long long findGPS(vector<vector<int>>& map) {
    int h = map.size();
    int w = map[0].size();

    long long gps{};
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (map[i][j] == LEFT_BOX) {
                gps += i*100 + j; 
            }
        }
    }
    return gps;
}

void parse(vector<vector<int>>& map, vector<int>& commands) {
    fstream in("input15");
    bool mapRead{false};
    for (string line; getline(in, line);) {
        if (line.empty()) {
            mapRead = true;
            continue;
        }
        if (!mapRead) {
            vector<int> row;
            for (char c : line) {
               switch (c) {
                   case '#' : row.push_back(WALL); row.push_back(WALL); break;
                   case 'O' : row.push_back(LEFT_BOX); row.push_back(RIGHT_BOX); break;
                   case '@' : row.push_back(ROBOT); row.push_back(EMPTY); break;
                   default : row.push_back(EMPTY); row.push_back(EMPTY); break;
               }
            }
            map.push_back(row);
        } else {
            for (char c : line) {
                switch (c) {
                   case '<' : commands.push_back(LEFT); break;
                   case '>' : commands.push_back(RIGHT); break;
                   case '^' : commands.push_back(UP); break;
                   case 'v' : commands.push_back(DOWN); break;
                }
            }
        }
    }
}

pair<int, int> findRobot(vector<vector<int>>& map) {
    int h = map.size();
    int w = map[0].size();
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (map[i][j] == ROBOT) {
                return {i, j};
            }
        }
    }
    return {};
}

void draw(const vector<vector<int>>& map) {
    int h = map.size();
    int w = map[0].size();
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            char c{};
            switch (map[i][j]) {
                case 0: c = '.'; break;
                case 1: c = '#'; break;
                case 2: c = '['; break;
                case 3: c = ']'; break;
                case 4: c = '@'; break;
            }
            cout << c << "  ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    vector<vector<int>> map;
    vector<int> commands;
    parse(map, commands);
    draw(map);
    pair<int, int> pos{findRobot(map)};
    simulate(map, pos, commands);
    auto gps = findGPS(map);
    cout << "Result: " << gps << endl;
    return 0;
}
