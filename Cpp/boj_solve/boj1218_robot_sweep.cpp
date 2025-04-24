#include <iostream>
#include <algorithm>
typedef int ll;
const int RIGHT = 1;
const int LEFT = 2;
const int UP = 4;
const int DOWN = 8;
const int INF = 1e9 + 7;
const int LEN = 2e4;
int N;

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
	//ld mag() const { return hypot(x, y); }
} robots[5], pos[LEN];
int dir[LEN];
int quad_check(const Pos& O, const Pos& t) {
	if (O.x == t.x && O.y == t.y) return 0;
	else if (O.x == t.x) {
		if (O.y < t.y) return 12;
		if (O.y > t.y) return 34;
	}
	else if (O.x < t.x) {
		if (O.y == t.y) return 14;
		if (O.y < t.y) return 1;
		if (O.y > t.y) return 4;
	}
	else if (O.x > t.x) {
		if (O.y == t.y) return 23;
		if (O.y < t.y) return 2;
		if (O.y > t.y) return 3;
	}
}
bool check(const Pos& p) {
	int l = -1, r = -1, u = -1, d = -1;
	for (int i = 0; i < N; ++i) {
		Pos cur = pos[i], nxt = pos[(i + 1) % N];
		if (dir[i] == LEFT) {
			if (quad_check(cur, p) == 2) {
				if (nxt.x < p.x || nxt.x == p.x && dir[(i + 1) % N] == DOWN)
					return 0;
			}
			if (cur.y >= p.y && cur.x >= p.x && nxt.x < p.x) {
				if (!~u || cur.y > pos[u].y) u = i;
			}
		}
		if (dir[i] == RIGHT) {
			if (quad_check(cur, p) == 4) {
				if (nxt.x > p.x || nxt.x == p.x && dir[(i + 1) % N] == UP)
					return 0;
			}
			if (cur.y <= p.y && cur.x <= p.x && nxt.x > p.x) {
				if (!~d || cur.y < pos[d].y) d = i;
			}
		}
		if (dir[i] == UP) {
			if (quad_check(cur, p) == 1) {
				if (nxt.y > p.y || nxt.y == p.y && dir[(i + 1) % N] == LEFT)
					return 0;
			}
			if (cur.x >= p.x && cur.y <= p.y && nxt.y > p.y) {
				if (!~r || cur.x > pos[r].x) r = i;
			}
		}
		if (dir[i] == DOWN) {
			if (quad_check(cur, p) == 3) {
				if (nxt.y < p.y || nxt.y == p.y && dir[(i + 1) % N] == RIGHT)
					return 0;
			}
			if (cur.x <= p.x && cur.y >= p.y && nxt.y < p.y) {
				if (!~l || cur.x < pos[l].x) l = i;
			}
		}
	}
	for (int i = d, bound = -1, pivotY = pos[i].y; i != r; i = (i + 1) % N) {
		if (dir[i] == DOWN) bound = pos[i].x;
		if (dir[i] == UP) pivotY = std::max(pivotY, pos[(i + 1) % N].y);
		if (dir[i] == RIGHT) {
			if (pos[i].y < pivotY)
				bound = pos[(i + 1) % N].x;
		}
		if (dir[i] == LEFT) {
			if ((i + 1) % N == r) continue;
			if (pos[(i + 1) % N].x < bound)
				return 0;
		}
	}
	for (int i = r, bound = -1, pivotX = pos[i].x; i != u; i = (i + 1) % N) {
		if (dir[i] == RIGHT) bound = pos[i].y;
		if (dir[i] == LEFT) pivotX = std::min(pivotX, pos[(i + 1) % N].x);
		if (dir[i] == UP) {
			if (pos[i].x > pivotX)
				bound = pos[(i + 1) % N].y;
		}
		if (dir[i] == DOWN) {
			if ((i + 1) % N == u) continue;
			if (pos[(i + 1) % N].y < bound)
				return 0;
		}
	}
	for (int i = u, bound = INF, pivotY = pos[i].y; i != l; i = (i + 1) % N) {
		if (dir[i] == UP) bound = pos[i].x;
		if (dir[i] == DOWN) pivotY = std::min(pivotY, pos[(i + 1) % N].y);
		if (dir[i] == LEFT) {
			if (pos[i].y > pivotY)
				bound = pos[(i + 1) % N].x;
		}
		if (dir[i] == RIGHT) {
			if ((i + 1) % N == l) continue;
			if (pos[(i + 1) % N].x > bound)
				return 0;
		}
	}
	for (int i = l, bound = INF, pivotX = pos[i].x; i != d; i = (i + 1) % N) {
		if (dir[i] == LEFT) bound = pos[i].y;
		if (dir[i] == RIGHT) pivotX = std::max(pivotX, pos[(i + 1) % N].x);
		if (dir[i] == DOWN) {
			if (pos[i].x < pivotX)
				bound = pos[(i + 1) % N].y;
		}
		if (dir[i] == UP) {
			if ((i + 1) % N == d) continue;
			if (pos[(i + 1) % N].y > bound)
				return 0;
		}
	}
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; ++i) std::cin >> pos[i].x >> pos[i].y;
	for (int i = 0; i < N; ++i) {
		Pos cur = pos[i], nxt = pos[(i + 1) % N];
		if (cur.x > nxt.x) dir[i] = LEFT;
		if (cur.x < nxt.x) dir[i] = RIGHT;
		if (cur.y > nxt.y) dir[i] = DOWN;
		if (cur.y < nxt.y) dir[i] = UP;
	}
	for (int i = 0; i < 5; ++i) {
		std::cin >> robots[i].x >> robots[i].y;
		if (check(robots[i])) std::cout << "YES\n";
		else std::cout << "NO\n";
	}
	return;
}
int main() { solve(); return 0; }//boj1218