#include <iostream>
#include "args.h"
#include <queue>
#include <algorithm>

using namespace std;

class KLL
{
private:
    double eps, delta, C;
    int K;
    deque<deque<int>> B;
    int H;

public:

    /**
     * Public constructor.
     */
    KLL(double e, double c, double d) : eps(e), C(c), delta(d)
    {
        H = 0;
        K = 1.0 * sqrt(1.0 / C / C / (C + C - 1.0) * log(2 / delta)) / eps;
        B.assign(1, deque<int>(0));

    }

    void update(int x){
        B[0].push_back(x);
        // Compress
        for (int l = 0; l <= H; l++)
        {
            bool addedList = false;
            if(B[l].size() >= max(2, K*int(ceil(pow(C, H-l))))){
                sort(B[l].begin(), B[l].end());
                int coin = rand() % 2; // TODO: add random.h
                if(B.size() - 1 == l){
                    deque<int> aux;
                    B.push_back(aux);
                    addedList = true;
                }
                for(int i = coin; i < B[l].size(); i+=2){
                    B[l+1].push_back(B[l][i]);
                }
                B[l].clear();
            }
            if(addedList){
                H ++;
            }
        }
    }

    int rank(int x, int l){
        if(B[l].size() == 0){
                return 0;
        }
        if(B[l].back() < x){
            return B[l].size();
        }
        int left = 0, right = B[l].size() - 1;
        while(left < right){
            int mid = left + (right - left)/2;
            if(B[l][mid] >= x) right = mid;
            else left = mid + 1;
        }
        return right; // first position in B[l] that is bigger or equal than x
    }

    int rankQuery(int x){
        int result = 0;
        for(int l = 0; l <= H; l++){
            int rankAtLevel = rank(x,l);
            result += rankAtLevel * (1 << l);
        }
        return result;
    }

    int quant(int x){
        // TODO: dale aqui mano

    }
};

int main(int args, char **argv)
{
}
