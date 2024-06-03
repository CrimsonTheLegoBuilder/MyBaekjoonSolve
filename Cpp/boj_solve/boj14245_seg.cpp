#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
const int LEN = 500'001;
const int INF = 2e9;
int T, N, M, X, I, J; ll K;
ll arr[LEN];
ll segtree[LEN << 2];

void update(int l, int r, ll diff, int s = 1, int e = N, int i = 1) {
	if (r < s || e < l) return;
	if (l <= s && e <= r) { segtree[i] ^= diff; return; }
	int m = s + e >> 1;
	update(l, r, diff, s, m, i << 1);
	update(l, r, diff, m + 1, e, i << 1 | 1);
	return;
}
ll search(int idx, int s = 1, int e = N, int i = 1) {
	if (e < idx || idx < s) return 0;
	if (s == e) return segtree[i] ^ arr[idx];
	int m = s + e >> 1;
	ll L = search(idx, s, m, i << 1);
	ll R = search(idx, m + 1, e, i << 1 | 1);
	return segtree[i] ^ L ^ R;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	memset(arr, 0, sizeof arr);
	memset(segtree, 0, sizeof segtree);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> arr[i];
	std::cin >> M;
	while (M--) {
		std::cin >> X;
		if (X == 1) {
			std::cin >> I >> J >> K;
			if (I > J) std::swap(I, J);
			update(I + 1, J + 1, K);
		}
		else {
			std::cin >> I;
			std::cout << search(I + 1) << "\n";
		}
	}
	return;
}
int main() { solve(); return 0; }//boj14245 XOR