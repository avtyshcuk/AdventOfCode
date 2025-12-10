#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <glpk.h>

struct MachineSpec {
    std::vector<bool> lightDiagram;
    std::vector<std::vector<int>> buttons;
    std::vector<int> joltage;
};

std::vector<MachineSpec> parseInput(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<MachineSpec> specs;
    std::string line;

    while (std::getline(file, line)) {
        MachineSpec spec;
        std::stringstream ss(line);
        
        char ch;
        ss >> ch;
        std::string pattern;
        while (ss >> ch && ch != ']') {
            pattern += ch;
        }
        
        for (char c : pattern) {
            spec.lightDiagram.push_back(c == '#');
        }
        
        while (ss >> ch) {
            if (ch == '(') {
                std::vector<int> coord;
                std::string coordStr;
                while (ss.get(ch) && ch != ')') {
                    if (ch != ' ') coordStr += ch;
                }
                
                if (!coordStr.empty()) {
                    std::stringstream coordSS(coordStr);
                    int val;
                    while (coordSS >> val) {
                        coord.push_back(val);
                        if (coordSS.peek() == ',') coordSS.ignore();
                    }
                    spec.buttons.push_back(coord);
                }
            } else if (ch == '{') {
                std::string joltageStr;
                while (ss.get(ch) && ch != '}') {
                    if (ch != ' ') joltageStr += ch;
                }
                
                std::stringstream joltSS(joltageStr);
                int val;
                while (joltSS >> val) {
                    spec.joltage.push_back(val);
                    if (joltSS.peek() == ',') joltSS.ignore();
                }
            }
        }
        
        specs.push_back(spec);
    }

    return specs;
}

void printMachineSpec(const MachineSpec& spec) {
    std::cout << "Light Diagram: [";
    for (bool light : spec.lightDiagram) {
        std::cout << (light ? '#' : '.');
    }
    std::cout << "] (count=" << spec.lightDiagram.size() << ")\n";
    
    std::cout << "Buttons: ";
    for (const auto& button : spec.buttons) {
        std::cout << "(";
        for (size_t i = 0; i < button.size(); i++) {
            std::cout << button[i];
            if (i + 1 < button.size()) std::cout << ",";
        }
        std::cout << ") ";
    }
    std::cout << "\n";
    
    std::cout << "Joltage: {";
    for (size_t i = 0; i < spec.joltage.size(); i++) {
        std::cout << spec.joltage[i];
        if (i + 1 < spec.joltage.size()) std::cout << ",";
    }
    std::cout << "}\n\n";
}

long long solvePart1(const std::vector<MachineSpec>& specs) {
    long long totalSteps = 0;
    
    for (const auto& spec : specs) {
        int lightCount = spec.lightDiagram.size();
        std::vector<std::vector<bool>> currentStates;
        currentStates.push_back(std::vector<bool>(lightCount, false));
        
        int steps = 0;
        bool found = false;
        
        while (!found) {
            steps++;
            std::vector<std::vector<bool>> nextStates;
            
            for (const auto& state : currentStates) {
                for (const auto& button : spec.buttons) {
                    std::vector<bool> newState = state;
                    for (int lightIndex : button) {
                        if (lightIndex >= 0 && lightIndex < lightCount) {
                            newState[lightIndex] = !newState[lightIndex];
                        }
                    }
                    
                    if (newState == spec.lightDiagram) {
                        found = true;
                        break;
                    }
                    
                    nextStates.push_back(newState);
                }
                if (found) break;
            }
            
            if (!found) {
                currentStates = nextStates;
            }
        }
        
        totalSteps += steps;
    }
    
    return totalSteps;
}

int solveILP(const MachineSpec& spec) {
    int numButtons = spec.buttons.size();
    int joltageSize = spec.joltage.size();
    
    glp_prob *lp = glp_create_prob();
    glp_set_prob_name(lp, "joltage");
    glp_set_obj_dir(lp, GLP_MIN);
    glp_term_out(GLP_OFF);
    
    glp_add_cols(lp, numButtons);
    for (int i = 0; i < numButtons; i++) {
        glp_set_col_kind(lp, i + 1, GLP_IV);
        glp_set_col_bnds(lp, i + 1, GLP_LO, 0.0, 0.0);
        glp_set_obj_coef(lp, i + 1, 1.0);
    }
    
    glp_add_rows(lp, joltageSize);
    for (int j = 0; j < joltageSize; j++) {
        glp_set_row_bnds(lp, j + 1, GLP_FX, spec.joltage[j], spec.joltage[j]);
    }
    
    std::vector<int> ia, ja;
    std::vector<double> ar;
    
    ia.push_back(0);
    ja.push_back(0);
    ar.push_back(0.0);
    
    for (int buttonIdx = 0; buttonIdx < numButtons; buttonIdx++) {
        for (int pos : spec.buttons[buttonIdx]) {
            if (pos >= 0 && pos < joltageSize) {
                ia.push_back(pos + 1);
                ja.push_back(buttonIdx + 1);
                ar.push_back(1.0);
            }
        }
    }
    
    glp_load_matrix(lp, ar.size() - 1, ia.data(), ja.data(), ar.data());
    glp_simplex(lp, NULL);
    glp_intopt(lp, NULL);
    
    int status = glp_mip_status(lp);
    int result = -1;
    
    if (status == GLP_OPT || status == GLP_FEAS) {
        result = (int)(glp_mip_obj_val(lp) + 0.5);
    }
    
    glp_delete_prob(lp);
    return result;
}

long long solvePart2(const std::vector<MachineSpec>& specs) {
    long long totalSteps = 0;
    
    for (size_t idx = 0; idx < specs.size(); idx++) {
        const auto& spec = specs[idx];
        
        int minSteps = solveILP(spec);
        
        if (minSteps != -1) {
            totalSteps += minSteps;
        } else {
            std::cerr << "Machine " << idx + 1 << ": NOT FOUND\n";
        }
    }
    
    return totalSteps;
}

int main() {
    auto specs = parseInput("input10");
    
    std::cout << "Part 1: " << solvePart1(specs) << '\n';
    std::cout << "Part 2: " << solvePart2(specs) << '\n';
}
