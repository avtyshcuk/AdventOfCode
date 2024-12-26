#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <unistd.h>

using namespace std;

struct Robot {int x{}; int y{}; int vx{}; int vy{}; };

int main() {
    fstream in("input14");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    regex pattern(R"(p=(-?\d+),(-?\d+)\s+v=(-?\d+),(-?\d+))");
    smatch matches;
    vector<Robot> robots;
    for (string line; getline(in, line);) {
        regex_search(line, matches, pattern); 
        int px = stoi(matches[1].str());
        int py = stoi(matches[2].str());
        int vx = stoi(matches[3].str());
        int vy = stoi(matches[4].str());
        Robot robot{px, py, vx, vy};
        robots.push_back(robot);
    }
    const int w = 101;
    const int h = 103;
    long long steps = 0;
    const int n = robots.size();
    for (;;) {
        for (int j = 0; j < n; ++j) {
            auto r = robots[j];
            int vx = r.vx;
            int vy = r.vy;
            int newX = r.x + r.vx;
            if (newX < 0) newX += w;
            if (newX >= w) newX -= w;
            int newY = r.y + r.vy;
            if (newY < 0) newY += h;
            if (newY >= h) newY -= h;
            robots[j] = Robot{newX, newY, vx, vy};
        }
        steps++;
        vector<vector<int>> field(h, vector<int>(w, 0));
        int stop{false};
        for (int i = 0; i < h; ++i) {
            int counter{};
            for (int j = 0; j < w; ++j) {
                for (int k = 0; k < n; ++k) {
                    if (robots[k].x == j && robots[k].y == i) {
                        field[i][j]++;
                    }
                }
                if (i > 0 && field[i - 1][j] > 0 && field[i][j] > 0) counter++;
                if (counter > 10) stop = true;
            }
        }

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                cout << field[i][j];
            }
            cout << endl;
        }
        if (stop) {
            break;
        }
    }
    cout << "Result: " << steps << endl;
    return 0;
}
