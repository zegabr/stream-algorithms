#include <algorithm>
#include <assert.h>     /* assert */
#include <list>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <climits>

using namespace std;

#ifndef GK_SKETCH
#define GK_SKETCH

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

#endif