#include<bits/stdc++.h>
#include "hasher.h"
#include "args.h"
#include "csv.h"

using namespace std;

Hasher hasher;
ArgsReader argsReader;

class KMV {
    private:
        long long k, a, b, P;
        set<long long> min_vals;

    public:
        KMV(double eps) {
            this->P = (1LL<<61)-1;
            this->k = 15 / pow(eps, 2); // chosen delta is 1/3

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

int main(int args, char **argv) {

    argsReader.checkHelpOption(args, argv, "./src/helpkmv.txt"); 
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
    argsReader.updateArgsKMV(possibleOptions, argsQueue, target, eps, delta);

    long long numSketches = getNumSketches(delta);
    vector<long long> sketchResults;

    for(int i = 0; i < numSketches; i++) {
        CSVReader reader(filename, target);
        KMV *sketch = new KMV(eps);

        while(reader.hasNext()){
            long long value = reader.getNextValue();
            sketch->update(value);
        }

        sketchResults.push_back(sketch->query());
    }

    sort(sketchResults.begin(), sketchResults.end());

    long long answer;
    if(numSketches % 2) 
        answer = (sketchResults[numSketches/2-1] + sketchResults[numSketches/2]) / 2;
    else
        answer = sketchResults[numSketches/2];

    cout << answer << endl;
}
