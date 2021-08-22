#include "csv.h"

#include <iostream>
#include <vector>
#include <set>
#include "args.h"
#include <algorithm>
#include <assert.h>
#include <list>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <climits>
#include <ctime>
using namespace std;

unsigned long long memInBytes = 0;
unsigned long long currMemInBytes = 0;
void updateMem(int toAdd){
    currMemInBytes += toAdd;
    memInBytes = max(memInBytes, currMemInBytes);
}

class GKTriple {
    public:
        int x;
        int g;
        int delta;

        GKTriple(int a, int b, int c) : x(a), g(b), delta(c) {
            updateMem(sizeof(GKTriple));
        }
        GKTriple(): x(INT_MAX), g(0), delta(0){}
        bool operator<(const GKTriple &other) const {
            return x < other.x;
        }

        string toString(){
            return "(" + to_string(x) + "," + to_string(g) + "," + to_string(delta) + ")";
        }
};

class GKSketch{
    private:
        int N;
        double eps;
        double cap;
        list<GKTriple> L;

    public:
        GKSketch(double e) : eps(e){
            N = 0;
            cap = 0;
            memInBytes = currMemInBytes = 0;
            auto it = L.end();
            L.insert(it, GKTriple(INT_MAX, 1, 0));

            updateMem(sizeof(GKSketch));
        }

        void compressOneIfPossible(){
            auto it = L.begin();
            updateMem(sizeof(int));
            GKTriple inf = GKTriple(INT_MAX,0,0);
            while(*(it) < inf){
                GKTriple &curr = *it;
            updateMem(sizeof(int));

                it++;
                GKTriple &next = *it;
            updateMem(sizeof(int));

                if(double(curr.g + next.g + next.delta) < cap){
                    next.g += curr.g;
                    it--;
                    L.erase(it);
                    updateMem(-(long long)sizeof(GKTriple));
                    return;
                }
            }
            updateMem(-(long long)sizeof(int));
            updateMem(-(long long)sizeof(int));
            updateMem(-(long long)sizeof(int));
        }

        void update(int x){
            N++;
            cap = ceil(2.0*getAcceptableError());
            GKTriple aux = GKTriple(x,0,0);
            auto firstBiggerIt = upper_bound(L.begin(), L.end(), aux);
            updateMem((long long)sizeof(int));

            GKTriple &firstBigger = *firstBiggerIt;
    
            if (double(firstBigger.g + firstBigger.delta + 1) < cap){
                firstBigger.g++;
            }else{
                int g = firstBigger.g, delta = firstBigger.delta;
            updateMem((long long)sizeof(int));
            updateMem((long long)sizeof(int));

                L.insert(firstBiggerIt, GKTriple(x, 1, g + delta - 1));
                compressOneIfPossible();
            updateMem(-(long long)sizeof(int));
            updateMem(-(long long)sizeof(int));
            
            }
        }

        double rank(int x){
            GKTriple aux = GKTriple(x,0,0);
            auto firstBiggerIt = upper_bound(L.begin(), L.end(), aux);
            updateMem((long long)sizeof(int));

            GKTriple &firstBigger = *firstBiggerIt;
            double rank = (firstBigger.g + firstBigger.delta)/2.0 - 1.0;
            updateMem((long long)sizeof(double));
            
            updateMem((long long)sizeof(int));
            for(auto it = L.begin(); it != firstBiggerIt; it++){
                GKTriple &triple = *it;
                updateMem((long long)sizeof(int));
                rank += triple.g;
            }


            updateMem(-(long long)sizeof(int));
            updateMem(-(long long)sizeof(int));
            updateMem(-(long long)sizeof(int));
            updateMem(-(long long)sizeof(double));

            return rank;
        }

        int quantile(double r){
            r *= N;
            GKTriple aux = GKTriple();
            if(r + 1 > N - getAcceptableError()){
                aux = *(++L.rbegin());
                return aux.x;
            }

            int gSum = 0;
            double currRank = 0;
            updateMem((long long)sizeof(int));
            updateMem((long long)sizeof(double));
            updateMem((long long)sizeof(int));
            for(auto it = L.begin(); it != L.end(); it++){
                auto nextIt = it; nextIt++;
                updateMem((long long)sizeof(int));

                GKTriple &curr = *it;
                GKTriple &next = *nextIt;
                updateMem((long long)sizeof(int));
                updateMem((long long)sizeof(int));


                gSum += curr.g;
                if(next.x > curr.x){
                    double currRank = double(next.g + next.delta)/2.0 - 1.0 + gSum;
                    updateMem((long long)sizeof(double));

                    if(currRank > r - getAcceptableError()){
                        updateMem(-(long long)sizeof(int));
                        updateMem(-(long long)sizeof(double));
                        updateMem(-(long long)sizeof(int));
                        updateMem(-(long long)sizeof(int));
                        updateMem(-(long long)sizeof(int));
                        updateMem(-(long long)sizeof(int));
                        updateMem(-(long long)sizeof(double));
                        return curr.x;
                    }
                }
            }
            assert(false); // hopefully never gets here
            return -1;
        }

        double getAcceptableError(){
            return eps*N;
        }

        int getSize(){
            return N;
        }
};


class Teste {
    public:
    Teste() {}

    void runTest(
        int column, 
        double eps, 
        int univ, 
        vector<int> &rankQueries, 
        vector<double> &quantQueries, 
        string datasetFilename, 
        string queryType
    ) {
        memInBytes = currMemInBytes = 0;

        GKSketch sketch = GKSketch(eps);
        double totalTime = 0;
        ofstream myfile;
        string filename = queryType + "-" + to_string(column) + ".csv";
        myfile.open(filename, fstream::app);

        // READ CSV AND UPDATE SKETCH
        CSVReader reader(datasetFilename, column);
        
        vector<int> trueRanks(univ);

        while(reader.hasNext()) {
            int value = reader.getNextValue();
            if(value >= univ) continue;
            trueRanks[value]++;
            clock_t timeBegin = clock();
            sketch.update(value);
            clock_t timeEnd = clock();
            totalTime += double(timeEnd - timeBegin) / CLOCKS_PER_SEC;
        }

        for(int i = 1; i < univ; i++) {
            trueRanks[i] += trueRanks[i-1];
        }
    

        if(eps == 0.05 && univ == 100) {
            if(queryType == "rank") {
                myfile << "column,eps,univ,queryType,time,memory,queryValue,estimate,correctValue,----,minEstimate,maxEstimate,withinBounds\n";
            } else {
                myfile << "column,eps,univ,queryType,time,memory,queryValue,estimate,proposedRank,achievedRank,minEstimate,maxEstimate,withinBounds\n";
            }
        }

        clock_t timeBegin = clock();
        // MAKE QUERIES
        if(queryType == "rank") {
            for(auto query : rankQueries) {
                int rank = sketch.rank(query);

                int minRank = (trueRanks[query] - eps*sketch.getSize());
                int maxRank = (trueRanks[query] + eps*sketch.getSize());
                // cout << minRank << " " << maxRank << endl;
                bool correctRank = minRank <= rank && rank <= maxRank;
                myfile << (to_string(column) + "," + to_string(eps) + "," + to_string(univ) + "," + queryType + ",,," + to_string(query) + "," + to_string(rank) + "," + to_string(trueRanks[query]) + ",," + to_string(minRank) + "," + to_string(maxRank) + "," + to_string(correctRank) + "\n");
            }
        } else {
            for(auto query : quantQueries) {
                int quantile = sketch.quantile(query);

                int proposedRank = query * sketch.getSize();
                int minRank = (proposedRank - eps*sketch.getSize());
                int maxRank = (proposedRank + eps*sketch.getSize());
                bool correctQuantile = minRank <= trueRanks[quantile] && trueRanks[quantile] <= maxRank;
                myfile << (to_string(column) + "," + to_string(eps) + "," + to_string(univ) + "," + queryType + ",,," + to_string(query) + "," + to_string(quantile) + "," + to_string(proposedRank) + "," + to_string(trueRanks[quantile]) + "," + to_string(minRank) + "," + to_string(maxRank) + "," + to_string(correctQuantile) + "\n");   
            }
        }

        clock_t timeEnd = clock();
        totalTime += double(timeEnd - timeBegin) / CLOCKS_PER_SEC;
        myfile << (to_string(column) + "," + to_string(eps) + "," + to_string(univ) + "," + queryType + "," + to_string(totalTime) + "," + to_string(memInBytes) + ",,,,,,," + "\n");
        myfile.close();
    }
};


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


int main(int args, char **argv){
    ios::sync_with_stdio(0); cin.tie(0);

    vector<double> epsVals = {0.05, 0.1, 0.15, 0.2};
    vector<int> univVals = {100, 1000, 10000};
    vector<int> columns = {4, 5, 6, 7};
    vector<double> quantQueries = getQuantQueries();
    vector<vector<int>> rankQueries = getRankQueries(univVals);
    string datasetFilename = "network_flows.csv";
    
    //RANK QUERIES
    for(auto column : columns) {
        // CHNAGE CSV
        int univCount = 0;

        for(auto eps : epsVals) {
            for(int u = 0; u < univVals.size(); u++) {
                int univ = univVals[u];
                Teste *teste = new Teste();
                teste->runTest(column, eps, univ, rankQueries[u], quantQueries, "network_flows.csv", "rank");
                univCount++;
            }
        }
    }

    // QUANT QUERIES
    for(auto column : columns) {
        // CHNAGE CSV

        for(auto eps : epsVals) {
            for(int u = 0; u < univVals.size(); u++) {
                int univ = univVals[u];
                Teste *teste = new Teste();
                teste->runTest(column, eps, univ, rankQueries[u], quantQueries, "network_flows.csv", "quant");
            }
        }
    }
}