#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <set>

using namespace std;

struct HashVector {
    template <typename T>
    size_t operator()(const vector<T>& v) const {
        stringstream ss;
        for (string s : v) {
            ss << s << "|";
        }
        string s = ss.str();
        return hash<string>{}(s);
    }
};


unordered_map<int, set<int>> adj;

void bronKerbosch(set<int>& R, set<int>& P, set<int>& X, vector<set<int>>& cliques) {
    if (P.empty() && X.empty()) {
        cliques.push_back(R);
        return;
    }
    set<int> P_copy = P;
    int pivot = *P_copy.begin();
    set<int> P_new = P;
    for (int v : P_copy) {
        if (adj[pivot].find(v) == adj[pivot].end()) {
            set<int> newR = R;
            newR.insert(v);

            set<int> newP, newX;
            for (int neighbor : adj[v]) {
                if (P.find(neighbor) != P.end()) {
                    newP.insert(neighbor);
                }
            }
            for (int neighbor : adj[v]) {
                if (X.find(neighbor) != X.end()) {
                    newX.insert(neighbor);
                }
            }
            bronKerbosch(newR, newP, newX, cliques);
            P.erase(v);
            X.insert(v);
        }
    }
}

int main() {
    fstream in("input23");
    if (!in) {
        cerr << "Input file not found!\n";
        return -1;
    }
    unordered_set<string> hostsNames;
    unordered_map<string, int> hostsIds;
    unordered_map<int, string> idsHosts;
    int uniqueCount{};
    for (string line; getline(in, line);) {
        stringstream ss(line);
        string hostName;
        getline(ss, hostName, '-');
        if (!hostsIds.count(hostName)) {
            hostsIds[hostName] = uniqueCount;
            idsHosts[uniqueCount] = hostName;
            uniqueCount++;
        }
        hostsNames.insert(hostName);
        int id1 = hostsIds[hostName];
        
        getline(ss, hostName);
        if (!hostsIds.count(hostName)) {
            hostsIds[hostName] = uniqueCount;
            idsHosts[uniqueCount] = hostName;
            uniqueCount++;
        }
        hostsNames.insert(hostName);
        int id2 = hostsIds[hostName];

        adj[id1].insert(id2);
        adj[id2].insert(id1);
    }
    set<int> R, P, X;
    for (int i = 0; i < uniqueCount; ++i) {
        P.insert(i);
    }
    vector<set<int>> cliques;
    bronKerbosch(R, P, X, cliques);
    vector<int> maxClique;
    for (const auto& clique : cliques) {
        if (clique.size() > maxClique.size()) {
            maxClique.assign(clique.begin(), clique.end());
        }
    }
    vector<string> names;
    for (auto node : maxClique) {
        names.push_back(idsHosts[node]);
    }
    sort(names.begin(), names.end());

    cout << "Result: ";
    copy(names.begin(), names.end(), ostream_iterator<string>(cout, ","));
    cout << endl;
    return 0;
}
