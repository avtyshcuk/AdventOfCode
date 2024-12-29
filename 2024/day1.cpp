#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int main() {
    ifstream in("input1");
    if (!in) {
        cerr << "Error: could not open the file!\n";
        return -1;
    }
    int val1, val2;
    vector<int> list1, list2;
    while (in >> val1 >> val2) {
        list1.push_back(val1);
        list2.push_back(val2);
    }
    unordered_map<int, int> freq;
    for (int num : list2) {
        freq[num]++;
    }
    int similarity{};
    for (int num : list1) {
        int score = freq.count(num) ? freq[num] : 0;
        similarity += num * score;
    }
    cout << "Result: " <<  similarity << endl;;
    return 0;
}
