#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

map<pair<long long, long long>, long long> memo;

struct TreeNode {
    long long val{};
    TreeNode* left{};
    TreeNode* right{};
    TreeNode(long long v) : val(v) {}
};

const int steps = 75;

long long generate(TreeNode* node, int depth) {
    if (depth == steps) {
        return 1;
    }
    if (memo[{node->val, steps - depth}] != 0) {
        return memo[{node->val, steps - depth}];
    }

    long long res{};
    string s = to_string(node->val);
    if (node->val == 0) {
        node->left = new TreeNode{1};
        res += generate(node->left, depth + 1);
    } else if (s.length() % 2 == 0) {
        int l = s.length();
        string left = s.substr(0, l/2);
        string right = s.substr(l/2);
        node->left = new TreeNode{stoll(left)};
        node->right = new TreeNode{stoll(right)};
        res += generate(node->left, depth + 1);
        res += generate(node->right, depth + 1);
    } else {
        node->left = new TreeNode{node->val * 2024};
        res += generate(node->left, depth + 1);
    }
    return memo[{node->val, steps - depth}] = res;
}

int main() {
    fstream in("input11");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    string line;
    getline(in, line);
    stringstream ss(line);
    vector<TreeNode> roots;
    long long num;
    while (ss >> num) {
        roots.push_back(TreeNode{num});
    }
    long long result{};
    for (auto node : roots) {
        result += generate(&node, 0);
    }
    cout << "Result: " << result << endl;
    return 0;
}
