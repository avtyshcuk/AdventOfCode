#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <climits>

struct Item {
    std::string name;
    int cost;
    int damage;
    int armor;
};

struct Boss {
    int hp;
    int damage;
    int armor;
};

const std::vector<Item> WEAPONS = {
    {"Dagger",      8, 4, 0},
    {"Shortsword", 10, 5, 0},
    {"Warhammer",  25, 6, 0},
    {"Longsword",  40, 7, 0},
    {"Greataxe",   74, 8, 0}
};

const std::vector<Item> ARMORS = {
    {"Leather",    13, 0, 1},
    {"Chainmail",  31, 0, 2},
    {"Splintmail", 53, 0, 3},
    {"Bandedmail", 75, 0, 4},
    {"Platemail", 102, 0, 5}
};

const std::vector<Item> RINGS = {
    {"Damage +1",  25, 1, 0},
    {"Damage +2",  50, 2, 0},
    {"Damage +3", 100, 3, 0},
    {"Defense +1", 20, 0, 1},
    {"Defense +2", 40, 0, 2},
    {"Defense +3", 80, 0, 3}
};

const int PLAYER_HP = 100;

Boss parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    Boss boss{0, 0, 0};
    std::string line;
    
    while (std::getline(input, line)) {
        size_t pos = line.find(": ");
        if (pos != std::string::npos) {
            int value = std::stoi(line.substr(pos + 2));
            if (line.find("Hit Points") != std::string::npos) boss.hp = value;
            else if (line.find("Damage") != std::string::npos) boss.damage = value;
            else if (line.find("Armor") != std::string::npos) boss.armor = value;
        }
    }
    
    return boss;
}

bool playerWins(int playerHp, int playerDamage, int playerArmor, Boss boss) {
    int playerDmg = std::max(1, playerDamage - boss.armor);
    int bossDmg = std::max(1, boss.damage - playerArmor);
    
    int turnsToKillBoss = (boss.hp + playerDmg - 1) / playerDmg;
    int turnsToKillPlayer = (playerHp + bossDmg - 1) / bossDmg;
    
    return turnsToKillBoss <= turnsToKillPlayer;
}

int part1(const Boss& boss) {
    int minCost = INT_MAX;
    
    for (const auto& weapon : WEAPONS) {
        for (int a = -1; a < (int)ARMORS.size(); a++) {
            for (int r1 = -1; r1 < (int)RINGS.size(); r1++) {
                for (int r2 = -1; r2 < (int)RINGS.size(); r2++) {
                    if (r1 != -1 && r1 == r2) continue;
                    if (r1 > r2 && r2 != -1) continue;
                    
                    int cost = weapon.cost;
                    int damage = weapon.damage;
                    int armor = weapon.armor;
                    
                    if (a >= 0) {
                        cost += ARMORS[a].cost;
                        damage += ARMORS[a].damage;
                        armor += ARMORS[a].armor;
                    }
                    if (r1 >= 0) {
                        cost += RINGS[r1].cost;
                        damage += RINGS[r1].damage;
                        armor += RINGS[r1].armor;
                    }
                    if (r2 >= 0) {
                        cost += RINGS[r2].cost;
                        damage += RINGS[r2].damage;
                        armor += RINGS[r2].armor;
                    }
                    
                    if (playerWins(PLAYER_HP, damage, armor, boss)) {
                        minCost = std::min(minCost, cost);
                    }
                }
            }
        }
    }
    
    return minCost;
}

int part2(const Boss& boss) {
    int maxCost = 0;
    
    for (const auto& weapon : WEAPONS) {
        for (int a = -1; a < (int)ARMORS.size(); a++) {
            for (int r1 = -1; r1 < (int)RINGS.size(); r1++) {
                for (int r2 = -1; r2 < (int)RINGS.size(); r2++) {
                    if (r1 != -1 && r1 == r2) continue;
                    if (r1 > r2 && r2 != -1) continue;
                    
                    int cost = weapon.cost;
                    int damage = weapon.damage;
                    int armor = weapon.armor;
                    
                    if (a >= 0) {
                        cost += ARMORS[a].cost;
                        damage += ARMORS[a].damage;
                        armor += ARMORS[a].armor;
                    }
                    if (r1 >= 0) {
                        cost += RINGS[r1].cost;
                        damage += RINGS[r1].damage;
                        armor += RINGS[r1].armor;
                    }
                    if (r2 >= 0) {
                        cost += RINGS[r2].cost;
                        damage += RINGS[r2].damage;
                        armor += RINGS[r2].armor;
                    }
                    
                    if (!playerWins(PLAYER_HP, damage, armor, boss)) {
                        maxCost = std::max(maxCost, cost);
                    }
                }
            }
        }
    }
    
    return maxCost;
}

int main() {
    auto boss = parse("input21");
    
    auto answer1 = part1(boss);
    std::cout << "Part 1: " << answer1 << "\n";
    
    auto answer2 = part2(boss);
    std::cout << "Part 2: " << answer2 << "\n";
    
    return 0;
}
