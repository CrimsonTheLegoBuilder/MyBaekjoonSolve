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
	while (s <= e) {
		m = s + e >> 1;
		tmp = sum(m);
		if (tmp == x) return m;
		else if (tmp < x) {
			s = m + 1;
		}
		else e = m - 1;
	}
	return m;
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