#include <iostream>
#include <string>

std::string lookAndSay(const std::string& input) {
    std::string result;
    size_t i = 0;
    
    while (i < input.length()) {
        char current = input[i];
        int count = 0;
        
        while (i < input.length() && input[i] == current) {
            ++count;
            ++i;
        }
        
        result += std::to_string(count) + current;
    }
    
    return result;
}

int part1(const std::string& input, int iterations) {
    std::string current = input;
    
    for (int i = 0; i < iterations; ++i) {
        current = lookAndSay(current);
    }
    
    return current.length();
}

int part2(const std::string& input, int iterations) {
    return part1(input, iterations);
}

int main() {
    std::string input = "3113322113";
    std::cout << "Part 1: " << part1(input, 40) << std::endl;
    std::cout << "Part 2: " << part2(input, 50) << std::endl;
    return 0;
}
