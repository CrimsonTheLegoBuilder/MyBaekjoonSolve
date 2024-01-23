#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-6;
const ld MAX = 2e8;
const int LEN = 50;
int K, N, M, Z;

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (ld n) const { return { x * n, y * n }; }
	Pos operator ~ () const { return { y, -x }; }
} p, FR, FL, RR, RL;
std::vector<Pos> potholes[LEN];
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& p : C) H.push_back(p);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
bool cross() {
	return 1;
}
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
	for (int i = 0; i < N; i++) {
		std::cin >> M;
		for (int j = 0; j < M; j++) {
			std::cin >> p.x >> p.y >> Z;
			if (!Z) potholes[i].push_back(p);
		}
		potholes[i] = monotone_chain(potholes[i]);
	}
	std::cout << bi_search(K);
	return;
}
int main() { solve(); return 0; }//boj18552