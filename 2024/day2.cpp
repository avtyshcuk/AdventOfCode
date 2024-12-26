#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

enum class SeqType {UNKNOWN = 0, UP, DOWN};

bool isSafe(const vector<int>& seq) {
    int diff = seq[1] - seq[0];
    if (diff == 0 || abs(diff) > 3) return false;
    SeqType seqType = diff > 0 ? SeqType::UP : SeqType::DOWN;
    for (int i = 2; i < seq.size(); ++i) {
        diff = seq[i] - seq[i - 1];
        if (diff == 0 || abs(diff) > 3) {
            return false;
        }
        if (seqType == SeqType::UP && diff < 0 || seqType == SeqType::DOWN && diff > 0) {
            return false;
        }
    }
    return true;
}

int main() {
    fstream in("input2");
    if (!in) {
        cerr << "Input file is not found!\n";
        return -1;
    }

    int total{};
    for (string line; getline(in, line);) {
        stringstream ss(line);
        int num;
        
        vector<int> numbers;
        while (ss >> num) {
            numbers.push_back(num);
        }

        int n = numbers.size();
        for (int i = 0; i < n; ++i) {
            vector<int> newNumbers(n - 1);
            if (i == 0) {
                for (int j = 1; j < n; ++j) {
                    newNumbers[j - 1] = numbers[j];
                }
            } else if (i == n - 1) {
                for (int j = 0; j < n - 1; ++j) {
                    newNumbers[j] = numbers[j];
                }
            } else {
                for (int j = 0; j < i; ++j) {
                    newNumbers[j] = numbers[j];
                }
                for (int j = i + 1; j < n; ++j) {
                    newNumbers[j - 1] = numbers[j];
                }
            }
            if (isSafe(newNumbers)) {
                total++;        
                break;
            }
        }
    } 
    cout << "Result: " <<  total << endl;
    return 0;
}
