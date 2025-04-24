#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef double ld;
const int LEN = 100005;
const ld TOL = 1e-8;
int T, N, len;
ld D;
bool F;

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	int i;
	bool operator < (const Pos& p) const { return i < p.i; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator * (const ld& s) const { return { x * s, y * s }; }
} pos[LEN];
std::vector<Pos> hull;
struct Vec {
	ld vy, vx;
	bool operator < (const Vec& v) const {
		return z(vy - v.vy) ? vx < v.vx : vy < v.vy;
	}
	bool operator == (const Vec& v) const {
		return (z(vy - v.vy) && z(vx - v.vx));
	}
	ld operator / (const Vec& v) const {
		return vy * v.vx - vx * v.vy;  // cross
	}
	Vec operator ~ () const { return { -vx, vy }; }  //rotate PI/2
};
const Vec Zero = { 0, 0 };
struct Line {
	Vec s;
	ld c;
	int i;
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld ccw = s / l.s;
		return z(ccw) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : ccw > 0;
	}
	ld operator / (const Line& l) const { return s / l.s; }  //cross
	Line operator ~ () const { return { ~s, c }; }  //rotate PI/2
};
std::vector<Line> bisectors;
Line L(const Pos& s, const Pos& e) {  //line consist with two point
	ld dy = e.y - s.y;
	ld dx = s.x - e.x;  // -(e.x - s.x)
	ld c = dy * s.x + dx * s.y;//-hypot(dy, dx) * TOL;
	return { { dy, dx }, c, e.i };
}
Line L(const Line& l, const Pos& p, int i) {  //line consist with vector && pos
	Vec v = l.s;
	ld c = v.vy * p.x + v.vx * p.y;
	return { { v.vy, v.vx }, c, i };
}
Line line_bisector(const Pos& s, const Pos& e) {
	Pos m = (s + e) * .5;  //middle point
	Line l = ~L(s, e);     //s=>e line => rotate PI/2
	return L(l, m, e.i);
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
		l1.i
	};
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
		//hull.push_back(intersection(cur, nxt));
		Pos p = intersection(cur, nxt);
		if (p.i > -1) hull.push_back(p);
	}
	return 1;
}
void voronoi_brute() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		for (int i = 0; i < N; i++) {
			std::cin >> pos[i].x >> pos[i].y;
			pos[i].i = i;
		}
		bisectors.clear(); hull.clear();
		bisectors.push_back({ { 1, 0 }, 1e17, -1 });
		bisectors.push_back({ { 0, 1 }, 1e17, -1 });
		bisectors.push_back({ { -1, 0 }, 1e17, -1 });
		bisectors.push_back({ { 0, -1 }, 1e17, -1 });
		for (int i = 1; i < N; i++) {
			bisectors.push_back(line_bisector(pos[0], pos[i]));
		}
		F = half_plane_intersection(bisectors, hull);
		std::sort(hull.begin(), hull.end());
		std::cout << hull.size();
		for (const Pos& p : hull) std::cout << " " << p.i;
		std::cout << "\n";

	}
	return;
}
int main() { voronoi_brute(); return 0; }

//bool operator < (const Pos& p) const {
//	return z(x - p.x) ? y < p.y : x < p.x;
//}

////#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <deque>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//typedef long double ld;
//const int LEN = 100005;
//const ld TOL = 1e-9;
//int T, N;
//bool f;
//
//bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
//struct Pos {
//	ld x, y;
//	int i;
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator * (const ld& s) const { return { x * s, y * s }; }
//}pos[LEN], pu, pr, pl, pd;
//struct Line {
//	ld vy, vx, c;  // a(vy) * x + b(vx) * y - c == 0;
//	int i;
//	bool operator < (const Line& l) const {
//		bool f1 = z(vy) ? vx > 0 : vy > 0;
//		bool f2 = z(l.vy) ? l.vx > 0 : l.vy > 0;  // sort CCW
//		if (f1 != f2) return f1 > f2;
//		ld ccw = vy * l.vx - vx * l.vy;  // ccw == 0 : parallel
//		return z(ccw) ? c * hypot(l.vx, l.vy) < l.c * hypot(vx, vy) : ccw > 0;  // sort by distance
//	}
//	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; }  //cross
//	Line operator ~ () const { return { -vx, vy, c }; }  //rotate PI/2
//};
//Line L(const Pos& s, const Pos& e) {  //line consist with two point
//	ld dy = e.y - s.y;
//	ld dx = s.x - e.x;  // -(e.x - s.x)
//	ld c = dy * s.x + dx * s.y;// -hypot(dy, dx) * TOL;
//	return { dy, dx, c, s.i };
//}
//Line L(const Line& l, const Pos& p, int i) {  //line consist with vector && pos
//	ld c = l.vy * p.x + l.vx * p.y;
//	return { l.vy, l.vx, c, i };
//}
//Line LBX(const Pos& s, const Pos& e) {  //line bisector
//	Pos m = (s + e) * .5;  //middle point
//	Line l = ~L(s, e);     //s=>e line => rotate PI/2
//	return L(l, m, e.i);
//}
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//ld dist(const Pos& d1, const Pos& d2) {
//	return hypot((d1.x - d2.x), (d1.y - d2.y));
//}
//ld MD(const Pos& d1, std::vector<Pos>& H) {
//	ld md = 0;
//	for (const Pos& p : H) {
//		md = std::max(md, dist(d1, p));
//	}
//	return md;
//}
//Pos IP(const Line& l1, const Line& l2) {
//	ld det = l1 / l2;  //ld det = l1.vy * l2.vx - l1.vx * l2.vy;
//	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
//}
//bool CW(const Line& l1, const Line& l2, const Line& l) {
//	if (l1 / l2 <= 0) return 0;
//	Pos p = IP(l1, l2);
//	return l.vy * p.x + l.vx * p.y >= l.c;
//}
//std::vector<Line> hull;
//bool HPI(std::vector<Line>& HP, std::vector<Pos>& INX) {
//	std::deque<Line> D;
//	std::sort(HP.begin(), HP.end());
//	for (const Line& l : HP) {
//		if (!D.empty() && z(D.back() / l)) continue;
//		while (D.size() >= 2 && CW(D[D.size() - 2], D.back(), l)) D.pop_back();
//		while (D.size() >= 2 && CW(l, D[0], D[1])) D.pop_front();
//		D.push_back(l);
//	}
//	while (D.size() > 2 && CW(D[D.size() - 2], D.back(), D[0])) D.pop_back();
//	while (D.size() > 2 && CW(D.back(), D[0], D[1])) D.pop_front();
//	std::vector<Pos> h;
//	for (int i = 0; i < D.size(); i++) {
//		Line cur = D[i], nxt = D[(i + 1) % D.size()];
//		if (cur / nxt < TOL) return 0;
//		h.push_back(IP(cur, nxt));
//		hull.push_back(cur);
//	}
//	INX = h;
//	return 1;
//}
//std::vector<Line> HP;
//std::vector<Pos> INX;
//std::vector<int> seq;
//void voronoi_brute() {
//	std::cin >> N;
//	pu = { 1e17, 1e17, -1 }, pr = { 1e17, -1e17, -1 }, pl = { -1e17, 1e17, -1 }, pd = { -1e17, -1e17, -1 };
//	ld ans = 0;
//	for (int i = 0; i < N; i++) {
//		std::cin >> pos[i].x >> pos[i].y;
//		pos[i].i = i;
//	}
//	HP.clear(); INX.clear(); hull.clear(); seq.clear();
//	HP.push_back(L(pr, pu)); HP.push_back(L(pu, pl));
//	HP.push_back(L(pl, pd)); HP.push_back(L(pd, pr));
//	for (int i = 1; i < N; i++) {
//		Line lbx = LBX(pos[0], pos[i]);
//		HP.push_back(lbx);
//	}
//	f = HPI(HP, INX);
//	for (const Line& l : hull) {
//		if (l.i > -1) seq.push_back(l.i);
//	}
//	std::sort(seq.begin(), seq.end());
//	std::cout << seq.size();
//	for (const int& i : seq) {
//		std::cout << " " << i;
//	}
//	std::cout << "\n";
//	return;
//}
//void solve() {
//	std::cin >> T;
//	while(T--) {
//		voronoi_brute();
//	}
//	return;
//}
//
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	solve();
//	return 0;
//}