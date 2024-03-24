#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
const int LEN = 100'001;
const int INF = 2e9;
int T, N, B, P, Q;
char X;
ll arr[LEN];
ll segtree[LEN << 2];

ll init(int l, int r, int n) {
	if (l == r) return segtree[n] = arr[l];
	int m = l + r >> 1;
	ll L = init(l, m, n << 1);
	ll R = init(m + 1, r, (n << 1) + 1);
	return segtree[n] = L + R;
}
void update(int l, int r, int idx, int n, ll diff) {
	if (r < idx || idx < l) return;
	segtree[n] += diff;
	if (l != r) {
		int m = l + r >> 1;
		update(l, m, idx, n << 1, diff);
		update(m + 1, r, idx, n << 1 | 1, diff);
	}
	return;
}
ll search(int s, int e, int l = 1, int r = LEN - 1, int n = 1) {
	if (r < s || e < l) return 0;
	if (s <= l && r <= e) return segtree[n];
	int m = l + r >> 1;
	ll L = search(s, e, l, m, n << 1);
	ll R = search(s, e, m + 1, r, n << 1 | 1);
	return L + R;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		std::cin >> B >> P >> Q;
		N = P + Q;
		memset(arr, 0, sizeof arr);
		memset(segtree, 0, sizeof segtree);
		ll s, e;
		while (N--) {
			std::cin >> X >> s >> e;
			if (X == 'P') {
				ll diff = e - arr[s];
				update(1, B, s, 1, diff);
				arr[s] = e;
			}
			else if (X == 'Q') {
				ll q = search(s, e);
				std::cout << q << "\n";
			}
		}
	}
	return;
}
int main() { solve(); return 0; }//boj2357