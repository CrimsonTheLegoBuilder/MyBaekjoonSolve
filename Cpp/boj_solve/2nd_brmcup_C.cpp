#include <iostream>
#include <queue>
#include <set>
typedef long long ll;
const ll MOD = 1'000'000'007;
const int LEN = 200'001;
std::set<int> graph[LEN];
int V[LEN]{};
int D[LEN]{};
void BFS(int v) {
	std::queue<int> Q;
	int p;
	Q.push(v);
	V[v] = 1;
	while (Q.size()) {
		p = Q.front(); Q.pop();
		//std::cout << p << " ";
		for (const int& w : graph[p]) {
			if (!V[w]) {
				V[w] = V[p] + 1;
				Q.push(w);
			}
		}
	}
	return;
}
void make_graph(int M) {
	int s, e;
	for (int i = 0; i < M; i++) {
		std::cin >> s >> e;
		graph[s].insert(e);
		graph[e].insert(s);
	}
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int N, M;
	ll total = 1;
	std::cin >> N >> M;
	make_graph(M);
	BFS(1);
	for (int i = 1; i <= N; i++) {
		D[V[i]] += 1;
	}
	for (int i = 1; i <= N; i++) {
		if (D[i]) {
			total *= (D[i] + 1);
			total %= MOD;
			
		}
	}
	if (!total) total = MOD;
	std::cout << total - 1;
	return 0;
}