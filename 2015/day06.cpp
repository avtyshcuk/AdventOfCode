#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

enum class Action { ON, OFF, TOGGLE };

struct Instruction {
    Action action;
    int top, left, bottom, right;
};

std::vector<Instruction> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<Instruction> instructions;
    std::string line;
    std::regex pattern(R"((turn on|turn off|toggle) (\d+),(\d+) through (\d+),(\d+))");
    
    while (std::getline(input, line)) {
        std::smatch match;
        if (std::regex_match(line, match, pattern)) {
            Instruction instr;
            
            std::string actionStr = match[1];
            if (actionStr == "turn on") {
                instr.action = Action::ON;
            } else if (actionStr == "turn off") {
                instr.action = Action::OFF;
            } else {
                instr.action = Action::TOGGLE;
            }
            
            instr.top = std::stoi(match[2]);
            instr.left = std::stoi(match[3]);
            instr.bottom = std::stoi(match[4]);
            instr.right = std::stoi(match[5]);
            
            instructions.push_back(instr);
        }
    }
    return instructions;
}

int part1(const std::vector<Instruction>& instructions) {
    std::vector<std::vector<int>> grid(1000, std::vector<int>(1000, 0));
    
    for (const auto& instr : instructions) {
        for (int row = instr.top; row <= instr.bottom; row++) {
            for (int col = instr.left; col <= instr.right; col++) {
                switch (instr.action) {
                    case Action::ON:
                        grid[row][col] = 1;
                        break;
                    case Action::OFF:
                        grid[row][col] = 0;
                        break;
                    case Action::TOGGLE:
                        grid[row][col] = grid[row][col] ? 0 : 1;
                        break;
                }
            }
        }
    }
    
    int count = 0;
    for (const auto& row : grid) {
        for (int cell : row) {
            count += cell;
        }
    }
    return count;
}

int part2(const std::vector<Instruction>& instructions) {
    std::vector<std::vector<int>> grid(1000, std::vector<int>(1000, 0));
    
    for (const auto& instr : instructions) {
        for (int row = instr.top; row <= instr.bottom; row++) {
            for (int col = instr.left; col <= instr.right; col++) {
                switch (instr.action) {
                    case Action::ON:
                        grid[row][col]++;
                        break;
                    case Action::OFF:
                        if (grid[row][col] > 0) {
                            grid[row][col]--;
                        }
                        break;
                    case Action::TOGGLE:
                        grid[row][col] += 2;
                        break;
                }
            }
        }
    }
    
    long long brightness = 0;
    for (const auto& row : grid) {
        for (int cell : row) {
            brightness += cell;
        }
    }
    return brightness;
}

int main() {
    auto instructions = parse("input06");
    std::cout << "Part 1: " << part1(instructions) << std::endl;
    std::cout << "Part 2: " << part2(instructions) << std::endl;
    return 0;
}
