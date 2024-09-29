#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cstring>
typedef long long ll;
typedef long double ld;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const int LEN = 5e5 + 5;
const ll MOD = 1'000'000'007;
ll sq(ll x) { return x * x % MOD; }

int N, M, K, T, Q;
ll cur, H[LEN];
struct Num {
	ll h, n;
	Num(ll X = 0, ll Y = 0) : h(X), n(Y) {}
};
std::vector<Num> S;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	ll cnt = 0;
	ll tmp;
	while (N--) {
		std::cin >> cur;
		tmp = 1;
		while (S.size() && S.back().h < cur) {
			cnt += sq(S.back().h - cur) % MOD * (ll)S.back().n;
			cnt %= MOD;
			S.pop_back();
		}
		if (S.size()) {
			if (S.back().h == cur) {
				cnt += 0;
				tmp = S.back().n + 1;
				if (S.size() > 1) cnt += 0;
				S.pop_back();
			}
			else {
				cnt += sq(S.back().h - cur) % MOD;
				cnt %= MOD;	
			}
		}
		S.push_back(Num(cur, tmp));
		cnt %= MOD;
	}
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }