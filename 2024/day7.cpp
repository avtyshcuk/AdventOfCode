#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

bool canEvaluate(const vector<long long>& nums, int index, long long sum) {
    int n = nums.size();
    if (index == n - 1) {
        return sum == nums[index];
    }
    if (canEvaluate(nums, index + 1, sum + nums[index])) {
        return true;
    }
    if (canEvaluate(nums, index + 1, sum * nums[index])) {
        return true;
    }
    return canEvaluate(nums, index + 1, stoll(to_string(sum) + to_string(nums[index])));
}

int main() {
    fstream in("input7");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    vector<vector<long long>> eqs;
    for (string line; getline(in, line);) {
        stringstream ss(line);
        string sumString;
        getline(ss, sumString, ':'); 
        long long sum = stol(sumString);
        vector<long long> ops;
        long long num;
        while (ss >> num) {
            ops.push_back(num);
        }
        ops.push_back(sum);
        eqs.push_back(ops);
    }
    long long result{};
    for (const vector<long long>& eq : eqs) {
        if (canEvaluate(eq, 1, eq[0])) {
            result += eq[eq.size() - 1];
        }
    }
    cout << "Result: " << result << endl;
    return 0;
}
