#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 2e5;
int N, C;
ll X[LEN];

ll bi_search(ll c) {
	auto check = [&](ll m) -> bool {
		ll cur = X[0];
		int cnt = 1;
		for (int i = 1; i < N; i++) {
			if (cur + m <= X[i]) cur = X[i], cnt++;
		}
		return cnt >= c;
		};

	std::sort(X, X + N);
	ll s = 0, e = X[N - 1] - X[0] + 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (check(m)) s = m;
		else e = m;
	}
	return s;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cin >> N >> C;
	for (int i = 0; i < N; i++) std::cin >> X[i];
	std::cout << bi_search(C) << "\n";
	return;
}
int main() { solve(); return 0; }