#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef double ld;
typedef std::vector<ld> vld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int N, M, K, T, Q;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

//freopen("../../../input_data/triathlon_tests/triath.20", "r", stdin);
//freopen("../../../input_data/triathlon_tests/triathlon_out.txt", "w", stdout);
struct Pos {
	int x, y;
	int hi, i;
	Pos(int X = 0, int Y = 0, int HI = 0, int I = 0) : x(X), y(Y), hi(HI), i(I) {}
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
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
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
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
//bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ll ret = cross(d1, d2, d3, d4);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0;
}
int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
}
void get_round_memo(Polygon& H, vld& memo) {
	int sz = H.size();
	memo.resize(sz + 1, 0);
	memo[0] = .0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		memo[i + 1] = (cur - nxt).mag() + memo[i];//memo[sz] == convex hull's round
	}
	return;
}
std::vector<Pos> upper_monotone_chain(std::vector<Pos>& C) {
	Polygon H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) { for (const Pos& p : C) H.push_back(p); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H.back(), C[i]) >= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	return H;
}
struct HullNode {
	Pos l, r, top;
	Polygon hull;
	vld memo;
	HullNode(Pos l = Pos(), Pos r = Pos(), Polygon h = {}) : l(l), r(r) {
		hull = h;
		get_round_memo(hull, memo);
		top = hull[0];
		for (const Pos& p : hull) if (top.y < p.y) top = p;
	}
	HullNode operator + (const HullNode& R) const {
		Polygon C, H;
		for (const Pos& p : hull) C.push_back(p);
		for (const Pos& p : R.hull) C.push_back(p);
		H = upper_monotone_chain(C);
		return HullNode(H[0], H.back(), H);
	}
	ld len(int s, int e) const {
		if (!memo.size() || s == e) return 0;
		return memo[e] - memo[s];
	}
} hull_tree[LEN << 2];//semi-hull index tree (shit)
struct Seg {
	Pos s, e;
	int i;
	Seg(Pos s = Pos(), Pos e = Pos(), int I = 0) : s(s), e(e), i(I) {}
	Seg(int I = 0, const HullNode& HN) : i(I) {
		s = HN.l;
		e = HN.r;
	}
	Pos S() const { return e - s; }
	bool operator < (const Seg& p) const { return s == p.s ? e < p.e : s < p.s; }
	ll operator / (const Seg& p) const { return S() / p.S(); }
	int ccw(const Pos& p) const { return sign((e - s) / (p - e)); }
	friend ll ccw(const Seg& p, const Seg& q) { return p.S() / q.S(); }
};
Seg upper_tangent_bi_search(const int& I, const int& J) {
	Polygon& L = hull_tree[I].hull, R = hull_tree[J].hull;
	if (!L.size() || !R.size()) return Seg(Pos(-1, -1), Pos(-1, -1), -1);
	if (L.size() == 1 && R.size() == 1) return Seg(L[0], R[1], I);
	//auto on_hull = [&](const Pos& p, const Polygon& h) -> bool {
	//	return h[0].x <= p.x && p.x <= h.back().x;
	//	};
	int szl = L.size(), szr = R.size();
	if (L.size() == 1) {//corner case
		//divide hull
		Pos& p = L[0];
		int s = 0, e = szr - 1, k, m;
		while (s + 1 < e) {
			k = s + e >> 1;
			int CCW = ccw(R[0], R[k], p);
			if (CCW > 0) s = k;
			else e = k;
		}
		//r-search
		int s1 = 0, e1 = s;
		while (s1 < e1) {
			m = s1 + e1 >> 1;
			Pos p1 = p, cur = R[m], nxt = R[(m + 1) % szr];
			if (ccw(p1, cur, nxt) > 0) s1 = m + 1;
			else e1 = m;
		}
		int i1 = s1;
		if (!ccw(p, R[i1], R[(i1 + 1) % szr]) && dot(p, R[(i1 + 1) % szr], R[i1]) > 0) i1 = (i1 + 1) % szr;

		return Seg(O, O, I);
	}
	else if (R.size() == 1) {//corner case
		//divide hull

		//l-search

		return Seg(O, O, I);
	}
	int l = szl - 1, r = 0;
	auto tangent_check = [&](const int& i, const int& j) -> bool {
		bool f1 = 0, f2 = 0, f3 = 0, f4 = 0;
		if (i > 0) f1 = ccw(L[i], R[j], L[i - 1]) <= 0;
		else f1 = 1;
		if (i < szl - 1) f2 = ccw(L[i], R[j], L[i + 1]) <= 0;
		else f2 = 1;
		if (j > 0) f3 = ccw(L[i], R[j], R[i - 1]) <= 0;
		else f3 = 1;
		if (j < szr - 1) f4 = ccw(L[i], R[j], R[i + 1]) <= 0;
		else f4 = 1;
		return f1 && f2 && f3 && f4;
		};
	bool f = 0;
	while (!tangent_check(l, r)) {
		int s, e, m;
		if (!f) {//r search
			s = r, e = szr - 1;
			while (s < e) {
				m = s + e >> 1;
				if (L[l] == R[m]) { e = m; continue; }
				if (ccw(L[l], R[m], R[m + 1]) > 0) s = m + 1;
				else e = m;
			}
			if (s < szr - 1 && !ccw(L[l], R[s], R[s + 1])) s++;
			r = s;
		}
		else if (f) {//l search
			s = 0, e = l;
			while (s < e) {
				m = s + e >> 1;
				if (L[m] == R[r]) { s = m + 1; continue; }
				if (ccw(L[m], R[r], L[m + 1]) > 0) s = m + 1;
				else e = m;
			}
			if (s > 0 && !ccw(L[l], R[s], L[s - 1])) s--;
			l = s;
		}
		f ^= 1;
	}
	//if (l > 0 && !ccw(L[l], R[r], L[l - 1])) l--;
	//if (r < szr - 1 && !ccw(L[l], R[r], R[r + 1])) r++;
	return Seg(L[l], R[r], I);
}
typedef std::vector<Seg> Bridge;
bool search(const Pos& L, const Pos& R, Bridge& BBB , int s = 1, int e = K, int i = 1) {
	if (s == e) {
		bool f = 0;
		if (hull_tree[i].l.x <= L.x && L.x <= hull_tree[i].r.x)
			BBB.push_back(Seg(i, hull_tree[i]));
		else if (hull_tree[i].l.x <= R.x && R.x <= hull_tree[i].r.x) {
			BBB.push_back(Seg(i, hull_tree[i]));
			if (R.x <= hull_tree[i].top.x) f = 1;
		}
		return f;
	}
	if (hull_tree[i].r.x <= L.x || R.x <= hull_tree[i].l.x) return 0;
	int m = s + e >> 1;
	return search(L, R, BBB, s, m, i << 1) || search(L, R, BBB, m + 1, e, i << 1 | 1);
}
ld upper_monotone_chain(Pos L, Pos R) {
	ld ret = 0;
	if (R < L) std::swap(L, R);
	Bridge BBB, stack, H;
	bool f = search(L, R, BBB);
	std::sort(BBB.begin(), BBB.end());
	if (f) BBB.pop_back();
	hull_tree[0].hull = { L };
	stack.push_back(Seg(L, L, 0));
	hull_tree[N << 2 || 1].hull = { R };
	BBB.push_back(Seg(R, R, N << 2 | 1));
	int sz = BBB.size();
	for (int i = 0; i < sz; i++) {
		Seg B = BBB[i];
		while (stack.size() > 0 &&
			upper_tangent_bi_search(stack[stack.size() - 2].i, stack.back().i).S() /
			upper_tangent_bi_search(stack.back().i, B.i).S() >= 0)
			stack.pop_back();
		stack.push_back(B);
	}
	sz = stack.size();
	for (int i = 0; i < sz - 1; i++)
		H.push_back(upper_tangent_bi_search(stack[i].i, stack[i + 1].i));
	for (int i = 0; i < sz - 1; i++) {
		ret += (H[i].e - H[i].s).mag();
		if (i > 0) ret += hull_tree[H[i].i].len(H[i - 1].e.i, H[i].s.i);
	}
	return ret;
}
void query(Pos& pre) {
	Pos cur;
	std::cin >> cur;
	std::cout << upper_monotone_chain(pre, cur);
	pre = cur;
	return;
}
void query() {
	std::cin >> Q, Q--;
	Pos pre;
	std::cin >> pre;
	while (Q--) query(pre);
	return;
}
Polygon MT[LEN];
HullNode init(int s = 1, int e = K, int i = 1) {

}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N;
	Polygon C;
	for (Pos& p : C) std::cin >> p;

	//separating convex hulls
	K = 0;
	for (int i = 0; i < N; i++) {
		Polygon tmp;
		if (tmp.size() > 1 && ccw(tmp[tmp.size() - 2], tmp.back(), C[i]) < 0) {
			int j = 0;
			for (Pos& p : tmp) p.i = j++, p.hi = K, MT[K].push_back(p);
			K++;
			//Pos pre = tmp.back();
			Polygon().swap(tmp);
			//if (pre.y) tmp = { pre };
		}
		tmp.push_back(C[i]);
	}

	//tree init
	init();

	query();

	return;
}
int main() { solve(); return 0; }