#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <cassert>
typedef long long ll;
const int LEN = 1e5 + 5;
const ll INF = 1e17;

ll N, M, Q, I, J, K;
ll X, Y;
ll arr[LEN], step[LEN];
struct Seq {
	int i;
	ll diff;
	Seq(int i = 0, ll diff = 0) : i(i), diff(diff) {}
};
typedef std::vector<Seq> Sequence;
struct Node {//segment tree for cal subsequence max len
	int max, llen, rlen;
	bool all;
	Node(int M = 0, int L = 0, int R = 0, bool A = 0) : max(M), llen(L), rlen(R), all(A) {}
	inline Node operator + (const Node& r) const {
		return {
			std::max({max, r.max, rlen + r.llen}),
			all ? max + r.llen : llen,
			r.all ? rlen + r.max : r.rlen,
			all && r.all
		};
	}
} len_seg[LEN << 2];
inline Node node(const ll& x) { return Node(!x, !x, !x, !x); }
Node init(int s = 1, int e = N, int i = 1) {
	if (s == e) return len_seg[i] = node(step[s]);
	int m = s + e >> 1;
	return len_seg[i] = init(s, m, i << 1) + init(m + 1, e, i << 1 | 1);
}
Node update(const int& idx, const ll& v, int s = 1, int e = N, int i = 1) {
	if (e < idx || idx < s) return len_seg[i];
	if (s == e) return len_seg[i] = node(v);
	int m = s + e >> 1;
	return len_seg[i] = update(idx, v, s, m, i << 1) + update(idx, v, m + 1, e, i << 1 | 1);
}
inline void update_len(const int& i, const int& j, const ll& x, const ll& y) {
	Sequence seq = {
		Seq(i - 1, x),
		Seq(i, -x + y),
		Seq(j, -x - y * (j - i + 1)),
		Seq(j + 1, x + y * (j - i))
	};
	for (const Seq& s : seq) {
		step[s.i] += s.diff;
		if (s.diff) update(s.i, step[s.i], 2, N - 1);
	}
	return;
}
Node search(const int& l, const int& r, int s = 1, int e = N, int i = 1) {
	if (r < s || e < l) return Node(0, 0, 0, 0);
	if (l <= s && e <= r) return len_seg[i];
	int m = s + e >> 1;
	return search(l, r, s, m, i << 1) + search(l, r, m + 1, e, i << 1 | 1);
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> arr[i];
	for (int i = 2; i < N; i++) step[i] = arr[i + 1] + arr[i - 1] - (arr[i] << 1);
	/*
	 * arr =  [a1, a2, a3, a4, a5, a6, a7, a8, ...]
	 * arith =  [b1, b2, b3, b4, b5, b6, b7, ...]
	 * step = [__, c1, c2, c3, c4, c5, c6, ...]
	 * 
	 * step[i] = arr[i-1] + arr[i+1] - 2 * arr[i]
	 * (c1 = b2 - b1 = (a3 - a2) - (a2 - a1) = a3 + a1 - a2 - a2)
	 * 
	 * if arr[i ~ j] is arithmetic order, c[i+1 ~ j-1] == 0
	 * 
	 */
	init(2, N - 1);
	std::cin >> M;
	while (M--) {
		std::cin >> Q >> I >> J;
		if (Q == 1)	std::cin >> X >> Y, update_len(I, J, X, Y);
		if (Q == 2) std::cout << (2 + search(I + 1, J - 1, 2, N - 1).max) << '\n';
	}
	return;
}
int main() { solve(); return 0; }//boj19651 refer to unordered_map
