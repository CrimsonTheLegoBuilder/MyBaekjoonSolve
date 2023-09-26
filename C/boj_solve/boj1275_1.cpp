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