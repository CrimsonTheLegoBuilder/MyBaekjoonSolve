#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const int LEN = 121;
const ld TOL = 1e-9;
const ld PI = acos(-1);
int N, M;
ld x, y;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
ld norm(ld& th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}

struct Pos {
	ld x, y;
	int i;
	Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, 0 }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, 0 }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar, 0 }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar, 0 }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x, 0 }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y << "\n";
		return os;
	}
} POS[LEN << 1], T[LEN]; const Pos O = { 0, 0, -1 };
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
}; const Vec Zero = { 0, 0 };
struct Line {
	Vec s;
	ld c;
	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
	}
	ld operator / (const Line& l) const { return s / l.s; }
	Line operator ~ () const { return { { s.vx, -s.vy }, c }; }  //rotate pi/2
};
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& p) const { return c == p.c && std::abs(r - p.r) < TOL; }
	bool operator != (const Circle C) const { return std::abs(C.c.x - c.x) >= TOL || std::abs(C.c.y - c.y) >= TOL || std::abs(r - C.r) >= TOL; }
	Circle operator + (const Circle& C) const { return { {c.x + C.c.x, c.y + C.c.y}, r + C.r }; }
	Circle operator - (const Circle& C) const { return { {c.x - C.c.x, c.y - C.c.y}, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }// coord trans | check right
	friend std::ostream& operator << (std::ostream& os, const Circle& c) {
		//os << c.c.x << " " << c.c.y << " " << c.r << "\n";
		os << c.c.x << " " << c.c.y << " " << sqrtl(c.r) << "\n";
		return os;
	}
};
Pos mid(const Pos& d1, const Pos& d2) { return (d1 + d2) * .5; }
Line L(const Pos& d1, const Pos& d2) {
	return {
{ d2.y - d1.y, d1.x - d2.x },
(d2.y - d1.y) * d1.x - (d1.x - d2.x) * d1.y };
}
Line L(const Line& v, const Pos& p) {
	return
	{
	{ v.s.vy, v.s.vx },
	v.s.vy * p.x + v.s.vx * p.y
	};
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
		0
	};
}
Line rotate90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return { s, c };
}
Circle C(const Pos& d1, const Pos& d2) {
	Pos c = mid(d1, d2);
	ld r = (c - d1).Euc();
	return { c, r };
}
Circle C(const Pos& d1, const Pos& d2, const Pos& d3) {
	Line l1 = rotate90(L(d1, d2), mid(d1, d2)), l2 = rotate90(L(d2, d3), mid(d2, d3));
	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
	Pos c = intersection(l1, l2);
	ld r = (c - d1).Euc();
	return { c, r };
}
Circle enclose(const Pos& u, const Pos& v, const Pos& w) {
	Pos b = v - u, c = w - u;
	ld B = b * b, C = c * c;
	ld D = b / c;
	if (zero(D)) return { { 0, 0 }, -1 };
	ld X = (c.y * B - b.y * C) / (2 * D);
	ld Y = (b.x * C - c.x * B) / (2 * D);
	Pos m = Pos(X, Y);
	return Circle(u + m, m * m);
}
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	Pos B = v - u, C = w - u;
	Line B_ = Line({ B.x, B.y }, B.Euc() / 2);
	Line C_ = Line({ C.x, C.y }, C.Euc() / 2);
	if (zero(B_ / C_)) return { { 0, 0 }, -1 };
	Pos inx = intersection(B_, C_);
	return Circle(inx + u, inx.Euc());
}
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	Line l1 = rotate90(L(u, v), (u + v) * .5);
	Line l2 = rotate90(L(v, w), (v + w) * .5);
	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
	Pos c = intersection(l1, l2);
	ld r = (c - u).Euc();
	return Circle(c, r);
}
char F;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	if (N <= 2) {
		std::cout << "No\n";
		return 0;
	}
	for (int i = 0; i < N; i++) {
		std::cin >> T[i].x >> T[i].y >> F;
		T[i].i = F == 'I' ? 1 : 0;
	}
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			Pos d1 = T[i], d2 = T[j];
			Circle c = C(d1, d2);
			bool f = 1;
			for (int k = 0; k < N; k++) {
				ld d = (c.c - T[k]).Euc();
				if ((T[k].i && d > c.r) || (!T[k].i && d < c.r)) {
					f = 0; break;
				}
			}
			if (f) {
				std::cout << "No\n";
				return 0;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			for (int k = j + 1; k < N; k++) {
				Pos d1 = T[i], d2 = T[j], d3 = T[k];
				//Circle c = C(d1, d2, d3);
				Circle c = enclose_circle(d1, d2, d3);
				if (c.r < 0) continue;
				bool f = 1;
				for (int l = 0; l < N; l++) {
					ld d = (c.c - T[l]).Euc();
					if ((T[l].i && d > c.r) || (!T[l].i && d < c.r)) {
						f = 0; break;
					}
				}
				if (f) {
					std::cout << "No\n";
					return 0;
				}
			}
		}
	}
	std::cout << "Yes\n";
	return 0;
}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//const int LEN = 121;
//const ld TOL = 1e-9;
//const ld PI = acos(-1);
//int N, M;
//ld x, y;
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
//ld norm(ld& th) {
//	while (th < -TOL) th += PI * 2;
//	while (th > PI * 2) th -= PI * 2;
//	return th;
//}
//
//struct Pos {
//	ld x, y;
//	int i;
//	Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y), i(I) {}
//	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
//	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, 0 }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, 0 }; }
//	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar, 0 }; }
//	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar, 0 }; }
//	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos operator ~ () const { return { -y, x, 0 }; }
//	ld operator ! () const { return x * y; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
//	ld Euc() const { return x * x + y * y; }
//	ld mag() const { return hypot(x, y); }
//	friend std::istream& operator >> (std::istream& is, Pos& p) {
//		is >> p.x >> p.y;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
//		os << p.x << " " << p.y << "\n";
//		return os;
//	}
//} POS[LEN << 1], T[LEN]; const Pos O = { 0, 0, -1 };
//struct Vec {
//	ld vy, vx;
//	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
//	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
//	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
//	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
//	Vec operator ~ () const { return { -vx, vy }; }
//}; const Vec Zero = { 0, 0 };
//struct Line {
//	Vec s;
//	ld c;
//	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
//	bool operator < (const Line& l) const {
//		bool f1 = Zero < s;
//		bool f2 = Zero < l.s;
//		if (f1 != f2) return f1;
//		ld CCW = s / l.s;
//		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
//	}
//	ld operator / (const Line& l) const { return s / l.s; }
//	Line operator ~ () const { return { { s.vx, -s.vy }, c }; }  //rotate pi/2
//};
//struct Circle {
//	Pos c;
//	ld r;
//	Circle(Pos C = Pos(0, 0, 0), ld R = 0) : c(C), r(R) {}
//	bool operator == (const Circle& p) const { return c == p.c && std::abs(r - p.r) < TOL; }
//	bool operator != (const Circle C) const { return std::abs(C.c.x - c.x) >= TOL || std::abs(C.c.y - c.y) >= TOL || std::abs(r - C.r) >= TOL; }
//	Circle operator + (const Circle& C) const { return { {c.x + C.c.x, c.y + C.c.y}, r + C.r }; }
//	Circle operator - (const Circle& C) const { return { {c.x - C.c.x, c.y - C.c.y}, r - C.r }; }
//	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }// coord trans | check right
//	friend std::ostream& operator << (std::ostream& os, const Circle& c) {
//		//os << c.c.x << " " << c.c.y << " " << c.r << "\n";
//		os << c.c.x << " " << c.c.y << " " << sqrtl(c.r) << "\n";
//		return os;
//	}
//};
//Pos mid(const Pos& d1, const Pos& d2) { return (d1 + d2) * .5; }
//Line L(const Pos& d1, const Pos& d2) {
//	return {
//{ d2.y - d1.y, d1.x - d2.x },
//(d2.y - d1.y) * d1.x - (d1.x - d2.x) * d1.y };
//}
//Line L(const Line& v, const Pos& p) {
//	return
//	{
//	{ v.s.vy, v.s.vx },
//	v.s.vy * p.x + v.s.vx * p.y
//	};
//}
//Pos intersection(const Line& l1, const Line& l2) {
//	Vec v1 = l1.s, v2 = l2.s;
//	ld det = v1 / v2;
//	return {
//		(l1.c * v2.vx - l2.c * v1.vx) / det,
//		(l2.c * v1.vy - l1.c * v2.vy) / det,
//		0
//	};
//}
//Line rotate90(const Line& l, const Pos& p) {
//	Vec s = ~l.s;
//	ld c = s.vy * p.x + s.vx * p.y;
//	return { s, c };
//}
//Circle C(const Pos& d1, const Pos& d2) {
//	Pos c = mid(d1, d2);
//	ld r = (c - d1).Euc();
//	return { c, r };
//}
//Circle C(const Pos& d1, const Pos& d2, const Pos& d3) {
//	Line l1 = rotate90(L(d1, d2), mid(d1, d2)), l2 = rotate90(L(d2, d3), mid(d2, d3));
//	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
//	Pos c = intersection(l1, l2);
//	ld r = (c - d1).Euc();
//	return { c, r };
//}
//Circle enclose(const Pos& u, const Pos& v, const Pos& w) {
//	Pos B = v - u, C = w - u;
//	if (zero(B / C)) return { { 0, 0 }, -1 };
//	Line B_ = Line({ B.x, B.y }, B.Euc() / 2);
//	Line C_ = Line({ C.x, C.y }, C.Euc() / 2);
//	Pos inx = intersection(B_, C_);
//	return Circle(inx + u, (inx - u).Euc());
//}
//int main() {
//	char F;
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	if (N <= 2) {
//		std::cout << "No\n";
//		return 0;
//	}
//	for (int i = 0; i < N; i++) {
//		std::cin >> T[i].x >> T[i].y >> F;
//		T[i].i = F == 'I' ? 1 : 0;
//	}
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			Pos d1 = T[i], d2 = T[j];
//			Circle c = C(d1, d2);
//			bool f = 1;
//			for (int k = 0; k < N; k++) {
//				ld d = (c.c - T[k]).Euc();
//				if ((T[k].i && d > c.r) || (!T[k].i && d < c.r)) {
//					f = 0; break;
//				}
//			}
//			if (f) {
//				std::cout << "No\n";
//				return 0;
//			}
//		}
//	}
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			for (int k = j + 1; k < N; k++) {
//				Pos d1 = T[i], d2 = T[j], d3 = T[k];
//				//Circle c = C(d1, d2, d3);
//				Circle c = enclose(d1, d2, d3);
//				if (c.r < 0) continue;
//				bool f = 1;
//				for (int l = 0; l < N; l++) {
//					ld d = (c.c - T[l]).Euc();
//					if ((T[l].i && d > c.r) || (!T[l].i && d < c.r)) {
//						f = 0; break;
//					}
//				}
//				if (f) {
//					std::cout << "No\n";
//					return 0;
//				}
//			}
//		}
//	}
//	std::cout << "Yes\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//typedef long double ld;
//const int LEN = 100;
//const ld TOL = 1e-9;
//int N;
//ld x, y;
//char F;
//
//bool z(ld x) { return std::abs(x) < TOL; }  //zero?
//struct Pos {
//	ld x, y; bool i;
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator * (const ld& s) const { return { x * s, y * s }; }
//} T[LEN];
//struct Line {
//	ld vy, vx, c; //vy * x + vx * y - c = 0
//	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; }  //cross
//	Line operator ~ () const { return { vx, -vy, c }; }  //rotate pi/2
//};  
//struct Circle { Pos c; ld r; };
//
//ld dist(const Pos& d1, const Pos& d2) {
//	ld x = d1.x - d2.x, y = d1.y - d2.y;
//	return x * x + y * y;
//}
//Pos m(const Pos& d1, const Pos& d2) { return (d1 + d2) * .5; }
//Line L(const Pos& d1, const Pos& d2) { return { d2.y - d1.y, d1.x - d2.x, (d2.y - d1.y) * d1.x - (d1.x - d2.x) * d1.y }; }
//Line L(const Line& v, const Pos& p) { return { v.vy, v.vx, v.vy * p.x + v.vx * p.y }; }
//Pos IP(const Line& l1, const Line& l2) {
//	ld det = l1 / l2;  	//ld det = l1.vy * l2.vx - l1.vx * l2.vy;
//	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
//}
//Circle C(const Pos& d1, const Pos& d2) {
//	Pos c = m(d1, d2);
//	ld r = dist(c, d1);
//	return { c, r };
//}
//Circle C(const Pos& d1, const Pos& d2, const Pos& d3) {
//	Line l1 = L(~L(d1, d2), m(d1, d2)), l2 = L(~L(d2, d3), m(d2, d3));
//	if (z(l1 / l2)) return { (0, 0), -1 };
//	Pos c = IP(l1, l2);
//	ld r = dist(c, d1);
//	return { c, r };
//}
//
//
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	if (N <= 2) {
//		std::cout << "No\n";
//		return 0;
//	}
//	for (int i = 0; i < N; i++) {
//		std::cin >> T[i].x >> T[i].y >> F;
//		T[i].i = F == 'I' ? 1 : 0;
//	}
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			Pos d1 = T[i], d2 = T[j];
//			Circle c = C(d1, d2);
//			bool f = 1;
//			for (int k = 0; k < N; k++) {
//				ld d = dist(c.c, T[k]);
//				if ((T[k].i && d > c.r) || (!T[k].i && d < c.r)) {
//					f = 0; break;
//				}
//			}
//			if (f) {
//				std::cout << "No\n";
//				return 0;
//			}
//		}
//	}
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			for (int k = j + 1; k < N; k++) {
//				Pos d1 = T[i], d2 = T[j], d3 = T[k];
//				Circle c = C(d1, d2, d3);
//				if (c.r < 0) continue;
//				bool f = 1;
//				for (int l = 0; l < N; l++) {
//					ld d = dist(c.c, T[l]);
//					if ((T[l].i && d > c.r) || (!T[l].i && d < c.r)) {
//						f = 0; break;
//					}
//				}
//				if (f) {
//					std::cout << "No\n";
//					return 0;
//				}
//			}
//		}
//	}
//	std::cout << "Yes\n";
//	return 0;
//}
//
//
////std::cin >> x >> y >> F;
////if (F == 'I') T[i] = { x, y, 1 };
////else if (F == 'N') T[i] = { x, y, 0 };
////struct Vec {
////	ld vy, vx;
////	Vec operator ~ () const { return { vx, -vy }; }  //rotate pi/2
////};
//// //Vec V(const Pos& d1, const Pos& d2) { return { d2.y - d1.y, d1.x - d2.x }; }
//// //Line L(const Vec& v, const Pos& p) { return { v.vy, v.vx, v.vy * p.x + v.vx * p.y }; }
////Pos m1 = m(d1, d2), m2 = m(d2, d3);
////Vec v1 = V(d1, d2), v2 = V(d2, d3);
////Line l1 = L(~v1, m1), l2 = L(~v2, m2);
////if (z(cross(l1, l2))) return { (0.0, 0.0), -1 };
////ld cross(const Line& l1, const Line& l2) { return l1.vy * l2.vx - l1.vx * l2.vy; }