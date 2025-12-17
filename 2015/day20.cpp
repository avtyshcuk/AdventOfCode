#include <iostream>
#include <vector>

int part1() {
    const int TARGET = 33100000;
    const int MAX_SLOTS = TARGET / 10;
    
    std::vector<int> slots(MAX_SLOTS + 1, 0);
    
    for (int visitor = 1; visitor <= MAX_SLOTS; ++visitor) {
        for (int slot = visitor; slot <= MAX_SLOTS; slot += visitor) {
            slots[slot] += visitor * 10;
        }
    }
    
    for (int slot = 1; slot <= MAX_SLOTS; ++slot) {
        if (slots[slot] >= TARGET) {
            return slot;
        }
    }
    
    return -1;
}

int part2() {
    const int TARGET = 33100000;
    const int MAX_SLOTS = TARGET / 10;
    const int MAX_VISITS = 50;
    
    std::vector<int> slots(MAX_SLOTS + 1, 0);
    
    for (int visitor = 1; visitor <= MAX_SLOTS; ++visitor) {
        int visits = 0;
        for (int slot = visitor; slot <= MAX_SLOTS && visits < MAX_VISITS; slot += visitor) {
            slots[slot] += visitor * 11;
            visits++;
        }
    }
    
    for (int slot = 1; slot <= MAX_SLOTS; ++slot) {
        if (slots[slot] >= TARGET) {
            return slot;
        }
    }
    
    return -1;
}

int main() {
    auto answer1 = part1();
    std::cout << "Part 1: " << answer1 << "\n";
    
    auto answer2 = part2();
    std::cout << "Part 2: " << answer2 << "\n";
    
    return 0;
}
