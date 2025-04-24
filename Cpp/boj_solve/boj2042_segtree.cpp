#include <iostream>
#include <vector>
typedef long long ll;
using namespace std;
vector<ll> segtree;
vector<ll> arr;

ll sum(int s, int e, int l, int r, int n) {
	if (e < l || r < s) return 0;
	if (s <= l && r <= e) return segtree[n];
	int m = l + r >> 1;
	ll L = sum(s, e, l, m, n << 1);
	ll R = sum(s, e, m + 1, r, (n << 1) + 1);
	return L + R;
}
void update(int l, int r, int n, int idx, ll diff) {
	if (r < idx || idx < l) return;
	segtree[n] += diff;
	if (l != r) {
		int m = l + r >> 1;
		update(l, m, n << 1, idx, diff);
		update(m + 1, r, (n << 1) + 1, idx, diff);
		return;
	}
}
ll init(int l, int r, int n) {
	if (l == r) return segtree[n] = arr[l];
	int m = l + r >> 1;
	ll L = init(l, m, n << 1);
	ll R = init(m + 1, r, (n << 1) + 1);
	return segtree[n] = L + R;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int N, M, K, Q, a, b;
	ll x, c, diff;
	cin >> N >> M >> K;
	Q = M + K;
	arr.resize(N);
	segtree.resize(N * 4);
	for (int i = 0; i < N; i++) {
		cin >> arr[i];
	}
	init(0, N - 1, 1);
	while (Q--) {
		cin >> a >> b >> c;
		if (a == 1) {
			b--;
			diff = c - arr[b];
			update(0, N - 1, 1, b, diff);
			arr[b] += diff;
		}
		else if (a == 2) {
			b--; c--;
			cout << sum(b, c, 0, N - 1, 1) << "\n";
		}
	}
	return 0;
}