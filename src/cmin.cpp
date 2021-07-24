#include "hasher.h"
#include "args.h"
#include "csv.h"
#include <climits>
#include <map>

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

        int query(int x){
            int estimated_weight = INT_MAX;
            for(int i = 0; i < t; i++){
                int j = getHash(i, x);
                estimated_weight = min(estimated_weight, C[i][j]);
            }
            return estimated_weight;
        }
};

int main(int args, char **argv){

    set<string> possibleOptions = {
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
    for(int i = 1; i < args - 1; i++){ // get from second (since first is the command) to before last (since last is the csv file)
        argsQueue.push(argv[i]);
    }

    string datasetFilename = argv[args - 1]; // csv filename

    ArgsReader argsProcesser;
    argsProcesser.updateArgsCountMin(
            possibleOptions, 
            argsQueue, 
            id, 
            weight, 
            eps, 
            delta, 
            queryIds
            );

    //cout << "using: " << '\n';
    //cout << "id=" << id << '\n';
    //cout << "weight=" << weight << '\n';
    //cout << "eps=" << eps << '\n';
    //cout << "delta=" << delta << '\n';
    //cout << "queries="; utils.printvec(queryIds);
    //cout << "dataset filename = " << datasetFilename << '\n';

    CountMinSketch sketch(eps, delta);
    CSVReader reader(datasetFilename, id, weight);

    Hasher hasher;
    map<long long, long> freq;
    while(reader.hasNext()){
        vector<long long> idAndWeight = reader.getNextValueAndWeight();
        long long x = idAndWeight[0];
        long long w = idAndWeight[1];
        sketch.update(x,w);
        freq[x] += w; // soma frequencia de x
    }

    cout << "ESTIMATIVA" << endl;
    for(int q : queryIds){
        cout << sketch.query(q) << ' ';
    }
    cout << endl;

    cout << "RESPOSTA" << endl;
    for(int q : queryIds){
        string qs = to_string(q);
        long long h = hasher.getUniqueHash(qs);
        cout << freq[h] << ' ';
    }
    cout << '\n';
    cout << freq.size() << endl;

    return 0;
}
