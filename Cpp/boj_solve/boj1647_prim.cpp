#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
#include <cmath>
typedef long long ll;
const int LEN = 100'001;
int N, M;
ll MAX = -1;
bool MST[LEN];

struct Info {
	int i;
	ll c;
	bool operator < (const Info& x) const { return c > x.c; }
};
std::vector<Info> G[LEN];
std::priority_queue<Info> Q;

ll prim(int v, int N) {
	ll total = 0;
	int cnt = 0;
	Q.push({ v, 0 });
	while (!Q.empty()) {
		Info cur = Q.top(); Q.pop();
		if (MST[cur.i]) continue;
		MST[cur.i] = 1;
		total += cur.c;
		MAX = std::max(MAX, cur.c);
		cnt++;
		if (cnt == N) return total;
		for (const Info& nxt : G[cur.i]) {
			if (!MST[nxt.i]) Q.push(nxt);
		}
	}
	return total;
}
int init() {
	int s, e, c;
	std::cin >> N >> M;
	for (int i = 0; i < M; i++) {
		std::cin >> s >> e >> c;
		G[s].push_back({ e, c });
		G[e].push_back({ s, c });
	}
	return s;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int v = init();
	ll T = prim(v, N);
	std::cout << T - MAX << "\n";
	return 0;
	
}