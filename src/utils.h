#ifndef UTILS_TAA
#define UTILS_TAA 1
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
#include <set>
#include <queue>
#include <vector>
#include <climits>
#include <fstream>
#include <chrono>
#include <random>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

class Utils{
    // TODO: separar isso em mais arquivos?
    private:

    public: // Constructor, initialize global variables here
        Utils(){
            reset_seed();
        }

    public: // random generator and hash functions generator
        void reset_seed(){
            srand(unsigned(time(NULL)));
        }

        int randint(int l, int r){//random from l to r inclusive
            return rand() % (r-l+1) + l;   
        }

        // Random number in interval
        inline long long  random_longlong(long long  i, long long  j){
            return uniform_int_distribution<long long >(i, j)(rng);
        }

        // This was copied from a competitive programming lib
        // it is useful to avoid overflow when multiplying
        long long multiply(long long a, long long b, long long mod) {
            long long res = 0;
            while (b > 0) {
                if (b & 1)
                    res = (res + a) % mod ;
                a = (a + a) % mod;
                b >>= 1;
            }
            return res;
        }

        /**
         * Returns a pair (a,b) to be using in the hash function h(x) = ax+b
         */
        vector<int> getNewHashFunction(int P){
            int a = randint(0, P-1); // TODO: trocar isso pra randint(1, P-1)? pra que h(x) = ax+b nao degenere pra h(x) = b ??
            int b = randint(0, P-1);
            return {a,b};
        }

        vector<long long> getNewHashFunction(long long P){
            long long a = random_longlong(0, P-1);
            long long b = random_longlong(0, P-1);
            return {a,b};
        }

        long long hash(long long x, long long a, long long b, long long P, long long k) {
            return (((((a * x) % P + b) % P) + P) % P) % k;
        }

    public: // args processor
        void updateArgsCountMin(
                set<string> &possibleOptions, 
                queue<string> &argsQueue, 
                int &id, 
                int &weight, 
                double &eps, 
                double &delta, 
                vector<int> &queryIds
                )
        {
            while(argsQueue.size() > 0){
                string arg = argsQueue.front();
                argsQueue.pop();

                checkArg(possibleOptions, arg);
                tryUpdateArg(possibleOptions, argsQueue, arg, "--id", id);
                tryUpdateArg(possibleOptions, argsQueue, arg, "--weight", weight);
                tryUpdateArg(possibleOptions, argsQueue, arg, "--eps", eps);
                tryUpdateArg(possibleOptions, argsQueue, arg, "--delta", delta);
                tryUpdateQueryList(possibleOptions, argsQueue, arg, "--query", queryIds);
                tryUpdateQueryListByFile(possibleOptions, argsQueue, arg, "--qryfile", queryIds);
            }
        }

        void updateArgsKMV(
                set<string> &possibleOptions, 
                queue<string> &argsQueue, 
                int &target, 
                double &eps, 
                double &delta 
                )
        {
            while(argsQueue.size() > 0){
                string arg = argsQueue.front();
                argsQueue.pop();

                checkArg(possibleOptions, arg);
                tryUpdateArg(possibleOptions, argsQueue, arg, "--target", target);
                tryUpdateArg(possibleOptions, argsQueue, arg, "--eps", eps);
                tryUpdateArg(possibleOptions, argsQueue, arg, "--delta", delta);
            }

        }

        void invalidArgument(string &arg){
            cerr << "Invalid argument: " << arg << '\n';
            exit(1);
        }

        void missingArgumentValue(string &arg){
            cerr << "missing argument value for " << arg << '\n';
            exit(1);
        }

        void checkArg(set<string> &possibleOptions, string &arg){
            if(!possibleOptions.count(arg)){
                invalidArgument(arg);
            }
        }

        void tryUpdateArg(set<string> &possibleOptions, queue<string> &argsQueue, string &arg, string argName, int &var){
            if(arg == argName){
                if(argsQueue.empty() or possibleOptions.count(argsQueue.front())){
                    missingArgumentValue(arg);
                }
                var = stoi(argsQueue.front());
                argsQueue.pop();
            }
        }

        void tryUpdateArg(set<string> &possibleOptions, queue<string> &argsQueue, string &arg, string argName, double &var){
            if(arg == argName){
                if(argsQueue.empty() or possibleOptions.count(argsQueue.front())){
                    missingArgumentValue(arg);
                }
                var = stod(argsQueue.front());
                argsQueue.pop();
            }
        }

        void tryUpdateQueryList(set<string> &possibleOptions, queue<string> &argsQueue,string &arg, string argName, vector<int> &queryIds){
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

        void tryUpdateQueryListByFile(set<string> &possibleOptions, queue<string> &argsQueue,string &arg, string argName, vector<int> &queryIds){
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

        stringstream getFileAsStream(string &filename){
            ifstream f(filename);
            stringstream buffer;
            buffer << f.rdbuf();
            return buffer;
        }

    public: // debug utilities
        template<typename T>
            void printvec(vector<T> &v){
                for(int i = 0; i < (int)v.size(); i++){
                    cout << v[i] << " \n"[i==v.size()-1];
                }
            }
};

class CSVReader{
    private:
        ifstream fileInput;
        int idColumn;
        int weightColumn;

        void replaceCommasWithSpaces(string &line){
            for(char &c : line){
                if(c == ',') c = ' ';
            }
        }

        void updateRowVector(vector<string> &row, string &line){
            string word;
            replaceCommasWithSpaces(line);
            stringstream ss(line);
            while(ss >> word){
                row.push_back(word);
            }
        }

    public:
        CSVReader(string &filename, int idCol, int weightCol){
            fileInput.open(filename);
            idColumn = idCol;
            weightColumn = weightCol;
            ignoreFirstLine();
        }

        CSVReader(string &filename, int idCol){
            fileInput.open(filename);
            idColumn = idCol;
            ignoreFirstLine();
        }

        bool hasNext(){
            return fileInput.peek() != EOF;
        }

        vector<int> getNextValueAndWeight(){
            string line;
            vector<string> row;

            getline(fileInput, line);
            updateRowVector(row, line); // TODO: refatorar isso pra ao inves de pegar o vector inteiro, só pegar as vairaveis q a gt quer
            return {stoi(row[idColumn]), stoi(row[weightColumn])};
        }

        int getNextValue(){
            string line;
            vector<string> row;

            getline(fileInput, line);
            updateRowVector(row, line); // TODO: refatorar isso pra ao inves de pegar o vector inteiro, só pegar as vairaveis q a gt quer
            return stoi(row[idColumn]);
        }

        void ignoreFirstLine(){
            string firstLine;
            getline(fileInput, firstLine);
        }
};
#endif
