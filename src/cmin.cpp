#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
#include <set>
#include <queue>
#include <vector>
#include <climits>
#include "utils.h"


using namespace std;

class CountMinSketch{
    private:
        int P = INT_MAX; // mersenne prime => 2 ^31 - 1
        int t, k;
        vector<vector<int>> hashFunctions, C;
        Utils U;

        /**
         * Returns the i-th position of x
         * */
        int getHash(int i, int x){
            vector<int> &ab = hashFunctions[i];
            // return ((ab[0] * x % P + b) % P) % k;
            return 0;
        }

    public:
        CountMinSketch(double eps, double delta){
            k = int(2.0/eps);
            t = ceil(log(1.0/delta));
            for(int i = 0; i < t; i++){
                hashFunctions.push_back(U.getNewHashFunction(P));
            }
            C.assign(t, vector<int>(0,k)); // C matrix (t x k)
        }

        void update(int x, int w){
            for(int i = 0; i < t; i++){
                int j = getHash(i, x);
                C[i][j] += w;
            }
        }

        int query(int x){
            int estimated_weigth = INT_MAX;
            for(int i = 0; i < t; i++){
                int j = getHash(i, x);
                estimated_weigth = min(estimated_weigth, C[i][j]);
            }
            return estimated_weigth;
        }
};

const set<string> possibleOptions = {
    "--id",
    "--weight",
    "--eps",
    "--delta",
    "--query",
    "--qryfile"
};

int id = 0; // default column to be used as id
int weight = 4; // default column to be used as weights
double eps = 0.1; // default error bound
double delta = 0.05;// default error probability
vector<int> queryIds; // ids to be queried in the end

queue<string> argsQueue; // queue to store args temporarely

void invalidArgument(string &arg){
    cerr << "Invalid argument: " << arg << '\n';
    exit(1);
}

void missingArgumentValue(string &arg){
    cerr << "missing argument value for " << arg << '\n';
    exit(1);
}

void checkArg(string &arg){
    if(!possibleOptions.count(arg)){
        invalidArgument(arg);
    }
}

void tryUpdateArg(string &arg, string argName, int &var){
    if(arg == argName){
        if(argsQueue.empty()){
            missingArgumentValue(arg);
        }
        if(possibleOptions.count(argsQueue.front())){
            missingArgumentValue(arg);
        }
        var = stoi(argsQueue.front());
        argsQueue.pop();
    }
}

void tryUpdateArg(string &arg, string argName, double &var){
    if(arg == argName){
        if(argsQueue.empty()){
            missingArgumentValue(arg);
        }
        if(possibleOptions.count(argsQueue.front())){
            missingArgumentValue(arg);
        }
        var = stod(argsQueue.front());
        argsQueue.pop();
    }
}

void tryUpdateQueryList(string &arg, string argName){
    if(arg == argName){
        if(argsQueue.empty()){
            missingArgumentValue(arg);
        }
        queryIds.clear();
        while(argsQueue.size() > 0 and !possibleOptions.count(argsQueue.front())){
            queryIds.push_back(stoi(argsQueue.front()));
            argsQueue.pop();
        }
        if(queryIds.empty()){
            missingArgumentValue(arg);
        }
    }
}

stringstream getFileAsStream(string filename){
    ifstream f(filename);
    stringstream buffer;
    buffer << f.rdbuf();
    return buffer;
}

void tryUpdateQueryListByFile(string &arg, string argName){
    if(arg == argName){
        if(argsQueue.empty()){
            missingArgumentValue(arg);
        }

        queryIds.clear();
        if(!possibleOptions.count(argsQueue.front())){
            string filename = argsQueue.front();
            argsQueue.pop();

            stringstream ss = getFileAsStream(filename);

            int qid;
            while(ss >> qid){
                queryIds.push_back(qid);
            }

        }
        if(queryIds.empty()){
            cerr << "empty file" << '\n';
            exit(1);
        }
    }
}
void updateArgs(){
    while(argsQueue.size() > 0){
        string arg = argsQueue.front();
        argsQueue.pop();

        checkArg(arg);
        tryUpdateArg(arg, "--id", id);
        tryUpdateArg(arg, "--weight", weight);
        tryUpdateArg(arg, "--eps", eps);
        tryUpdateArg(arg, "--delta", delta);
        tryUpdateQueryList(arg, "--query");
        tryUpdateQueryListByFile(arg, "--qryfile");
    }
}

void printvec(vector<int> &v){
    for(int i = 0; i < (int)v.size(); i++){
        cout << v[i] << " \n"[i==v.size()-1];
    }
}

int main(int args, char **argv){
    cout << args << endl;
    for(int i = 1; i < args - 1; i++){
        argsQueue.push(argv[i]);
    }

    string datasetFilename = argv[args - 1];

    updateArgs();


    cout << "using: " << '\n';
    cout << "id=" << id << '\n';
    cout << "weight=" << weight << '\n';
    cout << "eps=" << eps << '\n';
    cout << "delta=" << delta << '\n';
    cout << "queries=";
    printvec(queryIds);

    CountMinSketch sketch(eps, delta);

    // TODO: ler x e w do dataset seguindo as informacoes nos parametros

    for(int q : queryIds){
        cout << sketch.query(q) << ' ';
    }
    cout << '\n';

    return 0;
}
