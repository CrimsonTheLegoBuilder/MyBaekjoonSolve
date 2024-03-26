#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <vector>
typedef long long ll;
const int LEN = 1e6 + 1;
int N;
ll a, b, c;
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
ll bi_search(const int& b) {
	int s = 1, e = 1'000'000, m;
	while (s < e) {
		m = s + e >> 1;
		ll cnt = search(1, m);
		if (cnt >= b) e = m;
		else s = m + 1;
	}
	//std::cout << "DEBUG:: " << s << " " << e << "\n";
	//if (s == LEN) return LEN - 1;
	return e;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	memset(arr, 0, sizeof arr);
	memset(segtree, 0, sizeof segtree);
	std::cin >> N;
	while (N--) {
		std::cin >> a;
		if (a == 1) {
			std::cin >> b;
			b = bi_search(b);
			std::cout << b << "\n";
			update(1, LEN - 1, b, 1, -1);
		}
		else if (a == 2) {
			std::cin >> b >> c;
			update(1, LEN - 1, b, 1, c);
		}
	}
	return;
}
int main() { solve(); return 0; }//boj2243 candy box
