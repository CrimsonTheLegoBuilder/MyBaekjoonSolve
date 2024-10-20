#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
const int LEN = 1e6 + 1;

ll N, S, M, L, XL, XXL, XXXL, T, P;
ll sum_t;
ll count_shirt(ll sz, ll t) {
	return sz / t + (sz % t != 0);
}
std::pair<ll, ll> count_pen(ll n, ll p) {
	ll doz = n / p;
	ll rem = n % p;
	return { doz, rem };
}
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	std::cin >> N >> S >> M >> L >> XL >> XXL >> XXXL >> T >> P;
	sum_t = count_shirt(S, T);
	sum_t += count_shirt(M, T);
	sum_t += count_shirt(L, T);
	sum_t += count_shirt(XL, T);
	sum_t += count_shirt(XXL, T);
	sum_t += count_shirt(XXXL, T);
	auto [doz_p, rem_p] = count_pen(N, P);
	std::cout << sum_t << "\n";
	std::cout << doz_p << " ";
	std::cout << rem_p << "\n";
	return 0;
}