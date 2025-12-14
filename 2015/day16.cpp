#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Aunt {
    int id = -1;
    int children = -1;
    int cats = -1;
    int samoyeds = -1;
    int pomeranians = -1;
    int akitas = -1;
    int vizslas = -1;
    int goldfish = -1;
    int trees = -1;
    int cars = -1;
    int perfumes = -1;
};

std::vector<Aunt> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<Aunt> aunts;
    std::string line;
    
    while (std::getline(input, line)) {
        Aunt aunt;
        std::istringstream iss(line);
        std::string sue;
        char colon;
        
        iss >> sue >> aunt.id >> colon;
        
        std::string property;
        int value;
        char comma;
        
        while (iss >> property) {
            if (property.back() == ':') {
                property.pop_back();
            }
            
            iss >> value;
            
            if (property == "children") aunt.children = value;
            else if (property == "cats") aunt.cats = value;
            else if (property == "samoyeds") aunt.samoyeds = value;
            else if (property == "pomeranians") aunt.pomeranians = value;
            else if (property == "akitas") aunt.akitas = value;
            else if (property == "vizslas") aunt.vizslas = value;
            else if (property == "goldfish") aunt.goldfish = value;
            else if (property == "trees") aunt.trees = value;
            else if (property == "cars") aunt.cars = value;
            else if (property == "perfumes") aunt.perfumes = value;
            
            if (iss >> comma) {
                continue;
            }
        }
        
        aunts.push_back(aunt);
    }
    
    return aunts;
}

void printAunts(const std::vector<Aunt>& aunts) {
    for (const auto& aunt : aunts) {
        std::cout << "Sue " << aunt.id << ":";
        if (aunt.children != -1) std::cout << " children: " << aunt.children;
        if (aunt.cats != -1) std::cout << " cats: " << aunt.cats;
        if (aunt.samoyeds != -1) std::cout << " samoyeds: " << aunt.samoyeds;
        if (aunt.pomeranians != -1) std::cout << " pomeranians: " << aunt.pomeranians;
        if (aunt.akitas != -1) std::cout << " akitas: " << aunt.akitas;
        if (aunt.vizslas != -1) std::cout << " vizslas: " << aunt.vizslas;
        if (aunt.goldfish != -1) std::cout << " goldfish: " << aunt.goldfish;
        if (aunt.trees != -1) std::cout << " trees: " << aunt.trees;
        if (aunt.cars != -1) std::cout << " cars: " << aunt.cars;
        if (aunt.perfumes != -1) std::cout << " perfumes: " << aunt.perfumes;
        std::cout << "\n";
    }
}

int part1(const std::vector<Aunt>& aunts) {
    const Aunt target = {
        -1,  // id
        3,   // children
        7,   // cats
        2,   // samoyeds
        3,   // pomeranians
        0,   // akitas
        0,   // vizslas
        5,   // goldfish
        3,   // trees
        2,   // cars
        1    // perfumes
    };
    
    std::vector<std::string> auntCandidates;
    
    for (const auto& aunt : aunts) {
        bool matches = true;
        
        if (aunt.children != -1 && aunt.children != target.children) matches = false;
        if (aunt.cats != -1 && aunt.cats != target.cats) matches = false;
        if (aunt.samoyeds != -1 && aunt.samoyeds != target.samoyeds) matches = false;
        if (aunt.pomeranians != -1 && aunt.pomeranians != target.pomeranians) matches = false;
        if (aunt.akitas != -1 && aunt.akitas != target.akitas) matches = false;
        if (aunt.vizslas != -1 && aunt.vizslas != target.vizslas) matches = false;
        if (aunt.goldfish != -1 && aunt.goldfish != target.goldfish) matches = false;
        if (aunt.trees != -1 && aunt.trees != target.trees) matches = false;
        if (aunt.cars != -1 && aunt.cars != target.cars) matches = false;
        if (aunt.perfumes != -1 && aunt.perfumes != target.perfumes) matches = false;
        
        if (matches) {
            auntCandidates.push_back("Sue " + std::to_string(aunt.id));
        }
    }
    
    std::cout << "Candidates after filtering:\n";
    for (const auto& candidate : auntCandidates) {
        std::cout << candidate << "\n";
    }
    
    return auntCandidates.empty() ? 0 : aunts[std::stoi(auntCandidates[0].substr(4)) - 1].id;
}

int part2(const std::vector<Aunt>& aunts) {
    const Aunt target = {
        -1,  // id
        3,   // children
        7,   // cats
        2,   // samoyeds
        3,   // pomeranians
        0,   // akitas
        0,   // vizslas
        5,   // goldfish
        3,   // trees
        2,   // cars
        1    // perfumes
    };
    
    std::vector<std::string> auntCandidates;
    
    for (const auto& aunt : aunts) {
        bool matches = true;
        
        if (aunt.children != -1 && aunt.children != target.children) matches = false;
        if (aunt.cats != -1 && aunt.cats <= target.cats) matches = false;
        if (aunt.samoyeds != -1 && aunt.samoyeds != target.samoyeds) matches = false;
        if (aunt.pomeranians != -1 && aunt.pomeranians >= target.pomeranians) matches = false;
        if (aunt.akitas != -1 && aunt.akitas != target.akitas) matches = false;
        if (aunt.vizslas != -1 && aunt.vizslas != target.vizslas) matches = false;
        if (aunt.goldfish != -1 && aunt.goldfish >= target.goldfish) matches = false;
        if (aunt.trees != -1 && aunt.trees <= target.trees) matches = false;
        if (aunt.cars != -1 && aunt.cars != target.cars) matches = false;
        if (aunt.perfumes != -1 && aunt.perfumes != target.perfumes) matches = false;
        
        if (matches) {
            auntCandidates.push_back("Sue " + std::to_string(aunt.id));
        }
    }
    
    std::cout << "Part 2 candidates after filtering:\n";
    for (const auto& candidate : auntCandidates) {
        std::cout << candidate << "\n";
    }
    
    return auntCandidates.empty() ? 0 : aunts[std::stoi(auntCandidates[0].substr(4)) - 1].id;
}

int main() {
    try {
        auto aunts = parse("input16");
        
        std::cout << "Parsed " << aunts.size() << " aunts:\n";
        printAunts(aunts);
        std::cout << "\n";
        
        auto answer1 = part1(aunts);
        std::cout << "Part 1: " << answer1 << "\n";
        
        auto answer2 = part2(aunts);
        std::cout << "Part 2: " << answer2 << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
