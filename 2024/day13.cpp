#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

struct Target { long long x; long long y; };
struct Button {long long x; long long y; };
struct State { long long currX; long long currY; int stepA; int stepB; }; 

long long play(State state, Button a, Button b, Target target, vector<vector<int>>& memo) {
    long long px = target.x;
    long long py = target.y;
    long long det = a.x*b.y - a.y*b.x;
    long long n = (px*b.y - py*b.x)/det; 
    long long m = (py*a.x - px*a.y)/det; 

    if (a.x*n + b.x*m == px && a.y*n + b.y*m == py) return 3*n + m;
    return -1;
}

int main() {
    fstream in("input13");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }

    long long tokens{};
    for (string line; getline(in, line);) {
        string_view sv{line};
        long long xA, yA, xB, yB, prizeX, prizeY;
        if (sv.starts_with("Button A:")) {
            regex rx(R"([-+]?\d+)");
            auto it = sregex_iterator(line.begin(), line.end(), rx);
            xA = stoi((*it).str());
            it++;
            yA = stoi((*it).str());
        } else if (sv.starts_with("Button B:")) {
            regex rx(R"([-+]?\d+)");
            auto it = sregex_iterator(line.begin(), line.end(), rx);
            xB = stoi((*it).str());
            it++;
            yB = stoi((*it).str());
        } else if (sv.starts_with("Prize:")) {
            regex rx(R"([-+]?\d+)");
            auto it = sregex_iterator(line.begin(), line.end(), rx);
            prizeX = 10000000000000ll + stoll((*it).str());
            it++;
            prizeY = 10000000000000ll + stoll((*it).str());


            Target target{prizeX, prizeY};
            Button a{xA, yA};
            Button b{xB, yB}; 
            State state{0, 0, 0, 0};

            vector<vector<int>> memo(101, vector<int>(101, -1));
            auto result = play(state, a, b, target, memo);
            if (result != -1) {
                tokens += result;
            }
        }
    }
    cout << "Result: " << tokens << endl;
    return 0;
}
