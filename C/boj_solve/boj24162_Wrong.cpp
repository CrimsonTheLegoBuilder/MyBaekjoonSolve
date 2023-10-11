#include <iostream>
#include <algorithm>
#include <queue>
typedef long long ll;
const int LEN = 101;
const ll MAX = 10'000'000'000'000'000;
int G[LEN][LEN];
ll W[LEN];

struct Pos {
	ll x, y;
	bool operator < (const Pos& p) const {
		if (x == p.x) { return y < p.y; }
		return x < p.x;
	}
}pos[LEN];

struct Toll {
	ll w;
	int n, d;
	Pos pre;
	bool operator < (const Toll& t) const {
		if (w == t.w) return n > t.n;
		return w > t.w;
	}
};
std::priority_queue<Toll> H;

ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.x - d3.x) + (d2.y - d1.y) * (d4.y - d3.y);
}
void dijkstra(int v, int d, int N) {
	for (int i = 1; i <= N; i++) { W[i] = MAX; }
	W[v] = 0;
	H.push({ 0, v, 0, pos[v] });
	while (!H.empty()) {
		Toll p = H.top(); H.pop();
		if (W[p.n] < p.w) continue;
		if (p.n == d) return;
		for (int w = 1; w <= N; w++) {
			if (G[p.n][w] == -1) continue;
			ll ct = p.w + G[p.n][w];
			if (W[w] >= ct && (p.d < 1 || dot(p.pre, pos[p.n], pos[p.n], pos[w]) >= 0)) {
			//if (W[w] > ct) {
				W[w] = ct;
				H.push({ ct, w, p.d + 1, pos[p.n] });
				//if (w == 1)
				//std::cout << p.d << " " << p.n << " " <<  w << " " << G[p.n][w] << " " << p.w << " " << ct << " " << dot(p.pre, pos[p.n], pos[p.n], pos[w]) << " DEBUG\n";
			}
		}
	}
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int N, M, u, v;
	ll c;
	std::cin >> N >> M;
	for (int i = 1; i <= N; i++) {
		std::cin >> pos[i].x >> pos[i].y;
	}
	memset(G, -1, sizeof G);
	for (int i = 0; i < M; i++) {
		std::cin >> u >> v >> c;
		G[u][v] = G[v][u] = c;
	}
	dijkstra(1, 2, N);
	ll MIN = W[2];
	//std::cout << "\n";
	dijkstra(2, 1, N);
	MIN = std::min({ MIN, W[1] });
	std::cout << (MIN < MAX ? MIN : -1) << "\n";
	return 0;
}

	//for (const ll w : W) {
	//	std::cout << w << " ";
	//}
	//std::cout << " DEBUG\n";

//void dijkstra1(int v, int N) {
//	for (int i = 1; i <= N; i++) { W[i] = MAX; }
//	W[v] = 0;
//	H.push({ 0, v, 0, pos[v] });
//	while (!H.empty()) {
//		Toll p = H.top(); H.pop();
//		if (W[p.n] < p.w) continue;
//		for (int w = 1; w <= N; w++) {
//			if (G[p.n][w] > -1) {
//				ll ct = p.w + G[p.n][w];
//				if (W[w] > ct) {
//					if (p.d < 1 || dot(p.pre, pos[p.n], pos[p.n], pos[w]) >= 0) {
//						W[w] = ct;
//						H.push({ ct, w, p.d + 1, pos[p.n] });
//						//std::cout << p.d << " " << dot(p.pre, pos[p.n], pos[p.n], pos[w]) << " DEBUG\n";
//					}
//				}
//			}
//		}
//	}
//}

/*

10 11
10 0
30 50
0 10
20 10
10 20
-10 15
40 40
20 40
30 30
50 25
1 3 2
1 4 1
3 5 2
4 5 1
5 6 1
5 9 10
2 7 2
2 8 1
7 9 2
8 9 1
9 10 1


*/