#include <iostream>
typedef long long ll;
ll N, total;

void dfs(ll u, ll l, ll r, ll d) {
	if (d == N) { total++; return; }
	l <<= 1; r >>= 1;
	ll a = u | l | r;
	for (int i = 0; i < N; i++)
		if (!(a & 1ll << i))
			dfs(u | 1ll << i, l | 1ll << i, r | 1ll << i, d + 1);
	return;
}

int main() {
	std::cin >> N;
	dfs(0, 0, 0, 0);
	std::cout << total << "\n";
	return 0;
}