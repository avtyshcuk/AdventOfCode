#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

vector<string> dict;
vector<string> words;
long total{};

void parse() {
    fstream in("input19");
    if (!in) {
        cerr << "Input file not found!\n";
        return;
    }
    string dictLine;
    getline(in, dictLine);
    stringstream ss(dictLine);
    string dictWord;
    while (getline(ss, dictWord, ',')) {
        dictWord.erase(0, dictWord.find_first_not_of(" "));
        dict.push_back(dictWord);
    }
    string line;
    getline(in, line);
    while (getline(in, line)) {
        words.push_back(line);
    }
}

bool inDict(string s) {
    for (auto& word : dict) {
        if (s == word) return true;
    }
    return false;
}

map<string, long> memo;

long canBreak(const string& word) {
    cout << word << endl;
    int n = word.length();
    if (n == 0) {
        return 1;
    }
    if (memo.count(word)) {
        return memo[word];
    }
    long result{};
    for (int i = 0; i < n; ++i) {
        auto sub = word.substr(0, i + 1);
        if (inDict(sub)) {
            result += canBreak(word.substr(i + 1));
        }
    }
    return memo[word] = result;
}

int main() {
    parse();
    vector<bool> answer;
    for (auto& word : words) {
        total += canBreak(word);
    }
    cout << "Result: " << total << endl;
    return 0;
}
