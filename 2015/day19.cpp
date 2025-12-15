#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

struct Input {
    std::vector<std::pair<std::string, std::string>> replacements;
    std::string target;
};

Input parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    Input data;
    std::string line;
    
    while (std::getline(input, line)) {
        if (line.empty()) {
            break;
        }
        
        std::string from, arrow, to;
        size_t arrowPos = line.find(" => ");
        if (arrowPos != std::string::npos) {
            from = line.substr(0, arrowPos);
            to = line.substr(arrowPos + 4);
            data.replacements.push_back({from, to});
        }
    }
    
    if (std::getline(input, line)) {
        data.target = line;
    }
    
    return data;
}

void printInput(const Input& data) {
    std::cout << "Replacements:\n";
    for (const auto& [from, to] : data.replacements) {
        std::cout << from << " => " << to << "\n";
    }
    std::cout << "\nTarget: " << data.target << "\n";
}

int part1(const Input& data) {
    std::unordered_set<std::string> molecules;
    
    for (const auto& [from, to] : data.replacements) {
        size_t pos = 0;
        while ((pos = data.target.find(from, pos)) != std::string::npos) {
            std::string newMolecule = data.target.substr(0, pos) + to + data.target.substr(pos + from.length());
            molecules.insert(newMolecule);
            pos++;
        }
    }
    
    return molecules.size();
}

int part2(const Input& data) {
    std::string current = data.target;
    int steps = 0;
    
    while (current != "e") {
        bool found = false;
        
        for (const auto& [from, to] : data.replacements) {
            size_t pos = current.find(to);
            if (pos != std::string::npos) {
                current = current.substr(0, pos) + from + current.substr(pos + to.length());
                steps++;
                found = true;
                break;
            }
        }
        
        if (!found) {
            return -1;
        }
    }
    
    return steps;
}

int main() {
    try {
        auto data = parse("input19");
        
        printInput(data);
        std::cout << "\n";
        
        auto answer1 = part1(data);
        std::cout << "Part 1: " << answer1 << "\n";
        
        auto answer2 = part2(data);
        std::cout << "Part 2: " << answer2 << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
