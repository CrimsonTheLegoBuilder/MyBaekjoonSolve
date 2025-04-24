#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
const int LEN = 1e5;
const ll MAX = 1e17;
int N;
ll pole[LEN], temp[LEN];

ll f(const ll& m) {
	ll total = 0;
	for (ll i = 0; i < N; i++) temp[i] = m * i;
	for (int i = 0; i < N; i++) total += std::abs(pole[i] - temp[i]);
	return total;
}
ll ternaty_search(ll limit) {
	ll s = 0, e = limit, l, r;
	ll L, R;
	while (e - s > 2) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		L = f(l), R = f(r);
		//std::cout << l << " " << r << "\n";
		if (L > R) s = l;
		else e = r;
	}
	ll ans = MAX;
	for (int i = s; i <= e; i++) {
		ans = std::min(ans, f(i));
	}
	return ans;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> pole[i];
	ll limit = pole[N - 1];
	std::cout << ternaty_search(limit);
	return;
}
int main() { solve(); return 0; }//boj8986