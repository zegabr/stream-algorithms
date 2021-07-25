#include "hasher.h"
#include "args.h"
#include "csv.h"
#include <climits>

using namespace std;

ArgsReader argsReader;
Hasher hasher;

class CountMinSketch{
    private:
        long long P = (1LL << 61) - 1;
        int t, k;
        vector<vector<long long>> hashFunctions;
        vector<vector<int>> C;

        
        // Returns the i-th hashed position of x
        int getHash(int i, int x){
            long long a = hashFunctions[i][0];
            long long b = hashFunctions[i][1];
            return hasher.hashCountMin(x,a,b,P,k);
        }

    public:
        // Constructor
        CountMinSketch(double eps, double delta){
            k = int(2.0/eps);
            t = ceil(log(1.0/delta));
            for(int i = 0; i < t; i++){
                hashFunctions.push_back(hasher.getNewHashFunction(P));
            }
            C.assign(t, vector<int>(k,0)); // C matrix (t x k)
        }

        // Updates all estimatives for a given id x
        void update(long long x, long long w){
            for(int i = 0; i < t; i++){
                long long j = getHash(i, x);
                C[i][j] += w;
            }
        }

        // Gets the minimum of the weight estimatives for a given id x
        long long query(long long x){
            int estimated_weight = INT_MAX;
            for(int i = 0; i < t; i++){
                int j = getHash(i, x);
                estimated_weight = min(estimated_weight, C[i][j]);
            }
            return estimated_weight;
        }
};

int main(int args, char **argv){
    ios::sync_with_stdio(0); cin.tie(0);
        
    argsReader.checkHelpOption(args, argv, "./src/helpcmin.txt"); 

    set<string> possibleOptions = {
        "--id",
        "--weight",
        "--eps",
        "--delta",
        "--query",
        "--qryfile"
    };

    int idColumn = 0;
    int weightColumn = 4;
    double eps = 0.1;
    double delta = 0.05;
    vector<long long> queryIds;
    vector<string> queryOriginalIds;

    queue<string> argsQueue; // queue to store args temporarely
    for(int i = 1; i < args - 1; i++){ // get from second (since first is the command) to before last (since last is the csv file)
        argsQueue.push(argv[i]);
    }

    string datasetFilename = argv[args - 1]; // csv filename

    
    argsReader.updateArgsCountMin(
            possibleOptions, 
            argsQueue, 
            idColumn, 
            weightColumn, 
            eps, 
            delta, 
            queryIds,
            queryOriginalIds
            );

    CountMinSketch sketch(eps, delta);
    CSVReader reader(datasetFilename, idColumn, weightColumn);

    while(reader.hasNext()){
        vector<long long> idAndWeight = reader.getNextValueAndWeight();
        long long x = idAndWeight[0];
        long long w = idAndWeight[1];
        sketch.update(x,w);
    }

    cout << "ID\t\tESTIMATIVA" << endl;
    for(int i = 0; i < queryIds.size(); i++){
        cout << queryOriginalIds[i] << "\t\t";
        cout << sketch.query(queryIds[i]) << '\n';
    }
    return 0;
}