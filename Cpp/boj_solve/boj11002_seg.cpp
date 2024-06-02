#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <unordered_map>
typedef long long ll;
//typedef double ld;
typedef long double ld;
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

//#define DEBUG
//#define MAP_search

struct Pos {
	int x, y;
	int i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
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
namespace std {
	template<>
	struct hash<Pos> {
		std::size_t operator()(const Pos& p) const {
			std::size_t h1 = std::hash<int>()(p.x);
			std::size_t h2 = std::hash<int>()(p.y);
			return h1 ^ (h2 << 1);
		}
	};
}
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
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
Polygon upper_monotone_chain(Polygon& C) {
	Polygon H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) { for (const Pos& p : C) H.push_back(p); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H.back(), C[i]) >= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
	}
	return H;
}
struct HullNode {
	Pos l, r;
	Polygon hull;
	vld memo;
	HullNode(Polygon h = {}) {
		hull = h;
		int idx = 0;
		for (Pos& p : hull) p.i = idx++;
		get_round_memo(hull, memo);
		if (hull.size()) l = hull[0];
		else l = Pos();
		if (hull.size()) r = hull.back();
		else r = Pos();
	}
	HullNode operator + (const HullNode& R) const {
		Polygon C, H;
		for (const Pos& p : hull) C.push_back(p);
		for (const Pos& p : R.hull) C.push_back(p);
		H = upper_monotone_chain(C);
		return HullNode(H);
	}
	ld len(int s, int e) const {
		if (memo.size() <= 1 || s == e) return 0;
		return memo[e] - memo[s];
	}
} hull_tree[LEN << 2];//semi-hull index tree (shit)
struct Seg {
	Pos s, e;
	int i;
	Seg(Pos s = Pos(), Pos e = Pos(), int I = 0) : s(s), e(e), i(I) {}
	//Seg(int I = 0, const HullNode& HN = HullNode(Polygon())) : i(I) {
	//	s = HN.l;
	//	e = HN.r;
	//}
	Pos S() const { return e - s; }
	bool operator < (const Seg& p) const { return s == p.s ? e < p.e : s < p.s; }
	bool operator = (const Seg& p) const { return s == p.s && e == p.e; }
	ll operator / (const Seg& p) const { return S() / p.S(); }
};
Seg make_seg(int I = 0, const HullNode& HN = HullNode(Polygon())) { return Seg(HN.l, HN.r, I); }
std::unordered_map<Pos, Seg> MAP;
int find_tangent_bi_search(const Polygon& H, const Pos& p, bool l = 1) {//from 18190
	int sz = H.size();
	int i1{ 0 }, i2{ 0 };
	int ccw1 = ccw(p, H[0], H[1]), ccwN = ccw(p, H[0], H[sz - 1]);
	if (on_seg_strong(H[0], H[1], p)) {
		int ans = l ? 1 : 0;
		return ans;
	}
	if (on_seg_strong(H[sz - 2], H[sz - 1], p)) {
		int ans = l ? sz - 1 : sz - 2;
		return ans;
	}
	if (ccw1 * ccwN >= 0) {
		i1 = 0;
		if (!ccw1 && dot(p, H[1], H[0]) > 0) i1 = 1;
		if (!ccwN && dot(p, H[sz - 1], H[0]) > 0) i1 = sz - 1;
		int s = 0, e = sz - 1, m;
		if (!ccw1) s += 1;
		if (!ccwN) e -= 1;
		bool f = ccw(p, H[s], H[s + 1]) >= 0;
		while (s < e) {
			m = s + e >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) > 0) s = m + 1;
			else e = m;
		}
		i2 = s;
		if (!ccw(p, H[i2], H[(i2 + 1) % sz]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	else {
		//divide hull
		int s = 0, e = sz - 1, k, m;
		bool f = ccw1 > 0 && ccwN < 0;//if H[k] is between H[0] && p
		while (s + 1 < e) {
			k = s + e >> 1;
			int CCW = ccw(H[0], H[k], p);
			if (!f) CCW *= -1;//normailze
			if (CCW > 0) s = k;
			else e = k;
		}
		if (on_seg_strong(H[s], H[e], p)) {
			int ans;
			if (l) ans = H[i1].x < H[i2].x ? i2 : i1;
			else ans = H[i1].x > H[i2].x ? i2 : i1;
			return ans;
		}

		//search lower hull
		int s1 = 0, e1 = s;
		while (s1 < e1) {
			m = s1 + e1 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) > 0) s1 = m + 1;
			else e1 = m;
		}
		i1 = s1;
		if (!ccw(p, H[i1], H[(i1 + 1) % sz]) && dot(p, H[(i1 + 1) % sz], H[i1]) > 0) i1 = (i1 + 1) % sz;

		//search upper hull
		int s2 = e, e2 = sz - 1;
		while (s2 < e2) {
			m = s2 + e2 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) < 0) s2 = m + 1;
			else e2 = m;
		}
		i2 = s2;
		if (!ccw(p, H[i2], H[(i2 + 1) % sz]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	int ans;
	if (l) ans = (ccw(p, H[i1], H[i2]) > 0 ? i2 : i1);
	else ans = (ccw(p, H[i1], H[i2]) < 0 ? i2 : i1);
	return ans;
}
Seg upper_tangent_bi_search(const int& I, const int& J) {
#ifdef MAP_search
	if (0 < I && J <= K * 4) {
#ifdef DEBUG
		std::cout << "find upper tangent [" << I << "][" << J << "], MAP search::\n";
#endif
		auto it = MAP.find(Pos(I, J));
		if (it != MAP.end()) return it->second;
	}
#endif
#ifdef DEBUG
	std::cout << "find upper tangent [" << I << "][" << J << "]\n";
#endif
	Polygon& L = hull_tree[I].hull, R = hull_tree[J].hull;
#ifdef DEBUG
	for (const Pos& p : L) std::cout << "L : " << p << "\n";
	for (const Pos& p : R) std::cout << "R : " << p << "\n";
#endif
	if (!L.size() || !R.size()) {
#ifdef DEBUG
		std::cout << "SHIT\n";
#endif
		return Seg(Pos(-1, -1), Pos(-1, -1), -1);
	}
	if (L.size() == 1 && R.size() == 1) {
#ifdef DEBUG
		std::cout << "0 Seg[" << I << "][" << J << "] : " << Seg(L[0], R[0], I).S() << "\n";
#endif
#ifdef MAP_search
		if (0 < I && J <= K * 4) MAP[Pos(I, J)] = Seg(L[0], R[0], I);
#endif
		return Seg(L[0], R[0], I);
	}
	int szl = L.size(), szr = R.size();
	if (L.size() == 1) {
		int i = find_tangent_bi_search(R, L[0]);
#ifdef DEBUG
		std::cout << "0 Seg[" << I << "][" << J << "] : " << Seg(L[0], R[i], I).S() << "\n";
#endif
#ifdef MAP_search
		if (0 < I && J <= K * 4) MAP[Pos(I, J)] = Seg(L[0], R[i], I);
#endif
		return Seg(L[0], R[i], I);
	}
	if (R.size() == 1) {
		int i = find_tangent_bi_search(L, R[0], 0);
#ifdef DEBUG
		std::cout << "0 Seg[" << I << "][" << J << "] : " << Seg(L[i], R[0], I).S() << "\n";
#endif
#ifdef MAP_search
		if (0 < I && J <= K * 4) MAP[Pos(I, J)] = Seg(L[i], R[0], I);
#endif
		return Seg(L[i], R[0], I);
	}
	int l = szl - 1, r = 0;
	auto tangent_check = [&](const int& i, const int& j) -> bool {
		bool f1 = 0, f2 = 0, f3 = 0, f4 = 0;
		if (i > 0) f1 = ccw(L[i], R[j], L[i - 1]) <= 0;
		else f1 = 1;
		if (i < szl - 1) f2 = ccw(L[i], R[j], L[i + 1]) <= 0;
		else f2 = 1;
		if (j > 0) f3 = ccw(L[i], R[j], R[j - 1]) <= 0;
		else f3 = 1;
		if (j < szr - 1) f4 = ccw(L[i], R[j], R[j + 1]) <= 0;
		else f4 = 1;
		return f1 && f2 && f3 && f4;
		};
	auto valid_check = [&](int& i, int& j) -> bool {
		if (i > 0 && j < szr - 1 && tangent_check(i - 1, j + 1)) { i--, j++; return 1; }
		if (i > 0 && tangent_check(i - 1, j)) { i--; return 1; }
		if (j < szr - 1 && tangent_check(i, j + 1)) { j++; return 1; }
		return tangent_check(i, j);
		};
	bool f = 1;
	while (!valid_check(l, r)) {
		if (f) {
			r = find_tangent_bi_search(R, L[l], f);
			if (r < szr - 1 && !ccw(L[l], R[r], R[r + 1])) r++;
		}
		else if (!f) {
			l = find_tangent_bi_search(L, R[r], f);
			if (l > 0 && !ccw(L[l], R[r], L[l - 1])) l--;
		}
		f ^= 1;
	}
#ifdef DEBUG
	std::cout << "4 Seg[" << I << "][" << J << "] : " << Seg(L[l], R[r], I).S() << "\n";
#endif
#ifdef MAP_search
	if (0 < I && J <= K * 4) MAP[Pos(I, J)] = Seg(L[l], R[r], I);
#endif
	return Seg(L[l], R[r], I);
}
typedef std::vector<Seg> Bridge;
bool search(const Pos& L, const Pos& R, Bridge& BBB , int s = 1, int e = K, int i = 1) {
	bool f = 0;
	if (s == e) {
#ifdef DEBUG
		std::cout << "DEBUG:: i, s, e : " << i << " " << s << " " << e << "\n";
#endif
		if (hull_tree[i].l.x <= L.x && L.x <= hull_tree[i].r.x)
			f = 1, BBB.push_back(make_seg(i, hull_tree[i]));
		else if (hull_tree[i].l.x <= R.x && R.x <= hull_tree[i].r.x) 
			f = 1, BBB.push_back(make_seg(i, hull_tree[i]));
		return f;
	}
	if (hull_tree[i].r.x <= L.x || R.x <= hull_tree[i].l.x) return 0;
	if (L.x <= hull_tree[i].l.x && hull_tree[i].r.x <= R.x) {
		f = 1, BBB.push_back(make_seg(i, hull_tree[i]));
		return f;
	}
	int m = s + e >> 1;
	bool ls = search(L, R, BBB, s, m, i << 1);
	bool rs = search(L, R, BBB, m + 1, e, i << 1 | 1);
	return ls || rs;
}
ld upper_monotone_chain(Pos L, Pos R) {
	if (L.x == R.x) { return std::abs(L.y - R.y); }
	ld ret = 0;
	if (R < L) std::swap(L, R);
	Bridge BBB, stack, H;
	bool f = search(L, R, BBB);
	if (!f) { return (L - R).mag(); }
	std::sort(BBB.begin(), BBB.end());
	//BBB.erase(unique(BBB.begin(), BBB.end()), BBB.end());
	hull_tree[0].hull = { L };
	stack.push_back(Seg(L, L, 0));
	hull_tree[N << 2 | 1].hull = { R };
	BBB.push_back(Seg(R, R, N << 2 | 1));

#ifdef DEBUG
	std::cout << "DEBUG:: Pos L : " << L << " Pos R : " << R << "\n";
	std::cout << "SEG BBB :\n";
	for (const Seg& B : BBB) std::cout << B.s << " " << B.e << "\n";
#endif

	int sz = BBB.size();
	for (int i = 0; i < sz; i++) {
		Seg B = BBB[i];
		while (stack.size() > 1 &&
			upper_tangent_bi_search(stack[stack.size() - 2].i, stack.back().i) /
			upper_tangent_bi_search(stack.back().i, B.i) >= 0)
			stack.pop_back();
		stack.push_back(B);
	}
	sz = stack.size();

#ifdef DEBUG
	std::cout << "SEG stack :\n";
	for (const Seg& B : stack) std::cout << B.s << " " << B.e << "\n";
#endif

	for (int i = 0; i < sz - 1; i++)
		H.push_back(upper_tangent_bi_search(stack[i].i, stack[i + 1].i));

#ifdef DEBUG
	std::cout << "SEG H :\n";
	for (const Seg& B : H) std::cout << B.s << " " << B.e << "\n";
#endif

	sz = H.size();
	for (int i = 0; i < sz; i++) {
		ret += (H[i].e - H[i].s).mag();
#ifdef DEBUG
		std::cout << "ln : " << (H[i].e - H[i].s).mag() << "\n";
#endif
		if (i < sz - 1) {
			ld ln = hull_tree[H[i + 1].i].len(H[i].e.i, H[i + 1].s.i);
#ifdef DEBUG
			std::cout << H[i].e.i << " " << H[i + 1].s.i << "\n";
			std::cout << "ln : " << ln << "\n";
#endif
			ret += ln;
		}
	}
	return ret;
}
inline ld query(Pos& pre) {
	Pos cur;
	std::cin >> cur;
	ld ret = upper_monotone_chain(pre, cur);
	pre = cur;
	return ret;
}
inline void query() {
	std::cin >> Q, Q--;
	Pos pre;
	std::cin >> pre;
	ld ret = 0;
	while (Q--) ret += query(pre);
	std::cout << ret << "\n";
	return;
}
Polygon MT[LEN];
HullNode init(int s = 1, int e = K, int i = 1) {
	if (s == e) return hull_tree[i] = HullNode(upper_monotone_chain(MT[s]));
	int m = s + e >> 1;
	return hull_tree[i] = init(s, m, i << 1) + init(m + 1, e, i << 1 | 1);
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N;
	Polygon C(N);
	for (Pos& p : C) std::cin >> p;

	//separating convex hulls
	K = 1;
	Polygon tmp;
	for (int i = 0; i < N; i++) {
		if (tmp.size() > 1 && ccw(tmp[tmp.size() - 2], tmp.back(), C[i]) > 0) {
			for (Pos& p : tmp) MT[K].push_back(p);
			K++;
			tmp.clear();
		}
		tmp.push_back(C[i]);

#ifdef DEBUG
		for (Pos& p : tmp) std::cout << p << " | ";
		std::cout << "\n";
#endif

	}
	for (Pos& p : tmp) MT[K].push_back(p);
	tmp.clear();

	//tree init
	init();
	
#ifdef DEBUG
	std::cout << "\nK : " << K << "\n\n";
	for (int i = 1; i <= K; i++) {
		int j = 0;
		for (const Pos& p : MT[i]) std::cout << "MT[" << i << "][" << j++ << "] : " << p << "\n";
		std::cout << "\n";
	}

	for (int i = 0; i <= K * 4; i++) {
		int j = 0;
		for (const Pos& p : hull_tree[i].hull) std::cout << "hull[" << i << "][" << j++ << "] : " << p << "\n";
		j = 0;
		for (const ld& ln : hull_tree[i].memo) std::cout << "memo[" << i << "][" << j++ << "] : " << ln << "\n";
		std::cout << "\n";
	}
#endif

	//query
	query();

	return;
}
int main() { solve(); return 0; }//boj11002 Crow

/*

8
0 0
1 2
2 3
3 2
4 0
6 0
8 4
10 0
3
0 1
5 0
9 2
14.307135789365

12
0 0
1 2
2 2
3 0
4 0
5 3
6 3
7 0
8 0
9 2
10 2
11 0
2
-1 0
12 0
14.9030655007

12
0 0
1 2
2 2
3 0
4 0
5 3
6 3
7 0
8 0
9 3
10 2
11 0
2
5 3
10 3
5.000000

12
0 0
1 2
2 2
3 0
4 0
5 3
6 3
7 0
8 0
9 3
10 2
11 0
2
3 3
4 3
1.000000

4
0 10
3 0
6 0
9 10
2
1 9
8 9
7.000000

12
0 5
1 4
2 3
3 2
4 1
5 0
6 0
7 1
8 2
9 3
10 4
11 5
10
3 4
8 4
3 4
8 4
3 4
8 4
3 4
8 4
3 4
8 4
45.0000000000

12
0 0
1 100
2 99
3 90
4 50
5 0
6 0
7 50
8 90
9 99
10 100
11 0
2
2 100
9 100



*/
