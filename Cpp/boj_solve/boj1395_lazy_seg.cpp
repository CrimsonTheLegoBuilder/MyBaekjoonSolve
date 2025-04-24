#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cassert>
typedef long long ll;
const int LEN = 1e5 + 5;
const ll INF = 1e17;

int N, M;
int o, s, t;
ll tree[LEN << 2], lazy[LEN << 2];
void lazy_propagate(int s, int e, int i) {
	if (lazy[i]) {
		if (lazy[i] & 1) tree[i] = ((ll)e - s + 1) - tree[i];
		if (s ^ e) {
			lazy[i << 1] += lazy[i];
			lazy[i << 1 | 1] += lazy[i];
		}
		lazy[i] = 0;
	}
	return;
}
void update(int l, int r, ll diff = 1, int s = 1, int e = N, int i = 1) {
	lazy_propagate(s, e, i);
	if (r < s || e < l) return;
	if (l <= s && e <= r) {
		tree[i] = ((ll)e - s + 1) - tree[i];
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
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	while (M--) {
		std::cin >> o >> s >> t;
		if (s > t) std::swap(s, t);
		if (o == 0) update(s, t);
		else if (o == 1) std::cout << search(s, t) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//1395
