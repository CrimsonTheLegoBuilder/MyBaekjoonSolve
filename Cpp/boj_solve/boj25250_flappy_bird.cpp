#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int N, M, Q, T;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
int sign(const ll& x) { return x < 0 ? -1 : !!x; }

//#define DEBUG
//#define TEST

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return ret < 0 ? -1 : !!ret;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
#ifdef TEST
	freopen("../../../input_data/flappy/", "r", stdin);
	freopen("../../../input_data/flappy/flappy_out.txt", "w", stdout);
#endif
	int x, yd, yu;
	Pos s, e;
	std::cin >> s >> e >> N;
	std::vector<Pos> U(N), D(N);
	for (int i = 0; i < N; i++) {
		std::cin >> x >> yd >> yu;
		assert(yd < yu);
		D[i] = Pos(x, yd);
		U[i] = Pos(x, yu);
	}

#ifdef DEBUG
	for (int i = 0; i < N - 1; i++) {
		assert(D[i] < U[i]);
		assert(D[i] < D[i + 1]);
	}
#endif

	std::deque<Pos> HU, HD;
	HU.push_back(s); HU.push_back(U[0]);
	U.push_back(e);
	HD.push_back(s); HD.push_back(D[0]);
	D.push_back(e);

	std::vector<Pos> ret;
	int ptr = 1;
	ret.push_back(s);
	for (int i = 1; i <= N; i++) {
		while (HU.size() > 1 && ccw(HU[HU.size() - 2], HU.back(), U[i]) <= 0) HU.pop_back();
		HU.push_back(U[i]);
		while (HD.size() > 1 && ccw(HD[HD.size() - 2], HD.back(), D[i]) >= 0) HD.pop_back();
		HD.push_back(D[i]);
		ptr = 1;
		if (HU.size() <= 2 && HD.size() <= 2) continue;
		else if (HU.size() == 2) {
#ifdef DEBUG
			std::cout << "DEBUG::\n";
#endif
			if (ccw(HU[0], HU[1], HD[1]) <= 0) continue;
			while (ptr < HD.size() && ccw(HU[0], HU[1], HD[ptr]) > 0) ptr++;
			ptr--;
			while (ptr--) ret.push_back(HD[0]), HD.pop_front();
			ret.push_back(HD[0]);
			HU[0] = HD[0];
#ifdef DEBUG
			std::cout << "DEBUG:: " << HU[0] << "\n";
#endif
		}
		else if (HD.size() == 2) {
#ifdef DEBUG
			std::cout << "DEBUG::\n";
#endif
			if (ccw(HD[0], HD[1], HU[1]) >= 0) continue;
			while (ptr < HU.size() && ccw(HD[0], HD[1], HU[ptr]) < 0) ptr++;
			ptr--;
			while (ptr--) ret.push_back(HU[0]), HU.pop_front();
			ret.push_back(HU[0]);
			HD[0] = HU[0];
#ifdef DEBUG
			std::cout << "DEBUG:: " << HD[0] << "\n";
#endif
		}
	}
	ret.push_back(e);
	std::sort(ret.begin(), ret.end());
	ret.erase(unique(ret.begin(), ret.end()), ret.end());
	for (const Pos& p : ret) std::cout << p << "\n";
	return;
}
int main() { solve(); return 0; }//GCPC 2021 F Flappy Bird boj25250



/*

0 0 10 0
4
2 1 3
4 2 3
7 0 2
9 -2 -1

0 0 20 0
19
1 1 2
2 -2 -1
3 1 2
4 -2 -1
5 1 2
6 -2 -1
7 1 2
8 -2 -1
9 1 2
10 -2 -1
11 1 2
12 -2 -1
13 1 2
14 -2 -1
15 1 2
16 -2 -1
17 1 2
18 -2 -1
19 1 2

0 0 10 0
4
1 -1 1
2 -2 2
3 -1 1
4 -2 2

*/