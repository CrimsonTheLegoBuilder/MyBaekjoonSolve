#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <cassert>
typedef long long ll;
const ll INF = 1e17;
const int LEN = 1e5 + 10;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

#define RIGHT 1
#define LEFT -1

int N;
bool V[LEN];
struct Pos {
	int x, y, d;
	Pos(int x_ = 0, int y_ = 0, int d_ = 0) : x(x_), y(y_), d(d_) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} P[LEN]; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Polygon S;
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
bool invisible(Pos p1, Pos p2, const Pos& q) {
	if (p1.x > p2.x) std::swap(p1, p2);
	return p1.x <= q.x && q.x <= p2.x && ccw(p1, p2, q) < 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> P[i];
	P[0] = P[1] + Pos(-1);
	P[N + 1] = P[N] + Pos(1);
	S.clear(); S.push_back(P[0]); S.push_back(P[1]);
	bool fvis = 1, bvis = 1, rvs = 0;
	for (int i = 1; i <= N; i++) {
		Pos p0 = P[i - 1], p1 = P[i], p2 = P[i + 1];
		Pos v0 = p1 - p0, v1 = p2 - p1;
		int dir = sign(v1.x);
		int hide = v0 / v1;
		if (fvis && bvis) {
			if (dir < 0) {//move backward
				p2.d = LEFT;
				if (!rvs && hide < 0) {
					rvs = 1;
					fvis = 0;
					continue;
				}
				rvs = 1;
				while (S.size() && invisible(p1, p2, S.back())) S.pop_back();
			}
			else if (!dir) {
				p2.d = LEFT;
			}
		}
		while (S.size()) {

		}
		S.push_back(P[i]);
	}
	return;
}
int main() { solve(); return 0; }//boj33519 Andrew the Diver