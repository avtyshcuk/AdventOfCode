#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int countXmas(vector<vector<char>>& m, int xPos, int yPos) {
    int x = m.size();
    int y = m[0].size();
    if (xPos + 2 >= x || yPos + 2 >= y) return 0;

    vector<vector<vector<char>>> patterns{
        {{'M', '.', 'M'}, {'.','A','.'}, {'S', '.', 'S'}},
        {{'S','.','S'},{'.','A','.'},{'M', '.','M'}},
        {{'S', '.', 'M'},{'.','A','.'},{'S', '.', 'M'}},
        {{'M', '.', 'S'},{'.', 'A', '.'},{'M', '.', 'S'}},
    };

    int result{};
    for (auto pattern : patterns) {
        int success{true};
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (pattern[i][j] != '.' && pattern[i][j] != m[xPos + i][yPos + j]) {
                    success = false;
                }
            }
        }
        if (success) {
            result++;
        }
    }
    return result;
}

int main() {
    fstream in("input4");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }

    vector<vector<char>> m;
    for (string line; getline(in, line);) {
        vector<char> l;
        for (char c : line) {
            l.push_back(c);
        }
        m.push_back(l);
    }

    int x = m.size();
    int y = m[0].size();
    int result{};
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            result += countXmas(m, i, j);
        }
    }
    cout << result << endl;
    return 0;
}
