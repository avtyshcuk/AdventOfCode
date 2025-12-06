#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

enum class Operation {
    PLUS,
    MULTIPLY
};

struct InputData {
    std::vector<std::vector<long long>> matrix;
    std::vector<Operation> operations;
};

InputData parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    InputData data;
    std::string line;
    std::vector<std::string> lines;
    
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    for (size_t i = 0; i < lines.size() - 1; i++) {
        std::vector<long long> row;
        std::stringstream ss(lines[i]);
        long long num;
        
        while (ss >> num) {
            row.push_back(num);
        }
        
        if (!row.empty()) {
            data.matrix.push_back(row);
        }
    }
    
    if (!lines.empty()) {
        std::string lastLine = lines.back();
        for (char c : lastLine) {
            if (c == '*') {
                data.operations.push_back(Operation::MULTIPLY);
            } else if (c == '+') {
                data.operations.push_back(Operation::PLUS);
            }
        }
    }
    
    return data;
}

long long solvePart1(const InputData& input) {
    long long totalSum = 0;
    
    for (size_t i = 0; i < input.operations.size(); i++) {
        long long columnResult = 0;
        bool first = true;
        
        for (const auto& row : input.matrix) {
            if (i < row.size()) {
                if (first) {
                    columnResult = row[i];
                    first = false;
                } else {
                    if (input.operations[i] == Operation::PLUS) {
                        columnResult += row[i];
                    } else { // Operation::MULTIPLY
                        columnResult *= row[i];
                    }
                }
            }
        }
        
        totalSum += columnResult;
    }
    
    return totalSum;
}

std::vector<std::string> parseInputPart2(const std::string& filename) {
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

long long solvePart2(const std::vector<std::string>& lines) {
    if (lines.empty()) {
        return 0;
    }
    
    long long total = 0;
    const std::string& opsLine = lines.back();
    Operation currentOp{};
    Operation prevOp{};
    long long localTotal = 0;
    bool firstInLocal = true;
    bool firstEver = true;
    
    // Iterate over last line (operations)
    for (size_t i = 0; i < opsLine.size(); i++) {
        char opChar = opsLine[i];
        
        // Check if we encounter an operation character
        if (opChar == '+' || opChar == '*') {
            Operation newOp = (opChar == '+') ? Operation::PLUS : Operation::MULTIPLY;
            
            // Add local total to final total and reset (whenever we see an operation)
            if (!firstEver) {
                total += localTotal;
                std::cout << "Op at column " << i << ", adding localTotal: " << localTotal << " (total now: " << total << ")\n";
                localTotal = 0;
                firstInLocal = true;
            }
            
            currentOp = newOp;
            firstEver = false;
        }
        
        long long currValue = 0;
        bool buildingNumber = false;
        
        for (size_t lineIdx = 0; lineIdx < lines.size() - 1; lineIdx++) {
            if (i < lines[lineIdx].size() && lines[lineIdx][i] != ' ') {
                char c = lines[lineIdx][i];
                
                if (std::isdigit(c)) {
                    long long digitValue = c - '0';
                    currValue = currValue * 10 + digitValue;
                    buildingNumber = true;
                }
            } else {
                // Empty char - finish current number and apply operation to local total
                if (buildingNumber) {
                    if (firstInLocal) {
                        localTotal = currValue;
                        firstInLocal = false;
                    } else {
                        if (currentOp == Operation::PLUS) {
                            localTotal += currValue;
                        } else { // Operation::MULTIPLY
                            localTotal *= currValue;
                        }
                    }
                    std::cout << "Column " << i << " value: " << currValue << " (localTotal now: " << localTotal << ")\n";
                    currValue = 0;
                    buildingNumber = false;
                }
            }
        }
        
        // Apply last number if we were building one
        if (buildingNumber) {
            if (firstInLocal) {
                localTotal = currValue;
                firstInLocal = false;
            } else {
                if (currentOp == Operation::PLUS) {
                    localTotal += currValue;
                } else { // Operation::MULTIPLY
                    localTotal *= currValue;
                }
            }
            std::cout << "Column " << i << " value: " << currValue << " (localTotal now: " << localTotal << ")\n";
        }
    }
    
    total += localTotal;
    std::cout << "Final localTotal: " << localTotal << "\n";
    
    return total;
}

int main() {
    try {
        auto input = parseInput("input6");
        std::cout << solvePart1(input) << '\n';
        
        auto linesPart2 = parseInputPart2("input6");
        std::cout << solvePart2(linesPart2) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
