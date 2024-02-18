#include <iostream>
#include <algorithm>
//#include <vector>
//#include <deque>
//#include <queue>
#include <cmath>
//typedef long long ll;
typedef long double ld;
const ld TOL = 1e-7;
//const ld INF = 1e17;
const int LEN = 1e4;
int N, t{ 0 };

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	int i;
	Pos(ld X, ld Y, int I) : x(X), y(Y), i(I) {}
	Pos() : x(0), y(0), i(0) {}
	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, 0 }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, 0 }; }
	Pos operator * (const ld& n) const { return { x * n, y * n, 0 }; }
	Pos operator / (const ld& n) const { return { x / n, y / n, 0 }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x, 0 }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	ld mag() const { return hypot(x, y); }
} shield[LEN], fr[3];
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld dot_ = dot(d1, d3, d2);
	return z(cross(d1, d2, d3)) && (dot_ > 0 || z(dot_));
}
bool inner_check(Pos H[], const int& sz, const Pos& p) {
	int cnt = 0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (z(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> shield[i].x >> shield[i].y;
	for (int i = 0; i < 3; i++) std::cin >> fr[i].x >> fr[i].y;
	for (int i = 0; i < 3; i++) std::cout << inner_check(shield, N, fr[i]) << "\n";
	return;
}
int main() { solve(); return 0; }//boj1688
