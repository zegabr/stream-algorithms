#ifndef ARGS
#define ARGS 1

#include "hasher.h"
#include <fstream>
#include <sstream>
#include <set>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class ArgsReader{
    private:
        Hasher hasher;
    public:
        void updateArgsQDigest(
            queue<string> &argsQueue,
            int &column,
            double &eps,
            long long &univ,
            vector<long long> &rankQueries,
            vector<double> &quantQueries,
            string &queryType,
            string &datasetFilename
        ) {
            while(!argsQueue.empty()) {
                string curArg = argsQueue.front();
                argsQueue.pop();

                if(curArg == "--val") {
                    if(argsQueue.empty()) missingArgumentValue(curArg);
                    column = stoi(argsQueue.front());
                    argsQueue.pop();
                } else if(curArg == "--eps") {
                    if(argsQueue.empty()) missingArgumentValue(curArg);
                    eps = stod(argsQueue.front());
                    argsQueue.pop();
                } else if(curArg == "--univ") {
                    if(argsQueue.empty()) missingArgumentValue(curArg);
                    univ = stoll(argsQueue.front());
                    argsQueue.pop();
                } else {
                    datasetFilename = curArg;

                    if(argsQueue.empty()) missingArgumentValue("queryType");
                    queryType = argsQueue.front();
                    argsQueue.pop();

                    if(argsQueue.empty()) missingArgumentValue("queryInfo");
                    string queryArg = argsQueue.front();

                    if(queryArg == "--in") {
                        argsQueue.pop();

                        if(argsQueue.empty()) missingArgumentValue("queriesFilename");
                        string queriesFilename = argsQueue.front();
                        argsQueue.pop();

                        stringstream ss = getFileAsStream(queriesFilename);
                        string queryArg;
                        while(ss >> queryArg){
                            if(queryType == "rank") {
                                rankQueries.push_back(stoll(queryArg));
                            } else {
                                quantQueries.push_back(stod(queryArg));
                            }
                        }
                    } else {
                        while(!argsQueue.empty()) {
                            queryArg = argsQueue.front();
                            argsQueue.pop();
                            if(queryType == "rank") {
                                rankQueries.push_back(stoll(queryArg));
                            } else {
                                quantQueries.push_back(stod(queryArg));
                            }
                        }
                    }
                }
            }

            if(rankQueries.empty() && quantQueries.empty()){
                cerr << "empty file" << '\n';
                exit(1);
            }
        }

        // Use argsQueue and possibleOptions to update Count Min variables
        void updateArgsCountMin(
                set<string> &possibleOptions, 
                queue<string> &argsQueue, 
                int &id, 
                int &weight, 
                double &eps, 
                double &delta, 
                vector<long long> &queryIds,
                vector<string> &queryOriginalIds
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
                tryUpdateQueryList(possibleOptions, argsQueue, arg, "--query", queryIds, queryOriginalIds);
                tryUpdateQueryListByFile(possibleOptions, argsQueue, arg, "--qryfile", queryIds, queryOriginalIds);
            }
        }

        void checkHelpOption(int args, char **argv, string filename){
            if(args <= 2){
                string arg = argv[1];
                if(arg == "-h" or arg == "--help"){
                    ifstream f(filename);
                    stringstream buffer;
                    buffer << f.rdbuf();
                    cout << buffer.str() << endl;
                    exit(0);
                }else{
                    cerr << "error" << endl;
                    exit(1);
                }
            }
        }

        // Use argsQueue and possibleOptions to update KMV variables
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
        // Prints an invalid argument error and exits
        void invalidArgument(string &arg){
            cerr << "Invalid argument: " << arg << '\n';
            exit(1);
        }

        // Prints an missing argument error and exits
        void missingArgumentValue(string arg){
            cerr << "missing argument value for " << arg << '\n';
            exit(1);
        }
        // Checks of arg os one of possible options
        void checkArg(set<string> &possibleOptions, string &arg){
            if(!possibleOptions.count(arg)){
                invalidArgument(arg);
            }
        }

        // Tries to update an integer var reference with next element in the argsQueue
        void tryUpdateArg(set<string> &possibleOptions, queue<string> &argsQueue, string &arg, string argName, int &var){
            if(arg == argName){
                if(argsQueue.empty() or possibleOptions.count(argsQueue.front())){
                    missingArgumentValue(arg);
                }
                var = stoi(argsQueue.front());
                argsQueue.pop();
            }
        }

        // Tries to update a double var reference with next element in the argsQueue
        void tryUpdateArg(set<string> &possibleOptions, queue<string> &argsQueue, string &arg, string argName, double &var){
            if(arg == argName){
                if(argsQueue.empty() or possibleOptions.count(argsQueue.front())){
                    missingArgumentValue(arg);
                }
                var = stod(argsQueue.front());
                argsQueue.pop();
            }
        }

        // Tries to update query list wtth next elements in argsQueue
        void tryUpdateQueryList(set<string> &possibleOptions, queue<string> &argsQueue,string &arg, string argName, vector<long long> &queryIds, vector<string> &queryOriginalIds){
            if(arg == argName){
                if(argsQueue.empty()){
                    missingArgumentValue(arg);
                }
                queryIds.clear();
                while(argsQueue.size() > 0 and !possibleOptions.count(argsQueue.front())){
                    queryIds.push_back(hasher.getUniqueHash(argsQueue.front()));
                    queryOriginalIds.push_back(argsQueue.front());
                    argsQueue.pop();
                }
                if(queryIds.empty()){
                    missingArgumentValue(arg);
                }
            }
        }

        // Tries to update query list wtth text file
        void tryUpdateQueryListByFile(set<string> &possibleOptions, queue<string> &argsQueue,string &arg, string argName, vector<long long> &queryIds, vector<string> &queryOriginalIds){
            if(arg == argName){
                if(argsQueue.empty()){
                    missingArgumentValue(arg);
                }
                queryIds.clear();
                if(!possibleOptions.count(argsQueue.front())){
                    string filename = argsQueue.front();
                    argsQueue.pop();

                    stringstream ss = getFileAsStream(filename);

                    string qid;
                    while(ss >> qid){
                        queryOriginalIds.push_back(qid);
                        queryIds.push_back(hasher.getUniqueHash(qid));
                    }
                }
                if(queryIds.empty()){
                    cerr << "empty file" << '\n';
                    exit(1);
                }
            }
        }

        // Creates a string stream from a file
        stringstream getFileAsStream(string &filename){
            ifstream f(filename);
            stringstream buffer;
            buffer << f.rdbuf();
            return buffer;
        }
};
#endif
