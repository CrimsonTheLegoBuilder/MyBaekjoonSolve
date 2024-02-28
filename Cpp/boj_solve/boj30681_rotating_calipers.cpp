#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const int LEN = 1e5 + 1;
int N;
ld memo[LEN];

struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	ll Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p);
	friend std::ostream& operator << (std::ostream& os, const Pos& p);
};
std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y << "\n"; return os; }
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
void get_round_memo(std::vector<Pos>& H, ld memo[]) {
	int sz = H.size();
	memo[0] = .0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		memo[i + 1] = (cur - nxt).mag() + memo[i];//memo[sz] == convex hull's round
	}
	return;
}
ll ccw(const std::vector<Pos>& H, const int& i, const int& f) {
	int sz = H.size();
	int idx1 = i % sz, idx2 = f % sz;
	return (H[(idx1 + 1) % sz] - H[idx1]) / (H[(idx2 + 1) % sz] - H[idx2]);
}
void rotating_calipers() {
	std::swap(C[0], *min_element(C.begin(), C.end()));//graham scan
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) {
		int ret = ccw(C[0], p, q);
		if (ret != 0) return ret > 0;
		return (C[0] - p).Euc() < (C[0] - q).Euc();
		}
	);
	for (int i = 0; i < N; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0) 
			H.pop_back();
		H.push_back(C[i]);
	}
	N = H.size();

	if (N == 2) { std::cout << (H[0] - H[1]).mag() << "\n"; return; }
	get_round_memo(H, memo);

	ld MIN = INF ;
	for (int i = 0, f = 1; i < N; i++) {
		while (ccw(H, i, f) > 0) f = (f + 1) % N;
		int idx1 = i, idx2 = f;
		if (idx2 < idx1) std::swap(idx1, idx2);
		ld R1 = memo[idx2] - memo[idx1];
		ld R2 = memo[N] - R1;
		MIN = std::min({ MIN, R1, R2 });
	}
	std::cout << MIN << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i];
	rotating_calipers();
}
int main() { solve(); return 0; }//boj30681 Star capture


//if (N == 3) {
//	ld ret = std::min({ (H[0] - H[1]).mag(), (H[1] - H[2]).mag(), (H[2] - H[0]).mag() });
//	std::cout << ret << "\n"; return;
//}