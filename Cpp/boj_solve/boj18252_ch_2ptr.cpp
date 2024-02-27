#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
int N, M, T, Q;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }

struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p);
	friend std::ostream& operator << (std::ostream& os, const Pos& p);
} NH[LEN], MH[LEN], seq[LEN]; const Pos O = { 0, 0 };
std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y << "\n"; return os; }
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	//C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
void solve() {
	Pos top, bot;
	int y, xst, xen;
	std::cin >> N >> top >> bot;
	C = { top, bot };
	for (int i = 0; i < N; i++) {
		std::cin >> y >> xst >> xen;
		if (ccw(bot, top, Pos(xst, y)) < 0) C.push_back(Pos(xst, y));
		if (ccw(bot, top, Pos(xen, y)) > 0) C.push_back(Pos(xen, y));
	}
	H = graham_scan(C);
	int sz = H.size();
	if (sz == 2) { std::cout << "0.0000000\n"; return; }
	if (sz == 3) {
		ll ret = cross(H[0], H[1], H[2]);
		std::cout << (ret >> 1) << (ret & 1 ? ".5000000\n" : ".0000000\n"); return;
	}

	ll pre = 0, ans = 0;
	for (int i = 0, j, k; i < sz; i++) {
		j = (i + 2) % sz;
		k = (i + 1) % sz;
		pre = 0;
		while (j != i) {
			while ((k + 1) % sz != j && cross(H[i], H[k], H[j]) > pre) k = (k + 1) % sz;
			ll cur = cross(H[i], H[k], H[j]);
			if (pre > cur) break;
			pre = cur;
			ans = std::max(ans, pre);
			j = (j + 1) % sz;
		}
	}
	//std::cout << ans << "\n";
	std::cout << (ans >> 1) << (ans & 1 ? ".5000000\n" : ".0000000\n"); return;
}
int main() { solve(); return 0; }

/*

4
2 4 0 0
1 2 4
1 2 3
2 3 5
3 2 4

*/