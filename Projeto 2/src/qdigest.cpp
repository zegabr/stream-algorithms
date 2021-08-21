#include<bits/stdc++.h>

using namespace std;

class Node {
    public:
    long long weight;
    vector<Node*> children;
    Node() {
        this->weight = 0;
        this->children = {NULL, NULL};
    }

    long long getTreeWeight() {
        long long treeWeight = this->weight;
        for(auto child : this->children) {
            treeWeight += child ? child->getTreeWeight() : 0;
        }
        return treeWeight;
    }
};

class QDigest {
    public:
    long long logUniv, univ, totalWeight;
    double eps;
    Node *root;

    QDigest(long long univ, double eps) {

        if(univ <= 0 || eps <= 0) {
            exit(1);
        }

        this->univ = univ;
        this->eps = eps;
        this->totalWeight = 0;
        this->root = new Node();
    }

    void update(long long x, long long w) {
        if(!(0 <= x && x < this->univ && 0 <= w)) {
            exit(1);
        }

        this->totalWeight += w;
        long long capacity = (long long)(this->eps * this->totalWeight / log2(this->univ));
        Node *cur = this->root;

        long long l = 0, r = this->univ;

        while(w && (r-l) > 1) {
            long long avail = capacity - cur->weight;
            long long added = min(w, avail);
            cur->weight += added;
            w -= added;
            long long mid = (l+r) / 2;

            long long nextCur;

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
            exit(1);
        }

        cur->weight += w;
    }

    long long rank(long long x) {
        if(x < 0 || this->univ <= x) {
            return 0;
        }

        Node *cur = this->root;
        long long l = 0, r = this->univ;
        long long ret = 0;
        long long previous = 0;

        while(cur && (r-l) > 1) {
            long long mid = (l+r) / 2;
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

    pair<Node*, long long> compress(Node *root, long long capacity, long long availUp) {
        if(!root) {
            cout << "root is null" << endl;
            exit(1);
        }

        long long moveUp = 0;

        for(int i = 0; i < 2; i++) {
            Node *child = root->children[i];

            if(child) {
                long long availHere = capacity - root->weight;
                pair<Node*, long long> nextCompress = this->compress(child, capacity, availUp + availHere);
                
                root->children[i] = nextCompress.first;
                long long putHere = min(availHere, nextCompress.second);
                root->weight += putHere;
                moveUp += (nextCompress.second - putHere);
                availUp -= (nextCompress.second - putHere);
            }
        }

        long long moveUpFromHere = min(availUp, root->weight);
        moveUp += moveUpFromHere;
        root->weight -= moveUpFromHere;

        if(root->weight == 0 && root->children[0] == NULL && root->children[1] == NULL) {
            delete(root);
            return {NULL, moveUp};
        }

        return {root, moveUp};
    }

    void compressTree() {
        long long capacity = (long long)(this->eps * this->totalWeight / log2(this->univ));
        this->compress(this->root, capacity, 0);
    }

    void printTree() {
        queue<Node*> order;
        order.push(this->root);

        //while(!order) {}
    }
};

int main() {
    srand( (unsigned)time(NULL) );

    long long univ = 256, maxW = 50, n = 100;
    double eps = 0.1;

    vector<pair<long long, long long>> stream;
    vector<long long> trueRanks(univ);
    QDigest *sketch = new QDigest(univ, eps);
    long long totalWeight = 0;

    for(int i = 0; i < n; i++) {
        stream.push_back({rand() % univ, rand() % maxW});
    }

    for(auto p : stream) {
        totalWeight += p.second;
        trueRanks[p.first] += p.second;
        sketch->update(p.first, p.second);
        sketch->compressTree();
    }

    for(int i = 1; i < univ; i++) {
        trueRanks[i] += trueRanks[i-1];
    }

    int wrongCount = 0;

    // QUERIES
    for(int x = 0; x < univ; x++) {
        long long rank = sketch->rank(x);
        cout << "x = " << x << "  true rank = " << trueRanks[x] << "  estimatedRank = " << rank << "  error = " << (trueRanks[x] - rank) << "  max error = " << (eps*totalWeight) << endl;
        if((trueRanks[x] - rank) > (eps*totalWeight)) wrongCount++;
    }

    cout << "wrong count: " << wrongCount << endl;

}