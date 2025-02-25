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
bool invisible(Pos p1, Pos p2, const Pos& q, const bool& f = 1) {
	if (p1.x > p2.x) std::swap(p1, p2);
	if (f == RIGHT && q.d == RIGHT) {
		return p1.x < q.x && q.x <= p2.x && ccw(p1, p2, q) < 0;
	}
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
	if ((P[2] - P[1]).x < 0) S.back().d = LEFT;
	bool fvis = 1, bvis = 1, rvs = 0;
	for (int i = 1; i <= N; i++) {
		Pos p0 = P[i - 1], p1 = P[i], p2 = P[i + 1];
		Pos v0 = p1 - p0, v1 = p2 - p1;
		int dir = sign(v1.x);
		ll tq = v0 / v1;
		if (fvis && bvis) {
			if (dir < 0) {//move backward
				if (!rvs && tq < 0) {//hide
					S.back().d = LEFT;
					rvs = 1;
					fvis = 0;
					continue;
				}
				rvs = 1;
				while (S.size() && invisible(p1, p2, S.back(), LEFT)) S.pop_back();
			}
			else if (!dir) {//move vertical
				if (v1.y < 0) S.push_back(p2);
				else if (v1.y > 0) {
					if (rvs) p2.d = RIGHT;
					S.push_back(p2);
				}
			}
			else if (dir > 0) {//move forward
				if (rvs && tq > 0) {//hide
					S.push_back(p1);
					rvs = 0;
					bvis = 0;
					continue;
				}
				if (S.size() && rvs &&
					(S.back().x < p1.x) ||
					(S.back().d == RIGHT && S.back().x <= p1.x)) p1.d = RIGHT, S.push_back(p1);
				rvs = 0;
				if (S.size() < 1 || S.back().x < p2.x) S.push_back(p2);
			}
		}
		else if (!fvis) {
			if (S.back().x <= p2.x) {
				rvs = 0;
				fvis = 1;
				p2.d = LEFT;
				S.push_back(p2);
			}
		}
		else if (!bvis) {
			if (S.back().x > p2.x) {
				S.pop_back();
				rvs = 1;
				bvis = 1;
				while (S.size() && invisible(p1, p2, S.back())) S.pop_back();
			}
		}
	}
	int cnt = 0;
	for (const Pos& p : S) cnt += p.d != 0;
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj33519 Andrew the Diver