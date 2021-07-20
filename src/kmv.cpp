#include<bits/stdc++.h>
#define ll unsigned long

using namespace std;

class KMV {
    private:
    ll k, m, R, a, b, P;
    priority_queue<ll> min_vals;
    unordered_set<ll> used_vals;

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
        if(!this->used_vals.count(num)) {
            min_vals.push(num);
            used_vals.insert(num);
        }
    }

    ll hash(ll num) {
        return ((this->a * num) % P) % R;
    }

    void update(ll num) {
        ll hashed_num = this->hash(num);
        minsert(hashed_num);
        if(this->min_vals.size() > k) {
            ll largest = min_vals.top();
            min_vals.pop();
            used_vals.erase(largest);
        }
    }

    double query() {
        if(this->min_vals.size() < this->k) {
            return this->min_vals.size();
        }

        cout << "r: " << this->R << endl;
        cout << "k: " << this->k << endl;
        cout << "windowSize: " << this->min_vals.top() << endl;

        return ((double)this->R * (double)this->k) / (double)(this->min_vals.top());
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