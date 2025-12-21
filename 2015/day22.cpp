#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <climits>

struct Boss {
    int hp;
    int damage;
};

struct State {
    int playerHp;
    int mana;
    int bossHp;
    int shieldTimer;
    int poisonTimer;
    int rechargeTimer;
    int manaSpent;
};

const int PLAYER_HP = 50;
const int PLAYER_MANA = 500;

enum Spell { MAGIC_MISSILE, DRAIN, SHIELD, POISON, RECHARGE, SPELL_COUNT };
const int SPELL_COST[] = {53, 73, 113, 173, 229};

Boss parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    Boss boss{0, 0};
    std::string line;
    
    while (std::getline(input, line)) {
        size_t pos = line.find(": ");
        if (pos != std::string::npos) {
            int value = std::stoi(line.substr(pos + 2));
            if (line.find("Hit Points") != std::string::npos) boss.hp = value;
            else if (line.find("Damage") != std::string::npos) boss.damage = value;
        }
    }
    
    return boss;
}

void applyEffects(State& s) {
    if (s.shieldTimer > 0) s.shieldTimer--;
    if (s.poisonTimer > 0) {
        s.bossHp -= 3;
        s.poisonTimer--;
    }
    if (s.rechargeTimer > 0) {
        s.mana += 101;
        s.rechargeTimer--;
    }
}

bool canCast(const State& s, Spell spell) {
    if (s.mana < SPELL_COST[spell]) return false;
    if (spell == SHIELD && s.shieldTimer > 0) return false;
    if (spell == POISON && s.poisonTimer > 0) return false;
    if (spell == RECHARGE && s.rechargeTimer > 0) return false;
    return true;
}

void cast(State& s, Spell spell) {
    s.mana -= SPELL_COST[spell];
    s.manaSpent += SPELL_COST[spell];
    
    switch (spell) {
        case MAGIC_MISSILE:
            s.bossHp -= 4;
            break;
        case DRAIN:
            s.bossHp -= 2;
            s.playerHp += 2;
            break;
        case SHIELD:
            s.shieldTimer = 6;
            break;
        case POISON:
            s.poisonTimer = 6;
            break;
        case RECHARGE:
            s.rechargeTimer = 5;
            break;
        default:
            break;
    }
}

int minManaToWin;

void solve(State s, bool playerTurn, int bossDamage, bool hardMode) {
    if (s.manaSpent >= minManaToWin) return;
    
    if (hardMode && playerTurn) {
        s.playerHp--;
        if (s.playerHp <= 0) return;
    }
    
    applyEffects(s);
    
    if (s.bossHp <= 0) {
        minManaToWin = std::min(minManaToWin, s.manaSpent);
        return;
    }
    
    if (playerTurn) {
        for (int i = 0; i < SPELL_COUNT; i++) {
            Spell spell = static_cast<Spell>(i);
            if (canCast(s, spell)) {
                State next = s;
                cast(next, spell);
                if (next.bossHp <= 0) {
                    minManaToWin = std::min(minManaToWin, next.manaSpent);
                } else {
                    solve(next, false, bossDamage, hardMode);
                }
            }
        }
    } else {
        int armor = (s.shieldTimer > 0) ? 7 : 0;
        s.playerHp -= std::max(1, bossDamage - armor);
        if (s.playerHp > 0) {
            solve(s, true, bossDamage, hardMode);
        }
    }
}

int part1(const Boss& boss) {
    minManaToWin = INT_MAX;
    State initial = {PLAYER_HP, PLAYER_MANA, boss.hp, 0, 0, 0, 0};
    solve(initial, true, boss.damage, false);
    return minManaToWin;
}

int part2(const Boss& boss) {
    minManaToWin = INT_MAX;
    State initial = {PLAYER_HP, PLAYER_MANA, boss.hp, 0, 0, 0, 0};
    solve(initial, true, boss.damage, true);
    return minManaToWin;
}

int main() {
    auto boss = parse("input22");
    
    auto answer1 = part1(boss);
    std::cout << "Part 1: " << answer1 << "\n";
    
    auto answer2 = part2(boss);
    std::cout << "Part 2: " << answer2 << "\n";
    
    return 0;
}
