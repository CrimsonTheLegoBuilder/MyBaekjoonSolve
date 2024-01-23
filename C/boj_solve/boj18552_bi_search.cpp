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
	Pos operator / (ld n) const { return { x / n, y / n }; }
	Pos operator ~ () const { return { -y, x }; }
	ld mag() { return hypot(x, y); }
} p, FR, FL, RR, RL, direction;
std::vector<Pos> potholes[LEN];
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
bool intersect(const Pos& d1, const Pos& d2, const Pos& s1, const Pos& s2) {
	bool f1 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	bool f2 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	return f1 && f2;
}
bool intersect(int i, const Pos& s1, const Pos& s2) {
	int sz = potholes[i].size();
	Pos U = potholes[i][0], D = potholes[i][0];
	ld cu = -MAX, cd = MAX;
	for (int j = 0; j < sz; j++) {
		if (cross(s1, s2, potholes[i][j]) > cu) cu = cross(s1, s2, potholes[i][j]), U = potholes[i][j];
		if (cross(s1, s2, potholes[i][j]) < cd) cd = cross(s1, s2, potholes[i][j]), D = potholes[i][j];
	}
	return intersect(U, D, s1, s2);
}
bool inner_check(int i, const Pos& s2) {
	int sz = potholes[i].size();
	for (int j = 0; j < sz; j++) {
		Pos cur = potholes[i][j], nxt = potholes[i][(j + 1) % sz];
		if (cross(cur, nxt, s2) < TOL) return 0;
	}
	return 1;
}
int intersect(ld D) {
	Pos FRd = FR + (direction * D), FLd = FL + (direction * D);
	Pos RRd = RR + (direction * D), RLd = RL + (direction * D);
	bool f1, f2, f3, f4, f5;
	int cnt{ 0 };
	for (int i = 0; i < N; i++) {
		f1 = intersect(i, FR, FRd);
		f2 = intersect(i, FL, FLd);
		f3 = intersect(i, RR, RRd);
		f4 = intersect(i, RL, RLd);
		f5 = inner_check(i, FRd) || inner_check(i, FLd) || inner_check(i, RRd) || inner_check(i, RLd);
		cnt += (f1 || f2 || f3 || f4 || f5);
	}
	return cnt;
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
ld bi_search(int K) {
	ld s = 0, e = MAX, m = 0;
	int cnt = 100;
	while (cnt--) {
		m = (s + e) * .5;
		std::cout << intersect(m) << "\n";
		if (K < intersect(m)) e = m;
		else s = m;
	}
	return m;
}

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
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
	direction = ~(FR - FL);
	std::cout << direction.x << " " << direction.y << "\n";
	ld MAG = direction.mag();
	direction = direction / MAG;
	std::cout << bi_search(K);
	return;
}
int main() { solve(); return 0; }//boj18552