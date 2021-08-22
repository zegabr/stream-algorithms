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

    int column = 0;
    double eps = 0.1;
    long long univ = 0;
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

    cout << column << endl;
    cout << eps << endl;
    cout << univ << endl;
    cout << datasetFilename << endl;
    if(queryType == "rank") {
        cout << rankQueries.size() << endl;
        for(auto num : rankQueries) cout << num << " ";
        cout << endl;
    } else {
        cout << quantQueries.size() << endl;
        for(auto num : quantQueries) cout << num << " ";
        cout << endl;
    }
    
    GKSketch gk(eps);
    CSVReader reader(datasetFilename,column);

    while(reader.hasNext()){
        int x = reader.getNextValue();
        cout << x << endl;
    }

    // vector<int> v = {1, 4, 2, 8, 5, 7, 6, 7, 6, 7, 2, 1};
    // for(int x : v){
    //     gk.update(x);
    // }

    // set<int> s(v.begin(), v.end());
    // sort(v.begin(), v.end());

    // for(int x : s){
    //     int rankReal = lower_bound(v.begin(), v.end(), x) - v.begin();
    //     double rankEstimado = gk.rank(x);
    //     cout << x << ' ' 
    //     << rankReal << ' '
    //     << rankEstimado << ' '
    //     << abs(rankReal - rankEstimado) << endl;
    // }

    // for(int x : v) cout << x << ' '; cout << endl;
    // int mediana = gk.quantile(0.5);
    // cout << "mediana =" << mediana << endl;
    // cout << gk.rank(mediana) << " > " << 0.5*gk.getSize() - gk.getAcceptableError() << endl;
}