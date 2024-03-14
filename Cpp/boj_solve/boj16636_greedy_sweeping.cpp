#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <cassert>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
int N, M;
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
ll gcd(ll a, ll b) {
	while (b) {
		ll tmp = a % b;
		a = b;
		b = tmp;
	}
	return a;
}

struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
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
	ll vy, vx;
	Vec(ll Y = 0, ll X = 0) : vy(Y), vx(X) {}
	bool operator < (const Vec& v) const { return vy == v.vy ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return vy == v.vy && vx == v.vx; }
	ll operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
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
	ll dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	ll _gcd = gcd(std::abs(dy), std::abs(dx));
	dy /= _gcd; dx /= _gcd;
	c = dy * s.x + dx * s.y;
	return Line(Vec(dy, dx), c);
}
struct Seg {
	Line l;
	Pos s, e;
	Seg(Line L = Line(Vec(0, 0), 0),
		Pos S = Pos(0, 0),
		Pos E = Pos(0, 0)
	) : l(L), s(S), e(E) {}
	bool operator < (const Seg& S) const {
		if (l == S.l) {
			if (s == S.s) return e < S.e;
			return s < S.s;
		}
		return l < S.l;
	}
	bool operator == (const Seg& S) const { return l == S.l && s == S.s && e == S.e; }
	bool operator != (const Seg& S) const { return !(*this == S); }
	friend std::ostream& operator << (std::ostream& os, const Seg& S) {
		os << "DEBUG::Seg l: " << S.l << " | s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n";
		return os;
	}
};
void make_seg(std::vector<Seg>& V, const Pos& x1, const Pos& x2) {
	Pos d1 = x1, d2 = x2;
	assert(d2 == d1);
	//if (d2 == d1) return;
	if (d2 < d1) std::swap(d1, d2);
	V.push_back(Seg(L(d1, d2), d1, d2));
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
void sweep(std::vector<Seg>& V, std::vector<Seg>& V2) {
	std::sort(V.begin(), V.end());
	Pos tmp;
	int sz = V.size();
	for (int i = 0, j; i < sz; i = j) {//sweeping
		j = i;
		while (V[i].l == V[j].l) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (V[k].e < V[nxt].s) continue;
			else if (V[k].e == V[nxt].s) {
				tmp = V[k].s;
				V[k].s = V[nxt].s;
				V[nxt].s = tmp;
			}
			else if (V[nxt].e < V[k].e) {
				tmp = V[k].e;
				V[k].e = V[nxt].s;
				V[nxt].s = V[nxt].e;
				V[nxt].e = tmp;
			}
			else if (V[k].e <= V[nxt].e) {
				tmp = V[k].e;
				V[k].e = V[nxt].s;
				V[nxt].s = tmp;
			}
		}
		for (int k = i; k < j; k++)
			if (V[k].s != V[k].e)
				V2.push_back(V[k]);
	}
	std::sort(V2.begin(), V2.end());
	return;
}
void solve() {
	init();
	sweep(G, G2);
	sweep(J, J2);
	int sz = G2.size();
	if (sz != J2.size()) {
		std::cout << "no\n";
		return;
	}
	for (int i = 0; i < sz; i++) {
		if (G2[i] != J2[i]) {
			std::cout << "no\n"; return;
		}
	}
	std::cout << "yes\n";
	return;
}
int main() { solve(); return 0; }//boj16636 Triangular Clouds



//void sweep(std::vector<Seg>& V, std::vector<Seg>& V2) {
//	std::sort(V.begin(), V.end());
//	Pos tmp;
//	int sz = V.size();
//	for (int i = 0, j; i < sz; i = j) {
//		j = i;
//		while (V[i].l == V[j].l) j++;
//
//		if (i + 1 == j) { if (V[i].s != V[i].e) V2.push_back(V[i]); continue; }
//
//		for (int k = i; k < j - 1; k++) {
//			int l = k + 1;
//			if (V[k].e < V[l].s) continue;
//			else if (V[k].e == V[l].s) {
//				tmp = V[k].s;
//				V[k].s = V[l].s;
//				V[k].s = tmp;
//			}
//			else if (V[k].s <= V[l].s && V[l].e < V[k].e) {
//				tmp = V[l].e;
//				V[l].e = V[k].e;
//				V[k].e = V[l].s;
//				V[l].s = tmp;
//			}
//			else if (V[l].s < V[k].e) {
//				tmp = V[k].e;
//				V[k].e = V[l].s;
//				V[l].s = tmp;
//			}
//		}
//		for (int k = i; k < j; k++)
//			if (V[k].s != V[k].e)
//				V2.push_back(V[k]);
//	}
//	std::sort(V2.begin(), V2.end());
//	return;
//}
//void init() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	G.clear();
//	J.clear();
//	G2.clear();
//	J2.clear();
//	//int ptr = 0;
//	std::cin >> N;
//	//G.resize(N * 3);
//	//ptr = 0;
//	for (int i = 0; i < N; i++) {
//		Pos p1, p2, p3, d1, d2;
//		std::cin >> p1 >> p2 >> p3;
//		d1 = p1, d2 = p2;
//		if (d2 < d1) std::swap(d1, d2);
//		//G[ptr++] = Seg(L(d1, d2), d1, d2);
//		G.push_back(Seg(L(d1, d2), d1, d2));
//		d1 = p2, d2 = p3;
//		if (d2 < d1) std::swap(d1, d2);
//		//G[ptr++] = Seg(L(d1, d2), d1, d2);
//		G.push_back(Seg(L(d1, d2), d1, d2));
//		d1 = p3, d2 = p1;
//		if (d2 < d1) std::swap(d1, d2);
//		//G[ptr++] = Seg(L(d1, d2), d1, d2);
//		G.push_back(Seg(L(d1, d2), d1, d2));
//	}
//	std::cin >> M;
//	//J.resize(M * 3);
//	//ptr = 0;
//	for (int j = 0; j < N; j++) {
//		Pos p1, p2, p3, d1, d2;
//		std::cin >> p1 >> p2 >> p3;
//		d1 = p1, d2 = p2;
//		if (d2 < d1) std::swap(d1, d2);
//		//J[ptr++] = Seg(L(d1, d2), d1, d2);
//		J.push_back(Seg(L(d1, d2), d1, d2));
//		d1 = p2, d2 = p3;
//		if (d2 < d1) std::swap(d1, d2);
//		//J[ptr++] = Seg(L(d1, d2), d1, d2);
//		J.push_back(Seg(L(d1, d2), d1, d2));
//		d1 = p3, d2 = p1;
//		if (d2 < d1) std::swap(d1, d2);
//		//J[ptr++] = Seg(L(d1, d2), d1, d2);
//		J.push_back(Seg(L(d1, d2), d1, d2));
//	}
//	std::sort(G.begin(), G.end());
//	std::sort(J.begin(), J.end());
//	return;
//}


//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <vector>
//typedef long long ll;
//typedef double ld;
//const ll INF = 1e17;
//const int LEN = 1e5 + 1;
////const ld TOL = 1e-7;
//int N, M;
////ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//ll gcd(ll a, ll b) {
//	while (b) {
//		ll tmp = a % b;
//		a = b;
//		b = tmp;
//	}
//	return a;
//}
//
//struct Pos {
//	ll x, y;
//	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	//Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	//Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	//Pos operator * (const ll& n) const { return { x * n, y * n }; }
//	//Pos operator / (const ll& n) const { return { x / n, y / n }; }
//	//ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	//ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	//Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	//Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	//Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	//Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
//	//Pos operator ~ () const { return { -y, x }; }
//	//ll operator ! () const { return x * y; }
//	//ll Euc() const { return x * x + y * y; }
//	//ll Man() const { return std::abs(x) + std::abs(y); }
//	//ld mag() const { return hypot(x, y); }
//	//friend ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//	//friend ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//	//friend int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	//	ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1;
//	//}
//	//friend bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
//	//	ll ret = dot(d1, d3, d2);
//	//	return !cross(d1, d2, d3) && ret >= 0;
//	//}
//	//friend bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
//	//	ll ret = dot(d1, d3, d2);
//	//	return !cross(d1, d2, d3) && ret > 0;
//	//}
//	//friend int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
//	//	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
//	//}
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
//	bool operator < (const Vec& v) const { return vy == v.vy ? vx < v.vx : vy < v.vy; }
//	bool operator == (const Vec& v) const { return vy == v.vy && vx == v.vx; }
//	ll operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
//	//ll operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
//	//Vec operator ~ () const { return { -vx, vy }; }
//	//Vec& operator *= (const ll& scalar) { vy *= scalar; vx *= scalar; return *this; }
//	//Vec& operator /= (const ll& scalar) { vy /= scalar; vx /= scalar; return *this; }
//}; const Vec Zero = { 0, 0 };
//struct Line {//ax + by = c
//	Vec s;
//	ll c;
//	Line(Vec V = Vec(0, 0), ll C = 0) : s(V), c(C) {}
//	bool operator == (const Line& L) const { return s == L.s && c == L.c; }
//	bool operator < (const Line& l) const {
//		bool f1 = Zero < s;
//		bool f2 = Zero < l.s;
//		if (f1 != f2) return f1;
//		ll CCW = s / l.s;
//		return !CCW ? c < l.c : CCW > 0;
//	}
//	ll operator / (const Line& l) const { return s / l.s; }
//	//ll operator * (const Line& l) const { return s * l.s; }
//	//Line operator * (const ll& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
//	//Line& operator *= (const ll& scalar) { s *= scalar, c *= scalar; return *this; }
//	//Line operator + (const ll& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
//	//Line operator - (const ll& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
//	//Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
//	//Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
//	//ll dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
//	//ll above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
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
//	Seg(Line L = Line(Vec(0, 0), 0),
//		Pos S = Pos(0, 0),
//		Pos E = Pos(0, 0))
//		: l(L), s(S), e(E) {}
//	bool operator < (const Seg& S) const {
//		if (l == S.l) {
//			if (s == S.s) return S.e < e;
//			return s < S.s;
//		}
//		return l < S.l;
//	}
//	bool operator == (const Seg& S) const { return l == S.l && s == S.s && e == S.e; }
//	bool operator != (const Seg& S) const { return !(*this == S); }
//	friend std::ostream& operator << (std::ostream& os, const Seg& S) {
//		os << "DEBUG::Seg: l: " << S.l << " s: " << S.s << " e: " << S.e << "DEBUG::Seg\n";
//		return os;
//	}
//};
//void make_seg(std::vector<Seg>& V, const Pos& x1, const Pos& x2) {
//	Pos d1 = x1, d2 = x2;
//	//if (d2 == d1) return;
//	if (d2 < d1) std::swap(d1, d2);
//	V.push_back(Seg(L(d1, d2), d1, d2));
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
//	std::cin >> N;
//	Pos p1, p2, p3;
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
//void sweep(std::vector<Seg>& V, std::vector<Seg>& V2) {
//	std::sort(V.begin(), V.end());
//	Pos tmp;
//	int sz = V.size();
//	for (int i = 0, j; i < sz; i = j) {
//		j = i;
//		while (V[i].l == V[j].l) j++;
//
//		if (i + 1 == j) { if (V[i].s != V[i].e) V2.push_back(V[i]); continue; }
//
//		for (int k = i; k < j - 1; k++) {
//			int l = k + 1;
//			if (V[k].e < V[l].s) continue;
//			else if (V[l].e < V[k].e) {
//				tmp = V[k].e;
//				V[k].e = V[l].s;
//				V[l].s = V[l].e;
//				V[l].e = tmp;
//			}
//			else if (V[k].e <= V[l].e) {
//				tmp = V[k].e;
//				V[k].e = V[l].s;
//				V[l].s = tmp;
//			}
//		}
//		for (int k = i; k < j; k++)
//			if (V[k].s != V[k].e)
//				V2.push_back(V[k]);
//	}
//	std::sort(V2.begin(), V2.end());
//	return;
//}
//void solve() {
//	init();
//	sweep(G, G2);
//	sweep(J, J2);
//	int sz = G2.size();
//	if (sz != J2.size()) {
//		std::cout << "no\n";
//		return;
//	}
//	for (int i = 0; i < sz; i++) {
//		if (G2[i] != J[i]) {
//			std::cout << "no\n"; return;
//		}
//	}
//	std::cout << "yes\n";
//	return;
//}
//int main() { solve(); return 0; }