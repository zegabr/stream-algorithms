// #include "csv.h"

#include <iostream>
#include <vector>
#include <set>

#include "GKSketch.h"

using namespace std;


int main(int args, char **argv){
    ios::sync_with_stdio(0); cin.tie(0);
    GKSketch gk(0.2);

    vector<int> v = {1, 4, 2, 8, 5, 7, 6, 7, 6, 7, 2, 1};
    for(int x : v){
        gk.update(x);
    }

    set<int> s(v.begin(), v.end());
    sort(v.begin(), v.end());

    for(int x : s){
        int rankReal = lower_bound(v.begin(), v.end(), x) - v.begin();
        double rankEstimado = gk.rank(x);
        cout << x << ' ' 
        << rankReal << ' '
        << rankEstimado << ' '
        << abs(rankReal - rankEstimado) << endl;
    }

    for(int x : v) cout << x << ' '; cout << endl;
    int mediana = gk.quantile(0.5);
    cout << "mediana =" << mediana << endl;
    cout << gk.rank(mediana) << " > " << 0.5*gk.getSize() - gk.getAcceptableError() << endl;
}