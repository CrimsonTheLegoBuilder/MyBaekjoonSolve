#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
const int LEN = 2'005;
int N;

struct Pos {
	ll x, y, a, b;
	Pos(ll x = 0, ll y = 0, ll a = 0, ll b = 0) : x(x), y(y), a(a), b(b) {}
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y >> p.a >> p.b; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y << " " << p.a << " " << p.b; return os; }
} M[LEN];
std::vector<Pos> MY[LEN], MX[LEN];

struct Coord {
	int c, i;
	bool operator < (const Coord& x) const { return c < x.c; }
} X[LEN], Y[LEN], A[LEN], B[LEN];

struct Seg {
	ll max, lsum, rsum, sum;
	Seg operator + (const Seg& r) {
		return {
			std::max({max, r.max, rsum + r.lsum}),
			std::max({lsum, sum + r.lsum}),
			std::max({rsum + r.sum, r.rsum}),
			sum + r.sum
		};
	}
} TA[LEN << 2], TB[LEN << 2];

void update(Seg T[], ll x, ll v, int s = 0, int e = N - 1, int n = 1) {
	if (s > x || x > e) return;
	if (s == e) {
		ll cur = T[n].max;
		T[n] = { cur + v, cur + v, cur + v, cur + v };
		return;
	}
	int m = s + e >> 1;
	update(T, x, v, s, m, n << 1);
	update(T, x, v, m + 1, e, n << 1 | 1);
	T[n] = T[n << 1] + T[n << 1 | 1];
}
void init(Seg T[]) {
	for (int i = 0; i < LEN << 2; i++) {
		T[i] = { 0, 0, 0, 0 };
	}
}



int main() {
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> X[i].c >> Y[i].c >> M[i].a >> M[i].b;
		X[i].i = Y[i].i = i;
	}

	std::sort(X, X + N);
	int x = -1;
	for (int i = 0, j = -1; i < N; i++) {
		if (X[i].c != x) {
			x = X[i].c, j++;
		}
		M[X[i].i].x = j;
	}
	std::sort(Y, Y + N);
	int y = -1;
	for (int i = 0, j = -1; i < N; i++) {
		if (Y[i].c != y) {
			y = Y[i].c, j++;
		}
		M[Y[i].i].y = j;
	}

	ll Y = 0, X = 0;//y - max value
	for (int i = 0; i < N; i++) {
		MY[M[i].y].push_back(M[i]);
		MX[M[i].x].push_back(M[i]);
		Y = std::max(Y, M[i].y);
		X = std::max(X, M[i].x);
	}

	ll ret = 0;
	ll F = 0;
	for (int i = 0; i <= Y; i++) {
		init(TA);
		F = 0;
		for (int j = i; j <= Y; j++) {
			for (const Pos& g : MY[j]) {
				update(TA, g.x, g.a);
			}
			F = std::max(F, TA[1].max);
			for (int k = j + 1; k <= Y; k++) {
				init(TB);
				for (int l = k; l <= Y; l++) {
					for (const Pos& g : MY[l]) {
						update(TB, g.x, g.b);
					}
				}
				ret = std::max(ret, F + TB[1].max);
			}
		}
	}

	for (int i = 0; i <= Y; i++) {
		init(TB);
		F = 0;
		for (int j = i; j <= Y; j++) {
			for (const Pos& g : MY[j]) {
				update(TB, g.x, g.b);
			}
			F = std::max(F, TB[1].max);
			for (int k = j + 1; k <= Y; k++) {
				init(TA);
				for (int l = k; l <= Y; l++) {
					for (const Pos& g : MY[l]) {
						update(TA, g.x, g.a);
					}
				}
				ret = std::max(ret, F + TA[1].max);
			}
		}
	}

	for (int i = 0; i <= X; i++) {
		init(TA);
		F = 0;
		for (int j = i; j <= X; j++) {
			for (const Pos& g : MX[j]) {
				update(TA, g.y, g.a);
			}
			F = std::max(F, TA[1].max);
			for (int k = j + 1; k <= X; k++) {
				init(TB);
				for (int l = k; l <= X; l++) {
					for (const Pos& g : MX[l]) {
						update(TB, g.y, g.b);
					}
				}
				ret = std::max(ret, F + TB[1].max);
			}
		}
	}

	for (int i = 0; i <= X; i++) {
		init(TB);
		F = 0;
		for (int j = i; j <= X; j++) {
			for (const Pos& g : MX[j]) {
				update(TB, g.y, g.b);
			}
			F = std::max(F, TB[1].max);
			for (int k = j + 1; k <= X; k++) {
				init(TA);
				for (int l = k; l <= X; l++) {
					for (const Pos& g : MX[l]) {
						update(TA, g.y, g.a);
					}
				}
				ret = std::max(ret, F + TA[1].max);
			}
		}
	}

	std::cout << ret << "\n";
	return 0;
}