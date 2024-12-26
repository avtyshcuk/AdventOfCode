#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <sstream>

using namespace std;

long long step(long long num) {
    auto first = num * 64;
    num ^= first;
    num %= 16777216;
    auto second = num / 32;
    num ^= second;
    num %= 16777216;
    auto third = num * 2048;
    num ^= third;
    num %= 16777216;
    return num;
}

int lastDigit(long long num) {
    string s = to_string(num);
    int n = s.length();
    string sub = s.substr(n - 1);
    return stoi(sub);
}

vector<int> seqToVec(const string& s) {
    vector<int> res;
    stringstream ss(s);
    int num;
    while (ss >> num) {
        res.push_back(num);
    }
    return res;
}

int findProfit(string& diff, vector<int>& diffVec, vector<int>& price, const string& seq) {
    auto pos = diff.find(seq);
    if (pos != string::npos) {
        auto seqVec = seqToVec(seq);
        for (int i = 3; i < diffVec.size(); ++i) {
            if (diffVec[i - 3] == seqVec[0] && diffVec[i - 2] == seqVec[1] && diffVec[i - 1] == seqVec[2] && diffVec[i] == seqVec[3]) {
                return price[i + 1];
            }
        }
    }
    return 0;
}

int main() {
    fstream in("input22");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    vector<long long> nums;
    for (string line; getline(in, line);) {
        nums.push_back(stoll(line));
    }
    long long result{};
    vector<vector<int>> prices;
    for (long long num : nums) {
        vector<int> price{lastDigit(num)};
        for (int i = 0; i < 2000; ++i) {
            num = step(num);
            price.push_back(lastDigit(num));
        }
        prices.push_back(price);
    }
    vector<vector<int>> diffs;
    for (auto& price : prices) {
        vector<int> diff;
        for (int i = 1; i < price.size(); ++i) {
            diff.push_back(price[i] - price[i - 1]);
        }
        diffs.push_back(diff);
    }
    unordered_set<string> sequences;
    for (auto& diff : diffs) {
        for (int i = 3; i < diff.size(); ++i) {
            string key;
            for (int j = 3; j >= 0; --j) {
                key += to_string(diff[i - j]) + " ";
            }
            sequences.insert(key);
        }
    }
    vector<string> strDiffs;
    for (auto& diff : diffs) {
        stringstream ss;
        for (auto num : diff) {
            ss << num << " ";
        }
        strDiffs.push_back(ss.str());
    }
    int count{};
    for (const auto& seq : sequences) {
        long long profit{};
        for (int i = 0; i < strDiffs.size(); ++i) {
            profit += findProfit(strDiffs[i], diffs[i], prices[i], seq);
        }
        result = max(result, profit);
        count++;
    }
    cout << "Result: " << result << endl;
    return 0;
}
