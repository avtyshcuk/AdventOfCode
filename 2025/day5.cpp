#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct InputData {
    std::vector<std::pair<long long, long long>> freshIngredients;
    std::vector<long long> availableIngredients;
};

InputData parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    InputData data;
    std::string line;
    bool parsingRanges = true;

    while (std::getline(file, line)) {
        if (line.empty()) {
            parsingRanges = false;
            continue;
        }

        if (parsingRanges) {
            std::stringstream ss(line);
            long long from, to;
            char dash;
            ss >> from >> dash >> to;
            data.freshIngredients.push_back({from, to});
        } else {
            data.availableIngredients.push_back(std::stoll(line));
        }
    }

    return data;
}

std::vector<std::pair<long long, long long>> mergeRanges(const std::vector<std::pair<long long, long long>>& ranges) {
    if (ranges.empty()) {
        return {};
    }

    auto sortedRanges = ranges;
    std::sort(sortedRanges.begin(), sortedRanges.end());

    std::vector<std::pair<long long, long long>> mergedRanges;
    auto currentRange = sortedRanges[0];

    for (size_t i = 1; i < sortedRanges.size(); ++i) {
        const auto& range = sortedRanges[i];
        
        if (range.first <= currentRange.second) {
            currentRange.second = std::max(currentRange.second, range.second);
        } else {
            mergedRanges.push_back(currentRange);
            currentRange = range;
        }
    }
    
    mergedRanges.push_back(currentRange);
    return mergedRanges;
}

long long solvePart1(const InputData& input, const std::vector<std::pair<long long, long long>>& mergedRanges) {
    long long freshAvailableCount = 0;
    
    for (long long available : input.availableIngredients) {
        for (const auto& range : mergedRanges) {
            if (available >= range.first && available <= range.second) {
                freshAvailableCount++;
                break;
            }
        }
    }

    return freshAvailableCount;
}

long long solvePart2(const std::vector<std::pair<long long, long long>>& mergedRanges) {
    long long total = 0;

    for (const auto& range : mergedRanges) {
        total += range.second - range.first + 1;
    }

    return total;
}

int main() {
    try {
        auto input = parseInput("input5");
        auto mergedRanges = mergeRanges(input.freshIngredients);
        std::cout << solvePart1(input, mergedRanges) << '\n';
        std::cout << solvePart2(mergedRanges) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
