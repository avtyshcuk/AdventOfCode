#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

std::string parse(const std::string& fileName) {
    std::ifstream input(fileName);
    if (!input) {
        throw std::runtime_error("Input file '" + fileName + "' is invalid or missing.");
    }
    std::string instructions;
    std::getline(input, instructions);
    return instructions;
}

int part1(const std::string& instructions) {
    int floorNumber{};
    for (char instruction : instructions) {
        if (instruction == '(') {
            floorNumber++;
        } else {
            floorNumber--;
        }
    }
    return floorNumber;
}

int part2(const std::string& instructions) {
    int currentFloor{};
    int stepsCount{1};
    for (char instruction : instructions) {
        if (instruction == '(') {
            currentFloor++;
        } else {
            currentFloor--;
        }
        if (currentFloor < 0) {
            return stepsCount;
        }
        stepsCount++;
    }
    throw std::runtime_error("There should be a negative step to basement in the input");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Please provide an input file.\n";
        return 1;
    }
    try {
        auto instructions = parse(argv[1]);
        std::cout << "Result part1: " << part1(instructions) << std::endl;
        std::cout << "Result part2: " << part2(instructions) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
