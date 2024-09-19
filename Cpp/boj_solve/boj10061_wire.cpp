#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <map>
#include <set>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ll INF = 1e17;
const int LEN = 105;
const ld TOL = 1e-7;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

#define __FUCK__ ;

int P[LEN * LEN * LEN];//disjoint set
int find(int i) { return P[i] < 0 ? i : P[i] = find(P[i]); }
bool join(int i, int j) {
	i = find(i), j = find(j);
	if (i == j) return 0;
	if (P[i] < P[j]) P[i] += P[j], P[j] = i;
	else P[j] += P[i], P[i] = j;
	return 1;
}
int N, M;
struct Pii {
	int x, y;
	int i;
	Pii(int X = 0, int Y = 0) : x(X), y(Y) { i = -1; }
	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	Pii operator * (const int& n) const { return { x * n, y * n }; }
	Pii operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pii& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pii& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
}; const Pii Oii = Pii(0, 0);
typedef std::vector<Pii> Polygon;
Pii p0, p1;
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
struct Pos {
	ld x, y;
	int pi, si;
	bool rv;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { pi = -1, si = -1, rv = 0; }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
}; const Pos O = Pos(0, 0); const Pos INVAL = Pos(INF, INF);
typedef std::vector<Pos> Polygonf;
bool cmpr(const Pos& p, const Pos& q) {
	bool f1 = O < p;
	bool f2 = O < q;
	if (f1 != f2) return f1;
	int tq = ccw(O, p, q);
	return !tq ? p.rv > q.rv : tq > 0;
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) > 0; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool inner_check(const Polygonf& H, const Pos& p) {//concave
	int sz = H.size(), cnt = 0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
inline Pos conv(const Pii& p) { return Pos((ld)p.x, (ld)p.y); }
inline Pii conv(const Pos& p) { return Pii(p.x + TOL, p.y + TOL); }
struct Segi {
	Pii a, b;
	int i;
	Polygonf INX;//intersections
	Segi(Pii A = Pii(), Pii B = Pii()) : a(A), b(B) { i = -1, INX.clear(); }
	Pos inx(const Segi& o) const {
		Pos x = intersection(conv(a), conv(b), conv(o.a), conv(o.b));
		if (on_seg_strong(conv(a), conv(b), x) || on_seg_strong(conv(o.a), conv(o.b), x)) return x;
		return INVAL;
	}
	void inx_sort() {
		std::sort(INX.begin(), INX.end(), [&](const Pos& p, const Pos& q) -> bool {
			return (conv(a) - p).Euc() < (conv(a) - q).Euc();
			});
		INX.erase(unique(INX.begin(), INX.end()), INX.end());
	}
} segi[LEN];
struct Seg {
	Pos a, b;
	int i, si;
	Polygonf INX;//intersections
	Seg(Pos A = Pos(), Pos B = Pos()) : a(A), b(B) { i = -1, si = -1, INX.clear(); }
	Pos inx(const Seg& o) const { return intersection(a, b, o.a, o.b); }
	void inx_sort() {
		std::sort(INX.begin(), INX.end(), [&](const Pos& p, const Pos& q) -> bool {
			return (a - p).Euc() < (a - q).Euc();
			});
		INX.erase(unique(INX.begin(), INX.end()), INX.end());
	}
} seg[LEN * LEN * LEN], border = Seg(Pos(-1e5 - 1, -1e5 - 1), Pos(-1e5 - 1, +1e5 + 1));
int I;
std::map<Pos, Polygonf> map_pos;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> M >> p0 >> p1;
	for (int i = 0; i < M; i++) {
		std::cin >> segi[i].a >> segi[i].b;
		segi[i].i = i;
	}
	Polygonf INXS;
	for (int i = 0; i < M; i++) {
		for (int j = i + 1; j < M; j++) {
			Pos X = segi[i].inx(segi[j]);
			segi[i].INX.push_back(X);
			segi[j].INX.push_back(X);
			INXS.push_back(X);
		}
	}
	std::sort(INXS.begin(), INXS.end());
	INXS.erase(unique(INXS.begin(), INXS.end()), INXS.end());
	I = 0;
	for (int i = 0; i < M; i++) {
		segi[i].inx_sort();
		Polygonf& V = segi[i].INX;
		int sz = V.size();
		for (int j = 0; j < sz - 1; j++) {
			seg[I] = Seg(V[j], V[j + 1]);
			seg[I].i = I;
			seg[I].si = i;
			I++;
			seg[I] = Seg(V[j + 1], V[j]);
			seg[I].i = I;
			seg[I].si = i;
			I++;
		}
	}
	for (int i = 0; i < I; i++) {
		Pos key = seg[i].a;
		Pos vec = seg[i].b - seg[i].a;
		vec.pi = seg[i].i;
		vec.si = seg[i].si;
		map_pos[key].push_back(vec);

		key = seg[i].b;
		vec = seg[i].a - seg[i].b;
		vec.pi = seg[i].i;
		vec.si = seg[i].si;
		vec.rv = 1;
		map_pos[key].push_back(vec);
	}
	memset(P, -1, sizeof P);
	for (const Pos& key : INXS) {
		Polygonf& V = map_pos[key];
		std::sort(V.begin(), V.end(), cmpr);
		int sz = V.size();
		for (int j = 0; j < sz; j += 2) {
			Pos cur = V[(j - 1) % sz], nxt = V[j];
			join(cur.pi, nxt.pi);
		}
	}
	return;
}
int main() { solve(); return 0; }//boj8883