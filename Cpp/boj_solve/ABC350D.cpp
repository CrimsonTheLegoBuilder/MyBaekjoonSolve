#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <unordered_set>
typedef long long ll;
const int LEN = 2e5 + 1;
int N, M, a, b, Q, T, x;
bool V[LEN];
std::unordered_set<int> US[LEN];
int P[LEN * 6];
int find(int v) {
    if (P[v] < 0) return v;
    int p = find(P[v]);
    return P[v] = p;
}
int join(int u, int v) {
    int i, j;
    i = find(u);
    j = find(v);
    if (i == j) return 0;
    if (P[i] < P[j]) {
        P[i] += P[j];
        P[j] = i;
    }
    else {
        P[j] += P[i];
        P[i] = j;
    }
    return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
    memset(P, -1, sizeof P);
	std::cin >> N >> M;
    for (int i = 0; i < N; i++) {
        std::cin >> a >> b;
        US[a].insert(b);
        US[b].insert(a);
        join(a, b);
    }
	int cnt = 0;


}
int main() { solve(); return 0; }//Fail