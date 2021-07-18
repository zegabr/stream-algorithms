#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <queue>
#include <vector>


using namespace std;

class CountMinSketch{

    private:
        // TODO
    public:
        CountMinSketch(double eps, double delta){
            // TODO
        }
        void update(int x, int w){
            // TODO
        }
        int query(int x){
            // TODO

            return 0;
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
int weight = 1; // default column to be used as weights
double eps = 0.5; // default error bound // TODO: entender isso aqui
double delta = 0.5;// default error probability // TODO: entender isso aqui tbm
vector<int> queryIds; // ids to be queried in the end
queue<string> argsQueue; // queue to store args temporarely

void invalidArgument(string &arg){
    cerr << "Invalid argument: " << arg << endl;
    exit(1);
}
void missingArgumentValue(string &arg){
    cerr << "missing argument value for " << arg << endl;
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
            cerr << "empty file" << endl;
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

    for(int i = 1; i < args; i++){
        argsQueue.push(argv[i]);
    }

    updateArgs();

    // TODO: ler x e w do dataset seguindo as informacoes nos parametros

    cout << "using: " << '\n';
    cout << "id=" << id << '\n';
    cout << "weight=" << weight << '\n';
    cout << "eps=" << eps << '\n';
    cout << "delta=" << delta << '\n';
    cout << "queries=";
    printvec(queryIds);

    for(int q : queryIds){
        // TODO: printar o retorno da query do count min sketch
    }

    return 0;
}
