#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using Grid = std::vector<std::vector<int>>;
using Memo = std::unordered_map<long long, long long>;

Grid parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    Grid grid;
    std::string line;
    
    while (std::getline(file, line)) {
        std::vector<int> gridRow;
        
        for (char c : line) {
            if (c == 'S' || c == '^') {
                gridRow.push_back(1);
            } else {
                gridRow.push_back(0);
            }
        }
        
        if (!gridRow.empty()) {
            grid.push_back(gridRow);
        }
    }
    
    return grid;
}

long long solvePart1(const Grid& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }
    
    int splitCount = 0;
    auto width = grid[0].size();
    std::vector<int> currentBeamsPos = grid[0];
    
    for (size_t i = 1; i < grid.size(); i++) {
        std::vector<int> nextBeamsPos(width, 0);
        
        for (size_t j = 0; j < width; j++) {
            if (grid[i][j] == 0 && currentBeamsPos[j] == 1) {
                nextBeamsPos[j] = 1;
            } else if (grid[i][j] == 1 && currentBeamsPos[j] == 1) {
                splitCount++;
                
                if (j > 0) {
                    nextBeamsPos[j - 1] = 1;
                }
                
                if (j < width - 1) {
                    nextBeamsPos[j + 1] = 1;
                }
            }
        }
        
        currentBeamsPos = nextBeamsPos;
    }
    
    return splitCount;
}

long long beamJourney(const Grid& grid, std::pair<int, int> currPos, Memo& memo) {
    auto lastRow = grid.size() - 1;
    auto width = grid[0].size();
    auto key = currPos.first * 1000000LL + currPos.second;
    
    if (memo.count(key)) {
        return memo[key];
    }
    
    if (currPos.first == lastRow) {
        memo[key] = 1;
        return 1;
    }
    
    auto nextRow = currPos.first + 1;
    auto col = currPos.second;
    long long result = 0;
    
    if (grid[nextRow][col] == 1) {
        if (col - 1 >= 0) {
            result += beamJourney(grid, {nextRow, col - 1}, memo);
        }
        
        if (col + 1 < width) {
            result += beamJourney(grid, {nextRow, col + 1}, memo);
        }
    } else {
        result = beamJourney(grid, {nextRow, col}, memo);
    }
    
    memo[key] = result;
    return result;
}

long long solvePart2(const Grid& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }
    
    Memo memo;
    
    long long totalTimelines = 0;
    for (size_t col = 0; col < grid[0].size(); col++) {
        if (grid[0][col] == 1) {
            totalTimelines += beamJourney(grid, {0, col}, memo);
        }
    }
    
    return totalTimelines;
}

int main() {
    try {
        auto grid = parseInput("input7");
        std::cout << solvePart1(grid) << '\n';
        std::cout << solvePart2(grid) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
