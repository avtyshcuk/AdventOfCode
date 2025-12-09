#include <iostream>
#include <string>

void incrementPassword(std::string& password) {
    for (int i = password.length() - 1; i >= 0; --i) {
        if (password[i] == 'z') {
            password[i] = 'a';
        } else {
            password[i]++;
            break;
        }
    }
}

bool hasThreeConsecutive(const std::string& password) {
    for (size_t i = 0; i < password.length() - 2; ++i) {
        if (password[i + 1] == password[i] + 1 && password[i + 2] == password[i] + 2) {
            return true;
        }
    }
    return false;
}

bool hasForbiddenLetters(const std::string& password) {
    for (char c : password) {
        if (c == 'i' || c == 'o' || c == 'l') {
            return true;
        }
    }
    return false;
}

bool hasTwoPairs(const std::string& password) {
    int pairCount = 0;
    for (size_t i = 0; i < password.length() - 1; ++i) {
        if (password[i] == password[i + 1]) {
            ++pairCount;
            ++i;
        }
    }
    return pairCount >= 2;
}

bool isValid(const std::string& password) {
    return hasThreeConsecutive(password) && !hasForbiddenLetters(password) && hasTwoPairs(password);
}

std::string findNextValid(std::string password) {
    do {
        incrementPassword(password);
    } while (!isValid(password));
    return password;
}

std::string part1(const std::string& input) {
    return findNextValid(input);
}

std::string part2(const std::string& input) {
    return findNextValid(input);
}

int main() {
    std::string input = "hepxcrrq";
    std::string result1 = part1(input);
    std::cout << "Part 1: " << result1 << std::endl;
    std::cout << "Part 2: " << part2(result1) << std::endl;
    return 0;
}
