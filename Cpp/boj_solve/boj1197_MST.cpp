#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <queue>
#include <cmath>
typedef long long ll;
const ll LEN = 10'001;
int N, E;
bool MST[LEN];

struct Info {
	int i;
	ll c;
	bool operator < (const Info& x) const { return c > x.c; }
};
std::vector<Info> G[LEN];
std::priority_queue<Info> Q;

ll prim(int v, int N) {
	int cnt = 0;
	ll total = 0;
	Q.push({ v, 0 });
	while (!Q.empty()) {
		Info cur = Q.top(); Q.pop();
		if (MST[cur.i]) continue;
		MST[cur.i] = 1;
		total += cur.c;
		cnt++;
		if (cnt == N) return total;
		for (const Info& i : G[cur.i]) {
			if (!MST[i.i]) Q.push(i);
		}
	}
	return total;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int s, e, c, start = 1;
	std::cin >> N >> E;
	for (int i = 0; i < E; i++) {
		std::cin >> s >> e >> c;
		G[s].push_back({ e, c });
		G[e].push_back({ s, c });
		start = s;
	}
	std::cout << prim(start, N) << "\n";
	return 0;
}