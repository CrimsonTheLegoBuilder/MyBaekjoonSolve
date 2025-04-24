#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef double ld;
const int LEN = 200'000;
const ld TOL = 1e-9;
int N;
ld D;

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos { ld x, y; };
const Pos Pivot = { 0, 0 };
ld dist_sq(const Pos& d1) {
	return d1.x * d1.x + d1.y * d1.y;
}
bool outside(std::vector<Pos>& H) {
	for (const Pos& p : H) {
		if (dist_sq(p) > D * D - TOL) return 1;
	}
	return 0;
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
void norm(Pos& d1, Pos& d2) {
	if (cross(Pivot, d2, d1) > 0) std::swap(d1, d2);
}
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
	ld operator / (const Line& l) const { return s / l.s; }
} crossline[LEN];
struct Seg {
	Pos s, e;
};
Line L(const Seg& seg, int i) {
	ld dy, dx, c;
	dy = seg.e.y - seg.s.y;
	dx = seg.s.x - seg.e.x;
	c = dy * seg.s.x + dx * seg.s.y;
	return { {dy, dx} , c, i };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det
	};
}
bool CW(const Line& l1, const Line& l2, const Line& target) {
	if (l1.s / l2.s < TOL) return 0;
	Pos p = intersection(l1, l2);
	return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
}
bool half_plane_intersection(std::vector<Line>& HP, int m, std::vector<Pos>& hull) {
	std::deque<Line> dq;
	//std::sort(HP.begin(), HP.end());
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
bool f(int m) {
	std::vector<Line> HP;
	for (int i = 0; i < N; i++) {
		if (crossline[i].i <= m) HP.push_back(crossline[i]);
	}
	std::vector<Pos> hull;
	if (!half_plane_intersection(HP, m, hull)) return 1;
	return outside(hull);
}
int bi_search() {
	if (f(N - 1)) return -1;
	int s = 0, e = N - 1, m;
	while (s < e) {
		m = s + e >> 1;
		if (f(m)) s = m + 1;
		else e = m;
	}
	return s + 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> D;
	for (int i = 0; i < N; i++) {
		Seg line;
		std::cin >> line.s.x >> line.s.y >> line.e.x >> line.e.y;
		norm(line.s, line.e);
		crossline[i] = L(line, i);
	}
	std::sort(crossline, crossline + N);
	int day = bi_search();
	if (day == -1) std::cout << "*\n";
	else std::cout << day << "\n";
	return;
}
int main() { solve(); return 0; }


//if (s == N - 1 && f(N - 1)) return s + 2; 
//if (day == N + 1) std::cout << "*\n";
//int bi_search() {
//	int s = 0, e = N - 1, m, ret = 0;
//	while (s <= e) {
//		m = s + e >> 1;
//		if (f(m)) {
//			ret = std::max(m, ret);
//			s = m + 1;
//		}
//		else e = m - 1;
//	}
//	if (s == N - 1 && f(N - 1)) return s + 2;
//	return s + 1;
//}


//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <deque>
//#include <cmath>
//
//typedef long long ll;
//typedef double ld;
//
//const int LEN = 200'000;
//const ld TOL = 1e-8;
//
//int N;
//ld D;
//
//bool z(ld x) { return std::fabs(x) < TOL; }
//
//struct Pos { ld x, y; };
//const Pos Pivot = { 0, 0 };
//
//ld dist_squared(const Pos& d1, const Pos& d2) {
//    ld dx = d1.x - d2.x;
//    ld dy = d1.y - d2.y;
//    return dx * dx + dy * dy;
//}
//
//ld max_dist_squared(std::vector<Pos>& H) {
//    ld ret = 0;
//    for (const Pos& p : H) {
//        ret = std::max(ret, dist_squared(Pivot, p));
//    }
//    return ret;
//}
//
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//    return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//
//void norm(Pos& d1, Pos& d2) {
//    if (cross(Pivot, d2, d1) > 0) std::swap(d1, d2);
//}
//
//struct Line {
//    ld vy, vx, c;
//    bool operator < (const Line& l) const {
//        bool f1 = z(vy) ? vx > 0 : vy > 0;
//        bool f2 = z(l.vy) ? l.vx > 0 : l.vy > 0;
//        if (f1 != f2) return f1 > f2;
//        ld ccw = vy * l.vx - vx * l.vy;
//        return z(ccw) ? c * hypot(l.vx, l.vy) < l.c * hypot(vx, vy) : ccw > 0;
//    }
//    ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; }
//} HP[LEN];
//
//std::vector<Line> crossline(LEN);
//
//Pos IP(const Line& l1, const Line& l2) {
//    ld det = l1 / l2;
//    return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
//}
//
//bool CW(const Line& l1, const Line& l2, const Line& l) {
//    if (l1 / l2 <= 0) return false;
//    Pos p = IP(l1, l2);
//    return l.vy * p.x + l.vx * p.y >= l.c;
//}
//
////bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& INX) {
//bool half_plane_intersection(Line HP[], int m, std::vector<Pos>& INX) {
//    std::deque<Line> D;
//    //std::sort(HP.begin(), HP.end());
//    std::sort(HP, HP + m + 1);
//    //for (const Line& l : HP) {
//    for (int i = 0; i <= m; i++) {
//        Line l = HP[i];
//        if (!D.empty() && z(D.back() / l)) continue;
//        while (D.size() >= 2 && CW(D[D.size() - 2], D.back(), l)) D.pop_back();
//        while (D.size() >= 2 && CW(l, D[0], D[1])) D.pop_front();
//        D.push_back(l);
//    }
//    while (D.size() > 2 && CW(D[D.size() - 2], D.back(), D[0])) D.pop_back();
//    while (D.size() > 2 && CW(D.back(), D[0], D[1])) D.pop_front();
//    //std::vector<Pos> h;
//    //h.resize(m + 1);
//    INX.resize(m + 1);
//    for (int i = 0; i < D.size(); i++) {
//        Line cur = D[i], nxt = D[(i + 1) % D.size()];
//        if (cur / nxt <= TOL) {
//            INX.clear();
//            return false;
//        }
//        INX.push_back(IP(cur, nxt));
//    }
//    return true;
//}
//
//struct Seg {
//    Pos s, e;
//};
//
//Line L(const Seg& seg) {
//    ld dy = seg.e.y - seg.s.y;
//    ld dx = seg.s.x - seg.e.x;
//    return { dy, dx, dy * seg.s.x + dx * seg.s.y };
//}
//
//bool f(int m) {
//    for (int i = 0; i <= m; i++) HP[i] = crossline[i];
//    //HP(crossline.begin(), crossline.begin() + m + 1);
//    std::vector<Pos> hull;
//    if (!half_plane_intersection(HP, m, hull)) return true;
//    ld ret_squared = max_dist_squared(hull);
//    return ret_squared > D * D - TOL;
//}
//
//int bi_search() {
//    int s = 0, e = N - 1, m, ret = 0;
//    while (s <= e) {
//        m = s + e >> 1;
//        if (f(m)) {
//            ret = std::max(m, ret);
//            s = m + 1;
//        }
//        else e = m - 1;
//    }
//    if (s == N - 1 && f(N - 1)) return s + 2;
//    return s + 1;
//}
//
//void solve() {
//    std::ios_base::sync_with_stdio(false);
//    std::cin.tie(nullptr);
//
//    std::cin >> N >> D;
//    for (int i = 0; i < N; i++) {
//        Seg line;
//        std::cin >> line.s.x >> line.s.y >> line.e.x >> line.e.y;
//        norm(line.s, line.e);
//        crossline[i] = L(line);
//    }
//    int day = bi_search();
//    if (day == N + 1) std::cout << "*\n";
//    else std::cout << day << "\n";
//    return;
//}
//
//int main() {
//    solve();
//    return 0;
//}



//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <deque>
//#include <cmath>
//typedef long long ll;
//typedef double ld;
//const int LEN = 200'000;
//const ld TOL = 1e-8;
//int N;
//ld D;
//
//bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
//struct Pos { ld x, y; };
//const Pos Pivot = { 0, 0 };
//ld dist(const Pos& d1, const Pos& d2) {
//	//return hypot(d1.x - d2.x, d1.y - d2.y);
//	return (d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y);
//}
//ld dist(const Pos& d1) {
//	return d1.x * d1.x + d1.y * d1.y;
//}
//ld max_dist(std::vector<Pos>& H) {
//	ld ret = 0;
//	for (const Pos& p : H) {
//		ret = std::max(ret, dist(p));
//	}
//	return ret;
//}
//bool outside(std::vector<Pos>& H) {
//	for (const Pos& p : H) {
//		if (dist(p) > D * D - TOL) return 1;
//	}
//	return 0;
//}
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//void norm(Pos& d1, Pos& d2) {
//	if (cross(Pivot, d2, d1) > 0) std::swap(d1, d2);
//}
//struct Line {
//	ld vy, vx, c;  // a(vy) * x + b(-vx) * y - c == 0;
//	bool operator < (const Line& l) const {
//		bool f1 = z(vy) ? vx > 0 : vy > 0;
//		bool f2 = z(l.vy) ? l.vx > 0 : l.vy > 0;  // sort CCW
//		if (f1 != f2) return f1 > f2;
//		ld ccw = vy * l.vx - vx * l.vy;  // ccw == 0 : parallel
//		return z(ccw) ? c * hypot(l.vx, l.vy) < l.c * hypot(vx, vy) : ccw > 0;  // sort by distance
//	}
//	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; }  //cross
//}crossline[LEN];
//Pos IP(const Line& l1, const Line& l2) {
//	ld det = l1 / l2;	//ld det = l1.vy * l2.vx - l1.vx * l2.vy;
//	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
//}
//bool CW(const Line& l1, const Line& l2, const Line& l) {
//	if (l1 / l2 <= 0) return 0;
//	Pos p = IP(l1, l2);
//	return l.vy * p.x + l.vx * p.y >= l.c;
//}
//bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& INX) {
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
//		if (cur / nxt <= TOL) return 0;
//		h.push_back(IP(cur, nxt));
//	}
//	INX = h;
//	return 1;
//}
//struct Seg {
//	Pos s, e;
//};
//Line L(const Seg& seg) {
//	ld dy, dx, c;
//	dy = seg.e.y - seg.s.y;
//	dx = seg.s.x - seg.e.x;
//	c = dy * seg.s.x + dx * seg.s.y;
//	return { dy, dx, c };
//}
//bool f(int m) {
//	std::vector<Line> HP;
//	for (int i = 0; i <= m; i++) {
//		HP.push_back(crossline[i]);
//	}
//	std::vector<Pos> hull;
//	if (!half_plane_intersection(HP, hull)) return 1;
//	//ld ret = max_dist(hull);
//	//return ret > D * D - TOL;
//	return outside(hull);
//}
//int bi_search() {
//	int s = 0, e = N - 1, m, ret = 0;
//	while (s <= e) {
//		m = s + e >> 1;
//		if (f(m)) {
//			ret = std::max(m, ret);
//			s = m + 1;
//		}
//		else e = m - 1;
//	}
//	if (s == N - 1 && f(N - 1)) return s + 2;
//	return s + 1;
//}
//void solve() {
//	std::cin >> N >> D;
//	for (int i = 0; i < N; i++) {
//		Seg line;
//		std::cin >> line.s.x >> line.s.y >> line.e.x >> line.e.y;
//		norm(line.s, line.e);
//		crossline[i] = L(line);
//	}
//	int day = bi_search();
//	if (day == N + 1) std::cout << "*\n";
//	else std::cout << day << "\n";
//	return;
//}
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	solve();
//	return 0;
//}


/*
4 5.00000001
-3 4 3 4
-3 4 -3 -4
-3 -4 3 -4
3 -4 3 4
*/
