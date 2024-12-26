#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>
#include <iterator>
#include <cmath>

using namespace std;

enum OPCODE {ADV = 0, BXL, BST, JNZ, BXC, OUT, BDV, CDV};

int A, B, C;
vector<int> opcodes;

int initReg(const string& s) {
    regex pattern("\\d+");
    auto begin = sregex_iterator(s.begin(), s.end(), pattern);
    return stoi(begin->str());
}

vector<int> initOpcodes(const string& s) {
    regex pattern("\\d+");
    auto begin = sregex_iterator(s.begin(), s.end(), pattern);
    auto end = sregex_iterator();
    vector<int> opcodes;
    for (auto it = begin; it != end; ++it) {
        opcodes.push_back(stoi(it->str()));
    }
    return opcodes;
}

int combo(int op) {
    if (op < 4) return op;
    switch (op) {
        case 4: return A;
        case 5: return B;
        case 6: return C;
    }
    return 0;
}

vector<int> run(int value) {
    int ip{};
    vector<int> output;
    A = value;
    B = 0;
    C = 0;
    while (ip < opcodes.size()) {
        if (opcodes[ip] == JNZ && A != 0) {
            ip = opcodes[ip + 1];
        }
        switch (opcodes[ip]) {
            case ADV: A /= pow(2, combo(opcodes[ip + 1])); break;
            case BXL: B ^= opcodes[ip + 1]; break;
            case BST: B = combo(opcodes[ip + 1]) % 8; break;
            case BXC: B ^= C; break;
            case OUT: output.push_back(combo(opcodes[ip + 1]) % 8); break;
            case BDV: B = A / pow(2, combo(opcodes[ip + 1])); break;
            case CDV: C = A / pow(2, combo(opcodes[ip + 1])); break;
        }
        ip += 2;
    }
    copy(output.begin(), output.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    return output;
}

long long findRegA(int value) {
    for (int i = 0; i < 8; ++i) {
        int testA = (value << 3) + i;
        auto out = run(testA);
        if (equal(prev(opcodes.end(), out.size()), opcodes.end(), out.begin(), out.end())) { 
            if (out.size() == opcodes.size()) {
                return testA;
            } else {
                if (auto res = findRegA(testA)) return res;
            }
        }
    }
    return {};
}

int main() {
    fstream in("input17");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    string line;
    getline(in, line);
    A = initReg(line);
    getline(in, line);
    B = initReg(line);
    getline(in, line);
    C = initReg(line);
    getline(in, line);
    getline(in, line);
    opcodes = initOpcodes(line);

    cout << "Result: "; 
    run(A);

    return 0;
}
