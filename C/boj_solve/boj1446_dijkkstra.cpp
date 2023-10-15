#include <iostream>
#include <queue>
#include <set>
typedef long long ll;
const int LEN = 10'001;
int N, D, cost[LEN];

struct Info {
	int c, i;
	bool operator < (const Info& I) const {
		return c > I.c;
	}
};
std::priority_queue<Info> Q;
std::set<Info> G[LEN];


int dijkstra() {
	for (int i = 0; i <= D; i++) {
		cost[i] = i+1;
	}
	for (const Info& I : G[0]) {
		if (cost[I.i] > I.c) {
			cost[I.i] = I.c;
			Q.push(I);
			//std::cout << cost[I.i] << " " << I.c << " " << I.i << "\n";
		}
	}
	for (int i = 1; i <= D; i++) {
		Q.push({ i, i });
	}
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (cost[p.i] < p.c) continue;
		if (p.i == D) return p.c;
		for (const Info& w : G[p.i]) {
			//std::cout << w.i << " " << w.c << "\n";
			int cnxt = cost[p.i] + w.c;
			if (cost[w.i] > cnxt) {
				cost[w.i] = cnxt;
				Q.push({ cnxt, w.i });
			}
		}
	}
}


int main() {
	int s, e, d;
	std::cin >> N >> D;
	for (int i = 0; i < D; i++) {
		G[i].insert({ 1, i + 1 });
	}
	for (int i = 0; i < N; i++) {
		std::cin >> s >> e >> d;
		//std::cout << s << " " << e << " " << d << "\n";
		if (e <= D) G[s].insert({ d, e });
	}
	std::cout << dijkstra();
	return 0;
}