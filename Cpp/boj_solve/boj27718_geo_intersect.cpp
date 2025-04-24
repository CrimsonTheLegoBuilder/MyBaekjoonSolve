#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 2'001;
int N;// , T[LEN][LEN]{ 0 };

struct Pos { 
	ll x, y;
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
};
struct Seg { Pos l, r; } segs[LEN];
bool parallel(const Seg& s1, const Seg& s2) {
	ll ccw = (s1.r.x - s1.l.x) * (s2.r.y - s2.l.y) - (s1.r.y - s1.l.y) * (s2.r.x - s2.l.x);
	return !ccw;
	//return ccw > 0 ? 1 : ccw < 0 ? -1 : 0;
}
int CCW(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ccw = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return ccw > 0 ? 1 : ccw < 0 ? -1 : 0;
}
int between(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll dot = (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
	return dot > 0 ? 1 : dot < 0 ? -1 : 0;
}
int intersect(const Seg& s1, const Seg& s2) {
	bool F0 = (
		(!CCW(s1.l, s1.r, s2.l) && between(s1.l, s2.l, s1.r) >= 0) ||
		(!CCW(s1.l, s1.r, s2.r) && between(s1.l, s2.r, s1.r) >= 0) ||
		(!CCW(s2.l, s2.r, s1.l) && between(s2.l, s1.l, s2.r) >= 0) ||
		(!CCW(s2.l, s2.r, s1.r) && between(s2.l, s1.r, s2.r) >= 0)
		);
	bool F1 = (
		CCW(s1.l, s1.r, s2.l) * CCW(s1.r, s1.l, s2.r) > 0 &&
		CCW(s2.l, s2.r, s1.l) * CCW(s2.r, s2.l, s1.r) > 0
		);
	if (!F0 && !F1) return 0;  // not cross
	if (F1) return 2;  // cross but 4 points != intersection
	bool F2 = (
		(!CCW(s1.l, s1.r, s2.l) && between(s1.l, s2.l, s1.r) > 0) ||
		(!CCW(s1.l, s1.r, s2.r) && between(s1.l, s2.r, s1.r) > 0) ||
		(!CCW(s2.l, s2.r, s1.l) && between(s2.l, s1.l, s2.r) > 0) ||
		(!CCW(s2.l, s2.r, s1.r) && between(s2.l, s1.r, s2.r) > 0)
		);
	bool F3 = parallel(s1, s2);
	bool F4 = (s1.l == s2.l && s1.r == s2.r) || (s1.l == s2.r && s1.r == s2.l);
	if ((F2 && F3) || F4) return 3;  // parallel and infinite intersections
	return 1;  // one or two points of segments is intersection
	//bool F5 = s1.l == s2.l || s1.r == s2.r || s1.l == s2.r || s1.r == s2.l;  one point of segments is intersection
	//if ((!F3 && F2) || F5) return 1;
}
void pos_init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> segs[i].l.x >> segs[i].l.y >> segs[i].r.x >> segs[i].r.y;
	}
	return;
}
void brute() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) std::cout << intersect(segs[i], segs[j]);
		std::cout << "\n";
	}
	return;
}
//void brute() {
//	for (int i = 0; i < N; i++) {
//		for (int j = i; j < N; j++) {
//			int condition = intersect(segs[i], segs[j]);
//			T[i][j] = T[j][i] = condition;
//		}
//	}
//	return;
//}
//void print() {
//	for (int i = 0; i < N; i++) {
//		for (int j = 0; j < N; j++) std::cout << T[i][j];
//		std::cout << "\n";
//	}
//}
//int main() { pos_init(); brute(); print(); return 0; }
int main() { pos_init(); brute(); return 0; }

//bool intersect(const Pos& d1, const Pos& d2, const Seg& s) {
//	bool F1 = CCW(d1, d2, s.l) * CCW(d2, d1, s.r) > 0 && CCW(s.l, s.r, d1) * CCW(s.r, s.l, d2) > 0;
//	return F1;
//}
//bool intersect(const Pos& d1, const Pos& d2, const Seg& s) {
//	bool F1 = CCW(d1, d2, s.l) * CCW(d2, d1, s.r) > 0 && CCW(s.l, s.r, d1) * CCW(s.r, s.l, d2) > 0;
//	//bool F2 = (
//	//	(!CCW(d1, d2, s.l) && between(d1, s.l, d2) >= 0) ||
//	//	(!CCW(d1, d2, s.r) && between(d1, s.r, d2) >= 0) ||
//	//	(!CCW(s.l, s.r, d1) && between(s.l, d1, s.r) >= 0) ||
//	//	(!CCW(s.l, s.r, d2) && between(s.l, d2, s.r) >= 0)
//	//	);
//	return F1;// || F2;
//}

/*
20
0 0 9 9
1 1 2 2
2 2 3 3
3 3 4 4
4 4 5 5
5 5 6 6
6 6 7 7
7 7 8 8
8 8 9 9
0 0 9 9
1 2 2 1
2 3 3 2
3 4 4 3
4 5 5 4
5 6 6 5
6 7 7 6
7 8 8 7
8 9 9 8
0 0 0 9
0 0 9 0

6
0 0 1 0
1 0 1 1
1 1 0 1
0 1 0 0
0 0 1 1
1 0 0 1
*/