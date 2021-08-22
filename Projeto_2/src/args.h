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
        void updateArgs(
            queue<string> &argsQueue,
            int &column,
            double &eps,
            int &univ,
            vector<int> &rankQueries,
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
                    univ = stoi(argsQueue.front());
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
                                rankQueries.push_back(stoi(queryArg));
                            } else {
                                quantQueries.push_back(stod(queryArg));
                            }
                        }

                        if(rankQueries.empty() && quantQueries.empty()){
                            cerr << "empty file" << '\n';
                            exit(1);
                        }
                    } else {
                        while(!argsQueue.empty()) {
                            queryArg = argsQueue.front();
                            argsQueue.pop();
                            if(queryType == "rank") {
                                rankQueries.push_back(stoi(queryArg));
                            } else {
                                quantQueries.push_back(stod(queryArg));
                            }
                        }
                    }
                }
            }
        }

        void checkHelpOption(int args, char **argv, const string &docString){
            if(args <= 2){
                string arg = argv[1];
                if(arg == "-h" or arg == "--help"){
                    cout << docString << endl;
                    exit(0);
                }else{
                    cerr << "error" << endl;
                    exit(1);
                }
            }
        }

        // Prints an missing argument error and exits
        void missingArgumentValue(string arg){
            cerr << "missing argument value for " << arg << '\n';
            exit(1);
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
