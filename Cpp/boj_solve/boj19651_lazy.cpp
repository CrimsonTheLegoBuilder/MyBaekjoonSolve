#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <cassert>
typedef long long ll;
const int LEN = 1e5 + 5;
const ll INF = 1e17;

int N, M, I, J, K; ll X, Y;
ll arr[LEN], step[LEN];// , tree[LEN << 2], lazy[LEN << 2];
struct Seq {
	ll i, diff;
	Seq(ll i = 0, ll diff = 0) : i(i), diff(diff) {}
};
typedef std::vector<Seq> Sequence;
struct Node {//segment tree for cal subsequence max len
	ll max, llen, rlen, all;
	Node(ll M = 0, ll L = 0, ll R = 0, ll A = 0) : max(M), llen(L), rlen(R), all(A) {}
	Node operator + (const Node& r) const {
		return {
			std::max({max, r.max, rlen + r.llen}),
			all ? max + r.llen : llen,
			r.all ? rlen + r.max : r.rlen,
			all & r.all
		};
	}
} seg_len[LEN << 1];
inline Node node(int x) { return Node(x == 0, x == 0, x == 0, x == 0); }
Node init(int s = 1, int e = N, int i = 1) {
	if (s == e) return seg_len[i] = node(step[s]);
	int m = s + e >> 1;
	return seg_len[i] = init(s, m, i << 1) + init(m + 1, e, i << 1 | 1);
}
void update(int idx, int v, int s = 0, int e = N - 1, int n = 1) {
	if (e < idx || idx < s) return;
	if (s == e) {
		seg_len[n] = node(v);
		return;
	}
	int m = s + e >> 1;
	update(idx, v, s, m, n << 1);
	update(idx, v, m + 1, e, n << 1 | 1);
	seg_len[n] = seg_len[n << 1] + seg_len[n << 1 | 1];
}
void update_len(ll i, ll j, ll x, ll y) {
	Sequence seq = {
		Seq(i - 1, x),
		Seq(i, -x + y),
		Seq(j, -x - (j - i + 1) * y),
		Seq(j + 1, x + (j - i) * y)
	};
	for (const Seq& s : seq) {
		if (step[s.i] == 0) update(s.i, 1);
		step[s.i] += s.diff;
		if (step[s.i] == 1) update(s.i, 0);
	}
	return;
}
Node search(int l, int r, int s = 0, int e = N - 1, int n = 1) {
	if (r < s || e < l) return Node(0, 0, 0, 0);
	if (l <= s && e <= r) return seg_len[n];
	int m = s + e >> 1;
	return search(l, r, s, m, n << 1) + search(l, r, m + 1, e, n << 1 | 1);
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> arr[i];
	for (int i = 2; i < N; i++) step[i] = arr[i - 1] + arr[i + 1] - 2 * arr[i];
	init(2, N - 1);
	int cnt = M, a;
	while (cnt--) {
		std::cin >> a >> I >> J;
		if (a == 1)	std::cin >> X >> Y, update_len(I, J, X, Y);
		else if (a == 2) std::cout << search(I, J).max + 2 << "\n";
	}
	return;
}
int main() { solve(); return 0; }//16975
