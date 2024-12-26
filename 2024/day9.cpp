#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main() {
    fstream in("input9");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    string code;
    getline(in, code);
    vector<string> out;
    int fileID{};
    for (int i = 0; i < code.length(); ++i) {
        int num = code[i] - '0';
        for (int j = 0; j < num; ++j) {
            if (i % 2 == 0) {
                out.push_back(to_string(fileID));
            } else {
                out.push_back(".");
            }
        }
        if (i % 2 == 0) fileID++;
    }
    int n = out.size();
    vector<pair<int, int>> free;
    int count{};
    for (int i = 0; i < n; ++i) {
        if (out[i] == ".") {
            count++;
        } else {
            if (count != 0) {
                free.push_back({i - count, count});
            }
            count = 0;
        }
    }
    vector<pair<int, int>> occup;
    count = 0;
    for (int i = n - 1; i >= 0; --i) {
        if (out[i] == ".") { 
            if (count > 0) {
                occup.push_back({i + 1, count});
            }
            count = 0;
        } else {
            if (out[i] == out[i + 1]) {
                count++;
            } else {
                if (count > 0) {
                    occup.push_back({i + 1, count});
                }
                count = 1; 
            }
        }
    }
    if (count > 0) {
        occup.push_back({0, count});
    }
    for (pair p : occup) {
        cout << p.first << " " << p.second << endl; 
    }

    for (auto file : occup) {
        for (auto& p : free) {
            int size = file.second;
            if (p.first < file.first && p.second >= size) {
                int posFile = file.first;
                string s = out[posFile];
                int posFree = p.first;
                for (int i = posFree; i < posFree + size; ++i) {
                    out[i] = s;
                }
                for (int i = posFile; i < posFile + size; ++i) {
                    out[i] = ".";
                }
                p.second -= size;
                p.first += size;
                break;
            }
        }
    }

    copy(out.begin(), out.end(), ostream_iterator<string>(cout, ""));
    cout << endl;
        
    long long checksum{};
    int cnt{};
    for (string s : out) {
        if (s != ".") { 
            checksum += stoll(s) * cnt;
        }
        cnt++;
    }
    cout << "Result: " << checksum << endl;
    return 0;
}
