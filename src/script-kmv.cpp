#include<bits/stdc++.h>
#include "hasher.h"
#include "args.h"
#include "csv.h"
#include <ctime>

using namespace std;

Hasher hasher;
ArgsReader argsReader;

long long valueOfK;

class KMV {
    private:
        long long k, m, a, b, P;
        set<long long> min_vals;

    public:
        KMV(double error_bound) {
            this->P = (1LL<<61)-1;
            this->k = 15 / pow(error_bound, 2); // chosen delta is '/3
            valueOfK = this->k;

            vector<long long> hashValues = hasher.getNewHashFunction(P);
            this->a = hashValues[0], this->b = hashValues[1];
        }

        void minsert(long long num) {
            if(!this->min_vals.count(num)) {
                this->min_vals.insert(num);
            }
        }

        long long hash(long long num) {
            return (hasher.multiply(num,a,P) + b) % P;
        }

        void update(long long num) {
            long long hashed_num = hash(num);
            minsert(hashed_num);
            if(this->min_vals.size() > k) {
                long long largest = *(this->min_vals).rbegin();
                min_vals.erase(largest);
            }
        }

        double query() {
            if(this->min_vals.size() < this->k) {
                return this->min_vals.size();
            }

            long long largest = *(this->min_vals).rbegin();
            return ((double)this->P * (double)this->k) / (double)largest;
        }
};

long long getNumSketches(double delta) {
    return 3 * log(1/delta);
}

void runKmv(int target, double eps, double delta) {
    string filename =  "./dataset/network_flows.csv";
    clock_t beginTime = clock();

    long long numSketches = getNumSketches(delta);
    vector<long long> sketchResults;

    unordered_set<string> realNumbers;

    for(int i = 0; i < numSketches; i++) {
        KMV *sketch = new KMV(eps);
        CSVReader reader(filename, target);

        while(reader.hasNext()){
            long long value = reader.getNextValue(realNumbers);
            sketch->update(value);
        }

        sketchResults.push_back(sketch->query());
    }

    sort(sketchResults.begin(), sketchResults.end());

    long long estimate;
    if(numSketches % 2) 
        estimate = (sketchResults[numSketches/2-1] + sketchResults[numSketches/2]) / 2;
    else
        estimate = sketchResults[numSketches/2];

    clock_t endTime = clock();

    long long realAns = realNumbers.size();    
    long long minimumExpected = realAns * (1 - eps);
    long long maximumExpected = realAns * (1 + eps);
    long long isGoodEstimate = minimumExpected < estimate && estimate < maximumExpected;
    long long timeElapsed = double(endTime - beginTime) / CLOCKS_PER_SEC;


    cout << target << "," << eps << "," << delta << "," << estimate << "," << realAns << "," << minimumExpected << "," << maximumExpected << "," << isGoodEstimate << "," << numSketches << "," << valueOfK << "," << timeElapsed << endl;
}

int main() {
    cout << "column,eps,delta,estimate,real answer,minimum acceptable answer,maximum acceptable answer,good estimate,number of sketches,value of K,time elapsed" << endl;

    vector<double> epsilons = {0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5};
    vector<double> deltas = {0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5};

    for(int column = 0; column < 9; column++) {
        for(auto eps : epsilons) {
            for(auto delta :  deltas) {
                runKmv(column, eps, delta);
            }
        }
    }
}