#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
std::vector<ll> TREE;
std::vector<ll> arr;

ll sum(int l, int r, int s, int e, int n) {
	if (e < l || r < s) return 0;
	if (s <= l && r <= e) return TREE[n];
	int m = l + r >> 1;
	ll L = sum(l, m, s, e, n << 1);
	ll R = sum(m + 1, r, s, e, (n << 1) + 1);
	return L + R;
}
void update(int l, int r, int idx, int diff, int n) {
	if (r < idx || idx < l) return;
	TREE[n] += diff;
	if (l != r) {
		int m = l + r >> 1;
		update(l, m, idx, diff, n << 1);
		update(m + 1, r, idx, diff, (n << 1) + 1);
		return;
	}
}
ll init(int l, int r, int n) {
	if (l == r) return TREE[n] = arr[l];
	int m = l + r >> 1;
	ll L = init(l, m, n << 1);
	ll R = init(m + 1, r, (n << 1) + 1);
	return TREE[n] = L + R;
}
int bi_search(ll x, int N) {
	int s = 0, e = N - 1;
	int m;
	ll tmp;
	while (s < e) {
		m = s + e >> 1;
		tmp = sum(0, N - 1, 0, m, 1);
		if (tmp >= x) e = m;
		else s = m + 1;
	}
	return e + 1;
}



int main(void) {
	std::cin.tie(0)->sync_with_stdio(0);
	int N, Q;
	std::cin >> N;
	arr.resize(N);
	TREE.resize(4 * N);
	for (int i = 0; i < N; i++) {
		std::cin >> arr[i];
	}
	init(0, N - 1, 1);
	std::cin >> Q;
	while (Q--) {
		int q, a;
		ll I;
		std::cin >> q;
		if (q == 1) {
			std::cin >> I >> a;
			I--;
			update(0, N - 1, I, a, 1);
			arr[I] += a;
		}
		else if (q == 2) {
			std::cin >> I;
			a = bi_search(I, N);
			std::cout << a << "\n";
		}
	}
	return 0;
}

/*

#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
ll fenwick[500'001];
ll arr[500'001];

ll sum(int idx) {
	ll s = 0;
	while (idx > 0) {
		s += fenwick[idx];
		idx -= idx & -idx;
	}
	return s;
}
void update(int idx, int N, int diff) {
	while (idx <= N) {
		fenwick[idx] += diff;
		idx += idx & -idx;
	}
}
void init(int N) {
	for (int i = 1; i <= N; i++) {
		update(i, N, arr[i]);
	}
}
int bi_search(ll x, int N) {
	int s = 1, e = N;
	int m;
	ll tmp;
	while (s < e) {
		m = s + e >> 1;
		tmp = sum(m);
		//std::cout << tmp << " " << s << " " << m << " " << e << "\n";
		if (tmp >= x) e = m;
		else s = m + 1;
	}
	return e;
}



int main(void) {
	std::cin.tie(0)->sync_with_stdio(0);
	int N, Q;
	std::cin >> N;
	for (int i = 1; i <= N; i++) {
		std::cin >> arr[i];
	}
	init(N);
	std::cin >> Q;
	while (Q--) {
		int q, a;
		ll I;
		std::cin >> q;
		if (q == 1) {
			std::cin >> I >> a;
			update(I, N, a);
			arr[I] += a;
		}
		else if (q == 2) {
			std::cin >> I;
			a = bi_search(I, N);
			std::cout << a << "\n";
		}
	}
	return 0;
}

*/