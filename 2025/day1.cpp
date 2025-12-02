#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Movement {
    char direction;
    int distance;
};

constexpr int DIAL_SIZE = 100;
constexpr int START_POSITION = 50;

std::vector<Movement> parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<Movement> movements;
    std::string line;
    while (std::getline(file, line)) {
        movements.push_back({line[0], std::stoi(line.substr(1))});
    }
    return movements;
}

int wrapPosition(int position) {
    position %= DIAL_SIZE;
    return position < 0 ? position + DIAL_SIZE : position;
}

int solvePart1(const std::vector<Movement>& movements) {
    int current = START_POSITION;
    int password = 0;

    for (const auto& [direction, distance] : movements) {
        current = wrapPosition(current + (direction == 'L' ? -distance : distance));
        if (current == 0) {
            ++password;
        }
    }
    return password;
}

int solvePart2(const std::vector<Movement>& movements) {
    int current = START_POSITION;
    int password = 0;

    for (const auto& [direction, distance] : movements) {
        int step = (direction == 'R') ? 1 : -1;
        for (int i = 0; i < distance; ++i) {
            current = wrapPosition(current + step);
            if (current == 0) {
                ++password;
            }
        }
    }
    return password;
}

int main() {
    try {
        auto movements = parseInput("input1");
        std::cout << solvePart1(movements) << '\n';
        std::cout << solvePart2(movements) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}


