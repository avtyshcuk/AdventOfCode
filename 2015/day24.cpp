#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <climits>

std::vector<int> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<int> weights;
    int w;
    while (input >> w) {
        weights.push_back(w);
    }
    
    return weights;
}

long long minQE;
int minSize;

void findGroups(const std::vector<int>& weights, int idx, int target, 
                int sum, int count, long long qe) {
    if (sum == target) {
        if (count < minSize || (count == minSize && qe < minQE)) {
            minSize = count;
            minQE = qe;
        }
        return;
    }
    
    if (idx >= (int)weights.size()) return;
    if (sum > target) return;
    if (count >= minSize) return;
    
    // Prune: even if we take all remaining, can't reach target
    int remaining = 0;
    for (int i = idx; i < (int)weights.size(); i++) {
        remaining += weights[i];
    }
    if (sum + remaining < target) return;
    
    // Take current weight
    findGroups(weights, idx + 1, target, sum + weights[idx], 
               count + 1, qe * weights[idx]);
    
    // Skip current weight
    findGroups(weights, idx + 1, target, sum, count, qe);
}

long long solve(const std::vector<int>& weights, int groups) {
    int total = 0;
    for (int w : weights) total += w;
    
    if (total % groups != 0) return -1;
    int target = total / groups;
    
    minQE = LLONG_MAX;
    minSize = INT_MAX;
    
    // Sort descending to find smaller groups first
    std::vector<int> sorted = weights;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    
    findGroups(sorted, 0, target, 0, 0, 1);
    
    return minQE;
}

long long part1(const std::vector<int>& weights) {
    return solve(weights, 3);
}

long long part2(const std::vector<int>& weights) {
    return solve(weights, 4);
}

int main() {
    auto weights = parse("input24");
    
    auto answer1 = part1(weights);
    std::cout << "Part 1: " << answer1 << "\n";
    
    auto answer2 = part2(weights);
    std::cout << "Part 2: " << answer2 << "\n";
    
    return 0;
}
