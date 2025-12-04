#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

// 8 directions: up, down, left, right, and 4 diagonals
const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

// Returns count of cells marked and updates 'next' matrix
long long markPaperCells(const Matrix& current, Matrix& next) {
    if (current.empty()) return 0;
    
    int rows = current.size();
    int cols = current[0].size();
    long long count = 0;
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (current[r][c] != 1) continue;
            
            int adjacentOnes = 0;
            
            for (int d = 0; d < 8; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    adjacentOnes += current[nr][nc];
                }
            }
            
            if (adjacentOnes < 4) {
                next[r][c] = 0;  // Mark as removed in the next matrix
                ++count;
            }
        }
    }
    
    return count;
}

Matrix parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    Matrix matrix;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row(line.size(), 0);
        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] == '@') {
                row[i] = 1;
            }
        }
        matrix.push_back(row);
    }
    return matrix;
}

long long solvePart1(const Matrix& input) {
    Matrix next = input;
    return markPaperCells(input, next);
}

long long solvePart2(const Matrix& input) {
    if (input.empty()) return 0;
    
    long long totalCount = 0;
    Matrix current = input;
    
    while (true) {
        Matrix next = current;
        long long countThisRound = markPaperCells(current, next);
        
        if (countThisRound == 0) {
            break;
        }
        
        totalCount += countThisRound;
        current = next;
    }
    
    return totalCount;
}

int main() {
    try {
        auto input = parseInput("input4");
        std::cout << solvePart1(input) << '\n';
        std::cout << solvePart2(input) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
