#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
typedef long long ll;
const int LEN = 2e5 + 5;
//const ll INF = (1ll << 62) + ((1ll << 62) - 1);
const ll INF = 9223372036854775807;
int N, M;
ll C[LEN][2];

struct Info {
	int i;
	ll c_cur, c_nxt;
	Info(int i = 0, ll cc = 0, ll cn = 0) : i(i), c_cur(cc), c_nxt(cn) {}
	bool operator < (const Info& I) const {
		return c_cur > I.c_cur;
	}
};
std::priority_queue<Info> Q;
std::vector<Info> G[LEN];

ll dijkstra() {
	for (int i = 0; i < N; i++) C[i][0] = C[i][1] = INF;
	Q.push({ 0, 0 });
	C[0][0] = C[0][1] = 0;
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (C[p.i][0] < p.c_cur) continue;
		//if (p.i == D) return p.c;
		for (const Info& w : G[p.i]) {
			//std::cout << w.i << " " << w.c << "\n";
			ll cnxt = p.c_cur + w.c_cur * 9 / 10;
			if (C[w.i][0] > cnxt) {
				C[w.i][0] = cnxt;
				Q.push({ w.i, cnxt });
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (Info& I : G[i]) {
			C[I.i][1] = std::min(C[I.i][1], C[i][0] + I.c_cur);
		}
	}
	ll ret = 0;
	for (int i = 1; i < N; i++) ret += C[i][1];
	return ret;
}


int main() {
	int s, e; ll c;
	std::cin >> N >> M;
	for (int i = 0; i < M; i++) {
		std::cin >> s >> e >> c;
		s--, e--;
		G[s].push_back(Info(e, c));
		G[e].push_back(Info(s, c));
	}
	std::cout << dijkstra();
	return 0;
}