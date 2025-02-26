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
	//ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	//ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} P[LEN]; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
bool invisible(Pos p1, Pos p2, const Pos& q, const bool& f = 1) {
	if (p1.x > p2.x) std::swap(p1, p2);
	if (f == RIGHT && q.d == RIGHT) return p1.x < q.x && q.x <= p2.x && ccw(p1, p2, q) <= 0;
	return p1.x <= q.x && q.x <= p2.x && ccw(p1, p2, q) <= 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> P[i];
	P[0] = P[1] + Pos(-1);
	P[N + 1] = P[N] + Pos(1);
	Polygon S;
	S.clear(); S.push_back(P[0]); S.push_back(P[1]);
	if ((P[2] - P[1]).x < 0) S.back().d = LEFT;
	bool fvis = 1, bvis = 1, rvs = 0;
	for (int i = 1; i <= N; i++) {
		Pos p0 = P[i - 1], p1 = P[i], p2 = P[i + 1];
		Pos v0 = p1 - p0, v1 = p2 - p1;
		int dir = sign(v1.x);
		//std::cout << "p1:: " << p1 << " p2:: " << p2 << "\n";
		//std::cout << "rvs:: " << rvs << "\n";
		//std::cout << "fvis:: " << fvis << " bvis:: " << bvis << "\n";
		//for (Pos& p : S) std::cout << "(" << p << "), ";
		//std::cout << "\n";
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
				if (v1.y < 0) continue;
				else if (v1.y > 0) {
					//std::cout << "p1:: " << p1 << " p2:: " << p2 << "\n";
					//std::cout << "rvs:: " << rvs << "\n";
					if (rvs) {
						if ((S.back().x < p1.x) ||
							(S.back().d == RIGHT && S.back().x <= p1.x))
						p1.d = RIGHT, S.push_back(p1), S.push_back(p2);
					}
					else S.pop_back(), S.push_back(p2);
				}
			}
			else if (dir > 0) {//move forward
				if (rvs && tq > 0) {//hide
					S.push_back(p1);
					rvs = 0;
					bvis = 0;
					continue;
					//std::cout << "SUCK:: " << p1 << "\n";
				}
				//std::cout << "FUCK:: " << p1 << "\n";
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
	//for (const Pos& p : S) std::cout << "s:: " << p << " " << p.d << "\n";
	//for (const Pos& p : S) if (p.d) std::cout << "p:: " << p << "\n";
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj33519 Andrew the Diver

/*

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


*/