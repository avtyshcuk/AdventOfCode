#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

long long solvePart1(const std::vector<std::string>& input) {
    long long sum = 0;

    for (const auto& line : input) {
        int size = line.size();

        // First iteration: find max value and its position
        char maxLeft = '0';
        int maxPos = 0;
        for (int i = 0; i < size; ++i) {
            if (line[i] > maxLeft) {
                maxLeft = line[i];
                maxPos = i;
            }
        }

        // Second iteration: find maxRight
        char maxRight = '0';
        if (maxPos < size - 1) {
            // Max is not at rightmost, find max from maxPos+1 to end
            for (int i = maxPos + 1; i < size; ++i) {
                if (line[i] > maxRight) {
                    maxRight = line[i];
                }
            }
        } else {
            // Max is at rightmost, find max from 0 to last-1
            for (int i = 0; i < size - 1; ++i) {
                if (line[i] > maxRight) {
                    maxRight = line[i];
                }
            }
            std::swap(maxLeft, maxRight);
        }

        // Convert two chars to integer: e.g. '9' and '2' -> 92
        int value = (maxLeft - '0') * 10 + (maxRight - '0');
        sum += value;
    }

    return sum;
}

uint64_t solvePart2(const std::vector<std::string>& input) {
    uint64_t sum = 0;

    for (const auto& line : input) {
        int size = static_cast<int>(line.size());
        
        // Recursive lambda to find max digits
        // left: start of search range
        // remaining: how many digits we still need to find
        std::string result;
        
        std::function<void(int, int)> findMaxDigits = [&](int left, int remaining) {
            if (remaining == 0) return;
            
            // Search in range [left, size - remaining + 1)
            // This ensures we leave enough positions for remaining-1 digits after this one
            int right = size - remaining + 1;
            
            char maxChar = '0';
            int maxPos = left;
            for (int i = left; i < right; ++i) {
                if (line[i] > maxChar) {
                    maxChar = line[i];
                    maxPos = i;
                }
            }
            
            result += maxChar;
            findMaxDigits(maxPos + 1, remaining - 1);
        };
        
        findMaxDigits(0, 12);
        
        // Convert result string to uint64_t
        uint64_t value = 0;
        for (char c : result) {
            value = value * 10 + (c - '0');
        }
        
        sum += value;
    }

    return sum;
}

int main() {
    try {
        auto input = parseInput("input3");
        std::cout << solvePart1(input) << '\n';
        std::cout << solvePart2(input) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
