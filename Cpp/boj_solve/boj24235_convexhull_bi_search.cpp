#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef double ld;
const int LEN = 500'000;
const ld TOL = 1e-9;
int N;
ld X, min_x, max_x, area;
bool F;

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	bool operator < (const Pos& p) const {
		return x == p.x ? y < p.y : x < p.x;
	}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator * (const ld& s) const { return { x * s, y * s }; }
};
std::vector<Pos> pos, hull, HPI;
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
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
void monotone_chain(std::vector<Pos>& C, std::vector<Pos>& H) {
	std::sort(C.begin(), C.end());
	min_x = C[0].x, max_x = C[C.size() - 1].x;
	if (C.size() <= 2) {
		for (const Pos& p : C) H.push_back(p);
		return;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return;
}
ld A(std::vector<Pos>& H) {
	Pos P = { 0, 0 };
	ld a = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		a += cross(P, cur, nxt);
	}
	return a / 2;
}
Line L(const Pos& d1, const Pos& d2) {
	ld vy = d2.y - d1.y;
	ld vx = d1.x - d2.x;
	ld c = vy * d1.x + vx * d1.y;
	return { { vy, vx }, c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det
	};
}
ld f(ld x) {
	HPI.clear();
	int sz = hull.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = hull[i], nxt = hull[(i + 1) % sz];
		if (cur.x < x && nxt.x < x) HPI.push_back(cur);
		else if (cur.x < x && (x < nxt.x || z(x - nxt.x)))
			HPI.push_back(cur), HPI.push_back(intersection(L(cur, nxt), { { 1, 0 }, x }));
		else if (nxt.x < x && (x < cur.x || z(x - cur.x)))
			HPI.push_back(intersection(L(nxt, cur), { { 1, 0 }, x }));
	}
	return A(HPI);
}
ld bi_search() {
	ld s = min_x, e = max_x, m = min_x;
	//int t = 50;
	//while (t--) {
	while (e - s > 1e-3) {
		m = (s + e) * .5;
		ld tmp = f(m);
		//std::cout << "m x: " << m << " area: " << tmp << "\n";
		if (tmp > area) e = m;
		else s = m;
	}
	return m;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	pos.resize(N);
	for (int i = 0; i < N; i++) std::cin >> pos[i].x >> pos[i].y;
	monotone_chain(pos, hull);
	area = A(hull);
	area *= .5;
	//std::cout << "min x: " << min_x << " max x: " << max_x << " area: " << area << " DEBUG\n";
	X = bi_search();
	std::cout << X << "\n";
}
int main() { solve(); return 0; }

//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <deque>
//#include <cmath>
//typedef long long ll;
//typedef double ld;
//const int LEN = 500'000;
//const ld TOL = 1e-8;
//int N;
//ld X, min_x, max_x, area;
//bool F;
//
//bool z(ld x) { return std::abs(x) < TOL; }
//struct Pos {
//	ld x, y;
//	bool operator < (const Pos& p) const {
//		return x == p.x ? y < p.y : x < p.x;
//	}
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator * (const ld& s) const { return { x * s, y * s }; }
//};
//std::vector<Pos> pos, hull, HPI;
//struct Vec {
//	ld vy, vx;
//	bool operator < (const Vec& v) const {
//		return z(vy - v.vy) ? vx < v.vx : vy < v.vy;
//	}
//	bool operator == (const Vec& v) const {
//		return (z(vy - v.vy) && z(vx - v.vx));
//	}
//	ld operator / (const Vec& v) const {
//		return vy * v.vx - vx * v.vy;  // cross
//	}
//	Vec operator ~ () const { return { -vx, vy }; }  //rotate PI/2
//};
//const Vec Zero = { 0, 0 };
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
//	ld operator / (const Line& l) const { return s / l.s; }  //cross
//	Line operator ~ () const { return { ~s, c }; }  //rotate PI/2
//};
//std::vector<Line> border, HP;
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//void monotone_chain(std::vector<Pos>& C, std::vector<Pos>& H) {
//	std::sort(C.begin(), C.end());
//	min_x = C[0].x, max_x = C[C.size() - 1].x;
//	if (C.size() <= 2) {
//		for (const Pos& p : C) H.push_back(p);
//		return;
//	}
//	for (int i = 0; i < C.size(); i++) {
//		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
//			H.pop_back();
//		}
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	int s = H.size() + 1;
//	for (int i = C.size() - 1; i >= 0; i--) {
//		while (H.size() > s && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
//			H.pop_back();
//		}
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	return;
//}
//ld A(std::vector<Pos>& H) {
//	Pos P = { 0, 0 };
//	ld a = 0;
//	int h = H.size();
//	for (int i = 0; i < h; i++) {
//		Pos cur = H[i], nxt = H[(i + 1) % h];
//		a += cross(P, cur, nxt);
//	}
//	return a / 2;
//}
//Line L(const Pos& d1, const Pos& d2) {
//	ld vy = d2.y - d1.y;
//	ld vx = d1.x - d2.x;
//	ld c = vy * d1.x + vx * d1.y;
//	return { { vy, vx }, c };
//}
//Pos intersection(const Line& l1, const Line& l2) {
//	Vec v1 = l1.s, v2 = l2.s;
//	ld det = v1 / v2;
//	return {
//		(l1.c * v2.vx - l2.c * v1.vx) / det,
//		(l2.c * v1.vy - l1.c * v2.vy) / det
//	};
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
//		Pos p = intersection(cur, nxt);
//		hull.push_back(p);
//	}
//	return 1;
//}
//ld f(ld x) {
//	HP.clear(); HPI.clear();
//	for (const Line& l : border) HP.push_back(l);
//	HP.push_back({ { 1, 0 }, x });
//	if (!half_plane_intersection(HP, HPI)) return 0;
//	return A(HPI);
//}
//ld bi_search() {
//	ld s = min_x, e = max_x, m;
//	int t = 50;
//	while (t--) {
//		m = (s + e) * .5;
//		ld tmp = f(m);
//		//std::cout << "m x: " << m << " area: " << tmp << "\n";
//		if (tmp > area) e = m;
//		else s = m;
//	}
//	return m;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(8);
//	std::cin >> N;
//	pos.resize(N);
//	for (int i = 0; i < N; i++) std::cin >> pos[i].x >> pos[i].y;
//	monotone_chain(pos, hull);
//	area = A(hull);
//	area *= .5;
//	//std::cout << "min x: " << min_x << " max x: " << max_x << " area: " << area << " DEBUG\n";
//	int sz = hull.size();
//	for (int i = 0; i < sz; i++) {
//		Pos cur = hull[i], nxt = hull[(i + 1) % sz];
//		border.push_back(L(cur, nxt));
//	}
//	X = bi_search();
//	std::cout << X << "\n";
//}
//int main() { solve(); return 0; }