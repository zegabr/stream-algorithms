#include "QDigestSketchTest.h"
#include<bits/stdc++.h>

using namespace std;

vector<double> getQuantQueries() {
    vector<double> queries;
    for(double x = 0.005; x < 1; x += 0.005) {
        queries.push_back(x);
    }
    return queries;
}

vector<vector<int>> getRankQueries(vector<int> &univVals) {
    vector<vector<int>> queries;

    for(int k = 0; k < univVals.size(); k++) {
        int val = univVals[k];
        queries.push_back({});
        for(int i = 0; i < val; i++) {
            queries[k].push_back(i);
        }
    }

    return queries;
}

void printArr(vector<int> &arr) {
    for(auto item : arr) cout << item << " ";
    cout << endl;
}

void printArr(vector<double> &arr) {
    for(auto item : arr) cout << item << " ";
    cout << endl;
}

int main() {
    vector<double> epsVals = {0.05, 0.1, 0.15, 0.2};
    vector<int> univVals = {100, 1000, 10000};
    vector<int> columns = {4, 5, 6, 7};
    vector<double> quantQueries = getQuantQueries();
    vector<vector<int>> rankQueries = getRankQueries(univVals);

    RunQDigest qdigest;

    cout << "column,eps,univ,queryType,time,memory,queryValue,estimate,correctValue/proposedRank,----/achievedRank,minEstimate,maxEstimate,withinBounds\n";

    // RANK QUERIES
    for(auto column : columns) {
        // CHNAGE CSV
        int univCount = 0;

        for(auto eps : epsVals) {
            for(auto univ : univVals) {
                qdigest.runQdigest(column, eps, univ, rankQueries[univCount], quantQueries, "network_flows.csv", "rank");
                univCount++;
            }
        }
    }

    // QUANT QUERIES
    for(auto column : columns) {
        // CHNAGE CSV

        for(auto eps : epsVals) {
            for(auto univ : univVals) {
                qdigest.runQdigest(column, eps, univ, rankQueries[0], quantQueries, "network_flows.csv,", "quant");
            }
        }
    }
}