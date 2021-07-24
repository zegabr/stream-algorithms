#ifndef ARGS
#define ARGS 1

#include <fstream>
#include <sstream>
#include <set>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;
class ArgsReader{
    public:
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
};
#endif
