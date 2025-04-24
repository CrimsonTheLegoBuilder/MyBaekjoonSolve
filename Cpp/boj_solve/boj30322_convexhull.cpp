#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
int N;
bool zero(const ld& x) { return std::abs(x) < TOL; }

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y << "\n";
		return os;
	}
}; const Pos O = { 0, 0 };
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
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
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
ld ternary_search(const Pos& d1, const Pos& v1, const Pos& d2, const Pos& v2, const ld& len) {
	auto diagonal = [&](const ld& s) -> ld {
		return ((d1 + v1 * s) - (d2 + v2 * s)).mag();
		};
	ld s = 0, e = len, l, r;
	int cnt = 20;
	while (cnt--) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		if (diagonal(l) < diagonal(r)) e = r;
		else s = l;
	}
	return diagonal((s + e) * .5);
}
std::vector<Pos> C, H;
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i];
	H = graham_scan(C);
	return;
}
void rotating_calipers() {
	init();
	ld R = 0;
	int sz = H.size();
	//if (sz == 2) { std::cout << "0\n"; return; }
	for (int i = 0; i < sz; i++) R += (H[i] - H[(i + 1) % sz]).mag();

	Pos S = H[0], E = H[0];
	int s = 0, e = 0;
	ld cur = 0;
	while (1) {
		ld seg = (H[(e + 1) % sz] - H[e]).mag();
		if (cur + seg < R * .5 + TOL) {
			cur += seg;
			e = (e + 1) % sz;
			E = H[e];
		}
		else {
			ld rem = R * .5 - cur;
			E += (H[(e + 1) % sz] - H[e]) * (rem / seg);
			break;
		}
	}

	ld ret = (S - E).mag();
	int cnt = sz << 1;
	while (cnt--) {
		Pos v1 = H[(s + 1) % sz] - S;
		Pos v2 = H[(e + 1) % sz] - E;
		ld l = v1.mag();
		ld r = v2.mag();
		ret = std::min(ret, ternary_search(S, v1 / l, E, v2 / r, std::min(l, r)));
		if (zero(l - r)) {
			s = (s + 1) % sz;
			e = (e + 1) % sz;
			S = H[s], E = H[e];
		}
		else if (l < r) {
			s = (s + 1) % sz;
			S = H[s];
			E += v2 * (l / r);
		}
		else {
			e = (e + 1) % sz;
			E = H[e];
			S += v1 * (r / l);
		}
		if (!e) break;
	}
	std::cout << ret << "\n";
	return;
}
int main() { rotating_calipers(); return 0; }//boj30322 Divide a Convex