#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct JunctionPosition {
    int x;
    int y;
    int z;
};

struct UnionFind {
    std::vector<int> root;
    std::vector<int> rank;
    std::vector<int> size;
    
    UnionFind(int n) : root(n), rank(n, 0), size(n, 1) {
        for (int i = 0; i < n; i++) {
            root[i] = i;
        }
    }
    
    int find(int x) {
        if (root[x] == x) {
            return x;
        }
        return root[x] = find(root[x]);
    }
    
    void unionByRank(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                root[rootY] = rootX;
                size[rootX] += size[rootY];
            } else if (rank[rootX] < rank[rootY]) {
                root[rootX] = rootY;
                size[rootY] += size[rootX];
            } else {
                root[rootY] = rootX;
                rank[rootX]++;
                size[rootX] += size[rootY];
            }
        }
    }
    
    std::vector<int> getComponentSizes() const {
        std::vector<int> sizes;
        for (size_t i = 0; i < root.size(); i++) {
            if (root[i] == i) {
                sizes.push_back(size[i]);
            }
        }
        return sizes;
    }
    
    int getComponentCount() const {
        int count = 0;
        for (size_t i = 0; i < root.size(); i++) {
            if (root[i] == i) {
                count++;
            }
        }
        return count;
    }
};

std::vector<JunctionPosition> parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<JunctionPosition> junctions;
    std::string line;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int x, y, z;
        char comma;
        
        ss >> x >> comma >> y >> comma >> z;
        junctions.push_back({x, y, z});
    }
    
    return junctions;
}

long long solvePart1(const std::vector<JunctionPosition>& junctions) {
    using DistancePair = std::pair<double, std::pair<int, int>>;
    std::priority_queue<DistancePair, std::vector<DistancePair>, std::greater<DistancePair>> pq;
    
    for (size_t i = 0; i < junctions.size(); i++) {
        for (size_t j = i + 1; j < junctions.size(); j++) {
            long long dx = junctions[i].x - junctions[j].x;
            long long dy = junctions[i].y - junctions[j].y;
            long long dz = junctions[i].z - junctions[j].z;
            
            double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
            pq.push({distance, {i, j}});
        }
    }
    
    UnionFind uf(junctions.size());
    
    for (int count = 0; count < 1000 && !pq.empty(); count++) {
        auto [distance, indices] = pq.top();
        pq.pop();
        
        auto [i, j] = indices;
        uf.unionByRank(i, j);
    }
    
    auto sizes = uf.getComponentSizes();
    std::sort(sizes.begin(), sizes.end(), std::greater<int>());
    
    std::cout << "Component sizes: ";
    for (auto s : sizes) {
        std::cout << s << " ";
    }
    std::cout << '\n';
    
    if (sizes.size() >= 3) {
        return 1LL * sizes[0] * sizes[1] * sizes[2];
    }
    
    return 0;
}

long long solvePart2(const std::vector<JunctionPosition>& junctions) {
    using DistancePair = std::pair<double, std::pair<int, int>>;
    std::priority_queue<DistancePair, std::vector<DistancePair>, std::greater<DistancePair>> pq;
    
    for (size_t i = 0; i < junctions.size(); i++) {
        for (size_t j = i + 1; j < junctions.size(); j++) {
            long long dx = junctions[i].x - junctions[j].x;
            long long dy = junctions[i].y - junctions[j].y;
            long long dz = junctions[i].z - junctions[j].z;
            
            double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
            pq.push({distance, {i, j}});
        }
    }
    
    UnionFind uf(junctions.size());
    int lastI = -1, lastJ = -1;
    
    while (uf.getComponentCount() > 1 && !pq.empty()) {
        auto [distance, indices] = pq.top();
        pq.pop();
        
        auto [i, j] = indices;
        lastI = i;
        lastJ = j;
        uf.unionByRank(i, j);
    }
    
    if (lastI != -1 && lastJ != -1) {
        return 1LL * junctions[lastI].x * junctions[lastJ].x;
    }
    
    return 0;
}

int main() {
    try {
        auto junctions = parseInput("input8");
        std::cout << solvePart1(junctions) << '\n';
        std::cout << solvePart2(junctions) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
