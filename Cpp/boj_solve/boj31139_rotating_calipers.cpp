#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>
typedef long long ll;
typedef double ld;
const ld TOL = 1e-13;
const double PI = acos(-1);
const int LEN = 2e5 + 10;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

int N, M;
ld R[LEN], ix, T;
struct Pos {
	int x, y;
	Pos(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	return a1;
}
bool inner_check(const Pos& t1, const Pos& t2, const Pos& t3, const Pos& q) {
	return ccw(t1, t2, q) >= 0 && ccw(t2, t3, q) >= 0 && ccw(t3, t1, q) >= 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(13);
	std::cin >> N >> M;
	Polygon A(N); for (Pos& p : A) std::cin >> p;
	Polygon B(M); for (Pos& p : B) std::cin >> p;
	for (int i = 0; i < N; i++) R[i + 1] = R[i] + (A[i] - A[(i + 1) % N]).mag();
	int r = 0, l = 0;
	for (int b0 = 0, b1, a0, a1; b0 < M; b0++) {
		b1 = (b0 + 1) % M;
		ld d = (B[b0] - B[b1]).mag();
		while (!inner_check(B[b1], A[r % N], A[(r + 1) % N], B[b0])) r++;
		while (!inner_check(B[b0], A[l % N], A[(l + 1) % N], B[b1])) l++;
		a0 = (r + 1) % N; a1 = l % N;
		ld D = (a0 <= a1) ? (R[a1] - R[a0]) : (R[N] - (R[a0] - R[a1]));
		ix = intersection(A[(r + 1) % N], A[r % N], B[b1], B[b0]);
		D += (A[r % N] - A[(r + 1) % N]).mag() * ix;
		ix = intersection(A[l % N], A[(l + 1) % N], B[b0], B[b1]);
		D += (A[l % N] - A[(l + 1) % N]).mag() * ix;
		T += d * (D / R[N]);
	}
	std::cout << T << "\n";
	return;
}
int main() { solve(); return 0; }//boj31139