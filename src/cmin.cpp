#include "utils.h"

using namespace std;

Utils utils; // global utility functions

class CountMinSketch{
    private:
        long long P = INT_MAX;
        int t, k;
        vector<vector<long long>> hashFunctions;
        vector<vector<int>> C;

        /**
         * Returns the i-th hashed position of x
         * */
        int getHash(int i, int x){
            long long a = hashFunctions[i][0];
            long long b = hashFunctions[i][1];
            return ((a * x % P + b) % P) % k;
        }

    public:
        CountMinSketch(double eps, double delta){
            k = int(2.0/eps);
            t = ceil(log(1.0/delta));
            for(int i = 0; i < t; i++){
                hashFunctions.push_back(utils.getNewHashFunction(P));
            }
            C.assign(t, vector<int>(k,0)); // C matrix (t x k)
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

    utils.updateArgsCountMin(
            possibleOptions, 
            argsQueue, 
            id, 
            weight, 
            eps, 
            delta, 
            queryIds
            );

    cout << "using: " << '\n';
    cout << "id=" << id << '\n';
    cout << "weight=" << weight << '\n';
    cout << "eps=" << eps << '\n';
    cout << "delta=" << delta << '\n';
    cout << "queries="; utils.printvec(queryIds);
    cout << "dataset filename = " << datasetFilename << '\n';

    CountMinSketch sketch(eps, delta);
    CSVReader reader(datasetFilename, id, weight);

    while(reader.hasNext()){
        vector<int> idAndWeight = reader.getNextValueAndWeight();
        int x = idAndWeight[0];
        int w = idAndWeight[1];
        sketch.update(x,w);
    }

    for(int q : queryIds){
        cout << sketch.query(q) << ' ';
    }
    cout << '\n';

    return 0;
}
