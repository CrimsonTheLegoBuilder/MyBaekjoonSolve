#include <iostream>
#include <algorithm>
#include <vector>
const int LEN = 1'000'001;
const int MAX = 1000;
int N, K, P[MAX], depth, bro;
std::vector<int> n, TREE[MAX];
struct Info { int d, bro; }I[LEN];

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	while (1) {
		for (int i = 0; i < MAX; i++) { TREE[i].clear(); }
		std::cin >> N >> K;
		if (!N && !K) return 0;
		n.resize(N);
		memset(P, 0, sizeof P);
		for (int i = 0; i < N; i++) { std::cin >> n[i]; }
		int s = 0, m = 0, e = 0, d = 0, cnt = 0, p = 1;
		while (++e <= N) {
			if (e >= N || n[m] + 1 < n[e]) {
				for (int i = s; i < e; i++) {
					I[n[i]] = { d, e - s };
					TREE[d].push_back(n[i]);
					P[d]++;
				}
				s = e, m = e;
				cnt++;
				if (cnt == p) {
					p = P[d];
					cnt = 0;
					d++;
					continue;
				}
				continue;
			}
			m++;
		}
		depth = I[K].d; bro = I[K].bro;
		std::cout << TREE[depth].size() - bro << "\n";
		//std::cout << TREE[depth].size() - bro << " " << depth <<  " " << TREE[depth].size() << " " << bro << "\n";
	}
	return 0;
}

