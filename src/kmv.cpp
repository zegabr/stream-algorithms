#include<bits/stdc++.h>
#define ll unsigned long

using namespace std;

class KMV {
    private:
    ll k, m, R, a, b, P;
    set<ll> min_vals;

    public:
    KMV(ll k, ll m) {
        this->k = k;
        this->m = m;
        this->R = pow(m,3);
        this->a = (1UL<<61) -1;
        this->b = 563;
        this->P = (1UL<<63);
    }

    void minsert(ll num) {
        if(!this->min_vals.count(num)) {
            this->min_vals.insert(num);
        }
    }

    ll hash(ll num) {
        return ((this->a * num) % P) % R;
    }

    void update(ll num) {
        ll hashed_num = this->hash(num);
        minsert(hashed_num);
        if(this->min_vals.size() > k) {
            ll largest = *(this->min_vals).rbegin();
            min_vals.erase(largest);
        }
    }

    double query() {
        if(this->min_vals.size() < this->k) {
            return this->min_vals.size();
        }

        ll largest = *(this->min_vals).rbegin();

        cout << "r: " << this->R << endl;
        cout << "k: " << this->k << endl;
        cout << "windowSize: " << largest << endl;

        return ((double)this->R * (double)this->k) / (double)largest;
    }
};

int main() {
    srand( (unsigned)time(NULL) );
    ll m = 1000, n = 1000, k = 50;
    KMV *sketch = new KMV(k,m);

    set<ll> unique_vals;

    for(ll i = 0; i < n; i++) {
        ll num = rand() % (m);
        unique_vals.insert(num);
        sketch->update(num);
    }

    double ans = sketch->query();
    cout << "estimate --> " << ans << endl;
    cout << "real --> " << unique_vals.size() << endl;
}