#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
const ll INF = 1e17;
const int LEN = 1e6 + 5;

int N;
ll P[LEN], Q[LEN], SUM[LEN], DIF[LEN];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> P[i];
	//for (int i = 0; i < N; i++) std::cin >> Q[i];
	for (int i = 0; i < N; i++) {
		std::cin >> Q[i];
		SUM[i] = P[i] + Q[i];
		DIF[i] = P[i] - Q[i];
	}
	ll ret = 0;
	std::sort(P, P + N);
	std::sort(Q, Q + N);
	std::sort(SUM, SUM + N);
	std::sort(DIF, DIF + N);
	for (ll i = 0; i < N; i++) {
		ret -= (SUM[i] + DIF[i]) * (2 * i + 1 - N);//(N - i - (i - 1)))
		ret += (P[i] + Q[i]) * 2 * (2 * i + 1 - N);
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj22878
