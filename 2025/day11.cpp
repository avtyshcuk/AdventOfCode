#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Graph = std::unordered_map<std::string, std::vector<std::string>>;
using NodeSet = std::unordered_set<std::string>;

Graph parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    Graph adj;
    std::string line;

    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;
        
        std::string node = line.substr(0, colonPos);
        std::string rest = line.substr(colonPos + 1);
        
        std::stringstream ss(rest);
        std::string neighbor;
        while (ss >> neighbor) {
            adj[node].push_back(neighbor);
        }
    }

    return adj;
}

long long solvePart1(const Graph& adj) {
    long long pathCount = 0;
    NodeSet visited;
    
    std::function<void(const std::string&)> dfs = [&](const std::string& node) {
        if (node == "out") {
            pathCount++;
            return;
        }
        
        if (visited.count(node)) return;
        
        auto it = adj.find(node);
        if (it == adj.end()) return;
        
        visited.insert(node);
        for (const auto& neighbor : it->second) {
            dfs(neighbor);
        }
        visited.erase(node);
    };
    
    dfs("you");
    return pathCount;
}

long long solvePart2(const Graph& adj) {
    std::unordered_map<std::string, int> inDegree;
    NodeSet allNodes;
    
    for (const auto& [node, neighbors] : adj) {
        allNodes.insert(node);
        for (const auto& neighbor : neighbors) {
            allNodes.insert(neighbor);
        }
    }
    for (const auto& node : allNodes) {
        inDegree[node] = 0;
    }
    for (const auto& [node, neighbors] : adj) {
        for (const auto& neighbor : neighbors) {
            inDegree[neighbor]++;
        }
    }
    
    std::vector<std::string> topoOrder;
    std::queue<std::string> q;
    for (const auto& [node, deg] : inDegree) {
        if (deg == 0) q.push(node);
    }
    while (!q.empty()) {
        std::string node = q.front();
        q.pop();
        topoOrder.push_back(node);
        auto it = adj.find(node);
        if (it != adj.end()) {
            for (const auto& neighbor : it->second) {
                if (--inDegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
    }
    
    std::reverse(topoOrder.begin(), topoOrder.end());
    
    std::unordered_map<std::string, std::array<long long, 4>> dp;
    for (const auto& node : allNodes) {
        dp[node] = {0, 0, 0, 0};
    }
    dp["out"] = {0, 0, 0, 1};
    
    for (const auto& node : topoOrder) {
        if (node == "out") continue;
        
        auto it = adj.find(node);
        if (it == adj.end()) continue;
        
        int addState = 0;
        if (node == "dac") addState |= 1;
        if (node == "fft") addState |= 2;
        
        for (int inState = 0; inState < 4; inState++) {
            int outState = inState | addState;
            for (const auto& neighbor : it->second) {
                dp[node][inState] += dp[neighbor][outState];
            }
        }
    }
    
    return dp["svr"][0];
}

int main() {
    auto adj = parseInput("input11");
    std::cout << solvePart1(adj) << '\n';
    std::cout << solvePart2(adj) << '\n';
}
