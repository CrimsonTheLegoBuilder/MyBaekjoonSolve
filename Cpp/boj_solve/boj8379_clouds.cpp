#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
typedef std::vector<bool> Vbool;
const int LEN = 2005;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }

#define STRONG 0
#define WEAK 1
#define LO x
#define HI y

int N, M, T;
struct Pos {
	int x, y, ni, i;
	bool f;
	Pos(int x_ = 0, int y_ = 0, int ni_ = 0, int i_ = -1) : x(x_), y(y_), ni(ni_), i(i_) { f = 0; }
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
	//bool operator < (const Pos& p) const { return *this / p > 0; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const int& n) { x /= n; y /= n; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
Polygon P[LEN];
bool cmp(const Pos& p, const Pos& q) { return p / q > 0; }
Pos norm(Pos p) {
	ll g = gcd(std::abs(p.x), std::abs(p.y));
	return p / (int)g;
}
bool same_dir(const Pos& p, const Pos& q) { return !(p / q) && (p * q >= 0); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, bool f = 0) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	if (f) return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
ll area(const Polygon& H) {
	ll a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return sz;
}
int inner_check(const std::vector<Pos>& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y <= p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
int count(Pos s, Pos e, const Pos& q) {
	if (on_seg_strong(s, e, q)) return 2;
	if (s.y == e.y) return 0;
	if (s.y > e.y) std::swap(s, e);
	if (s.y >= q.y || q.y > e.y) return 0;
	return ccw(s, e, q) > 0;
}
bool tri_inner_check(const Pos& p0, Pos p1, Pos p2, const Pos& q) {
	if (ccw(p0, p1, p2) < 0) std::swap(p1, p2);
	return ccw(p0, p1, q) >= 0 && ccw(p1, p2, q) >= 0 && ccw(p2, p0, q) >= 0;
}
int front(const Pos& s, const Pos& v, Pos p, Pos q) {
	int sp = ccw(s, v, p), sq = ccw(s, v, q);
	if (sp * sq == 1) return -1;
	if (!sp && !sq) return dot(v, s, p) <= 0 && dot(v, s, q) <= 0;
	if (!sp) return dot(v, s, p) <= 0;
	if (!sq) return dot(v, s, q) <= 0;
	return tri_inner_check(v, p, q, s) ? -1 : 2;
}
int count_(const Pos& s, const Pos& v, const Polygon& H) {
	//int si = 0, ei = 0;
	int cnt = 0;
	//int s_in = 0, e_in = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		const Pos& p0 = H[i], & p1 = H[(i + 1) % sz], & p2 = H[(i + 2) % sz], & p3 = H[(i + 3) % sz];
		//int f;
		//if (p1 == s) continue;
		//if (p0 == s) {
		//	if (!on_seg_strong(v, p1, s)) cnt += ccw(p0, p1, p2) > 0;
		//	continue;
		//}
		//if (p2 == s) {
		//	if (!on_seg_strong(v, p1, s)) cnt += ccw(p0, p1, p2) < 0;
		//	continue;
		//}
		int tq0 = ccw(s, v, p0);
		int tq1 = ccw(s, v, p1);
		int tq2 = ccw(s, v, p2);
		int tq3 = ccw(s, v, p3);
		if (tq1 * tq2 > 0) continue;
		if (tq1 * tq2 < 0) { cnt++; continue; }
		if (!tq1 && !tq0) continue;
		else if (!tq1 && !tq2) {
			if (tq0 * tq3 < 0) cnt++;
			//else cnt += (ccw(p0, p1, p2) > 0) * 2;
			else cnt += 2;
		}
		else if (!tq1) {
			if (tq0 * tq2 < 0) cnt++;
			//else cnt += (ccw(p0, p1, p2) > 0) * 2;
			else cnt += 2;
		}
	}
	return cnt;
}
int count(const Pos& o, const Polygon& H, const int& i) {
	int sz = H.size();
	const Pos& p0 = H[(i - 1 + sz) % sz], & p1 = H[i], & p2 = H[(i + 1) % sz], & p3 = H[(i + 2) % sz];
	if (p0 == o) return ccw(p0, p1, p2) > 0 ? 1 : -1;
	if (p2 == o) return ccw(p0, p1, p2) < 0 ? 1 : -1;
	int tq0 = ccw(o, p1, p0);
	int tq2 = ccw(o, p1, p2);
	int tq3 = ccw(o, p1, p3);
	if (!tq0) return 0;
	if (tq0 * tq2 < 0) return 0;
	if (tq0 > 0 && tq2 > 0) return 2;
	if (tq0 < 0 && tq2 < 0) return -2;
	assert(tq2 == 0);
	if (tq0 * tq3 < 0) return 0;
	if (tq0 > 0 && tq3 > 0) return 2;
	if (tq0 < 0 && tq3 < 0) return -2;
	return 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("../../../input_data/clouds/t2000.txt", "r", stdin);
	//freopen("../../../input_data/clouds/t2000.txt", "w", stdout);
	//std::cout << "1\n2000\n";
	//Pos a1 = Pos(-1e9, 0), a2 = Pos(0, -1e9), a3 = Pos(1e9, 0);
	//std::cout << a1 << "\n" << a2 << "\n" << a3 << "\n";
	//int x = 998, y = -1;
	//Pos a0 = Pos(x, y);
	//for (int i = 0; i < 1997; i++) {
	//	std::cout << a0 << "\n";
	//	if (i & 1) a0 += Pos(-1, -1);
	//	else a0 += Pos(-1, 1);
	//}
	//return;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		int ni;
		std::cin >> ni; P[i].resize(ni);
		for (Pos& p : P[i]) std::cin >> p, T++;
		Vbool F(ni, 0);
		for (int j = 0; j < ni; j++) {
			if (!ccw(P[i][(j - 1 + ni) % ni], P[i][j], P[i][(j + 1) % ni])) F[j] = 1;
		}
		Polygon C;
		for (int j = 0; j < ni; j++) if (!F[j]) C.push_back(P[i][j]);
		P[i] = C;
	}
	assert(T < 2005);
	ll ret = 0;
	//for (int i = 0; i < N; i++) {
	//	for (int j = 0; j < N; j++) {
	//		if (j == i) continue;
	//		int sz = P[j].size();
	//		for (int k = 0; k < sz; k++) {
	//			int f = inner_check(P[i], P[j][k]);
	//			assert(f == 0);
	//		}
	//	}
	//	int n = P[i].size();
	//	for (int j = 0; j < n; j++) {
	//		Pos p1 = P[i][j], p2 = P[i][(j + 1) % n];
	//		for (int k = 0; k < N; k++) {
	//			if (k == i) continue;
	//			int sz = P[k].size();
	//			for (int l = 0; l < sz; l++) {
	//				Pos p3 = P[k][l], p4 = P[k][(l + 1) % sz];
	//				bool f = intersect(p1, p2, p3, p4);
	//				assert(!f);
	//			}
	//		}
	//	}
	//}
	//return;
	for (int i = 0; i < N; i++) {
		const Polygon& H = P[i];
		ll a = area(H);
		assert(a > 0);
		int sz = H.size();
		for (int j = 0; j < sz; j++) {
			const Pos& p = H[j];
			//std::cout << "\np:: " << p << "\n";
			Polygon V;
			for (int k = 0; k < N; k++) {
				M = P[k].size();
				for (int l = 0; l < M; l++) {
					if (k == i && l == j) continue;
					Pos v = P[k][l] - p;
					v = norm(v);
					if (v < O) v *= -1, v.f = 1;
					v.ni = k; v.i = l;
					V.push_back(v);
				}
			}
			ll hi = 0, lo = 0, hif = 0, hir = 0, lof = 0, lor = 0, sm = 0;
			std::sort(V.begin(), V.end(), cmp);
			int szv = V.size();
			assert(szv);
			for (int k = 0; k < N; k++) sm += count_(p, p + V[0], P[k]);
			//std::cout << "sm:: 0:: " << sm << "\n";
			for (int k = 0; k < szv - 1; k++) {
				bool f0 = V[k].f;
				ll c = count(p, P[V[k].ni], V[k].i);
				if (c > 0) {
					hi += c;
					if (f0) hir += c;
					else hif += c;
				}
				if (c < 0) {
					lo += c;
					if (f0) lor += c;
					else lof += c;
				}
				if (V[k] / V[k + 1]) break;
			}
			ret = std::max(ret, sm);
			ret = std::max(ret, sm + lo + hi);
			ret = std::max(ret, sm + lo - lor + hif);
			ret = std::max(ret, sm + lo - lof + hir);
			sm -= hi;
			//sm -= lo;
			hi = 0; lo = 0; hif = 0; hir = 0; lof = 0; lor = 0;
			//std::cout << "sm:: 1:: " << sm << "\n";
			for (int k = 0; k < szv; k++) {
				bool f0 = V[k].f;
				ll c = count(p, P[V[k].ni], V[k].i);
				//std::cout << "p[ni][i]:: " << P[V[k].ni][V[k].i] << "\n";
				//std::cout << "v:: " << V[k] << "\n";
				//std::cout << "ni:: " << V[k].ni << " i:: " << V[k].i << " c:: " << c << "\n";
				if (c > 0) {
					hi += c;
					if (f0) hir += c;
					else hif += c;
				}
				if (c < 0) {
					lo += c;
					if (f0) lor += c;
					else lof += c;
				}
				if (k == szv - 1 || (k < szv - 1 && V[k] / V[k + 1])) {
					//std::cout << "v:: " << V[k] << "\n";
					//std::cout << "sm+:: " << sm << "\n";
					ret = std::max(ret, sm);
					ret = std::max(ret, sm + lo + hi);
					ret = std::max(ret, sm + lo - lof + hif);
					ret = std::max(ret, sm + lo - lor + hir);
					sm += lo;
					sm += hi;
					//std::cout << "sm-:: " << sm << "\n";
					hi = 0; lo = 0; hif = 0; hir = 0; lof = 0; lor = 0;
				}
			}
		}
	}
	std::cout << std::min((ret >> 1), 1000ll) << "\n";
	return;
}
int main() { solve(); return 0; }//boj8379 Clouds

/*

5
4 0 0 1 0 1 1 0 1
4 -1 2 0 2 0 3 -1 3
4 0 4 1 4 1 5 0 5
4 -1 6 0 6 0 7 -1 7
4 0 8 1 8 1 9 0 9




*/