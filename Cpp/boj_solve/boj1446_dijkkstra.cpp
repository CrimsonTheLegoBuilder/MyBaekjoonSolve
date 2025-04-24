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
		cost[i] = LEN << 1;
	}
	Q.push({0, 0});
	cost[0] = 0;
	//for (int i = 1; i <= D; i++) {
	//	Q.push({ i, i });
	//}
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (cost[p.i] < p.c) continue;
		//if (p.i == D) return p.c;
		for (const Info& w : G[p.i]) {
			//std::cout << w.i << " " << w.c << "\n";
			int cnxt = p.c + w.c;
			if (cost[w.i] > cnxt) {
				cost[w.i] = cnxt;
				Q.push({ cnxt, w.i });
			}
		}
	}
	return cost[D];
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