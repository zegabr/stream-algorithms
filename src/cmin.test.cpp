#include "hasher.h"
#include "args.h"
#include "csv.h"
#include "utils.h"
#include <climits>
#include <map>
#include <ctime>

using namespace std;


class CountMinSketch{
    private:
        Hasher hasher;
        long long P = (1LL << 61) - 1;
        int t, k;
        vector<vector<long long>> hashFunctions;
        vector<vector<int>> C;

        /**
         * Returns the i-th hashed position of x
         * */
        int getHash(int i, int x){
            long long a = hashFunctions[i][0];
            long long b = hashFunctions[i][1];
            return hasher.hashCountMin(x,a,b,P,k);
        }

    public:
        CountMinSketch(double eps, double delta){
            k = int(2.0/eps);
            t = ceil(log(1.0/delta));
            for(int i = 0; i < t; i++){
                hashFunctions.push_back(hasher.getNewHashFunction(P));
            }
            C.assign(t, vector<int>(k,0)); // C matrix (t x k)
        }

        void update(long long x, long long w){
            for(int i = 0; i < t; i++){
                long long j = getHash(i, x);
                C[i][j] += w;
            }
        }

        long long query(long long x){
            int estimated_weight = INT_MAX;
            for(int i = 0; i < t; i++){
                int j = getHash(i, x);
                estimated_weight = min(estimated_weight, C[i][j]);
            }
            return estimated_weight;
        }

        int getT(){return t;}
        int getK(){return k;}
};

void runTests(double eps, double delta, int id, int weight, string &datasetFilename, vector<long long> &queryIds, vector<string> &queriesOriginalIds){
    clock_t begin = clock();
    CountMinSketch sketch(eps, delta);
    int t = sketch.getT();
    int k = sketch.getK();
    CSVReader reader(datasetFilename, id, weight);

    map<long long, long> freq;
    long long W = 0; 
    while(reader.hasNext()){
        vector<long long> idAndWeight = reader.getNextValueAndWeight();
        long long x = idAndWeight[0];
        long long w = idAndWeight[1];
        sketch.update(x,w);

        freq[x] += w;
        W += w;
    }

    Hasher hasher;
    
    long long erroAceitavel = (long long)((double)W*eps);
    for(int i = 0; i < queryIds.size(); i++){
        long long q = queryIds[i];
        string idText = queriesOriginalIds[i];
        long long erro = abs(freq[q] - sketch.query(q));
        
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        cout << idText << "," 
        << sketch.query(q) << "," 
        << freq[q] << "," 
        << erro << ',' 
        << erroAceitavel << ',' 
        << (erro < erroAceitavel ? "TRUE" : "FALSE") << ',' 
        << eps << ',' 
        << delta << ','
        << W << ','
        << t << ','
        << k << ','
        << elapsed_secs << '\n';
    }
    // cout.flush();
}

int main(int args, char **argv){
    ios::sync_with_stdio(0); cin.tie(0);
    const vector<int> idColumn = {0};
    const vector<int> weightColumn = {3};
    const vector<double> epss = {0.01, 0.05, 0.1, 0.15, 0.2};
    const vector<double> deltas = {0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1};
    vector<long long> queryIds;
    vector<string> queriesOriginalIds;
    string datasetFilename = "./dataset/network_flows.csv";
    string qryFilename = "queryIds.txt";
    ArgsReader argsReader;
    Hasher hasher;

    // stringstream ss = argsReader.getFileAsStream(qryFilename);
    // string qid;
    // while(ss >> qid){
    //     queriesOriginalIds.push_back(qid);
    //     queryIds.push_back(hasher.getUniqueHash(qid));
    // }

    queryIds = {1,2,3,4,5};
    queriesOriginalIds = {"1", "2", "3", "4", "5"};

    cout << "ID,ESTIMATIVA,RESPOSTA,ERRO,ERRO_ACEITAVEL,ERRO < ERRO_ACEITAVEL,EPS,DELTA,SOMA_PESOS,T,K,TIME" << '\n';
    for(double eps : epss){
        for(double delta : deltas){
            for(int id : idColumn){
                for(int weight : weightColumn){
                    if(id != weight){
                        runTests(eps, delta, id, weight, datasetFilename, queryIds, queriesOriginalIds);
                    }
                }
            }
        }
    }
    return 0;
}
