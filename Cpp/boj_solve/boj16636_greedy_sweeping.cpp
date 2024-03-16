#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <cassert>
typedef long long ll;
int N, M;
int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y;
		return os;
	}
}; const Pos O = { 0, 0 };
struct Vec {
	int vy, vx;
	Vec(int Y = 0, int X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return vy == v.vy && vx == v.vx; }
	bool operator < (const Vec& v) const { return vy == v.vy ? vx < v.vx : vy < v.vy; }
	ll operator / (const Vec& v) const { return (ll)vy * v.vx - (ll)vx * v.vy; }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	ll c;
	Line(Vec V = Vec(0, 0), ll C = 0) : s(V), c(C) {}
	bool operator == (const Line& l) const { return s == l.s && c == l.c; }
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ll CCW = s / l.s;
		return !CCW ? c < l.c : CCW > 0;
	}
	friend std::ostream& operator << (std::ostream& os, const Line& l) {
		os << l.s.vy << " " << l.s.vx << " " << l.c;
		return os;
	}
};
Line L(const Pos& s, const Pos& e) {
	int dy, dx; ll c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	int _gcd = gcd(std::abs(dy), std::abs(dx));
	dy /= _gcd; dx /= _gcd;
	c = (ll)dy * s.x + (ll)dx * s.y;
	return Line(Vec(dy, dx), c);
}
struct Seg {
	Line l;
	Pos s, e;
	Seg(Pos S = Pos(0, 0), Pos E = Pos(0, 0)) : s(S), e(E) { l = L(S, E); }
	bool operator == (const Seg& S) const { return l == S.l && s == S.s && e == S.e; }
	bool operator != (const Seg& S) const { return !(*this == S); }
	bool operator < (const Seg& S) const { return (l == S.l) ? (s == S.s) ? e < S.e : s < S.s : l < S.l; }
	friend std::ostream& operator << (std::ostream& os, const Seg& S) {
		os << "DEBUG::Seg l: " << S.l << " | s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n";
		return os;
	}
};
void make_seg(std::vector<Seg>& V, const Pos& x1, const Pos& x2) {
	Pos d1 = x1, d2 = x2;
	assert(d2 != d1);
	if (d2 < d1) std::swap(d1, d2);
	V.push_back(Seg(d1, d2));
	return;
}
std::vector<Seg> G, J, G2, J2;
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	G.clear();
	J.clear();
	G2.clear();
	J2.clear();
	Pos p1, p2, p3;
	std::cin >> N;
	for (int g = 0; g < N; g++) {
		std::cin >> p1 >> p2 >> p3;
		make_seg(G, p1, p2);
		make_seg(G, p2, p3);
		make_seg(G, p3, p1);
	}
	std::cin >> M;
	for (int j = 0; j < M; j++) {
		std::cin >> p1 >> p2 >> p3;
		make_seg(J, p1, p2);
		make_seg(J, p2, p3);
		make_seg(J, p3, p1);
	}
	return;
}
void sweep(std::vector<Seg>& V, std::vector<Seg>& V2) {//remove greedily all overlapping segments
	std::sort(V.begin(), V.end());
	Pos tmp;
	int sz = V.size();
	for (int i = 0, j; i < sz; i = j) {//sweeping
		j = i;
		while (j < sz && V[i].l == V[j].l) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (V[k].e < V[nxt].s) continue;
			else if (V[k].e == V[nxt].s)
				std::swap(V[k].s, V[nxt].s);
			else if (V[nxt].e < V[k].e)
				std::swap(V[k].e, V[nxt].e),
				std::swap(V[k].e, V[nxt].s);
			else if (V[k].e <= V[nxt].e)
				std::swap(V[k].e, V[nxt].s);
		}
		for (int k = i; k < j; k++) if (V[k].s != V[k].e) V2.push_back(V[k]);
	}
	//std::sort(V2.begin(), V2.end());
	return;
}
void solve() {
	init();
	sweep(G, G2);
	sweep(J, J2);
	int sz = G2.size();
	if (sz != J2.size()) { std::cout << "no\n"; return; }
	for (int i = 0; i < sz; i++) if (G2[i] != J2[i]) { std::cout << "no\n"; return; }
	std::cout << "yes\n";
	return;
}
int main() { solve(); return 0; }//boj16636 Triangular Clouds
//typedef double ld;
//const ll INF = 1e17;
//ll gcd(ll a, ll b) {
//	while (b) {
//		ll tmp = a % b;
//		a = b;
//		b = tmp;
//	}
//	return a;
//}


//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <vector>
//#include <cassert>
//typedef long long ll;
//int N, M;
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//
//struct Pos {
//	ll x, y;
//	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	friend std::istream& operator >> (std::istream& is, Pos& p) {
//		is >> p.x >> p.y;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
//		os << p.x << " " << p.y;
//		return os;
//	}
//}; const Pos O = { 0, 0 };
//struct Vec {
//	ll vy, vx;
//	Vec(ll Y = 0, ll X = 0) : vy(Y), vx(X) {}
//	bool operator == (const Vec& v) const { return vy == v.vy && vx == v.vx; }
//	bool operator < (const Vec& v) const { return vy == v.vy ? vx < v.vx : vy < v.vy; }
//	ll operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
//}; const Vec Zero = { 0, 0 };
//struct Line {//ax + by = c
//	Vec s;
//	ll c;
//	Line(Vec V = Vec(0, 0), ll C = 0) : s(V), c(C) {}
//	bool operator == (const Line& l) const { return s == l.s && c == l.c; }
//	bool operator < (const Line& l) const {
//		bool f1 = Zero < s;
//		bool f2 = Zero < l.s;
//		if (f1 != f2) return f1;
//		ll CCW = s / l.s;
//		return !CCW ? c < l.c : CCW > 0;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Line& l) {
//		os << l.s.vy << " " << l.s.vx << " " << l.c;
//		return os;
//	}
//};
//Line L(const Pos& s, const Pos& e) {
//	ll dy, dx, c;
//	dy = e.y - s.y;
//	dx = s.x - e.x;
//	ll _gcd = gcd(std::abs(dy), std::abs(dx));
//	dy /= _gcd; dx /= _gcd;
//	c = dy * s.x + dx * s.y;
//	return Line(Vec(dy, dx), c);
//}
//struct Seg {
//	Line l;
//	Pos s, e;
//	//Seg(Line LN = Line(Vec(0, 0), 0), Pos S = Pos(0, 0), Pos E = Pos(0, 0)) : l(LN), s(S), e(E) {}
//	Seg(Pos S = Pos(0, 0), Pos E = Pos(0, 0)) : s(S), e(E) { l = L(S, E); }
//	bool operator == (const Seg& S) const { return l == S.l && s == S.s && e == S.e; }
//	bool operator != (const Seg& S) const { return !(*this == S); }
//	bool operator < (const Seg& S) const { return (l == S.l) ? (s == S.s) ? e < S.e : s < S.s : l < S.l; }
//	friend std::ostream& operator << (std::ostream& os, const Seg& S) {
//		os << "DEBUG::Seg l: " << S.l << " | s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n";
//		return os;
//	}
//};
//void make_seg(std::vector<Seg>& V, const Pos& x1, const Pos& x2) {
//	Pos d1 = x1, d2 = x2;
//	assert(d2 != d1);
//	if (d2 < d1) std::swap(d1, d2);
//	V.push_back(Seg(d1, d2));
//	return;
//}
//std::vector<Seg> G, J, G2, J2;
//void init() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	G.clear();
//	J.clear();
//	G2.clear();
//	J2.clear();
//	Pos p1, p2, p3;
//	std::cin >> N;
//	for (int g = 0; g < N; g++) {
//		std::cin >> p1 >> p2 >> p3;
//		make_seg(G, p1, p2);
//		make_seg(G, p2, p3);
//		make_seg(G, p3, p1);
//	}
//	std::cin >> M;
//	for (int j = 0; j < M; j++) {
//		std::cin >> p1 >> p2 >> p3;
//		make_seg(J, p1, p2);
//		make_seg(J, p2, p3);
//		make_seg(J, p3, p1);
//	}
//	return;
//}
//void sweep(std::vector<Seg>& V, std::vector<Seg>& V2) {//remove greedily all overlapping segments
//	std::sort(V.begin(), V.end());
//	Pos tmp;
//	int sz = V.size();
//	for (int i = 0, j; i < sz; i = j) {//sweeping
//		j = i;
//		//std::cout << sz << " " << i << " " << j << " DEBUG\n";
//		while (j < sz && V[i].l == V[j].l) j++;
//		//std::cout << sz << " " << i << " " << j << " DEBUG\n";
//		for (int k = i; k < j - 1; k++) {
//			int nxt = k + 1;
//			if (V[k].e < V[nxt].s) continue;
//			else if (V[k].e == V[nxt].s) {
//				//tmp = V[k].s;
//				//V[k].s = V[nxt].s;
//				//V[nxt].s = tmp;
//				std::swap(V[k].s, V[nxt].s);
//			}
//			else if (V[nxt].e < V[k].e) {
//				//tmp = V[k].e;
//				//V[k].e = V[nxt].s;
//				//V[nxt].s = V[nxt].e;
//				//V[nxt].e = tmp;
//				//V[k].e = V[nxt].s;
//				std::swap(V[k].e, V[nxt].e);
//				std::swap(V[k].e, V[nxt].s);
//			}
//			else if (V[k].e <= V[nxt].e) {
//				//tmp = V[k].e;
//				//V[k].e = V[nxt].s;
//				//V[nxt].s = tmp;
//				std::swap(V[k].e, V[nxt].s);
//			}
//		}
//		for (int k = i; k < j; k++) if (V[k].s != V[k].e) V2.push_back(V[k]);
//	}
//	//std::cout << "DEBUG_end\n";
//	//std::sort(V2.begin(), V2.end());
//	return;
//}
//void solve() {
//	init();
//	sweep(G, G2);
//	sweep(J, J2);
//	int sz = G2.size();
//	if (sz != J2.size()) { std::cout << "no\n"; return; }
//	for (int i = 0; i < sz; i++) if (G2[i] != J2[i]) { std::cout << "no\n"; return; }
//	std::cout << "yes\n";
//	return;
//}
//int main() { solve(); return 0; }//boj16636 Triangular Clouds