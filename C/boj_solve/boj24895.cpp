#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 10'000;
int n, m, k, out;
struct Pos { ll x, y; }N[LEN], M[LEN], K[LEN * 30];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
bool I(const Pos& p, Pos H[], int h) {
	if (h < 3 || cross(H[0], H[1], p) <= 0 || cross(H[0], H[h - 1], p) >= 0) return 0;
	int s = 0, e = h - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	return cross(H[s], H[e], p) > 0;
}
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> n >> m >> k;
	for (int i = 0; i < n; i++) { std::cin >> N[i].x >> N[i].y; }
	for (int i = 0; i < m; i++) { std::cin >> M[i].x >> M[i].y; }
	for (int i = 0; i < k; i++) { std::cin >> K[i].x >> K[i].y; }
	for (int i = 0; i < k; i++) { if (!I(K[i], N, n) || I(K[i], M, m)) out++; }
	if (!out) std::cout << "YES" << "\n";
	else std::cout << out << "\n";
	return 0;
}