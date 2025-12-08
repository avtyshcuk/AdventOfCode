#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <limits>
#include <functional>

struct LocationPair {
    std::string location1;
    std::string location2;
    int distance;
};

using Graph = std::unordered_map<std::string, std::unordered_map<std::string, int>>;
using Visited = std::unordered_map<std::string, bool>;

std::vector<LocationPair> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<LocationPair> pairs;
    std::string line;
    
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        LocationPair pair;
        std::string to, equals;
        
        iss >> pair.location1 >> to >> pair.location2 >> equals >> pair.distance;
        pairs.push_back(pair);
    }
    
    return pairs;
}

std::pair<Graph, std::unordered_set<std::string>> buildGraph(const std::vector<LocationPair>& pairs) {
    Graph graph;
    std::unordered_set<std::string> locations;
    
    for (const auto& pair : pairs) {
        graph[pair.location1][pair.location2] = pair.distance;
        graph[pair.location2][pair.location1] = pair.distance;
        locations.insert(pair.location1);
        locations.insert(pair.location2);
    }
    
    return {graph, locations};
}

void travel(const std::string& current,
            const Graph& graph,
            Visited& visited,
            int currentDistance,
            int visitedCount,
            int totalLocations,
            int& bestDistance,
            std::function<int(int, int)> comparator) {
    
    if (visitedCount == totalLocations) {
        bestDistance = comparator(bestDistance, currentDistance);
        return;
    }
    
    for (const auto& [neighbor, distance] : graph.at(current)) {
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            travel(neighbor, graph, visited, currentDistance + distance, visitedCount + 1, totalLocations, bestDistance, comparator);
            visited[neighbor] = false;
        }
    }
}

int findBestRoute(const std::vector<LocationPair>& pairs, int initialValue, std::function<int(int, int)> comparator) {
    auto [graph, locations] = buildGraph(pairs);
    
    int bestDistance = initialValue;
    
    for (const auto& start : locations) {
        Visited visited;
        for (const auto& loc : locations) {
            visited[loc] = false;
        }
        visited[start] = true;
        travel(start, graph, visited, 0, 1, locations.size(), bestDistance, comparator);
    }
    
    return bestDistance;
}

int part1(const std::vector<LocationPair>& pairs) {
    return findBestRoute(pairs, std::numeric_limits<int>::max(), [](int a, int b) { return std::min(a, b); });
}

int part2(const std::vector<LocationPair>& pairs) {
    return findBestRoute(pairs, 0, [](int a, int b) { return std::max(a, b); });
}

int main() {
    auto pairs = parse("input09");
    std::cout << "Part 1: " << part1(pairs) << std::endl;
    std::cout << "Part 2: " << part2(pairs) << std::endl;
    return 0;
}
