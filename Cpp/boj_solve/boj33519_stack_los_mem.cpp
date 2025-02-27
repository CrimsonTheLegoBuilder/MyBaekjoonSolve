#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cassert>
typedef long long ll;
typedef std::vector<int> Vint;
const int LEN = 1e5 + 5;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

#define RIGHT 1
#define LEFT -1

int N, D[LEN];
struct Pos {
	int x, y;
	Pos(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
} P[LEN]; const Pos O = { 0, 0 };
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
bool invisible(const Pos& p1, const Pos& p2, const Pos& q, const int& d) {
	if (d == RIGHT) return p1.x < q.x && q.x <= p2.x && ccw(p1, p2, q) <= 0;
	return p1.x <= q.x && q.x <= p2.x && ccw(p1, p2, q) <= 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> P[i].x >> P[i].y;
	P[0] = P[1] + Pos(-1); P[N + 1] = P[N] + Pos(1);
	Vint S = { 0, 1 };
	bool fvis = 1, bvis = 1, rvs = 0;
	for (int i = 1; i <= N; i++) {
		Pos p0 = P[i - 1], p1 = P[i], p2 = P[i + 1];
		Pos v0 = p1 - p0, v1 = p2 - p1;
		int dir = sign(v1.x), tq = sign(v0 / v1);
		if (fvis && bvis) {
			if (dir < 0) {//move backward
				if (!rvs && tq < 0) {//hide
					D[S.back()] = LEFT;
					rvs = 1; fvis = 0;
					continue;
				}
				rvs = 1;
				while (S.size() && invisible(p2, p1, P[S.back()], D[S.back()])) S.pop_back();
			}
			else if (!dir) {//move vertical
				if (v1.y < 0) continue;
				else if (v1.y > 0) {
					if (rvs) {
						if ((P[S.back()].x < p1.x) ||
							(D[S.back()] == RIGHT && P[S.back()].x <= p1.x))
							D[i] = RIGHT, S.push_back(i), S.push_back(i + 1);
					}
					else {
						if (P[S.back()] == p1) S.pop_back();
						if (P[S.back()].x < p2.x) S.push_back(i + 1);
					}
				}
			}
			else if (dir > 0) {//move forward
				if (rvs && tq > 0) {//hide
					S.push_back(i);
					rvs = 0; bvis = 0;
					continue;
				}
				if (S.size() && rvs &&
					(
						(P[S.back()].x < p1.x) ||
						(D[S.back()] == RIGHT && P[S.back()].x <= p1.x)
					)
				) D[i] = RIGHT, S.push_back(i);
				rvs = 0;
				if (S.size() < 1 || P[S.back()].x < p2.x) S.push_back(i + 1);
			}
		}
		else if (!fvis) {
			if (P[S.back()].x <= p2.x) {
				rvs = 0; fvis = 1;
				S.push_back(i + 1);
			}
		}
		else if (!bvis) {
			if (P[S.back()].x > p2.x) {
				S.pop_back();
				rvs = 1; bvis = 1;
				while (S.size() && invisible(p2, p1, P[S.back()], D[S.back()])) S.pop_back();
			}
		}
	}
	int cnt = 0;
	for (const int& i : S) cnt += D[i] != 0;
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj33519 Andrew the Diver
