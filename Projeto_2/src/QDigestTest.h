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

    int countNodes() {
        if(!this->root) return 0;

        queue<Node*> q;
        q.push(this->root);
        int nodeCount = 0;

        while(!q.empty()) {
            nodeCount++;
            Node *cur = q.front();
            q.pop();

            if(cur->children[0]) q.push(cur->children[0]);
            if(cur->children[1]) q.push(cur->children[1]);
        }

        return nodeCount;
    }
};

class Teste {
    public:
    Teste() {}

    void runTest(
        int column, 
        double eps, 
        int univ, 
        vector<int> &rankQueries, 
        vector<double> &quantQueries, 
        string datasetFilename, 
        string queryType
    ) {
        QDigest *sketch = new QDigest(univ, eps);
        double totalTime = 0;
        ofstream myfile;
        string filename = queryType + "-" + to_string(column) + ".csv";
        myfile.open(filename, fstream::app);

        // READ CSV AND UPDATE SKETCH
        CSVReader reader(datasetFilename, column);
        
        vector<int> trueRanks(univ);

        while(reader.hasNext()) {
            int value = reader.getNextValue();
            if(value >= univ) continue;

            trueRanks[value]++;
            clock_t timeBegin = clock();
            sketch->update(value, 1);
            sketch->compressTree();
            clock_t timeEnd = clock();
            totalTime += double(timeEnd - timeBegin) / CLOCKS_PER_SEC;
        }

        for(int i = 1; i < univ; i++) {
            trueRanks[i] += trueRanks[i-1];
        }

        int nodeCount = sketch->countNodes();
        int memBytes = nodeCount * 96;

        if(eps == 0.05 && univ == 100) {
            if(queryType == "rank") {
                myfile << "column,eps,univ,queryType,time,memory,queryValue,estimate,correctValue,----,minEstimate,maxEstimate,withinBounds\n";
            } else {
                myfile << "column,eps,univ,queryType,time,memory,queryValue,estimate,proposedRank,achievedRank,minEstimate,maxEstimate,withinBounds\n";
            }
        }

        clock_t timeBegin = clock();
        // MAKE QUERIES
        if(queryType == "rank") {
            for(auto query : rankQueries) {
                int rank = sketch->rank(query);

                int minRank = (trueRanks[query] - eps*sketch->totalWeight);
                int maxRank = (trueRanks[query] + eps*sketch->totalWeight);
                bool correctRank = minRank <= rank && rank <= maxRank;
                myfile << (to_string(column) + "," + to_string(eps) + "," + to_string(univ) + "," + queryType + ",,," + to_string(query) + "," + to_string(rank) + "," + to_string(trueRanks[query]) + ",," + to_string(minRank) + "," + to_string(maxRank) + "," + to_string(correctRank) + "\n");
            }
        } else {
            for(auto query : quantQueries) {
                int quantile = sketch->quantile(query);

                int proposedRank = query * sketch->totalWeight;
                int minRank = (proposedRank - eps*sketch->totalWeight);
                int maxRank = (proposedRank + eps*sketch->totalWeight);
                bool correctQuantile = minRank <= trueRanks[quantile] && trueRanks[quantile] <= maxRank;
                myfile << (to_string(column) + "," + to_string(eps) + "," + to_string(univ) + "," + queryType + ",,," + to_string(query) + "," + to_string(quantile) + "," + to_string(proposedRank) + "," + to_string(trueRanks[quantile]) + "," + to_string(minRank) + "," + to_string(maxRank) + "," + to_string(correctQuantile) + "\n");   
            }
        }

        clock_t timeEnd = clock();
        totalTime += double(timeEnd - timeBegin) / CLOCKS_PER_SEC;

        myfile << (to_string(column) + "," + to_string(eps) + "," + to_string(univ) + "," + queryType + "," + to_string(totalTime) + "," + to_string(memBytes) + ",,,,,,," + "\n");
        myfile.close();
    }
};