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


class GKTriple {
    public:
        int x;
        int g;
        int delta;

        GKTriple(int a, int b, int c) : x(a), g(b), delta(c) {}
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
            auto it = L.end();
            L.insert(it, GKTriple(INT_MAX, 1, 0));
        }

        void compressOneIfPossible(){
            auto it = L.begin();
            GKTriple inf = GKTriple(INT_MAX,0,0);
            while(*(it) < inf){
                GKTriple &curr = *it;
                it++;
                GKTriple &next = *it;
                if(double(curr.g + next.g + next.delta) < cap){
                    next.g += curr.g;
                    it--;
                    L.erase(it);
                    return;
                }
            }
        }

        void update(int x){
            N++;
            cap = ceil(2.0*getAcceptableError());
            GKTriple aux = GKTriple(x,0,0);
            auto firstBiggerIt = upper_bound(L.begin(), L.end(), aux);
            GKTriple &firstBigger = *firstBiggerIt;
    
            if (double(firstBigger.g + firstBigger.delta + 1) < cap){
                firstBigger.g++;
            }else{
                int g = firstBigger.g, delta = firstBigger.delta;
                L.insert(firstBiggerIt, GKTriple(x, 1, g + delta - 1));
                compressOneIfPossible();
            }
        }

        double rank(int x){
            GKTriple aux = GKTriple(x,0,0);
            auto firstBiggerIt = upper_bound(L.begin(), L.end(), aux);
            GKTriple &firstBigger = *firstBiggerIt;
            double rank = (firstBigger.g + firstBigger.delta)/2.0 - 1.0;
            for(auto it = L.begin(); it != firstBiggerIt; it++){
                GKTriple &triple = *it;
                rank += triple.g;
            }
            return rank;   
        }

        int quantile(double r){
            r *= N;
            GKTriple aux = GKTriple();
            if(r + 1 > N - getAcceptableError()){
                aux = *(L.rbegin()++);
                return aux.x;
            }

            int gSum = 0;
            double currRank = 0;
            for(auto it = L.begin(); it != L.end(); it++){
                auto nextIt = it; nextIt++;
                GKTriple &curr = *it;
                GKTriple &next = *nextIt;

                gSum += curr.g;
                if(next.x > curr.x){
                    double currRank = double(next.g + next.delta)/2.0 - 1.0 + gSum;
                    if(currRank > r - getAcceptableError()){
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


using namespace std;

int main(int args, char **argv){
    ios::sync_with_stdio(0); cin.tie(0);

vector<double> epsVals = {0.05, 0.1, 0.15, 0.2};
    vector<int> univVals = {100, 1000, 10000};
    vector<int> columns = {4, 5, 6, 7};
    vector<double> quantQueries = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8 ,0.9, 1};
    vector<vector<int>> rankQueries = getRankQueries(univVals);
    

    int column = 5;
    double eps = 0.1;
    int univ = INT_MAX;
    vector<int> rankQueries;
    vector<double> quantQueries;

    string datasetFilename = "../dataset/network_flows.csv";
    string queryType;
    
    GKSketch gk(eps);
    CSVReader reader(datasetFilename,column);
    vector<int> v;

    while(reader.hasNext()){
        int x = reader.getNextValue();
        // clock_t begin = clock();
        // clock_t end = clock();
        // double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        if(x <= univ){
            gk.update(x);
            v.push_back(x);
        }
    }

    // if(rankQueries.size() > 0){
    //     cout << "x" << "\t\t" << "rank(x)" << '\n';
    //     for(int x : rankQueries){
    //         cout << x << "\t\t" << gk.rank(x) << endl;
    //     }
    // }

    // if(quantQueries.size() > 0){
    //     cout << "r" << "\t\t" << "quant(r)" << "\t\t" << "asExpected" << endl;
    //     for(double r : quantQueries){
    //         int q = gk.quantile(r);
    //         cout << r << "\t\t" << q << "\t\t";
    //         cout << (gk.rank(q) > r*gk.getSize() - gk.getAcceptableError()) << endl;
    //     }
    // }
    sort(v.begin(), v.end());
    cout << gk.quantile(0.5) << endl;
    cout << v[v.size()/2 - eps*gk.getSize()] << ' ' << v[v.size()/2] << ' ' << v[v.size()/2 + eps*gk.getSize()] << endl;
    

    // int counter = 0;
    // set<int> s(v.begin(), v.end());
    // cout << "x\trankReal\trankEstimado\terror\tasExpected" << endl;
    // for(int x : s){
    //     int rankReal = lower_bound(v.begin(), v.end(), x) - v.begin();
    //     double rankEstimado = gk.rank(x);
    //     double error = abs(rankReal - rankEstimado);
    //     cout << x << '\t' 
    //     << rankReal << '\t'
    //     << rankEstimado << "\t\t"
    //     << error << "\t\t";
    //     cout << (error < eps * v.size()) << endl;
    //     counter += !(error < eps * v.size());
    // }
    // cout << "errors = " << counter<<"/" << s.size() << endl;

}