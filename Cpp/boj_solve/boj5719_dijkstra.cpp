#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <queue>
typedef long long ll;
const int LEN = 500;
const ll INF = 1e18;
int N, M, s, d, u, v;
ll p;

struct Info {
	int i;
	ll c;
	Info(int I = 0, ll C = 0) : i(I), c(C) {}
	bool operator < (const Info& info) const { return c > info.c; }
};
ll C[LEN], G[LEN][LEN];
std::priority_queue<Info> PQ;
std::queue<int> Q;
std::vector<int> P[LEN];//footprint
bool V[LEN];

ll dijkstra(int s, int e, bool f = 0) {
	for (int i = 0; i < N; i++) C[i] = INF;
	for (int i = 0; i < N; i++) std::vector<int>().swap(P[i]);
	C[s] = 0;
	PQ.push(Info(s, 0));
	while (PQ.size()) {
		Info cur = PQ.top(); PQ.pop();
		if (cur.c > C[cur.i]) continue;
		v = cur.i;
		for (int w = 0; w < N; w++) {
			ll nxt = cur.c + G[v][w];
			if (~G[v][w] && nxt < C[w]) {
				C[w] = nxt;
				PQ.push(Info(w, nxt));
				if (f) std::vector<int>().swap(P[w]), P[w].push_back(v);
			}
			if (f && ~G[v][w] && nxt == C[w]) {
				P[w].push_back(v);
			}
		}
	}
	return C[e];
}
void bfs(int s, int e) {
	memset(V, 0, sizeof V);
	Q.push(s);
	V[s] = 1;
	while (Q.size()) {
		v = Q.front(); Q.pop();
		for (const int& w : P[v]) {
			G[w][v] = -1;
			if (V[w]) continue;
			V[w] = 1;
			Q.push(w);
		}
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	while (1) {
		std::cin >> N >> M;
		if (!N && !M) return;
		std::cin >> s >> d;
		memset(G, -1, sizeof G);
		while (M--) {
			std::cin >> u >> v >> p;
			G[u][v] = p;
		}
		dijkstra(s, d, 1);
		bfs(d, s);
		ll ret = dijkstra(s, d);
		if (ret == INF) ret = -1;
		std::cout << ret << "\n";
	}
}
int main() { solve(); return 0; }//boj5719 refer to kibbomi