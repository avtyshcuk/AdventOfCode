#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iterator>
#include <set>
#include <bitset>

using namespace std;

unordered_map<string, int> values;

enum Op {AND = 0, OR, XOR};

struct Rule {
    string gate1;
    string gate2;
    Op operation;
    string output;
};

Op getOp(const string& s) {
    if (s == "AND") return AND;
    if (s == "OR") return OR;
    if (s == "XOR") return XOR;
    return {};
}

int applyOp(int value1, int value2, Op op) {
    switch (op) {
        case AND: return value1 & value2;
        case OR: return value1 | value2;
        case XOR: return value1 ^ value2;
    }
    return {};
}

string findGateByC(const string& gate, vector<Rule>& rules) {
    vector<string> newGates;
    for (const auto& rule : rules) {
        if (rule.gate1 == gate || rule.gate2 == gate) {
            cout << rule.gate1 << "  " << rule.gate2 << "  " << rule.output << endl;
            if (rule.output[0] == 'z') {
                string indexGate = rule.output.substr(1);
                int decValue = stoi(indexGate) - 1;
                string minusOne = to_string(decValue);
                if (minusOne.length() == 1) {
                    minusOne = "0" + minusOne;
                }
                return "z" + minusOne;
            } else {
                newGates.push_back(rule.output);
            }
        }
    }
    for (const auto& gate : newGates) {
        string res = findGateByC(gate, rules);
        if (!res.empty()) {
            return res;
        }
    }
    return {};
}

int main() {
    fstream in("input24");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    for (string line; getline(in, line);) {
        if (line.empty()) break;
        stringstream ss{line};
        string gate;
        getline(ss, gate, ':');
        getline(ss, line, ' ');
        string val;
        getline(ss, val);
        values[gate] = stoi(val);
    }
    vector<Rule> rules;
    for (string line; getline(in, line);) {
        stringstream ss{line};
        string gate1;
        getline(ss, gate1, ' ');
        string operation;
        getline(ss, operation, ' ');
        string gate2;
        getline(ss, gate2, ' ');
        getline(ss, line, ' ');
        string output;
        getline(ss, output, ' ');
        Rule rule{gate1, gate2, getOp(operation), output};
        rules.push_back(rule);
    }
    auto findInput = [&](unordered_map<string, int>& values, char c) -> unsigned long long {
        vector<pair<string, unsigned long long>> outputValues;
        for (auto& [gate, value] : values) {
            if (gate[0] != c) continue;
            outputValues.push_back({gate, value});
        }
        sort(outputValues.begin(), outputValues.end());

        unsigned long long result{};
        int count{};
        for (auto val : outputValues) {
            result |= (val.second << count);
            count++;
        }
        return result;
    };

    auto step = [&](const vector<Rule>& rules) {
        queue<Rule> q;
        for (auto& rule : rules) {
            q.push(rule);
        }
        while (!q.empty()) {
            auto rule = q.front();
            q.pop();
            if (values.count(rule.gate1) && values.count(rule.gate2)) {
                int out = applyOp(values[rule.gate1], values[rule.gate2], rule.operation);
                values[rule.output] = out;
            } else {
                q.push(rule);
            }
        }
    };

    unsigned long long x{}, y{};
    x = findInput(values, 'x');
    y = findInput(values, 'y');

    vector<string> notXorZ;
    for (const auto& rule : rules) {
        if (rule.output == "z45") continue;
        if (rule.operation != XOR && rule.output[0] == 'z') {
            notXorZ.push_back(rule.output);
        }
    }
    copy(notXorZ.begin(), notXorZ.end(), ostream_iterator<string>(cout, " "));

    vector<string> xorNotXY;
    for (const auto& rule : rules) {
        if (rule.operation == XOR && rule.gate1[0] != 'x' && rule.gate2[0] != 'x' && rule.output[0] != 'z') {
            xorNotXY.push_back(rule.output);
        }
    }
    copy(xorNotXY.begin(), xorNotXY.end(), ostream_iterator<string>(cout, " "));

    for (auto& gate : xorNotXY) {
        auto fail = findGateByC(gate, rules);
        cout << fail << "  " << gate << endl; 
        
        int index1{}, index2{};
        for (int i = 0; i < rules.size(); ++i) {
            if (rules[i].output == gate) {
                index1 = i; 
            } else if (rules[i].output == fail) {
                index2 = i;
            }
        }
        swap(rules[index1].output, rules[index2].output);
    }
    step(rules);

    unsigned long long z = findInput(values, 'z');
    auto xorExpCurr = (x + y) ^ z;
    cout << "x + y " << x + y << endl;
    cout << "z " << z << endl;

    int trailing = __builtin_ctzll(xorExpCurr);
    cout << "Trailing: " << trailing << endl;
    string gateIndex = to_string(trailing);
    if (gateIndex.length() == 1) {
        gateIndex = "0" + gateIndex;
    }

    vector<string> failedCarry;
    for (const auto& rule : rules) {
        if (rule.gate1 == "x" + gateIndex && rule.gate2 == "y" + gateIndex) {
            failedCarry.push_back(rule.output);
        }
    }

    vector<string> finalGatesList;
    finalGatesList.reserve(failedCarry.size() + notXorZ.size() + xorNotXY.size());
    finalGatesList.insert(finalGatesList.end(), failedCarry.begin(), failedCarry.end());
    finalGatesList.insert(finalGatesList.end(), notXorZ.begin(), notXorZ.end());
    finalGatesList.insert(finalGatesList.end(), xorNotXY.begin(), xorNotXY.end());
    sort(finalGatesList.begin(), finalGatesList.end());

    stringstream ss;
    for (int i = 0; i < finalGatesList.size(); ++i) {
        string delimiter = i != finalGatesList.size() - 1 ? "," : "";
        ss << finalGatesList[i] << delimiter; 
    }
    cout << "Result: " << ss.str() << endl;

    return 0;
}
