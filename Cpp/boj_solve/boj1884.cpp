#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <cmath>
typedef double ld;
typedef long long ll;
const int LEN = 51;
const ld MAX = 1e17;
int K, N, R, s, d, l, t;

struct Info {
	int i;
	ll c;
	bool operator < (const Info& x) const { return c > x.c; }
};
std::priority_queue<Info> Q;
std::vector<Info> G[LEN];
ll C[LEN];
ll dijkstra(int s, int e) {
	for (int i = 1; i <= N; i++) C[i] = MAX;
	Q.push({ s, 0 });
	C[s] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > C[p.i]) continue;
		for (const Info& w : G[p.i]) {
			ll cost = w.c + p.c;
			if (C[w.i] > cost) Q.push({ w.i, C[w.i] = cost });
		}
	}
	return C[e];
}

int init() {
	std::cin >> K >> N >> R;
	for (int i = 1; i <= N; i++) {
		for (int j = i + 1; j <= N; j++) {
			std::cin >> s >> d >> l >> t;

		}
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);

	return;
}
int main() { solve(); return 0; }//boj5379