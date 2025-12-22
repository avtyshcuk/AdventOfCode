#include <fstream>
#include <iostream>
#include <string>

struct Input {
    int row;
    int col;
};

Input parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    Input data{};
    std::string line;
    std::getline(input, line);
    
    size_t rowPos = line.find("row ");
    size_t colPos = line.find("column ");
    
    if (rowPos != std::string::npos) {
        data.row = std::stoi(line.substr(rowPos + 4));
    }
    if (colPos != std::string::npos) {
        data.col = std::stoi(line.substr(colPos + 7));
    }
    
    return data;
}

long long part1(const Input& input) {
    // Grid is filled diagonally:
    // (1,1)=1, (2,1)=2, (1,2)=3, (3,1)=4, (2,2)=5, (1,3)=6, ...
    //
    // Position at (row, col) is on diagonal (row + col - 1)
    // Number at start of diagonal d is: 1 + 2 + ... + (d-1) + 1 = d*(d-1)/2 + 1
    // Position within diagonal is col
    // So index = d*(d-1)/2 + col where d = row + col - 1
    
    int d = input.row + input.col - 1;
    long long index = (long long)d * (d - 1) / 2 + input.col;
    
    // Code generation:
    // Start: 20151125
    // Next: (prev * 252533) % 33554393
    
    const long long START = 20151125;
    const long long MUL = 252533;
    const long long MOD = 33554393;
    
    long long code = START;
    for (long long i = 1; i < index; i++) {
        code = (code * MUL) % MOD;
    }
    
    return code;
}

int main() {
    auto input = parse("input25");
    
    auto answer1 = part1(input);
    std::cout << "Part 1: " << answer1 << "\n";
    
    return 0;
}
