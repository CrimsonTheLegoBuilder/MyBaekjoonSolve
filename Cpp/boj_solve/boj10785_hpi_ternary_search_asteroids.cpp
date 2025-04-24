#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef double ld;
const ld TOL = 1e-7;
const ld INF = 1e7;
int N[2];

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	Pos(ld X, ld Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	ld mag() const { return hypot(x, y); }
} vel[2], rel; const Pos O = { 0, 0 };
std::vector<Pos> H[2];
struct Vec {
	ld vy, vx;
	bool operator < (const Vec& v) const { return z(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (z(vy - v.vy) && z(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
}; const Vec Zero = { 0, 0 };
struct Line {
	Vec s;
	ld c;
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld ccw = s / l.s;
		return z(ccw) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : ccw > 0;
	}
	ld operator / (const Line& l) const { return s / l.s; }
};
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { {dy, dx} , c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ld ret = cross(d1, d2, d3, d4);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld dot_ = dot(d1, d3, d2);
	return z(cross(d1, d2, d3)) && (dot_ > 0 || z(dot_));
}
bool CW(const Line& l1, const Line& l2, const Line& target) {
	if (l1.s / l2.s < TOL) return 0;
	Pos p = intersection(l1, l2);
	return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
	std::deque<Line> dq;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
		if (!dq.empty() && z(dq.back() / l)) continue;
		while (dq.size() >= 2 && CW(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
		while (dq.size() >= 2 && CW(l, dq.front(), dq[1])) dq.pop_front();
		dq.push_back(l);
	}
	while (dq.size() >= 3 && CW(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
	while (dq.size() >= 3 && CW(dq.back(), dq.front(), dq[1])) dq.pop_front();
	for (int i = 0; i < dq.size(); i++) {
		Line cur = dq[i], nxt = dq[(i + 1) % dq.size()];
		if (cur / nxt < TOL) {
			hull.clear();
			return 0;
		}
		hull.push_back(intersection(cur, nxt));
	}
	return 1;
}
ld cal_dist(Pos& rel, bool f = 0) {
	ld d = !f ? INF : -INF;
	for (int i = 0; i < 2; i++) {
		rel *= -1;
		for (int j = 0; j < N[i]; j++) {
			Pos& p = H[i][j];
			for (int k = 0; k < N[i ^ 1]; k++) {
				Pos& cur = H[i ^ 1][k], nxt = H[i ^ 1][(k + 1) % N[i ^ 1]];
				if (!ccw(cur, nxt, p, p + rel) && !ccw(cur, nxt, p) && !(dot(p + rel, p, cur) > 0)) {
					if (!f) d = std::min({ d, (cur - p).mag(), (nxt - p).mag() });
					if (f) d = std::max({ d, (cur - p).mag(), (nxt - p).mag() });
				}
				else if (ccw(cur, nxt, p, p + rel)) {
					Pos inx = intersection(L(cur, nxt), L(p, p + rel));
					if (on_seg(cur, nxt, inx) && !(dot(p + rel, p, inx) > 0)) {
						if (!f) d = std::min(d, (p - inx).mag());
						if (f) d = std::max(d, (p - inx).mag());
					}
				}
			}
		}
	}
	return d;
}
ld area(std::vector<Pos>& H) {
	int sz = H.size();
	ld a{ 0 };
	for (int i = 0; i < sz; i++) a += cross(O, H[i], H[(i + 1) % sz]);
	return a;
}
ld overlapped_area(const Pos& rel, const ld& t) {
	std::vector<Pos> MH, HPI;
	for (const Pos& h : H[1]) MH.push_back(h + rel * t);
	std::vector<Line> HP;
	int sz0 = H[0].size(), sz1 = MH.size();
	for (int i = 0; i < sz0; i++) HP.push_back(L(H[0][i], H[0][(i + 1) % sz0]));
	for (int i = 0; i < sz1; i++) HP.push_back(L(MH[i], MH[(i + 1) % sz1]));
	if (!half_plane_intersection(HP, HPI)) return 0;
	return area(HPI);
}
ld ternary_search(const ld& t1, const ld& t2, const Pos& rel) {
	ld s = t1, e = t2, l, r, AL, AR;
	int cnt = 50;
	while (cnt--) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		AL = overlapped_area(rel, l);
		AR = overlapped_area(rel, r);
		if (z(AL - AR)) e = r;
		else if (AL > AR) e = r;
		else s = l;
	}
	return s;
}
void solve() {
	//std::cin.tie(0)->sync_with_stdio(0);
	//std::cout.tie(0);
	//std::cout << std::fixed;
	//std::cout.precision(6);
	for (int i = 0; i < 2; i++) {
		//std::cin >> N[i];
		scanf("%d", &N[i]);
		H[i].resize(N[i]);
		//for (int j = 0; j < N[i]; j++) std::cin >> H[i][j].x >> H[i][j].y;
		for (int j = 0; j < N[i]; j++) scanf("%lf%lf", &H[i][j].x, &H[i][j].y);
		std::reverse(H[i].begin(), H[i].end());
		//std::cin >> vel[i].x >> vel[i].y;
		scanf("%lf%lf", &vel[i].x, &vel[i].y);
	}
	rel = vel[1] - vel[0];
	ld MIN = cal_dist(rel);
	ld MAX = cal_dist(rel, 1);
	//if (z(rel.mag()) || MIN > 1e6) { std::cout << "never\n"; return; }
	if (z(rel.mag()) || MIN > 1e6) { printf("never\n"); return; }
	ld s = MIN / rel.mag();
	ld e = MAX / rel.mag();
	//std::cout << ternary_search(s, e, rel) << "\n";
	printf("%.7lf\n", ternary_search(s, e, rel));
	return;
}
int main() { solve(); return 0; }//boj10785

/*

4 0 0 0 10 10 10 10 0 0 0
4 11 5 11 6 12 6 12 5 -1 0
2.000

4 0 0 0 10 10 10 10 0 0 0
4 11 10 11 11 12 11 12 10 -1 0
1.000

4 0 0 0 10 10 10 10 0 0 0
4 11 9 11 11 12 11 12 9 -1 0
2.000

4 0 0 0 10 10 10 10 0 0 0
4 12 9 11 10 12 11 13 10 -1 0
3.000

4 0 0 0 10 10 10 10 0 -1 1
4 12 0 12 10 22 10 22 0 -2 0
6.000

4 0 0 0 10 10 10 10 0 -1 0
4 12 0 12 10 22 10 22 0 -2 0
12.000

3 0 0 1 1 2 0 0 0
3 2 1 0 2 2 2 -1 0
1.000

*/

//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <deque>
//#include <cmath>
//typedef long long ll;
//typedef long double ld;
//const ld TOL = 1e-7;
//const ld INF = 1e7;
//int N[2];
//
//bool z(const ld& x) { return std::abs(x) < TOL; }
//struct Pos {
//	ld x, y;
//	Pos(ld X, ld Y) : x(X), y(Y) {}
//	Pos() : x(0), y(0) {}
//	//bool operator == (const Pos& p) const { return x == p.x && y == p.y; }//ll
//	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }//ll
//	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }//ld
//	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }//ld
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const ld& n) const { return { x * n, y * n }; }
//	Pos operator / (const ld& n) const { return { x / n, y / n }; }
//	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos operator ~ () const { return { -y, x }; }
//	ld operator ! () const { return x * y; }
//	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
//	ld mag() const { return hypot(x, y); }
//} vel[2], rel; const Pos O = {0, 0};
//std::vector<Pos> H[2];
//struct Vec {
//	ld vy, vx;
//	bool operator < (const Vec& v) const { return z(vy - v.vy) ? vx < v.vx : vy < v.vy; }
//	bool operator == (const Vec& v) const { return (z(vy - v.vy) && z(vx - v.vx)); }
//	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
//}; const Vec Zero = { 0, 0 };
//struct Line {
//	Vec s;
//	ld c;
//	bool operator < (const Line& l) const {
//		bool f1 = Zero < s;
//		bool f2 = Zero < l.s;
//		if (f1 != f2) return f1;
//		ld ccw = s / l.s;
//		return z(ccw) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : ccw > 0;
//	}
//	ld operator / (const Line& l) const { return s / l.s; }
//};
//Line L(const Pos& s, const Pos& e) {
//	ld dy, dx, c;
//	dy = e.y - s.y;
//	dx = s.x - e.x;
//	c = dy * s.x + dx * s.y;
//	return { {dy, dx} , c };
//}
//Pos intersection(const Line& l1, const Line& l2) {
//	Vec v1 = l1.s, v2 = l2.s;
//	ld det = v1 / v2;
//	return {
//		(l1.c * v2.vx - l2.c * v1.vx) / det,
//		(l2.c * v1.vy - l1.c * v2.vy) / det
//	};
//}
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ld ret = cross(d1, d2, d3);
//	return z(ret) ? 0 : ret > 0 ? 1 : -1;
//}
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
//	ld ret = cross(d1, d2, d3, d4);
//	return z(ret) ? 0 : ret > 0 ? 1 : -1;
//}
//ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ld dot_ = dot(d1, d3, d2);
//	return z(cross(d1, d2, d3)) && (dot_ > 0 || z(dot_));
//}
//bool CW(const Line& l1, const Line& l2, const Line& target) {
//	if (l1.s / l2.s < TOL) return 0;
//	Pos p = intersection(l1, l2);
//	return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
//}
//bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
//	std::deque<Line> dq;
//	std::sort(HP.begin(), HP.end());
//	for (const Line& l : HP) {
//		if (!dq.empty() && z(dq.back() / l)) continue;
//		while (dq.size() >= 2 && CW(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
//		while (dq.size() >= 2 && CW(l, dq.front(), dq[1])) dq.pop_front();
//		dq.push_back(l);
//	}
//	while (dq.size() >= 3 && CW(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
//	while (dq.size() >= 3 && CW(dq.back(), dq.front(), dq[1])) dq.pop_front();
//	for (int i = 0; i < dq.size(); i++) {
//		Line cur = dq[i], nxt = dq[(i + 1) % dq.size()];
//		if (cur / nxt < TOL) {
//			hull.clear();
//			return 0;
//		}
//		hull.push_back(intersection(cur, nxt));
//	}
//	return 1;
//}
////int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {
////	int sz = H.size();
////	if (sz < 3 || cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return 0;
////	if (on_seg(H[0], H[1], p) || on_seg(H[0], H[sz - 1], p)) return 1;
////	int s = 0, e = sz - 1, m;
////	while (s + 1 < e) {
////		m = s + e >> 1;
////		if (cross(H[0], H[m], p) > 0) s = m;
////		else e = m;
////	}
////	if (cross(H[s], H[e], p) > 0 || on_seg(H[s], H[e], p)) return 1;
////	else 0;
////}
//ld cal_dist(Pos& rel, bool f = 0) {
//	ld d = !f ? INF : -INF;
//	for (int i = 0; i < 2; i++) {
//		rel *= -1;
//		for (int j = 0; j < N[i]; j++) {
//			Pos& p = H[i][j];
//			//if (!f && inner_check_bi_search(H[i ^ 1], p)) return 0;
//			for (int k = 0; k < N[i ^ 1]; k++) {
//				Pos& cur = H[i ^ 1][k], nxt = H[i ^ 1][(k + 1) % N[i ^ 1]];
//				if (!ccw(cur, nxt, p, p + rel) && !ccw(cur, nxt, p) && !(dot(p + rel, p, cur) > 0)) {
//					if (!f) d = std::min({ d, (cur - p).mag(), (nxt - p).mag() });
//					if (f) d = std::max({ d, (cur - p).mag(), (nxt - p).mag() });
//				}
//				else if (ccw(cur, nxt, p, p + rel)) {
//					Pos inx = intersection(L(cur, nxt), L(p, p + rel));
//					if (on_seg(cur, nxt, inx) && !(dot(p + rel, p, inx) > 0)) {
//						if (!f) d = std::min(d, (p - inx).mag());
//						if (f) d = std::max(d, (p - inx).mag());
//					}
//				}
//			}
//		}
//	}
//	return d;
//}
//ld area(std::vector<Pos>& H) {
//	int sz = H.size();
//	ld a{ 0 };
//	for (int i = 0; i < sz; i++) a += cross(O, H[i], H[(i + 1) % sz]);
//	return a;
//}
//ld overlapped_area(const Pos& rel, const ld& t) {
//	std::vector<Pos> MH, HPI;
//	for (const Pos& h : H[1]) MH.push_back(h + rel * t);
//	std::vector<Line> HP;
//	int sz0 = H[0].size(), sz1 = MH.size();
//	for (int i = 0; i < sz0; i++) HP.push_back(L(H[0][i], H[0][(i + 1) % sz0]));
//	for (int i = 0; i < sz1; i++) HP.push_back(L(MH[i], MH[(i + 1) % sz1]));
//	if (!half_plane_intersection(HP, HPI)) return 0;
//	return area(HPI);
//}
//ld ternary_search(const ld& t1, const ld& t2, const Pos& vel) {
//	ld s = t1, e = t2, l, r, AL{ 0 }, AR{ 0 };
//	int cnt = 50;
//	while (cnt--) {
//		l = (s + s + e) / 3;
//		r = (s + e + e) / 3;
//		AL = overlapped_area(rel, l);
//		AR = overlapped_area(rel, r);
//		//std::cout << "s: " << s << " e: " << e << "\n";
//		//std::cout << "l: " << l << " r: " << r << "\n";
//		//std::cout << "AL: " << AL << " AR: " << AR << "\n";
//		if (z(AL - AR)) e = r;
//		else if (AL > AR) e = r;
//		else s = l;
//	}
//	return s;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(6);
//	for (int i = 0; i < 2; i++) {
//		std::cin >> N[i];
//		H[i].resize(N[i]);
//		for (int j = 0; j < N[i]; j++) std::cin >> H[i][j].x >> H[i][j].y;
//		std::reverse(H[i].begin(), H[i].end());
//		std::cin >> vel[i].x >> vel[i].y;
//	}
//	rel = vel[1] - vel[0];
//	ld MIN = cal_dist(rel);
//	ld MAX = cal_dist(rel, 1);
//	//std::cout << MIN << " " << MAX << "\n";
//	if (z(rel.mag()) || MIN > 1e6) { std::cout << "never\n"; return; }
//	ld s = MIN / rel.mag();
//	ld e = MAX / rel.mag();
//	std::cout << ternary_search(s, e, rel) << "\n";
//	return;
//}
//int main() { solve(); return 0; }//boj10785
