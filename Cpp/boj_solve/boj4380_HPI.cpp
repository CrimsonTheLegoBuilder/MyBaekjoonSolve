#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-9;
bool f;
std::string F;

bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
struct Pos {
	ld x, y;
	bool c;
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator * (const ld& s) const { return { x * s, y * s }; }
} s, e;
struct Line {
	ld vy, vx, c;  // a(vy) * x + b(vx) * y - c == 0;
	bool operator < (const Line& l) const {
		bool f1 = z(vy) ? vx > 0 : vy > 0;
		bool f2 = z(l.vy) ? l.vx > 0 : l.vy > 0;  // sort CCW
		if (f1 != f2) return f1 > f2;
		ld ccw = vy * l.vx - vx * l.vy;  // ccw == 0 : parallel
		return z(ccw) ? c * hypot(l.vx, l.vy) < l.c * hypot(vx, vy) : ccw > 0;  // sort by distance
	}
	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; }  //cross
	Line operator ~ () const { return { -vx, vy, c }; }  //rotate PI/2
};
Line L(const Pos& s, const Pos& e) {  //line consist with two point
	ld dy = e.y - s.y;
	ld dx = s.x - e.x;  // -(e.x - s.x)
	ld c = dy * s.x + dx * s.y;// -hypot(dy, dx) * 1e-4;
	return { dy, dx, c };
}
Line L(const Line& l, const Pos& p) {  //line consist with vector && pos
	ld c = l.vy * p.x + l.vx * p.y;// -hypot(l.vy, l.vx) * 1e-4;
	return { l.vy, l.vx, c };
}
Line LBX(const Pos& s, const Pos& e) {  //line bisector
	Pos m = (s + e) * .5;  //middle point
	Line l = ~L(s, e);     //s=>e line => rotate PI/2
	return L(l, m);
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld A(std::vector<Pos>& H) {
	ld area = 0;
	int h = H.size();
	Pos pivot = { 0, 0 };
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		area += cross(pivot, cur, nxt);
	}
	return area / 2;
}
Pos IP(const Line& l1, const Line& l2) {
	ld det = l1 / l2;  //ld det = l1.vy * l2.vx - l1.vx * l2.vy;
	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
}
bool CW(const Line& l1, const Line& l2, const Line& l) { 
	if (l1 / l2 <= 0) return 0;
	Pos p = IP(l1, l2);
	return l.vy * p.x + l.vx * p.y >= l.c;
}
bool HPI(std::vector<Line>& L, std::vector<Pos>& INX) {
	std::deque<Line> D;
	std::sort(L.begin(), L.end());
	for (const Line& l : L) {
		if (!D.empty() && z(D.back() / l)) continue;
		while (D.size() >= 2 && CW(D[D.size() - 2], D.back(), l)) D.pop_back();
		while (D.size() >= 2 && CW(l, D[0], D[1])) D.pop_front();
		D.push_back(l);
	}
	while (D.size() > 2 && CW(D[D.size() - 2], D.back(), D[0])) D.pop_back();
	while (D.size() > 2 && CW(D.back(), D[0], D[1])) D.pop_front();
	//if (D.size() < 3) return 0;
	std::vector<Pos> h;
	for (int i = 0; i < D.size(); i++) {
		Line cur = D[i], nxt = D[(i + 1) % D.size()];
		if (cur / nxt <= TOL) return 0;
		h.push_back(IP(cur, nxt));
	}
	INX = h;
	return 1;
}
std::vector<Line> HP;
std::vector<Pos> inx;
void brute() {
	//freopen("hottercolder.in", "r", stdin);
	inx = { { 0, 0 }, { 10, 0 }, { 10, 10 }, { 0, 10 } };
	s = { 0, 0 };
	while (std::cin >> e.x >> e.y >> F) {
		HP.clear();
		//std::cout << inx.size() << "\n";
		for (int i = 0; i < inx.size(); i++) {
			HP.push_back(L(inx[i], inx[(i + 1) % inx.size()]));
			//std::cout << inx[i].x << " " << inx[i].y << "\n";
  		}
		if (F == "Hotter") {
			Line lbx = LBX(e, s);
			HP.push_back(lbx);
		}
		else if (F == "Colder") {
			Line lbx = LBX(s, e);
			HP.push_back(lbx);
		}
		else if (F == "Same") {
			Line l1 = LBX(s, e), l2 = LBX(e, s);
			HP.push_back(l1); HP.push_back(l2);
		}
		f = HPI(HP, inx);
		if (f) std::cout << A(inx) << "\n";
		else std::cout << "0.00\n", inx.clear();
		s = e;

	}
	return;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	brute();
	return 0;
}



//ld dist(const Pos& d1, const Pos& d2) {
//	return hypot((d2.x - d1.x), (d2.y - d1.y));
//}
//bool I(std::vector<Pos>& H, const Pos& x) {
//	int s = 0, e = H.size() - 1, m;
//	if (e < 2 || cross(H[0], H[1], x) < -TOL || cross(H[0], H[e], x) > TOL) return 0;
//	while (s + 1 < e) {
//		m = s + e >> 1;
//		if (cross(H[0], H[m], x) > 0) s = m;
//		else e = m;
//	}
//	return cross(H[s], H[e], x) > TOL;
//}
//ld cross(const Vec& v1, const Vec& v2) {
//	return v1.vy * v2.vx - v1.vx * v2.vy;  // a(vy) * x + b(vx) * y - c == 0;
//}


