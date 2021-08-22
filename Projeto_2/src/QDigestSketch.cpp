#include "args.h"
#include "csv.h"
#include "QDigestDocString.h"
#include<bits/stdc++.h>

using namespace std;

class Node {
    public:
    int weight;
    vector<Node*> children;
    Node() {
        this->weight = 0;
        this->children = {NULL, NULL};
    }

    int getTreeWeight() {
        int treeWeight = this->weight;
        for(auto child : this->children) {
            treeWeight += child ? child->getTreeWeight() : 0;
        }
        return treeWeight;
    }
};

class QDigest {
    public:
    int logUniv, univ, totalWeight;
    double eps;
    Node *root;

    QDigest(int univ, double eps) {

        if(univ <= 0 || eps <= 0) {
            cout << "EXIT IN CONSTRUCTOR" << endl;
            exit(1);
        }

        this->univ = univ;
        this->eps = eps;
        this->totalWeight = 0;
        this->root = new Node();
    }

    void update(int x, int w) {
        if(!(0 <= x && x < this->univ && 0 <= w)) {
            cout << "EXIT IN UPDATE" << endl;
            exit(1);
        }

        this->totalWeight += w;
        int capacity = (int)(this->eps * this->totalWeight / log2(this->univ));
        Node *cur = this->root;

        int l = 0, r = this->univ;

        while(w && (r-l) > 1) {
            int avail = capacity - cur->weight;
            int added = min(w, avail);
            cur->weight += added;
            w -= added;
            int mid = (l+r) / 2;

            int nextCur;

            if(x < mid) {
                nextCur = 0;
                r = mid;
            } else {
                nextCur = 1;
                l = mid;
            }

            if(!cur->children[nextCur]) {
                cur->children[nextCur] = new Node();
            }

            cur = cur->children[nextCur];
        }

        if(!(w == 0 || (r-l) == 1)) {
            cout << "EXIT IN END OF UPDATE" << endl;
            exit(1);
        }

        cur->weight += w;
    }

    int rank(int x) {
        if(x < 0 || this->univ <= x) {
            return 0;
        }

        Node *cur = this->root;
        int l = 0, r = this->univ;
        int ret = 0;
        int previous = 0;

        while(cur && (r-l) > 1) {
            int mid = (l+r) / 2;
            previous += cur->weight;
            if(x < mid) {
                r = mid;
                cur = cur->children[0];
            } else {
                ret += cur->children[0] ? cur->children[0]->getTreeWeight() : 0;
                l = mid;
                cur = cur->children[1];
            }
        }

        return ret + previous/2;
    }

    int quantile(double qPercent) {
        int q = qPercent * this->totalWeight;
        int l = 0, r = this->univ-1;

        while(l < r) {
            int mid = (l+r+1) / 2;
            int estimatedRank = this->rank(mid);

            if(estimatedRank < q) {
                l = mid;
            } else {
                r = mid-1;
            }
        }

        return l;
    }

    pair<Node*, int> compress(Node *root, int capacity, int availUp) {
        if(!root) {
            cout << "EXIT IN COMPRESS" << endl;
            exit(1);
        }

        int moveUp = 0;

        for(int i = 0; i < 2; i++) {
            Node *child = root->children[i];

            if(child) {
                int availHere = capacity - root->weight;
                pair<Node*, int> nextCompress = this->compress(child, capacity, availUp + availHere);
                
                root->children[i] = nextCompress.first;
                int putHere = min(availHere, nextCompress.second);
                root->weight += putHere;
                moveUp += (nextCompress.second - putHere);
                availUp -= (nextCompress.second - putHere);
            }
        }

        int moveUpFromHere = min(availUp, root->weight);
        moveUp += moveUpFromHere;
        root->weight -= moveUpFromHere;

        if(root->weight == 0 && root->children[0] == NULL && root->children[1] == NULL) {
            delete(root);
            return {NULL, moveUp};
        }

        return {root, moveUp};
    }

    void compressTree() {
        int capacity = (int)(this->eps * this->totalWeight / log2(this->univ));
        this->compress(this->root, capacity, 0);
    }
};

int main(int args, char **argv) {
    // CHECK FOR HELP FLAG
    ArgsReader argsReader;
    argsReader.checkHelpOption(args, argv, QDigestDocString::DOC_STRING);

    // GET ARGUMENTS
    int column = 0;
    double eps = 0.1;
    int univ = 10000;
    vector<int> rankQueries;
    vector<double> quantQueries;

    string datasetFilename;
    string queryType;

    queue<string> argsQueue;
    for(int i = 1; i < args; i++){
        argsQueue.push(argv[i]);
    }


    argsReader.updateArgs(
        argsQueue,
        column,
        eps,
        univ,
        rankQueries,
        quantQueries,
        queryType,
        datasetFilename
    );

    QDigest *sketch = new QDigest(univ, eps);

    // READ CSV AND UPDATE SKETCH
    CSVReader reader(datasetFilename, column);
    
    // vector<int> trueRanks(univ);

    while(reader.hasNext()) {
        int value = reader.getNextValue();
        if(value >= univ) continue;

        // trueRanks[value]++;
        sketch->update(value, 1);
        sketch->compressTree();
    }

    // for(int i = 1; i < univ; i++) {
    //     trueRanks[i] += trueRanks[i-1];
    // }

   // int wrongRank = 0, wrongQuant = 0;

    // MAKE QUERIES
    if(queryType == "rank") {
        for(auto query : rankQueries) {
            int rank = sketch->rank(query);
            cout << rank << endl;

            // int minRank = (trueRanks[query] - eps*sketch->totalWeight);
            // int maxRank = (trueRanks[query] + eps*sketch->totalWeight);
            // bool correctRank = minRank <= rank && rank <= maxRank;
            // wrongRank += !correctRank;
            // cout << rank << " " << trueRanks[query] << " " << correctRank << "  min = " << minRank << "  max = " << maxRank << endl;
        }
    } else {
        for(auto query : quantQueries) {
            int quantile = sketch->quantile(query);
            cout << quantile << endl;

            // int proposedRank = query * sketch->totalWeight;
            // int minRank = (proposedRank - eps*sketch->totalWeight);
            // int maxRank = (proposedRank + eps*sketch->totalWeight);
            // bool correctQuantile = minRank <= trueRanks[quantile] && trueRanks[quantile] <= maxRank;
            // wrongQuant += !correctQuantile;
            // cout << quantile << " " << " " << correctQuantile << " " << trueRanks[quantile] << " " << proposedRank << "  min = " << minRank << "  max = " << maxRank << endl;
        }
    }

    // cout << "wrongRank = " << wrongRank << endl;
    // cout << "wrongQuant = " << wrongQuant << endl;

}