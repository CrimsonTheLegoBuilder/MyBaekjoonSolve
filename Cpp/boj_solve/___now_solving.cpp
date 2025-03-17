#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <set>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 2e3 + 5;

ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }


int W, H, N, M, Q;
struct Pos {
	int x, y;
	Pos(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const int& n) { x /= n; y /= n; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
Polygon P[LEN];
struct Box {
	int s, e, h;
	bool operator < (const Box& o) const {
		if (h == o.h) {
			return s == o.s ? e < o.e : s < o.s;
		}
		return h < o.h;
	}
} B[LEN];
std::vector<Box> V, S;
struct Bin {
	int i;
	bool operator < (const Bin& o) const { return B[i] < B[o.i]; }
};
typedef std::set<Bin> Tree;
Tree T;
struct Event {
	int x, d, i;
	Event(int x_ = 0, int d_ = 0, int i_ = 0) : x(x_), d(d_), i(i_) {}
	bool operator < (const Event& o) const { return x == o.x ? d < o.d : x < o.x; }
};
std::vector<Event> E;
Vint X;
void rotate_all() {
	std::swap(W, H);
	for (int i = 0; i < N; i++) {
		for (Pos& p : P[i]) p = ~p;
	}
	return;
}
ll sweep() {
	V.clear();
	S.clear();
	T.clear();
	ll a = 0;
	int sz = X.size();
	int sze = E.size();
	int ei = 0;
	while (ei < sze && E[ei].x == X[0]) T.insert({ E[ei++].i });
	for (int x = 0; x < sz - 1; x++) {
		int s = X[x], e = X[x + 1];

	}
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> W >> H >> N;
	W >>= 1; H >>= 1;
	for (int i = 1; i <= N; i++) {
		Pos a, b;
		std::cin >> a >> b;
		P[i] = { a, b };
	}
	ll ret = 0;
	for (int _ = 0; _ < 4; _++) {
		int yi = -H;
		B[0] = { -W, W, H - yi };
		E.push_back(Event(-W, 0, 0));
		E.push_back(Event(W, 1, 0));
		X = { -W, W };
		for (int j = 1; j <= N; j++) {
			int x1 = std::min(P[j][0].x, P[j][1].x);
			int x2 = std::max(P[j][0].x, P[j][1].x);
			int yl = std::min(P[j][0].y, P[j][1].y);
			int yh = std::max(P[j][0].y, P[j][1].y);
			if (yi >= yh) continue;
			int y = std::max(0, yl);
			B[j] = { x1, x2, y - yi };
			E.push_back(Event(x1, 0, j));
			E.push_back(Event(x2, 1, j));
			X.push_back(x1);
			X.push_back(x2);
		}
		std::sort(E.begin(), E.end());
		std::sort(X.begin(), X.end());
		X.erase(unique(X.begin(), X.end()), X.end());
		ll tmp = sweep();
		ret = std::max(ret, tmp);
		E.clear();
		for (int i = 1; i <= N; i++) {
			yi = std::max(P[i][0].y, P[i][1].y);
			B[0] = { -W, W, H - yi };
			E.push_back(Event(-W, 0, 0));
			E.push_back(Event(W, 1, 0));
			X = { -W, W };
			for (int j = 1; j <= N; j++) {
				if (j == i) continue;
				int x1 = std::min(P[j][0].x, P[j][1].x);
				int x2 = std::max(P[j][0].x, P[j][1].x);
				int yl = std::min(P[j][0].y, P[j][1].y);
				int yh = std::max(P[j][0].y, P[j][1].y);
				if (yi >= yh) continue;
				int y = std::max(0, yl);
				B[j] = { x1, x2, y - yi };
				E.push_back(Event(x1, 0, j));
				E.push_back(Event(x2, 1, j));
				X.push_back(x1);
				X.push_back(x2);
			}
			std::sort(E.begin(), E.end());
			std::sort(X.begin(), X.end());
			X.erase(unique(X.begin(), X.end()), X.end());
			ll tmp = sweep();
			ret = std::max(ret, tmp);
			E.clear();
		}
		rotate_all();
	}
	std::cout << ret << "\n";
}
int main() { solve(); return 0; }