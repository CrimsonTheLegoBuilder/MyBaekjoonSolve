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

/*

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cassert>
typedef long long ll;
const int LEN = 1e5 + 5;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

#define RIGHT 1
#define LEFT -1

int N;
struct Pos {
	int x, y, d;
	Pos(int x_ = 0, int y_ = 0, int d_ = 0) : x(x_), y(y_), d(d_) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} P[LEN]; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
bool invisible(const Pos& p1, const Pos& p2, const Pos& q, const bool& f = 1) {
	if (f == RIGHT && q.d == RIGHT) return p1.x < q.x && q.x <= p2.x && ccw(p1, p2, q) <= 0;
	return p1.x <= q.x && q.x <= p2.x && ccw(p1, p2, q) <= 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> P[i];
	P[0] = P[1] + Pos(-1); P[N + 1] = P[N] + Pos(1);
	Polygon S = { P[0], P[1] };
	bool fvis = 1, bvis = 1, rvs = 0;
	for (int i = 1; i <= N; i++) {
		Pos p0 = P[i - 1], p1 = P[i], p2 = P[i + 1];
		Pos v0 = p1 - p0, v1 = p2 - p1;
		int dir = sign(v1.x), tq = sign(v0 / v1);
		if (fvis && bvis) {
			if (dir < 0) {//move backward
				if (!rvs && tq < 0) {//hide
					S.back().d = LEFT; rvs = 1; fvis = 0;
					continue;
				}
				rvs = 1;
				while (S.size() && invisible(p2, p1, S.back())) S.pop_back();
			}
			else if (!dir) {//move vertical
				if (v1.y < 0) continue;
				else if (v1.y > 0) {
					if (rvs) {
						if ((S.back().x < p1.x) ||
							(S.back().d == RIGHT && S.back().x <= p1.x))
						p1.d = RIGHT, S.push_back(p1), S.push_back(p2);
					}
					else {
						if (S.back() == p1) S.pop_back();
						if (S.back().x < p2.x) S.push_back(p2);
					}
				}
			}
			else if (dir > 0) {//move forward
				if (rvs && tq > 0) {//hide
					S.push_back(p1); rvs = 0; bvis = 0;
					continue;
				}
				if (S.size() && rvs &&
					((S.back().x < p1.x) ||
					(S.back().d == RIGHT && S.back().x <= p1.x)))
					p1.d = RIGHT, S.push_back(p1);
				rvs = 0;
				if (S.size() < 1 || S.back().x < p2.x) S.push_back(p2);
			}
		}
		else if (!fvis) {
			if (S.back().x <= p2.x) { rvs = 0; fvis = 1; S.push_back(p2); }
		}
		else if (!bvis) {
			if (S.back().x > p2.x) {
				S.pop_back(); rvs = 1; bvis = 1;
				while (S.size() && invisible(p2, p1, S.back())) S.pop_back();
			}
		}
	}
	int cnt = 0;
	for (const Pos& p : S) cnt += p.d != 0;
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj33519 Andrew the Diver

/*

16
0 20
0 0
10 0
20 1
20 10
10 10
10 8
15 8
15 9
17 9
17 7
10 7
9 10
10 11
20 11
20 20

11
4 10
2 9
2 0
5 0
10 1
10 3
6 3
6 2
5 3
10 4
10 10

8
2 8
1 7
2 6
0 5
10 5
8 6
9 7
8 8

6
2 8
2 7
1 6
10 6
9 7
9 8

6
4 8
1 4
3 0
5 0
4 2
8 8

15
4 10
4 9
3 9
4 8
4 7
1 7
4 2
4 1
3 0
10 0
9 1
9 2
5 3
9 3
9 10

19
4 10
4 9
3 9
4 8
4 7
1 7
4 2
4 1
3 0
10 0
9 1
9 2
4 3
9 3
4 4
9 4
5 5
9 5
9 10

22
4 10
3 9
4 8
3 7
4 6
3 5
4 4
3 3
4 2
3 1
4 0
5 0
4 1
5 2
4 3
5 4
4 5
5 6
4 7
5 8
4 9
5 10

22
4 10
3 9
4 8
3 7
4 6
3 5
4 4
3 3
4 2
3 1
4 0
6 0
5 1
6 2
5 3
6 4
5 5
6 6
5 7
6 8
5 9
6 10

6
3 10
5 1
3 0
10 0
4 9
10 10

11
4 10
0 9
0 4
4 4
4 5
3 5
4 6
7 5
0 0
10 0
10 10

17
4 10
0 9
0 4
4 4
4 5
3 5
4 6
7 5
0 0
20 0
7 6
6 7
15 7
20 7
20 8
15 8
15 10

22
4 10
4 9
3 8
3 7
4 6
4 5
3 4
3 3
4 2
4 1
3 0
4 0
5 1
5 2
4 3
4 4
5 5
5 6
4 7
4 8
5 9
5 10

6
4 10
10 8
4 6
2 6
12 8
6 10

6
10 10
2 8
10 6
12 6
4 8
12 10

8
4 10
3 6
4 6
4 8
5 6
4 0
10 0
10 10

14
4 10
3 6
4 6
4 8
5 6
5 5
4 0
10 0
9 5
9 6
10 8
10 6
11 6
10 10

15
4 10
4 9
3 9
4 7
3 7
4 5
3 5
4 3
3 3
4 1
4 0
10 0
4 2
10 2
10 10

23
4 10
3 9
3 0
10 0
4 1
10 1
5 2
10 2
4 3
10 3
5 4
10 4
4 5
10 5
5 6
10 6
4 7
10 7
5 8
10 8
4 9
10 9
10 10

41
4 20
0 19
0 0
20 0
20 1
19 1
19 2
18 2
18 3
19 3
19 4
20 4
20 5
19 5
19 6
18 6
18 7
19 7
19 8
20 8
20 9
19 9
19 10
18 10
18 11
19 11
19 12
20 12
20 13
19 13
19 14
18 14
18 15
19 15
19 16
20 16
20 17
19 17
19 18
18 18
18 20

10
4 10
4 0
5 0
5 1
6 2
5 3
5 4
6 5
5 6
5 10

*/

*/