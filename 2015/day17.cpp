#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<int> containers;
    int value;
    
    while (input >> value) {
        containers.push_back(value);
    }
    
    return containers;
}

void countCombinations(const std::vector<int>& containers, int index, 
                       int currentSum, int target, int& count) {
    if (currentSum == target) {
        count++;
        return;
    }
    
    if (currentSum > target || index >= containers.size()) {
        return;
    }
    
    countCombinations(containers, index + 1, currentSum + containers[index], target, count);
    countCombinations(containers, index + 1, currentSum, target, count);
}

void findMinContainers(const std::vector<int>& containers, int index, 
                       int currentSum, int target, int usedCount, 
                       int& minContainers, int& countWithMin) {
    if (currentSum == target) {
        if (usedCount < minContainers) {
            minContainers = usedCount;
            countWithMin = 1;
        } else if (usedCount == minContainers) {
            countWithMin++;
        }
        return;
    }
    
    if (currentSum > target || index >= containers.size()) {
        return;
    }
    
    findMinContainers(containers, index + 1, currentSum + containers[index], target, usedCount + 1, minContainers, countWithMin);
    findMinContainers(containers, index + 1, currentSum, target, usedCount, minContainers, countWithMin);
}

int part1(const std::vector<int>& containers) {
    const int TARGET = 25;
    
    std::vector<int> sorted = containers;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    
    int count = 0;
    countCombinations(sorted, 0, 0, TARGET, count);
    
    return count;
}

int part2(const std::vector<int>& containers) {
    const int TARGET = 25;
    
    std::vector<int> sorted = containers;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    
    int minContainers = containers.size() + 1;
    int countWithMin = 0;
    
    findMinContainers(sorted, 0, 0, TARGET, 0, minContainers, countWithMin);
    
    std::cout << "Minimum containers needed: " << minContainers << "\n";
    
    return countWithMin;
}

int main() {
    try {
        auto containers = parse("input17");
        
        std::cout << "Parsed " << containers.size() << " containers:\n";
        for (const auto& c : containers) {
            std::cout << c << " ";
        }
        std::cout << "\n\n";
        
        auto answer1 = part1(containers);
        std::cout << "Part 1: " << answer1 << "\n";
        
        auto answer2 = part2(containers);
        std::cout << "Part 2: " << answer2 << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
