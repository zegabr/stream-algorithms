#include<bits/stdc++.h>
#include "utils.h"

using namespace std;

Utils utils; // global utility functions

class KMV {
    private:
    long long k, m, a, b, P;
    set<long long> min_vals;

    public:
    KMV(double error_bound) {
        this->P = (1LL<<61)-1;
        this->k = 84 / (pow(error_bound, 2)); // chosen delta was 1/3

        vector<long long> hashValues = utils.getNewHashFunction(P);
        this->a = hashValues[0], this->b = hashValues[1];
    }

    void minsert(long long num) {
        if(!this->min_vals.count(num)) {
            this->min_vals.insert(num);
        }
    }

    long long hash(long long num) {
        return (utils.multiply(num,a,P) + b) % P;
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
    return 4 * log(1/delta);
}

int main(int args, char **argv) {
    set<string> possibleOptions = {
        "--target",
        "--eps",
        "--delta"
    };

    // default values
    int target = 0;
    double eps = 0.05;
    double delta = 0.01;

    // get arguments
    queue<string> argsQueue;
    for(int i = 1; i < args - 1; i++){
        argsQueue.push(argv[i]);
    }

    string filename = argv[args - 1];

    // update argument variables
    utils.updateArgsKMV(possibleOptions, argsQueue, target, eps, delta);

    // DEBUG #####################################
    cout << "TARGET: " << target << endl;
    cout << "EPS: " << eps << endl;
    cout << "DELTA: " << delta << endl;

    long long numSketches = getNumSketches(delta);
    vector<long long> sketchResults;
    set<long long> qtd;

    for(int i = 0; i < numSketches; i++) {
        CSVReader reader(filename, target);
        KMV *sketch = new KMV(eps);

        while(reader.hasNext()){
            long long value = reader.getNextValue();
            qtd.insert(value);
            sketch->update(value);
        }

        cout << "RESPOSTA: " << qtd.size() << endl;
        sketchResults.push_back(sketch->query());
    }

    sort(sketchResults.begin(), sketchResults.end());

    long long answer;
    if(numSketches % 2) 
        answer = (sketchResults[numSketches/2-1] + sketchResults[numSketches/2]) / 2;
    else
        answer = sketchResults[numSketches/2];

    cout << "ANSWER: " << answer << endl;
}