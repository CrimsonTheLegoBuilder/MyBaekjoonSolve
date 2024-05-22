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
int a, b, c;
ll d;
ll arr[LEN], tree[LEN << 2], lazy[LEN << 2];
struct LenNode {//segment tree for cal subsequence max len
	ll max, llen, rlen, all;
	LenNode(ll M = 0, ll L = 0, ll R = 0, ll A = 0) : max(M), llen(L), rlen(R), all(A) {}
	LenNode operator + (const LenNode& r) const {
		return {
			std::max({max, r.max, rlen + r.llen}),
			std::max({llen, all + r.llen}),
			std::max({rlen + r.all, r.rlen}),
			all + r.all
		};
	}
} seg_len[LEN << 1];
void lazy_propagate(int s, int e, int i) {
	if (lazy[i]) {
		tree[i] += lazy[i] * ((ll)e - s + 1);
		if (s ^ e) {
			lazy[i << 1] += lazy[i];
			lazy[i << 1 | 1] += lazy[i];
		}
		lazy[i] = 0;
	}
	return;
}
void update(int l, int r, ll diff, int s = 1, int e = N, int i = 1) {
	lazy_propagate(s, e, i);
	if (r < s || e < l) return;
	if (l <= s && e <= r) {
		tree[i] += diff * ((ll)e - s + 1);
		if (s ^ e) {
			lazy[i << 1] += diff;
			lazy[i << 1 | 1] += diff;
		}
		return;
	}
	int m = s + e >> 1;
	update(l, r, diff, s, m, i << 1);
	update(l, r, diff, m + 1, e, i << 1 | 1);
	tree[i] = tree[i << 1] + tree[i << 1 | 1];
	return;
}
ll search(int l, int r, int s = 1, int e = N, int i = 1) {
	lazy_propagate(s, e, i);
	if (r < s || e < l) return 0;
	if (l <= s && e <= r) return tree[i];
	int m = s + e >> 1;
	return search(l, r, s, m, i << 1) + search(l, r, m + 1, e, i << 1 | 1);
}
ll init(int s = 1, int e = N, int i = 1) {
	if (s == e) return tree[i] = arr[s];
	int m = s + e >> 1;
	tree[i << 1] = init(s, m, i << 1);
	tree[i << 1 | 1] = init(m + 1, e, i << 1 | 1);
	return tree[i] = tree[i << 1] + tree[i << 1 | 1];
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> arr[i];
	init();
	int cnt = M;
	while (cnt--) {
		std::cin >> a >> I >> J;
		if (a == 1) {
			std::cin >> X >> Y;
			update(I, J, X);
		}
		else if (a == 2) {
			std::cout << search(I, J) << "\n";
		}
	}
	return;
}
int main() { solve(); return 0; }//16975
