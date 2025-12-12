#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <limits>

using HappinessMap = std::unordered_map<std::string, std::unordered_map<std::string, int>>;
using PeopleSet = std::unordered_set<std::string>;

struct Relation {
    std::string person;
    std::string neighbor;
    int happiness;
};

std::vector<Relation> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<Relation> relations;
    std::string line;
    
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        Relation rel;
        std::string would, effectStr, happiness, units, by, sitting, next, to;
        
        iss >> rel.person >> would >> effectStr >> rel.happiness >> happiness >> units >> by >> sitting >> next >> to >> rel.neighbor;
        
        if (!rel.neighbor.empty() && rel.neighbor.back() == '.') {
            rel.neighbor.pop_back();
        }
        
        if (effectStr == "lose") {
            rel.happiness = -rel.happiness;
        }
        
        relations.push_back(rel);
    }
    
    return relations;
}

void printRelations(const std::vector<Relation>& relations) {
    std::cout << "=== Relations ===" << std::endl;
    for (const auto& rel : relations) {
        std::cout << rel.person << " -> " << rel.neighbor << ": " 
                  << (rel.happiness >= 0 ? "+" : "") << rel.happiness << std::endl;
    }
    std::cout << std::endl;
}

int part1(const std::vector<Relation>& relations) {
    HappinessMap happiness;
    PeopleSet peopleSet;
    
    for (const auto& rel : relations) {
        happiness[rel.person][rel.neighbor] = rel.happiness;
        peopleSet.insert(rel.person);
    }
    
    std::vector<std::string> people(peopleSet.begin(), peopleSet.end());
    std::sort(people.begin(), people.end());
    
    int maxHappiness = std::numeric_limits<int>::min();
    
    do {
        int totalHappiness = 0;
        
        for (size_t i = 0; i < people.size(); ++i) {
            size_t left = (i - 1 + people.size()) % people.size();
            size_t right = (i + 1) % people.size();
            
            totalHappiness += happiness[people[i]][people[left]];
            totalHappiness += happiness[people[i]][people[right]];
        }
        
        maxHappiness = std::max(maxHappiness, totalHappiness);
        
    } while (std::next_permutation(people.begin(), people.end()));
    
    return maxHappiness;
}

int part2(const std::vector<Relation>& relations) {
    HappinessMap happiness;
    PeopleSet peopleSet;
    
    for (const auto& rel : relations) {
        happiness[rel.person][rel.neighbor] = rel.happiness;
        peopleSet.insert(rel.person);
    }
    
    for (const auto& person : peopleSet) {
        happiness["Me"][person] = 0;
        happiness[person]["Me"] = 0;
    }
    peopleSet.insert("Me");
    
    std::vector<std::string> people(peopleSet.begin(), peopleSet.end());
    std::sort(people.begin(), people.end());
    
    int maxHappiness = std::numeric_limits<int>::min();
    
    do {
        int totalHappiness = 0;
        
        for (size_t i = 0; i < people.size(); ++i) {
            size_t left = (i - 1 + people.size()) % people.size();
            size_t right = (i + 1) % people.size();
            
            totalHappiness += happiness[people[i]][people[left]];
            totalHappiness += happiness[people[i]][people[right]];
        }
        
        maxHappiness = std::max(maxHappiness, totalHappiness);
        
    } while (std::next_permutation(people.begin(), people.end()));
    
    return maxHappiness;
}

int main() {
    auto relations = parse("input13");
    printRelations(relations);
    std::cout << "Part 1: " << part1(relations) << std::endl;
    std::cout << "Part 2: " << part2(relations) << std::endl;
    return 0;
}
