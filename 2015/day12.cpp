#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::stringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

int sumNumbers(const json& j) {
    int sum = 0;
    
    if (j.is_number()) {
        sum += j.get<int>();
    } else if (j.is_array()) {
        for (const auto& element : j) {
            sum += sumNumbers(element);
        }
    } else if (j.is_object()) {
        for (const auto& [key, value] : j.items()) {
            sum += sumNumbers(value);
        }
    }
    
    return sum;
}

int part1(const std::string& jsonString) {
    json j = json::parse(jsonString);
    return sumNumbers(j);
}

int sumNumbersNoRed(const json& j) {
    int sum = 0;
    
    if (j.is_number()) {
        sum += j.get<int>();
    } else if (j.is_array()) {
        for (const auto& element : j) {
            sum += sumNumbersNoRed(element);
        }
    } else if (j.is_object()) {
        for (const auto& [key, value] : j.items()) {
            if (value.is_string() && value.get<std::string>() == "red") {
                return 0;
            }
        }
        for (const auto& [key, value] : j.items()) {
            sum += sumNumbersNoRed(value);
        }
    }
    
    return sum;
}

int part2(const std::string& jsonString) {
    json j = json::parse(jsonString);
    return sumNumbersNoRed(j);
}

int main() {
    auto jsonString = parse("input12");
    std::cout << "Part 1: " << part1(jsonString) << std::endl;
    std::cout << "Part 2: " << part2(jsonString) << std::endl;
    return 0;
}
