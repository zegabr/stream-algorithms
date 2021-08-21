#ifndef HASHER
#define HASHER 1
#include <chrono>
#include <random>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

class Hasher{
    private:
        static const long long base = 137;// cover all ascii values
        const long long P = (1LL << 61) - 1; // mersenne prime

        //add char to cumulative hash
        void addChar(long long &hash, char &c){
            hash = (multiply(hash, base, P) + c) % P;
        }
    public: 

        // Random number in interval
        inline long long  random_longlong(long long  i, long long  j){
            return uniform_int_distribution<long long >(i, j)(rng);
        }

        // This was copied from a competitive programming lib
        // it is useful to avoid overflow when multiplying
        long long multiply(long long a, long long b, long long mod) {
            long long res = 0;
            while (b > 0) {
                if (b & 1) res = (res + a) % mod ;
                a = (a + a) % mod;
                b >>= 1;
            }
            return res;
        }

        // Returns two numbers, a and b, to be used as hash function (h(x) = ax + b mod P)
        vector<long long> getNewHashFunction(long long P){
            long long a = random_longlong(1, P-1);
            long long b = random_longlong(1, P-1);
            return {a,b};
        }

        // Gets the hash for x wich will be used as index inside the k-sized array
        long long hashCountMin(long long x, long long a, long long b, long long P, long long k) {
            long long ax = multiply(a,x,P);
            return ((ax + b) % P) % k;
        }

        // Gets unique hash for a string by using Rabin Karp's algorithm
        long long getUniqueHash(string &s){
            long long hash = 0;
            for(char c : s) addChar(hash, c);
            return hash;
        }
};
#endif
