#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::vector<bool>> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<std::vector<bool>> lights;
    std::string line;
    
    while (std::getline(input, line)) {
        std::vector<bool> row;
        for (char c : line) {
            row.push_back(c == '#');
        }
        lights.push_back(row);
    }
    
    return lights;
}

void printLights(const std::vector<std::vector<bool>>& lights) {
    for (const auto& row : lights) {
        for (bool light : row) {
            std::cout << (light ? '#' : '.');
        }
        std::cout << "\n";
    }
}

int countOnNeighbors(const std::vector<std::vector<bool>>& lights, int row, int col) {
    int count = 0;
    int rows = lights.size();
    int cols = lights[0].size();
    
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            
            int newRow = row + dr;
            int newCol = col + dc;
            
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                if (lights[newRow][newCol]) {
                    count++;
                }
            }
        }
    }
    
    return count;
}

std::vector<std::vector<bool>> simulateStep(const std::vector<std::vector<bool>>& lights) {
    int rows = lights.size();
    int cols = lights[0].size();
    std::vector<std::vector<bool>> newLights = lights;
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int onNeighbors = countOnNeighbors(lights, r, c);
            
            if (lights[r][c]) {
                newLights[r][c] = (onNeighbors == 2 || onNeighbors == 3);
            } else {
                newLights[r][c] = (onNeighbors == 3);
            }
        }
    }
    
    return newLights;
}

int countOnLights(const std::vector<std::vector<bool>>& lights) {
    int count = 0;
    for (const auto& row : lights) {
        for (bool light : row) {
            if (light) count++;
        }
    }
    return count;
}

int part1(std::vector<std::vector<bool>> lights) {
    const int STEPS = 4;
    
    for (int step = 0; step < STEPS; ++step) {
        lights = simulateStep(lights);
    }
    
    return countOnLights(lights);
}

void turnOnCorners(std::vector<std::vector<bool>>& lights) {
    int rows = lights.size();
    int cols = lights[0].size();
    
    lights[0][0] = true;
    lights[0][cols - 1] = true;
    lights[rows - 1][0] = true;
    lights[rows - 1][cols - 1] = true;
}

int part2(std::vector<std::vector<bool>> lights) {
    const int STEPS = 5;
    
    turnOnCorners(lights);
    
    for (int step = 0; step < STEPS; ++step) {
        lights = simulateStep(lights);
        turnOnCorners(lights);
    }
    
    return countOnLights(lights);
}

int main() {
    try {
        auto lights = parse("input18");
        
        std::cout << "Initial state (" << lights.size() << "x" << lights[0].size() << "):\n";
        printLights(lights);
        std::cout << "\n";
        
        auto answer1 = part1(lights);
        std::cout << "Part 1: " << answer1 << "\n";
        
        auto answer2 = part2(lights);
        std::cout << "Part 2: " << answer2 << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
