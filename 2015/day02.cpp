#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>


using DimT = std::vector<std::array<int, 3>>;

DimT parse(const std::string& fileName) {
    std::fstream input(fileName);
    if (!input) {
        throw std::runtime_error("Input file '" + fileName + "' is invalid or missing.");
    }

    auto parseDimension = [](const std::string& line) {
        std::regex rx(R"((\d+)x(\d+)x(\d+))");
        std::smatch matches;
        std::regex_search(line, matches, rx);
        int h = std::stoi(matches[1].str());
        int w = std::stoi(matches[2].str());
        int l = std::stoi(matches[3].str());
        return std::array<int, 3>{h, w, l};
    };

    DimT dimensions;
    for (std::string line; std::getline(input, line);) {
        dimensions.push_back(parseDimension(line));
    }
    return dimensions;
}

int part1(const DimT& dimensions) {
    int area{};
    for (const auto& dimension : dimensions) {
        int h = dimension[0];
        int w = dimension[1];
        int l = dimension[2];
        int a1 = h*w;
        int a2 = h*l;
        int a3 = w*l;
        int extra = std::min(a1, std::min(a2, a3));
        area += 2*a1 + 2*a2 + 2*a3 + extra;
    }
    return area;
}

int part2(const DimT& dimensions) {
    int length{};
    for (const auto& dimension: dimensions) {
        int h = dimension[0];
        int w = dimension[1];
        int l = dimension[2];
        int p1 = h + w;
        int p2 = h + l;
        int p3 = w + l;
        int smallest = std::min(p1, std::min(p2, p3));
        length += 2*smallest + h*w*l;
    }
    return length;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Please provide file name with the input!\n";
        return 1;
    }
    try {
        auto dimensions = parse(argv[1]);
        std::cout << "Result part1: " << part1(dimensions) << std::endl;
        std::cout << "Result part2: " << part2(dimensions) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
