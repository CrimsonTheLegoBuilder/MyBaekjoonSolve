#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const ld TOL = 1e-7;
int N, M, T;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

struct Pos {
	ld x, y;
	bool i;
	Pos(ld X = 0, ld Y = 0, bool I = 0) : x(X), y(Y), i(I) {}
	inline bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	inline ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	inline ld Euc() const { return x * x + y * y; }
	//inline ld mag() const { return sqrt(Euc()); }
	inline ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
inline bool cmpi(const Pos& p, const Pos& q) {
	return zero(p.x - q.x) ? zero(p.y - q.y) ? p.i > q.i : p.y < q.y : p.x < q.x;
}
typedef std::vector<Pos> Polygon;
Polygon H;
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ld ret = cross(d1, d2, d3, d4);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
inline ld area(std::vector<Pos>& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret;
}
inline bool norm(std::vector<Pos>& H) {
	if (area(H) < 0) {
		std::reverse(H.begin(), H.end());
		return 0;
	}
	return 1;
}
void query() {
	H.resize(N);
	for (Pos& p : H) std::cin >> p;
	norm(H);
	Pos s, e;
	while (M--) {
		std::vector<Pos> tmp;
		std::cin >> s >> e;
		if (e < s) std::swap(s, e);
		for (int i = 0; i < N; i++) {
			Pos& pre = H[(i - 1 + N) % N], cur = H[i], nxt = H[(i + 1) % N];
			if (ccw(s, e, cur) * ccw(s, e, nxt) < 0) {
				Pos inx = intersection(s, e, cur, nxt);
				if (ccw(s, e, cur, nxt) < 0) inx.i = 0;
				else inx.i = 1;
				tmp.push_back(inx);
			}
			else if (!ccw(s, e, cur)) {
				Pos inx = cur;
				if (ccw(s, e, pre) * ccw(s, e, nxt) > 0) continue;
				if (ccw(s, e, pre) > 0 || ccw(s, e, nxt) < 0) inx.i = 0;
				else if (ccw(s, e, pre) < 0 || ccw(s, e, nxt) > 0) inx.i = 1;
				tmp.push_back(inx);
				if (inx.i &&
					!ccw(s, e, pre) &&
					ccw(s, e, nxt) > 0 &&
					ccw(pre, cur, nxt) < 0) inx.i = 0, tmp.push_back(inx);
				if (inx.i &&
					!ccw(s, e, nxt) &&
					ccw(s, e, pre) < 0 &&
					ccw(pre, cur, nxt) < 0) inx.i = 0, tmp.push_back(inx);
			}
		}
		std::sort(tmp.begin(), tmp.end(), cmpi);
		int sz = tmp.size();
		ld ret = 0;
		//for (Pos& p : tmp) std::cout << p << " " << p.i << "\n";
		for (int i = 0, j = 0; i < sz; i = j) {
			while (j < sz && tmp[i].i == tmp[j].i) j++;
			while (j < sz && tmp[i].i != tmp[j].i) j++;
			ret += (tmp[i] - tmp[j - 1]).mag();
		}
		std::cout << ret << "\n";
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	while (1) {
		std::cin >> N >> M;
		if (!N && !M) break;
		query();
	}
	return;
}
int main() { solve(); return 0; }//boj4293


/*

16 2
0 0
7 0
7 2
6 2
6 3
5 3
5 2
4 2
4 3
3 3
3 2
2 2
2 3
1 3
1 2
0 2
0 2 7 2
2 0 2 1
0 0


*/
