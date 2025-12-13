#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Product {
    std::string name;
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
};

std::vector<Product> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<Product> products;
    std::string line;
    
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        Product product;
        std::string capacityLabel, durabilityLabel, flavorLabel, textureLabel, caloriesLabel;
        char colon, comma;
        
        iss >> product.name >> colon 
            >> capacityLabel >> product.capacity >> comma
            >> durabilityLabel >> product.durability >> comma
            >> flavorLabel >> product.flavor >> comma
            >> textureLabel >> product.texture >> comma
            >> caloriesLabel >> product.calories;
        
        if (product.name.back() == ':') {
            product.name.pop_back();
        }
        
        products.push_back(product);
    }
    
    return products;
}

void printProducts(const std::vector<Product>& products) {
    for (const auto& p : products) {
        std::cout << p.name << ": capacity " << p.capacity 
                  << ", durability " << p.durability 
                  << ", flavor " << p.flavor 
                  << ", texture " << p.texture 
                  << ", calories " << p.calories << "\n";
    }
}

long long calculateScore(const std::vector<Product>& products, const std::vector<int>& amounts) {
    int capacity = 0;
    int durability = 0;
    int flavor = 0;
    int texture = 0;
    
    for (size_t i = 0; i < products.size(); ++i) {
        capacity += products[i].capacity * amounts[i];
        durability += products[i].durability * amounts[i];
        flavor += products[i].flavor * amounts[i];
        texture += products[i].texture * amounts[i];
    }
    
    if (capacity <= 0 || durability <= 0 || flavor <= 0 || texture <= 0) {
        return 0;
    }
    
    return (long long)capacity * durability * flavor * texture;
}

void dfs(const std::vector<Product>& products, std::vector<int>& amounts, 
         int productIndex, int teasponsLeft, long long& maxScore) {
    if (productIndex == products.size() - 1) {
        amounts[productIndex] = teasponsLeft;
        long long score = calculateScore(products, amounts);
        maxScore = std::max(maxScore, score);
        return;
    }
    
    for (int i = 0; i <= teasponsLeft; ++i) {
        amounts[productIndex] = i;
        dfs(products, amounts, productIndex + 1, teasponsLeft - i, maxScore);
    }
}

void dfsWithCalories(const std::vector<Product>& products, std::vector<int>& amounts, 
                     int productIndex, int teasponsLeft, long long& maxScore, int targetCalories) {
    if (productIndex == products.size() - 1) {
        amounts[productIndex] = teasponsLeft;
        
        int totalCalories = 0;
        for (size_t i = 0; i < products.size(); ++i) {
            totalCalories += products[i].calories * amounts[i];
        }
        
        if (totalCalories == targetCalories) {
            long long score = calculateScore(products, amounts);
            maxScore = std::max(maxScore, score);
        }
        return;
    }
    
    for (int i = 0; i <= teasponsLeft; ++i) {
        amounts[productIndex] = i;
        dfsWithCalories(products, amounts, productIndex + 1, teasponsLeft - i, maxScore, targetCalories);
    }
}

int part1(const std::vector<Product>& products) {
    const int TOTAL_TEASPOONS = 100;
    long long maxScore = 0;
    std::vector<int> amounts(products.size(), 0);
    
    dfs(products, amounts, 0, TOTAL_TEASPOONS, maxScore);
    
    return maxScore;
}

int part2(const std::vector<Product>& products) {
    const int TOTAL_TEASPOONS = 100;
    const int TARGET_CALORIES = 500;
    long long maxScore = 0;
    std::vector<int> amounts(products.size(), 0);
    
    dfsWithCalories(products, amounts, 0, TOTAL_TEASPOONS, maxScore, TARGET_CALORIES);
    
    return maxScore;
}

int main() {
    try {
        auto products = parse("input15");
        
        std::cout << "Parsed " << products.size() << " products:\n";
        printProducts(products);
        std::cout << "\n";
        
        auto answer1 = part1(products);
        std::cout << "Part 1: " << answer1 << "\n";
        
        auto answer2 = part2(products);
        std::cout << "Part 2: " << answer2 << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
