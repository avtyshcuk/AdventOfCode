#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;

vector<vector<int>> dirs { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

bool perimeter(vector<vector<char>>& region, int i, int j) {
    int m = region.size();
    int n = region[0].size();
    for (auto& dir : dirs) {
        int newI = i + dir[0];
        int newJ = j + dir[1];
        if (newI < 0 || newI >= m || newJ < 0 || newJ >= n || region[i][j] != region[newI][newJ]) {
            return true;
        }
    }
    return false;
}

void findPrice(vector<vector<char>>& region, int i, int j, pair<int, int> &p, vector<vector<bool>>& visited, vector<vector<char>> &local) {
    p.first++;
    visited[i][j] = true;
    local[i][j] = region[i][j];
    int m = region.size();
    int n = region[0].size();
    for (auto& dir : dirs) {
        int newI = i + dir[0];
        int newJ = j + dir[1];
        if (newI >= 0 && newI < m && newJ >= 0 && newJ < n && !visited[newI][newJ] && region[i][j] == region[newI][newJ]) { 
            findPrice(region, newI, newJ, p, visited, local);
        }
    }
}

int findPer(vector<vector<char>>& region, char target) {
    int m = region.size();
    int n = region[0].size();
    int total{};
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) { 
            while (j < n && region[i][j] != target) {
                ++j;
            }
            int start{j}, end{j};
            bool wasFound{false};
            while (j < n && region[i][j] == target) {
                ++j; 
                end++;
            }

            char top{i == 0 ? 'x' : region[i - 1][start]};
            char bottom{i == m - 1 ? 'x' : region[i + 1][start]};
            if (top == '*') total++;
            if (bottom == '*') total++;
            if (i == 0 && start + 1 == end) total++;
            if (i == m - 1 && start + 1 == end) total++;
            for (int k = start + 1; k < end; ++k) {
                if (i != 0 && top == target && region[i - 1][k] == '*') total++;
                else if (i == 0 && top == 'x') total++;
                top = i == 0 ? '*' : region[i - 1][k];

                if (bottom != m - 1 && bottom == target && region[i + 1][k] == '*') total++;
                else if (i == m - 1 && bottom == 'x') total++;
                bottom = i == m - 1 ? '*' : region[i + 1][k];
            }
        }
    }
    return total;
}

vector<vector<char>> transpose(vector<vector<char>>& local) {
    int m = local.size();
    int n = local[0].size();
    vector<vector<char>> newMatrix(n, vector<char>(m));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            newMatrix[j][i] = local[i][j];
        }
    }
    return newMatrix;
}

int main() {
    fstream in("input12");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }

    vector<vector<char>> region;
    int rowCount{}, colCount{};
    for (string line{}; getline(in, line);) {
        colCount = line.length();
        vector<char> row;
        for (auto c : line) {
            row.push_back(c);
        }
        region.push_back(row);
        rowCount++;
    }
    vector<vector<bool>> visited(rowCount, vector<bool>(colCount, false));
    int price{};
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < colCount; ++j) {
            if (!visited[i][j]) {
                pair<int, int> p{};
                vector<vector<char>> local(rowCount, vector<char>(colCount, '*'));
                findPrice(region, i, j, p, visited, local); 
                int perimeter = findPer(local, local[i][j]);
                local = transpose(local);
                perimeter += findPer(local, region[i][j]);
                price += p.first * perimeter;
            }
        }
    }
    cout << "Result: " << price << endl;
    return 0;
}
