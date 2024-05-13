#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <cassert>
typedef long long ll;
const int LEN = 2e5 + 5;
const ll INF = 1e17;

int N, Q;
int a, b;
ll c, d;
char q;
ll sum_tree[LEN << 2], min_tree[LEN << 2], lazy[LEN << 2];
void lazy_propagate(int s, int e, int i) {
	if (lazy[i]) {
		min_tree[i] += lazy[i];
		sum_tree[i] += lazy[i] * ((ll)e - s + 1);
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
		min_tree[i] += diff;
		sum_tree[i] += diff * ((ll)e - s + 1);
		if (s ^ e) {
			lazy[i << 1] += diff;
			lazy[i << 1 | 1] += diff;
		}
		return;
	}
	int m = s + e >> 1;
	update(l, r, diff, s, m, i << 1);
	update(l, r, diff, m + 1, e, i << 1 | 1);
	min_tree[i] = std::min(min_tree[i << 1], min_tree[i << 1 | 1]);
	sum_tree[i] = sum_tree[i << 1] + sum_tree[i << 1 | 1];
	return;
}
ll sum_search(int l, int r, int s = 1, int e = N, int i = 1) {
	lazy_propagate(s, e, i);
	if (r < s || e < l) return 0;
	if (l <= s && e <= r) return sum_tree[i];
	int m = s + e >> 1;
	return sum_search(l, r, s, m, i << 1) + sum_search(l, r, m + 1, e, i << 1 | 1);
}
ll min_search(int l, int r, int s = 1, int e = N, int i = 1) {
	lazy_propagate(s, e, i);
	if (r < s || e < l) return INF;
	if (l <= s && e <= r) return min_tree[i];
	int m = s + e >> 1;
	return std::min(min_search(l, r, s, m, i << 1), min_search(l, r, m + 1, e, i << 1 | 1));
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> Q;
	for (int i = 1; i <= N; i++) std::cin >> d, update(i, i, d);
	while (Q--) {
		std::cin >> q >> a >> b;
		if (b < a) std::swap(a, b);
		if (q == 'M') std::cout << min_search(a, b) << "\n";
		else if (q == 'S') std::cout << sum_search(a, b) << "\n";
		else if (q == 'P') std::cin >> c, update(a, b, c);
	}
	return;
}
int main() { solve(); return 0; }//10999 Find sum of intervals 2
