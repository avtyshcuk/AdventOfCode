#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum class Operation { AND, OR, LSHIFT, RSHIFT, NOT, ASSIGN };

struct Instruction {
    std::string op1;
    std::string op2;
    Operation operation;
};

using Values = std::unordered_map<std::string, int>;
using Instructions = std::unordered_map<std::string, Instruction>;

struct Circuit {
    Values values;
    Instructions instructions;
};

bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

Circuit parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    Circuit circuit;
    std::string line;
    
    while (std::getline(input, line)) {
        size_t arrowPos = line.find(" -> ");
        if (arrowPos == std::string::npos) continue;
        
        std::string leftPart = line.substr(0, arrowPos);
        std::string wire = line.substr(arrowPos + 4);
        
        if (isNumber(leftPart)) {
            circuit.values[wire] = std::stoi(leftPart);
        } else {
            std::istringstream iss(leftPart);
            std::vector<std::string> tokens;
            std::string token;
            
            while (iss >> token) {
                tokens.push_back(token);
            }
            
            Instruction instr;
            
            if (tokens.size() == 1) {
                instr.op1 = tokens[0];
                instr.op2 = "";
                instr.operation = Operation::ASSIGN;
                circuit.instructions[wire] = instr;
            } else if (tokens[0] == "NOT") {
                instr.op1 = tokens[1];
                instr.op2 = "";
                instr.operation = Operation::NOT;
                circuit.instructions[wire] = instr;
            } else if (tokens.size() == 3) {
                instr.op1 = tokens[0];
                instr.op2 = tokens[2];
                
                if (tokens[1] == "AND") {
                    instr.operation = Operation::AND;
                } else if (tokens[1] == "OR") {
                    instr.operation = Operation::OR;
                } else if (tokens[1] == "LSHIFT") {
                    instr.operation = Operation::LSHIFT;
                } else if (tokens[1] == "RSHIFT") {
                    instr.operation = Operation::RSHIFT;
                }
                circuit.instructions[wire] = instr;
            }
        }
    }
    return circuit;
}

std::string operationToString(Operation op) {
    switch (op) {
        case Operation::AND: return "AND";
        case Operation::OR: return "OR";
        case Operation::LSHIFT: return "LSHIFT";
        case Operation::RSHIFT: return "RSHIFT";
        case Operation::NOT: return "NOT";
        case Operation::ASSIGN: return "ASSIGN";
        default: return "UNKNOWN";
    }
}

void printValues(const Values& values) {
    std::cout << "=== Values ===" << std::endl;
    for (const auto& [wire, value] : values) {
        std::cout << wire << " = " << value << std::endl;
    }
    std::cout << std::endl;
}

void printInstructions(const Instructions& instructions) {
    std::cout << "=== Instructions ===" << std::endl;
    for (const auto& [wire, instr] : instructions) {
        std::cout << wire << " = ";
        if (instr.operation == Operation::NOT) {
            std::cout << "NOT " << instr.op1;
        } else if (instr.operation == Operation::ASSIGN) {
            std::cout << instr.op1;
        } else {
            std::cout << instr.op1 << " " << operationToString(instr.operation) << " " << instr.op2;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printCircuit(const Circuit& circuit) {
    printValues(circuit.values);
    printInstructions(circuit.instructions);
}

int part1(const Circuit& circuit) {
    Values values = circuit.values;
    Instructions remaining = circuit.instructions;
    
    while (!remaining.empty()) {
        Instructions nextRemaining;
        
        for (const auto& [wire, instr] : remaining) {
            bool canEvaluate = false;
            int result = 0;
            
            if (instr.operation == Operation::ASSIGN) {
                if (values.count(instr.op1)) {
                    result = values[instr.op1];
                    canEvaluate = true;
                }
            } else if (instr.operation == Operation::NOT) {
                if (values.count(instr.op1)) {
                    int val1 = values[instr.op1];
                    result = ~val1 & 0xFFFF;
                    canEvaluate = true;
                }
            } else if (instr.operation == Operation::LSHIFT || instr.operation == Operation::RSHIFT) {
                if (values.count(instr.op1)) {
                    int val1 = values[instr.op1];
                    int shiftAmount = std::stoi(instr.op2);
                    canEvaluate = true;
                    
                    if (instr.operation == Operation::LSHIFT) {
                        result = (val1 << shiftAmount) & 0xFFFF;
                    } else {
                        result = val1 >> shiftAmount;
                    }
                }
            } else {
                int val1, val2;
                bool hasVal1 = false, hasVal2 = false;
                
                if (isNumber(instr.op1)) {
                    val1 = std::stoi(instr.op1);
                    hasVal1 = true;
                } else if (values.count(instr.op1)) {
                    val1 = values[instr.op1];
                    hasVal1 = true;
                }
                
                if (isNumber(instr.op2)) {
                    val2 = std::stoi(instr.op2);
                    hasVal2 = true;
                } else if (values.count(instr.op2)) {
                    val2 = values[instr.op2];
                    hasVal2 = true;
                }
                
                if (hasVal1 && hasVal2) {
                    canEvaluate = true;
                    
                    switch (instr.operation) {
                        case Operation::AND:
                            result = val1 & val2;
                            break;
                        case Operation::OR:
                            result = val1 | val2;
                            break;
                        default:
                            break;
                    }
                }
            }
            
            if (canEvaluate) {
                values[wire] = result;
            } else {
                nextRemaining[wire] = instr;
            }
        }
        
        remaining = nextRemaining;
    }
    
    return values["a"];
}

int part2(const Circuit& circuit, int signalFromA) {
    Values values = circuit.values;
    values["b"] = signalFromA;
    Instructions remaining = circuit.instructions;
    
    while (!remaining.empty()) {
        Instructions nextRemaining;
        
        for (const auto& [wire, instr] : remaining) {
            bool canEvaluate = false;
            int result = 0;
            
            if (instr.operation == Operation::ASSIGN) {
                if (values.count(instr.op1)) {
                    result = values[instr.op1];
                    canEvaluate = true;
                }
            } else if (instr.operation == Operation::NOT) {
                if (values.count(instr.op1)) {
                    int val1 = values[instr.op1];
                    result = ~val1 & 0xFFFF;
                    canEvaluate = true;
                }
            } else if (instr.operation == Operation::LSHIFT || instr.operation == Operation::RSHIFT) {
                if (values.count(instr.op1)) {
                    int val1 = values[instr.op1];
                    int shiftAmount = std::stoi(instr.op2);
                    canEvaluate = true;
                    
                    if (instr.operation == Operation::LSHIFT) {
                        result = (val1 << shiftAmount) & 0xFFFF;
                    } else {
                        result = val1 >> shiftAmount;
                    }
                }
            } else {
                int val1, val2;
                bool hasVal1 = false, hasVal2 = false;
                
                if (isNumber(instr.op1)) {
                    val1 = std::stoi(instr.op1);
                    hasVal1 = true;
                } else if (values.count(instr.op1)) {
                    val1 = values[instr.op1];
                    hasVal1 = true;
                }
                
                if (isNumber(instr.op2)) {
                    val2 = std::stoi(instr.op2);
                    hasVal2 = true;
                } else if (values.count(instr.op2)) {
                    val2 = values[instr.op2];
                    hasVal2 = true;
                }
                
                if (hasVal1 && hasVal2) {
                    canEvaluate = true;
                    
                    switch (instr.operation) {
                        case Operation::AND:
                            result = val1 & val2;
                            break;
                        case Operation::OR:
                            result = val1 | val2;
                            break;
                        default:
                            break;
                    }
                }
            }
            
            if (canEvaluate) {
                values[wire] = result;
            } else {
                nextRemaining[wire] = instr;
            }
        }
        
        remaining = nextRemaining;
    }
    
    return values["a"];
}

int main() {
    auto circuit = parse("input07");
    printCircuit(circuit);
    int resultPart1 = part1(circuit);
    std::cout << "Part 1: " << resultPart1 << std::endl;
    std::cout << "Part 2: " << part2(circuit, resultPart1) << std::endl;
    return 0;
}
