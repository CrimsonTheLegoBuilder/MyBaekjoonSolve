#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
std::vector<ll> tree;
std::vector<ll> arr;

ll sum(int l, int r, int s, int e, int n) {
	if (e < l || r < s) return 0;
	if (s <= l && r <= e) return tree[n];
	int m = (l + r) / 2;
	ll L = sum(l, m, s, e, n * 2);
	ll R = sum(m + 1, r, s, e, n * 2 + 1);
	return L + R;
}
void update(int l, int r, int idx, ll diff, int n) {
	if (idx < l || r < idx) return;
	tree[n] += diff;
	if (l != r) {
		int m = (l + r) / 2;
		update(l, m, idx, diff, n * 2);
		update(m + 1, r, idx, diff, n * 2 + 1);
	}
}
ll init(int l, int r, int n) {
	if (l == r) return tree[n] = arr[l];
	int m = (l + r) / 2;
	ll L = init(l, m, n * 2);
	ll R = init(m + 1, r, n * 2 + 1);
	tree[n] = L + R;
	return tree[n];
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	int N, Q;
	std::cin >> N >> Q;
	arr.resize(N);
	for (int i = 0; i < N; i++) {
		std::cin >> arr[i];
	}
	tree.resize(4 * N);
	init(0, N - 1, 1);
	for (int q = 0; q < Q; q++) {
		int x, y, a;
		ll b, diff, S;
		std::cin >> x >> y >> a >> b;
		x--; y--; a--;

		if (x > y) std::swap(x, y);
		diff = b - arr[a];

		S = sum(0, N - 1, x, y, 1);
		std::cout << S << "\n";

		update(0, N - 1, a, diff, 1);
		arr[a] = b;
	}
	return 0;
}

/*

#include <iostream>

typedef long long ll;
const int LEN = 100'001;

/// <summary>
/// point update
/// range query
/// </summary>

int N, Q;
ll x, y, a, b, d, A[LEN], fenwick[LEN];
ll sum(int i) {
	ll result = 0;
	while (i > 0) {
		result += fenwick[i];
		i -= i & -i;
	}
	return result;
}
void update(int i, ll d) {
	while (i <= N) {
		fenwick[i] += d;
		i += i & -i;
	}
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cin >> N >> Q;
	for (int i = 1; i <= N; ++i) {
		std::cin >> A[i];
		update(i, A[i]);
	}
	while (Q--) {
		std::cin >> x >> y >> a >> b;
		if (x > y) std::swap(x, y);
		std::cout << sum(y) - sum(x - 1) << '\n';
		d = b - A[a];
		A[a] = b;
		update(a, d);
	}
}

*/