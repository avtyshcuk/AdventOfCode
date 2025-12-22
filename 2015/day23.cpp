#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Instruction {
    std::string op;
    char reg;
    int offset;
};

std::vector<Instruction> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<Instruction> program;
    std::string line;
    
    while (std::getline(input, line)) {
        Instruction instr{};
        instr.op = line.substr(0, 3);
        
        if (instr.op == "jmp") {
            instr.reg = ' ';
            instr.offset = std::stoi(line.substr(4));
        } else if (instr.op == "jie" || instr.op == "jio") {
            instr.reg = line[4];
            instr.offset = std::stoi(line.substr(7));
        } else {
            instr.reg = line[4];
            instr.offset = 0;
        }
        
        program.push_back(instr);
    }
    
    return program;
}

long long execute(const std::vector<Instruction>& program, long long a, long long b) {
    int ip = 0;
    
    while (ip >= 0 && ip < (int)program.size()) {
        const auto& instr = program[ip];
        long long& reg = (instr.reg == 'a') ? a : b;
        
        if (instr.op == "hlf") {
            reg /= 2;
            ip++;
        } else if (instr.op == "tpl") {
            reg *= 3;
            ip++;
        } else if (instr.op == "inc") {
            reg++;
            ip++;
        } else if (instr.op == "jmp") {
            ip += instr.offset;
        } else if (instr.op == "jie") {
            ip += (reg % 2 == 0) ? instr.offset : 1;
        } else if (instr.op == "jio") {
            ip += (reg == 1) ? instr.offset : 1;
        }
    }
    
    return b;
}

long long part1(const std::vector<Instruction>& program) {
    return execute(program, 0, 0);
}

long long part2(const std::vector<Instruction>& program) {
    return execute(program, 1, 0);
}

int main() {
    auto program = parse("input23");
    
    auto answer1 = part1(program);
    std::cout << "Part 1: " << answer1 << "\n";
    
    auto answer2 = part2(program);
    std::cout << "Part 2: " << answer2 << "\n";
    
    return 0;
}
