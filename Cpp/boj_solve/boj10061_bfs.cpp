#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <map>
#include <set>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
const ll INF = 1e17;
const int LEN = 105;
const ld TOL = 1e-7;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

#define __FUCK__ ;

int N, M;
struct Pos {
	ld x, y;
	int i;
	bool rv;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { i = -1, rv = 0; }
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
} p0, p1, key, vec; const Pos O = Pos(0, 0); const Pos INVAL = Pos(INF, INF);
typedef std::vector<Pos> Polygon;
typedef std::deque<Pos> PosDeque;
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
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	//return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
bool inner_check(const Polygon& H, const Pos& p) {//concave
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
ld area(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
struct Seg {
	Pos a, b;
	int i;
	//Polygon INX;//intersections
	Seg(Pos A = Pos(), Pos B = Pos()) : a(A), b(B) { i = -1; }//INX.clear();
	Pos inx(const Seg& o) const { return intersection(a, b, o.a, o.b); }
	//void inx_sort() {
	//	std::sort(INX.begin(), INX.end(), [&](const Pos& p, const Pos& q) -> bool {
	//		return (a - p).Euc() < (a - q).Euc();
	//		});
	//	INX.erase(unique(INX.begin(), INX.end()), INX.end());
	//}
} seg[LEN], frag[LEN * LEN * 10];
Polygon INX[LEN];
void inx_sort(Polygon& INX, const Pos& a) {
	std::sort(INX.begin(), INX.end(), [&](const Pos& p, const Pos& q) -> bool {
		return (a - p).Euc() < (a - q).Euc();
		});
	INX.erase(unique(INX.begin(), INX.end()), INX.end());
}
int I, I0;
std::map<Pos, Polygon> map_pos;
ld A[LEN * LEN + 10];
Polygon cell[LEN * LEN + 10]; int ci;
std::set<int> cell_i[LEN * LEN + 10];
int P[LEN * LEN + 10];//disjoint set
int find(int i) { return P[i] < 0 ? i : P[i] = find(P[i]); }
bool join(int i, int j) {
	i = find(i), j = find(j);
	if (i == j) return 0;
	if (P[i] < P[j]) P[i] += P[j], P[j] = i;
	else P[j] += P[i], P[i] = j;
	return 1;
}
int V[LEN * LEN * 10];
Vint GS[LEN * LEN * 10];
void dfs(const int& i, int v) {
	V[v] = 1;
	cell[i].push_back(seg[v].a);
	cell_i[i].insert(v);
	for (const int& w : GS[v]) {
		if (V[w]) continue;
		dfs(i, w);
	}
	return;
}
struct Info {
	int i, c;
	Info(int i_ = 0, int c_ = 0) : i(i_), c(c_) {}
};
std::vector<Info> GC[LEN * LEN * 10];
int bfs(int v, int g) {
	std::queue<Info> Q;
	Q.push(Info(v, 0));
	V[v] = 0;
	while (Q.size()) {
		Info p = Q.front(); Q.pop();
		if (p.i == g) return V[g];
		for (const Info& w : GC[p.i]) {
			if (!~V[w.i]) {
				Q.push(w);
				V[w.i] = V[p.i] + w.c;
			}
		}
	}
	return V[g];
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> M >> p0 >> p1;
	if (M <= 2 || p0 == p1) { std::cout << "0\n"; return; }
	for (int i = 0; i < M; i++) {
		std::cin >> seg[i].a >> seg[i].b;
		seg[i].i = i;
	}
	bool f0 = 1;
	Polygon INXS;
	for (int i = 0; i < M; i++) {
		if (intersect(p0, p1, seg[i].a, seg[i].b)) f0 = 0;
		for (int j = i + 1; j < M; j++) {
			if (intersect(seg[i].a, seg[i].b, seg[j].a, seg[j].b)) {
				Pos X = seg[i].inx(seg[j]);
				INX[i].push_back(X);
				INX[j].push_back(X);
				INXS.push_back(X);
			}
		}
	}
	if (f0) { std::cout << "0\n"; return; }
	std::sort(INXS.begin(), INXS.end());
	INXS.erase(unique(INXS.begin(), INXS.end()), INXS.end());
	I = 0;
	for (int i = 0; i < M; i++) {
		inx_sort(INX[i], seg[i].a);
		Polygon& v = INX[i];
		int sz = v.size();
		for (int j = 0; j < sz - 1; j++) {
			frag[I] = Seg(v[j], v[j + 1]);
			frag[I].i = I;
			I++;
		}
	}
	I0 = I;
	for (int i = 0; i < M; i++) {
		Polygon& v = INX[i];
		int sz = v.size();
		for (int j = 0; j < sz - 1; j++) {
			frag[I] = Seg(v[j + 1], v[j]);
			frag[I].i = I;
			I++;
		}
	}

	for (int i = 0; i < I; i++) {
		key = frag[i].a;
		vec = frag[i].b - frag[i].a;
		vec.i = frag[i].i;
		map_pos[key].push_back(vec);

		key = frag[i].b;
		vec = frag[i].a - frag[i].b;
		vec.i = frag[i].i;
		vec.rv = 1;
		map_pos[key].push_back(vec);
	}
	for (const Pos& key : INXS) {
		Polygon& v = map_pos[key];
		std::sort(v.begin(), v.end(), cmpr);
		int sz = v.size();
		assert(!(sz & 1));
		for (int j = 0; j < sz; j += 2) {
			Pos cur = v[(j - 1) % sz], nxt = v[j];
			assert(cur.rv != nxt.rv);
			GS[nxt.i].push_back(cur.i);
		}
	}
	memset(V, 0, sizeof V);
	ci = 0;
	for (int i = 0; i < I; i++) {
		if (!V[i]) dfs(ci, i);
		A[ci] = area(cell[ci]);
		if (zero(A[ci])) {
			cell[ci].clear();
			cell_i[ci].clear();
			A[ci] = 0;
			ci--;
		}
		ci++;
	}
	memset(P, -1, sizeof P);
	for (int i = 0; i < ci; i++) {
		std::set<int>& CUR = cell_i[i];
		for (int j = i + 1; j < ci; j++) {
			std::set<int>& NXT = cell_i[j];
			for (const int& idx : CUR) {
				if (NXT.count(idx + I0) || NXT.count(idx - I0)) {
					GC[i].push_back(Info(j, 1));
					GC[j].push_back(Info(i, 1));
					join(i, j);
				}
			}
		}
	}
	int s = I, e = I;
	for (int i = 0; i < ci; i++) {
		int out = -1;
		if (sign(A[i]) > 0) {
			if (inner_check(cell[i], p0)) s = i;
			if (inner_check(cell[i], p1)) e = i;
		}
		else {
			for (int j = 0; j < ci; j++) {
				if (sign(A[j] < 0)) continue;
				if (i == j || find(i) == find(j)) continue;
				if (out >= 0 && find(out) == find(j)) continue;
				if (out >= 0 && (A[out] < A[j])) continue;
				if (inner_check(cell[j], cell[i][0])) {
					if (out < 0 || (A[out] > A[j])) out = j;
				}
			}
		}
		if (!~out) GC[i].push_back(Info(I, 0));
		else GC[i].push_back(Info(out, 0));
	}
	memset(V, -1, sizeof V);
	if (s == e) { std::cout << "0\n"; return; }
	std::cout << bfs(s, e) << "\n";
	return;
}
int main() { solve(); return 0; }//boj10061