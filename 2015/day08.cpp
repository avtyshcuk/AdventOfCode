#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<std::string> lines;
    std::string line;
    
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::string decodeString(const std::string& s) {
    std::string result;
    for (size_t i = 1; i < s.length() - 1; ++i) {
        if (s[i] == '\\') {
            if (s[i + 1] == '\\') {
                result += '\\';
                ++i;
            } else if (s[i + 1] == '"') {
                result += '"';
                ++i;
            } else if (s[i + 1] == 'x') {
                int value = std::stoi(s.substr(i + 2, 2), nullptr, 16);
                result += static_cast<char>(value);
                i += 3;
            }
        } else {
            result += s[i];
        }
    }
    return result;
}

int part1(const std::vector<std::string>& lines) {
    int totalCode = 0;
    int totalMemory = 0;
    
    for (const auto& line : lines) {
        totalCode += line.length();
        std::string decoded = decodeString(line);
        totalMemory += decoded.length();
    }
    
    return totalCode - totalMemory;
}

std::string encodeString(const std::string& s) {
    std::string result = "\"";
    for (char c : s) {
        if (c == '\\') {
            result += "\\\\";
        } else if (c == '"') {
            result += "\\\"";
        } else {
            result += c;
        }
    }
    result += "\"";
    return result;
}

int part2(const std::vector<std::string>& lines) {
    int totalOriginal = 0;
    int totalEncoded = 0;
    
    for (const auto& line : lines) {
        totalOriginal += line.length();
        std::string encoded = encodeString(line);
        totalEncoded += encoded.length();
    }
    
    return totalEncoded - totalOriginal;
}

int main() {
    auto lines = parse("input08");
    
    std::cout << "Input:" << std::endl;
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "Part 1: " << part1(lines) << std::endl;
    std::cout << "Part 2: " << part2(lines) << std::endl;
    return 0;
}
