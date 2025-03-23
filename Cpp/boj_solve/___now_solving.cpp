//freopen("../../../input_data//", "r", stdin);
//freopen("../../../input_data//", "w", stdout);
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
const ld INF = 1e21;
const ld TOL = 1e-9;
const ld PI = acos(-1);
const int LEN = 1e5 + 10;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

int N, K, Q;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(13);

	std::cin >> N;

	return;
}
int main() { solve(); return 0; }//boj30839 29688

