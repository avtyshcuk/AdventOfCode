#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

std::vector<std::string> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    return lines;
}

int part1(const std::vector<std::string>& strings) {
    const std::unordered_set<char> vowels{'a', 'e', 'i', 'o', 'u'};
    int niceCount = 0;
    
    for (const auto& str : strings) {
        int vowelCount = 0;
        bool doubleFound = false;
        bool disallowedFound = false;
        
        for (size_t i = 0; i < str.size(); i++) {
            char current = str[i];
            
            // Check vowels
            if (vowels.count(current)) {
                vowelCount++;
            }
            
            if (i > 0) {
                char prev = str[i - 1];
                
                // Check double letter
                if (current == prev) {
                    doubleFound = true;
                }
                
                // Check disallowed pairs
                if ((prev == 'a' && current == 'b') ||
                    (prev == 'c' && current == 'd') ||
                    (prev == 'p' && current == 'q') ||
                    (prev == 'x' && current == 'y')) {
                    disallowedFound = true;
                }
            }
        }
        
        if (vowelCount >= 3 && doubleFound && !disallowedFound) {
            niceCount++;
        }
    }
    
    return niceCount;
}

int part2(const std::vector<std::string>& strings) {
    int niceCount = 0;
    
    for (const auto& str : strings) {
        std::unordered_map<std::string, std::vector<size_t>> pairs;
        bool differentDoublesFound = false;
        bool inBetweenDoublesFound = false;
        
        for (size_t i = 0; i < str.size(); i++) {
            char current = str[i];
            
            if (i > 0) {
                char prev = str[i - 1];
                std::string pair{prev, current};
                
                if (pairs.count(pair)) {
                    // Check if 4 different indices
                    std::unordered_set<size_t> indices;
                    indices.insert(pairs[pair][0]);
                    indices.insert(pairs[pair][1]);
                    indices.insert(i - 1);
                    indices.insert(i);
                    if (indices.size() == 4) {
                        differentDoublesFound = true;
                    }
                } else {
                    pairs[pair] = {i - 1, i};
                }
            }
            
            if (i > 1) {
                char prevPrev = str[i - 2];
                if (prevPrev == current) {
                    inBetweenDoublesFound = true;
                }
            }
        }
        
        if (differentDoublesFound && inBetweenDoublesFound) {
            niceCount++;
        }
    }
    
    return niceCount;
}

int main() {
    auto strings = parse("input05");
    std::cout << "Part 1: " << part1(strings) << std::endl;
    std::cout << "Part 2: " << part2(strings) << std::endl;
    return 0;
}
