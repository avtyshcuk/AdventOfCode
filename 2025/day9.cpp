#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Location {
    int x;
    int y;
};

std::vector<Location> parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<Location> locations;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int x, y;
        char comma;
        ss >> x >> comma >> y;
        locations.push_back({x, y});
    }

    return locations;
}

long long solvePart1(const std::vector<Location>& locations) {
    long long maxArea = 0;

    for (size_t i = 0; i < locations.size(); i++) {
        for (size_t j = i + 1; j < locations.size(); j++) {
            long long width = std::abs(locations[i].x - locations[j].x) + 1;
            long long height = std::abs(locations[i].y - locations[j].y) + 1;
            maxArea = std::max(maxArea, width * height);
        }
    }

    return maxArea;
}

bool pointInOrOnPolygon(int px, int py, const std::vector<Location>& polygon) {
    int n = polygon.size();

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int x1 = polygon[i].x, y1 = polygon[i].y;
        int x2 = polygon[j].x, y2 = polygon[j].y;

        if (y1 == y2 && py == y1 && px >= std::min(x1, x2) && px <= std::max(x1, x2))
            return true;
        if (x1 == x2 && px == x1 && py >= std::min(y1, y2) && py <= std::max(y1, y2))
            return true;
    }

    int crossings = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int x1 = polygon[i].x, y1 = polygon[i].y;
        int x2 = polygon[j].x, y2 = polygon[j].y;

        if ((y1 < py && py <= y2) || (y2 < py && py <= y1)) {
            double xIntersect = x1 + (double)(py - y1) * (x2 - x1) / (y2 - y1);
            if (px < xIntersect) crossings++;
        }
    }

    return (crossings % 2) == 1;
}

bool rectangleInPolygon(int minX, int maxX, int minY, int maxY,
                        const std::vector<Location>& polygon) {
    if (!pointInOrOnPolygon(minX, minY, polygon)) return false;
    if (!pointInOrOnPolygon(maxX, minY, polygon)) return false;
    if (!pointInOrOnPolygon(minX, maxY, polygon)) return false;
    if (!pointInOrOnPolygon(maxX, maxY, polygon)) return false;

    int n = polygon.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int x1 = polygon[i].x, y1 = polygon[i].y;
        int x2 = polygon[j].x, y2 = polygon[j].y;

        if (y1 == y2) {
            int edgeMinX = std::min(x1, x2);
            int edgeMaxX = std::max(x1, x2);
            if (y1 > minY && y1 < maxY && edgeMinX < maxX && edgeMaxX > minX)
                return false;
        } else if (x1 == x2) {
            int edgeMinY = std::min(y1, y2);
            int edgeMaxY = std::max(y1, y2);
            if (x1 > minX && x1 < maxX && edgeMinY < maxY && edgeMaxY > minY)
                return false;
        }
    }

    return true;
}

long long solvePart2(const std::vector<Location>& locations) {
    long long maxArea = 0;

    for (size_t i = 0; i < locations.size(); i++) {
        for (size_t j = i + 1; j < locations.size(); j++) {
            int minX = std::min(locations[i].x, locations[j].x);
            int maxX = std::max(locations[i].x, locations[j].x);
            int minY = std::min(locations[i].y, locations[j].y);
            int maxY = std::max(locations[i].y, locations[j].y);

            if (minX == maxX || minY == maxY) continue;
            if (!rectangleInPolygon(minX, maxX, minY, maxY, locations)) continue;

            long long width = maxX - minX + 1;
            long long height = maxY - minY + 1;
            maxArea = std::max(maxArea, width * height);
        }
    }

    return maxArea;
}

int main() {
    auto locations = parseInput("input9");
    std::cout << solvePart1(locations) << '\n';
    std::cout << solvePart2(locations) << '\n';
}
