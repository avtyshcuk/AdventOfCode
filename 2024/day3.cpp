#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>

using namespace std;

int main() {
    fstream in("input3");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    string line;
    string firstPart = "mul(";
    int count{};
    bool isFirst{true};
    bool isSecond{false};
    bool isThird{false};
    int firstOp{}, secondOp{};
    unordered_set<char> digits{'0', '1', '2', '3','4','5','6','7','8','9'};
    long long sum{};
    string enable = "do";
    string disable = "don't";
    bool enabled{true};
    int countDisable{};
    int countEnable{};
    while (getline(in, line)) {
        for (char c : line) {
            if (enabled) {
                if (c == disable[countDisable]) {
                    if (countDisable == disable.size() - 1) {
                        enabled = false;
                        countDisable = 0;
                    } else {
                        countDisable++;
                    }
                } else {
                    countDisable = 0;
                }
            } else {
                if (countEnable == enable.size() && c != 'n') {
                    enabled = true;
                    countEnable = 0;
                } else if (c == enable[countEnable]) {
                    countEnable++;
                } else {
                    countEnable = 0;
                }
            }

            if (!enabled) continue;

            if (isFirst) {
                if (c == firstPart[count]) {
                    if (count == firstPart.size() - 1) {
                        count = 0;
                        isSecond = true;
                        isFirst = false;
                    } else {
                        count++;
                    }
                } else {
                    count = 0;
                }
            } else if (isSecond) {
                if (digits.count(c)) {
                    int d = c - '0';
                    if (firstOp == 0) {
                        firstOp = d;
                    } else {
                        firstOp = firstOp * 10 + d;
                    }
                } else if (c == ',' && firstOp != 0) {
                    isSecond = false;
                    isThird = true;
                } else {
                    isSecond = false;
                    isFirst = true;
                    firstOp = 0;
                }
            } else if (isThird) {
                if (digits.count(c)) {
                    int d = c - '0';
                    if (secondOp == 0) {
                        secondOp = d;
                    } else {
                        secondOp = secondOp * 10 + d;
                    }
                } else if (c == ')' && secondOp != 0) {
                    sum += firstOp * secondOp;
                    isThird = false;
                    isFirst = true;
                    firstOp = secondOp = 0;
                } else {
                    isThird = false;
                    isFirst = true;
                    firstOp = secondOp = 0;
                }
            }
        }
    }
    cout << "Result: " << sum << endl;
    return 0;
}
