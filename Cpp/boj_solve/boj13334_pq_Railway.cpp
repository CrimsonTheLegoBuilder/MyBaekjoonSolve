#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <cstring>
typedef long long ll;
const int LEN = 1e5 + 1;
int N, D, s, e;

struct Info {
	int s, e;
	Info(int S, int E) : s(S), e(E) {}
	bool operator < (const Info& x) const { return s == x.s ? e > x.e : s > x.s; }
	int L() const { return e - s; }
};
std::vector<Info> V;
std::priority_queue<Info> PQ;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> s >> e;
		if (s > e) std::swap(s, e);
		V.push_back(Info(s, e));
	}
	std::sort(V.begin(), V.end(), [&](const Info& p, const Info& q) -> bool {
		return p.e == q.e ? p.s < q.s : p.e < q.e;
		});
	std::cin >> D;
	int cnt = 0;
	for (const Info& x : V) {
		if (D < x.L()) continue;
		while (PQ.size() && PQ.top().s + D < x.e) PQ.pop();
		PQ.push(x);
		cnt = std::max(cnt, (int)PQ.size());
	}
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj13334 DNIC 2016 J Railway