#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>
#include <map>

using namespace std;

using P = pair<int, int>;

const int depth = 27;

vector<vector<char>> numpad {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {'x', '0', 'A'}
};

vector<vector<char>> keypad {
    {'x', '^', 'A'},
    {'<', 'v', '>'}
};

bool isValidNumPad(int x, int y) {
    int n = numpad.size();
    int m = numpad[0].size();
    if (x == 3 && y == 0) return false;
    if (x >= 0 && x < n && y >= 0 && y < m) return true; 
    return false;
}

bool isValidKeyPad(int x, int y) {
    int n = keypad.size();
    int m = keypad[0].size();
    if (x == 0 && y == 0) return false;
    if (x >= 0 && x < n && y >= 0 && y < m) return true;
    return false;
}

pair<int, int> posNumPad(char c) {
    int m = numpad.size();
    int n = numpad[0].size();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (c == numpad[i][j]) {
                return {i, j};
            }
        }
    }
    return {};
}

pair<int, int> posKeyPad(char c) {
    int m = keypad.size();
    int n = keypad[0].size();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (c == keypad[i][j]) {
                return {i, j};
            }
        }
    }
    return {};
}

vector<string> generateNumPadPath(P s, P e) {
    if (!isValidNumPad(s.first, s.second)) {
        return {};
    }
    if (s == e) return {"A"};
    vector<string> result;
    if (e.first < s.first) {
        for (const auto& path : generateNumPadPath(pair{s.first - 1, s.second}, e)) {
            result.push_back(path + '^');
        }
    }
    if (e.first > s.first) {
        for (const auto& path : generateNumPadPath(pair{s.first + 1, s.second}, e)) {
            result.push_back(path + 'v');
        }
    }
    if (e.second < s.second) {
        for (const auto& path : generateNumPadPath(pair{s.first, s.second - 1}, e)) {
            result.push_back(path + '<');
        }
    }
    if (e.second > s.second) {
        for (const auto& path : generateNumPadPath(pair{s.first, s.second + 1}, e)) {
            result.push_back(path + '>');
        }
    }
    return result; 
}

vector<string> generateKeyPadPath(P s, P e) {
    if (!isValidKeyPad(s.first, s.second)) {
        return {};
    }
    if (s == e) return {"A"};
    vector<string> result;
    if (e.first < s.first) {
        for (const auto& path : generateKeyPadPath(pair{s.first - 1, s.second}, e)) {
            result.push_back(path + '^');
        }
    }
    if (e.first > s.first) {
        for (const auto& path : generateKeyPadPath(pair{s.first + 1, s.second}, e)) {
            result.push_back(path + 'v');
        }
    }
    if (e.second < s.second) {
        for (const auto& path : generateKeyPadPath(pair{s.first, s.second - 1}, e)) {
            result.push_back(path + '<');
        }
    }
    if (e.second > s.second) {
        for (const auto& path : generateKeyPadPath(pair{s.first, s.second + 1}, e)) {
            result.push_back(path + '>');
        }
    }
    return result; 
}

map<pair<pair<char, char>, int>, long long> memo;

long long findPath(char s, char e, char l) {
    if (l == depth - 1) {
        return 1;
    }
    auto key = pair{pair{s, e}, l};
    if (memo.count(key)) return memo[key];

    long long length{-1};
    vector<string> paths;
    if (l == 0) {
        paths = generateNumPadPath(posNumPad(s), posNumPad(e));
    } else {
        paths = generateKeyPadPath(posKeyPad(s), posKeyPad(e));
    }
    for (auto& path : paths) {
        reverse(path.begin(), path.end());
        cout << path << endl;
    }
    for (const string& path : paths) {
        long long currLen{};
        char start{'A'};
        for (char c : path) {
            currLen += findPath(start, c, l + 1);
            start = c;
        }
        if (length == -1 || currLen < length) {
            length = currLen;
        }
    }
    return memo[key] = length;
}

int main() {
    fstream in("input21");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    vector<string> codes;
    for (string line; getline(in, line);) {
        codes.push_back(line);
    }
    long long result{};
    for (const auto& code : codes) {
        long long length{};
        char start{'A'};
        for (char c : code) {
            length += findPath(start, c, 0);
            start = c;
        }
        int digitPart = stoi(code.substr(0, 3));
        result += digitPart * length;
    }
    cout << "Result: " << result << endl;
    return 0;
}
