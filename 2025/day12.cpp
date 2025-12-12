#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using Shape = std::vector<std::vector<int>>;

struct Region {
    int width, height;
    std::vector<int> shapeQuantity;
};

struct Input {
    std::vector<Shape> shapes;
    std::vector<Region> regions;
};

Shape rotateMatrix90(const Shape& mat) {
    int rows = mat.size(), cols = mat[0].size();
    Shape rotated(cols, std::vector<int>(rows));
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            rotated[c][rows - 1 - r] = mat[r][c];
    return rotated;
}

Shape flipMatrixHorizontal(const Shape& mat) {
    Shape flipped = mat;
    for (auto& row : flipped) std::reverse(row.begin(), row.end());
    return flipped;
}

int countShapeCells(const Shape& grid) {
    int count = 0;
    for (const auto& row : grid)
        for (int val : row)
            if (val == 1) count++;
    return count;
}

std::vector<Shape> getAllTransformations(const Shape& original) {
    std::set<Shape> uniqueTransforms;
    Shape current = original;
    for (int rot = 0; rot < 4; rot++) {
        uniqueTransforms.insert(current);
        uniqueTransforms.insert(flipMatrixHorizontal(current));
        current = rotateMatrix90(current);
    }
    return {uniqueTransforms.begin(), uniqueTransforms.end()};
}

Input parseInput(const std::string& filename) {
    std::ifstream file{filename};
    Input input;
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        if (line.find(':') != std::string::npos && line.find('x') == std::string::npos) {
            Shape shape;
            for (int i = 0; i < 3; i++) {
                if (!std::getline(file, line)) break;
                std::vector<int> row;
                for (char c : line) {
                    if (c == '#') row.push_back(1);
                    else if (c == '.') row.push_back(0);
                }
                if (!row.empty()) shape.push_back(row);
            }
            if (!shape.empty()) input.shapes.push_back(shape);
        }
        else if (line.find('x') != std::string::npos && line.find(':') != std::string::npos) {
            Region region;
            size_t xPos = line.find('x'), colonPos = line.find(':');
            region.width = std::stoi(line.substr(0, xPos));
            region.height = std::stoi(line.substr(xPos + 1, colonPos - xPos - 1));
            std::stringstream ss(line.substr(colonPos + 1));
            int quantity;
            while (ss >> quantity) region.shapeQuantity.push_back(quantity);
            input.regions.push_back(region);
        }
    }
    return input;
}

bool canPlaceShape(const Shape& grid, const Shape& shape, int startRow, int startCol, int regionHeight, int regionWidth) {
    int shapeHeight = shape.size(), shapeWidth = shape[0].size();
    if (startRow + shapeHeight > regionHeight || startCol + shapeWidth > regionWidth) return false;
    for (int r = 0; r < shapeHeight; r++)
        for (int c = 0; c < shapeWidth; c++)
            if (shape[r][c] == 1 && grid[startRow + r][startCol + c] != 0) return false;
    return true;
}

void placeShape(Shape& grid, const Shape& shape, int startRow, int startCol, int shapeId) {
    for (size_t r = 0; r < shape.size(); r++)
        for (size_t c = 0; c < shape[0].size(); c++)
            if (shape[r][c] == 1) grid[startRow + r][startCol + c] = shapeId + 1;
}

void removeShape(Shape& grid, const Shape& shape, int startRow, int startCol) {
    for (size_t r = 0; r < shape.size(); r++)
        for (size_t c = 0; c < shape[0].size(); c++)
            if (shape[r][c] == 1) grid[startRow + r][startCol + c] = 0;
}

bool backtrack(Shape& grid, std::vector<int>& remaining, int regionHeight, int regionWidth, 
               const std::vector<std::vector<Shape>>& allTransforms) {
    size_t nextShapeId = 0;
    while (nextShapeId < remaining.size() && remaining[nextShapeId] == 0) nextShapeId++;
    if (nextShapeId >= remaining.size()) return true;
    
    for (const auto& shape : allTransforms[nextShapeId]) {
        int shapeHeight = shape.size(), shapeWidth = shape[0].size();
        for (int startRow = 0; startRow <= regionHeight - shapeHeight; startRow++) {
            for (int startCol = 0; startCol <= regionWidth - shapeWidth; startCol++) {
                if (canPlaceShape(grid, shape, startRow, startCol, regionHeight, regionWidth)) {
                    placeShape(grid, shape, startRow, startCol, nextShapeId);
                    remaining[nextShapeId]--;
                    if (backtrack(grid, remaining, regionHeight, regionWidth, allTransforms)) return true;
                    removeShape(grid, shape, startRow, startCol);
                    remaining[nextShapeId]++;
                }
            }
        }
    }
    return false;
}

long long solvePart1(const Input& input) {
    long long total = 0;
    
    std::vector<std::vector<Shape>> allTransforms(input.shapes.size());
    std::vector<int> shapeCellCounts(input.shapes.size());
    for (size_t i = 0; i < input.shapes.size(); i++) {
        allTransforms[i] = getAllTransformations(input.shapes[i]);
        shapeCellCounts[i] = countShapeCells(input.shapes[i]);
    }
    
    for (const auto& region : input.regions) {
        int totalCellsNeeded = 0;
        for (size_t i = 0; i < region.shapeQuantity.size(); i++)
            totalCellsNeeded += region.shapeQuantity[i] * shapeCellCounts[i];
        
        if (totalCellsNeeded > region.width * region.height) continue;
        
        Shape grid(region.height, std::vector<int>(region.width, 0));
        std::vector<int> remaining = region.shapeQuantity;
        if (backtrack(grid, remaining, region.height, region.width, allTransforms)) total++;
    }
    return total;
}

int main() {
    auto input = parseInput("input12");
    std::cout << solvePart1(input) << '\n';
}
