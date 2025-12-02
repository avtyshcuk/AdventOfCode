#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Range = std::pair<long long, long long>;

std::vector<Range> parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    std::getline(file, line);

    std::vector<Range> ranges;
    std::stringstream ss{line};
    std::string token;

    while (std::getline(ss, token, ',')) {
        std::stringstream pairStream{token};
        long long num1, num2;
        char dash;
        pairStream >> num1 >> dash >> num2;
        ranges.emplace_back(num1, num2);
    }
    return ranges;
}

bool isInvalidId(long long num) {
    std::string s = std::to_string(num);
    size_t len = s.length();
    if (len % 2 != 0) {
        return false;
    }
    std::string firstHalf = s.substr(0, len / 2);
    std::string secondHalf = s.substr(len / 2);
    return firstHalf == secondHalf;
}

long long solvePart1(const std::vector<Range>& input) {
    long long sum = 0;
    for (const auto& [start, end] : input) {
        for (long long num = start; num <= end; ++num) {
            if (isInvalidId(num)) {
                sum += num;
            }
        }
    }
    return sum;
}

bool isInvalidIdPart2(long long num) {
    std::string s = std::to_string(num);
    size_t len = s.length();

    // Check all divisors starting from len down to 1
    for (size_t partSize = len; partSize >= 1; --partSize) {
        if (len % partSize != 0) {
            continue;
        }
        std::string pattern = s.substr(0, partSize);
        bool allMatch = true;
        for (size_t i = partSize; i < len; i += partSize) {
            if (s.substr(i, partSize) != pattern) {
                allMatch = false;
                break;
            }
        }
        if (allMatch && partSize < len) {
            return true;
        }
    }
    return false;
}

long long solvePart2(const std::vector<Range>& input) {
    long long sum = 0;
    for (const auto& [start, end] : input) {
        for (long long num = start; num <= end; ++num) {
            if (isInvalidIdPart2(num)) {
                sum += num;
            }
        }
    }
    return sum;
}

int main() {
    try {
        auto input = parseInput("input2");
        std::cout << solvePart1(input) << '\n';
        std::cout << solvePart2(input) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
