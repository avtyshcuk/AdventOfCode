#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int main() {
    fstream in_rules("input5_rules");
    if (!in_rules) {
        cerr << "Input file not found!\n";
        return -1;
    }
    unordered_map<int, unordered_set<int>> deps;
    for (string line; getline(in_rules, line);) {
        stringstream ss(line);
        int in;
        ss >> in;
        if (ss.peek() == '|') ss.ignore();
        int out;
        ss >> out;
        deps[in].insert(out);
    }

    fstream in_pages("input5_pages");
    if (!in_pages) {
        cerr << "Input file not found!\n";
        return -1;
    }
    int result{};
    for (string line; getline(in_pages, line);) {
        stringstream ss(line);
        string num;
        vector<int> pages;
        while (getline(ss, num, ',')) {
            pages.push_back(stoi(num));
        }
        bool isValid{true};
        int n = pages.size();
        for (int i = 1; i < n; ++i) {
            for (int j = i - 1; j >= 0; --j) {
                if (deps[pages[i]].count(pages[j])) {
                    isValid = false;
                    break;
                }
            }
        }
        if (!isValid) {
            sort(pages.begin(), pages.end(), [&](int n1, int n2) {
                if (deps[n1].count(n2)) return true;
                return false;
            });
            copy(pages.begin(), pages.end(), ostream_iterator<int>(cout, " "));
            cout << endl;
            result += pages[n/2];
        }
    }
    cout << "Result: " << result << endl;;
    return 0;
}
