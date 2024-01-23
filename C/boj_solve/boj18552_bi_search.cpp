#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-6;
const ld MAX = 2e8;
const int LEN = 50;
int K, N;

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (ld n) const { return { x * n, y * n }; }
	Pos operator ~ () const { return { y, -x }; }
} FR, FL, RR, RL;
std::vector<Pos> potholes[LEN];

ld bi_search(int K) {
	ld s = 0, e = MAX, m = 0;
	int cnt = 100;
	while (cnt--) {
		m = (s + e) * .5;


	}
	return m;
}

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> K;
	std::cin >> FR.x >> FR.y >> FL.x >> FL.y >> RL.x >> RL.y >> RR.x >> RR.y;
	std::cin >> N;


	return;
}
int main() { solve(); return 0; }//boj18552