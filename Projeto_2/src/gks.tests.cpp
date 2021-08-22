#include "csv.h"

#include <iostream>
#include <vector>
#include <set>
#include "args.h"

#include "GKSketch.h"
#include "GKDocString.h"


using namespace std;

int main(int args, char **argv){
    ios::sync_with_stdio(0); cin.tie(0);
    ArgsReader argsReader;

    argsReader.checkHelpOption(args, argv, GKDocString::DOC_STRING);

    int column = 5;
    double eps = 0.1;
    long long univ = INT_MAX;
    vector<long long> rankQueries;
    vector<double> quantQueries;

    string datasetFilename;
    string queryType;

    queue<string> argsQueue;
    for(int i = 1; i < args; i++){
        argsQueue.push(argv[i]);
    }

    argsReader.updateArgs(
        argsQueue,
        column,
        eps,
        univ,
        rankQueries,
        quantQueries,
        queryType,
        datasetFilename
    );
    
    GKSketch gk(eps);
    CSVReader reader(datasetFilename,column);
    vector<int> v;

    while(reader.hasNext()){
        int x = reader.getNextValue();
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

    if(quantQueries.size() > 0){
        cout << "r" << "\t\t" << "quant(r)" << "\t\t" << "asExpected" << endl;
        for(double r : quantQueries){
            int q = gk.quantile(r);
            cout << r << "\t\t" << q << "\t\t";
            cout << (gk.rank(q) > eps*gk.getSize() - gk.getAcceptableError()) << endl;
        }
    }
    

    // int counter = 0;
    // set<int> s(v.begin(), v.end());
    // sort(v.begin(), v.end());
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