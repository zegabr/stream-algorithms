#include<bits/stdc++.h>
#include "utils.h"

using namespace std;

Utils utils; // global utility functions

class KMV {
    private:
    long long k, m, R, a, b, P;
    set<long long> min_vals;

    public:
    KMV(long long m, double error_bound, double error_prob) {
        this->m = m;

        this->R = (1LL<<61)-1;
        this->P = (1LL<<61)-1;
        long long k1 = 84 / (pow(error_bound, 2));
        long long k2 = (4*m) / (error_bound * this->R);
        this->k = max(k1,k2);

        vector<long long> hashValues = utils.getNewHashFunction(P);
        this->a = hashValues[0], this->b = hashValues[1];
    }

    void minsert(long long num) {
        if(!this->min_vals.count(num)) {
            this->min_vals.insert(num);
        }
    }

    void update(long long num) {
        long long hashed_num = utils.hash(num, this->a, this->b, this->P, this->R);
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
        return ((double)this->R * (double)this->k) / (double)largest;
    }
};

int main() {
    srand( (unsigned)time(NULL) );
    long long m = 1000000, n = 1000000;
    double error_bound = 0.5, error_prob = 0.5;
    vector<double> answers;

    vector<long long> vals;
    set<long long> unique_vals;

    for(long long i = 0; i < n; i++) {
        vals.push_back(rand() % (m));
        unique_vals.insert(vals.back());
    }

    long long times = 4 * log(1/error_prob);
    for(int i = 0; i < times; i++) {
        KMV *sketch = new KMV(m, error_bound, error_prob);
        for(auto num : vals) {
            sketch->update(num);
        }
        answers.push_back(sketch->query());
    }

    sort(answers.begin(), answers.end());
    double ans = answers[answers.size()/2];
    cout << "estimate --> " << ans << endl;
    cout << "real --> " << unique_vals.size() << endl;
}