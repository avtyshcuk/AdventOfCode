#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Position {
    int x{}, y{};

    void update(char direction) {
        switch (direction) {
            case '^' : x++; break;
            case '>' : y++; break;
            case 'v' : x--; break;
            case '<' : y--; break;
            default: break;
        }
    }

    bool operator==(const Position& p) const {
        return p.x == x && p.y == y;
    }
    
    struct Hash {
        size_t operator()(const Position& p) const {
            size_t hash_x = std::hash<int>{}(p.x);
            size_t hash_y = std::hash<int>{}(p.y);
            return hash_x ^ (hash_y << 1);
        }
    };
};

std::string parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    std::string directions;
    std::getline(input, directions);
    return directions;
}

int part1(const std::string& directions) {
    Position start{};
    std::unordered_set<Position, Position::Hash> positions{start};
    Position current{start};
    for (char direction : directions) {
        current.update(direction);
        positions.insert(current);
    }
    return positions.size();
}

int part2(const std::string& directions) {
    Position start{};
    std::unordered_set<Position, Position::Hash> positions{start};
    Position santa{start}, roboSanta{start};
    int turn{};
    for (char direction : directions) {
        if (turn % 2 == 0) {
            santa.update(direction);
            positions.insert(santa);
        } else {
            roboSanta.update(direction);
            positions.insert(roboSanta);
        }
        turn++;
    }
    return positions.size();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Please provide input file!\n";
        return 1;
    }
    try {
        auto directions = parse(argv[1]);
        std::cout << "Result part1: " << part1(directions) << std::endl;
        std::cout << "Result part2: " << part2(directions) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
