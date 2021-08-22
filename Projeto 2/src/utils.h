#ifndef UTILS_TAA
#define UTILS_TAA 1
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
#include <set>
#include <queue>
#include <vector>
#include <climits>
#include <fstream>
#include <chrono>
#include <random>
#include "GKSketch.h"

using namespace std;

class Utils{
    public: // debug utilities
        template<typename T>
            void printvec(vector<T> &v){
                for(int i = 0; i < (int)v.size(); i++){
                    cout << v[i] << " \n"[i==v.size()-1];
                }
            }
        
        void print(list<GKTriple> & L){
            for (auto& tri : L){
                cout << tri.toString() << ' ';
            }
            cout << endl;
        }
};

#endif
