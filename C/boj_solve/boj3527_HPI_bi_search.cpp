#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef double ld;
const int LEN = 50'000;
const ld TOL = 1e-8;
int N;

bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
struct Pos { ld x, y; }pos[LEN];
struct Line {
	ld vy, vx, c;  // a(vy) * x + b(-vx) * y - c == 0;
	bool operator < (const Line& l) const {
		bool f1 = z(vy) ? vx > 0 : vy > 0;
		bool f2 = z(l.vy) ? l.vx > 0 : l.vy > 0;  // sort CCW
		if (f1 != f2) return f1 > f2;
		ld ccw = vy * l.vx - vx * l.vy;  // ccw == 0 : parallel
		return z(ccw) ? c * hypot(l.vx, l.vy) < l.c * hypot(vx, vy) : ccw > 0;  // sort by distance
	}
	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; }  //cross
};
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld A(std::vector<Pos>& H) {
	Pos O = { 0, 0 };
	int l = H.size();
	ld a = 0;
	for (int i = 0; i < l; i++) {
		a += cross(O, H[i], H[(i + 1) % l]);
	}
	return a / 2;
}
Pos IP(const Line& l1, const Line& l2) {
	ld det = l1 / l2;	//ld det = l1.vy * l2.vx - l1.vx * l2.vy;
	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
}
bool CW(const Line& l1, const Line& l2, const Line& l) {
	if (l1 / l2 <= 0) return 0;
	Pos p = IP(l1, l2);
	return l.vy * p.x + l.vx * p.y >= l.c;
}
bool HPI(std::vector<Line>& HP, std::vector<Pos>& INX) {
	std::deque<Line> D;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
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
bool AREA(Pos p[], int m, int N) {  //area
	std::vector<Line> HP;
	for (int i = 0; i < N; i++) {
		ld dy = p[(i + m) % N].y - p[i].y;
		ld dx = p[i].x - p[(i + m) % N].x;  // -(p[i + 1].x - p[i].x)
		ld c = dy * p[i].x + dx * p[i].y;// -hypot(dy, dx) * m;
		HP.push_back({ dy, dx, c });
	}
	std::vector<Pos> INX;
	if (!HPI(HP, INX)) return 0;
	ld area = A(INX);
	return !z(area);

}
int bi_serach(Pos p[], int N) {
	int s = 1, e = N - 1, m;
	while (s < e) {
		m = s + e >> 1;
		if (AREA(p, m, N)) s = m + 1;
		else e = m;
	}
	return s;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = N - 1; i >= 0; i--) { std::cin >> pos[i].x >> pos[i].y; }
	if (N <= 4) {
		std::cout << "1\n";
		return 0;
	}
	std::cout << bi_serach(pos, N) - 1 << "\n";
	return 0;
}


//#include <cstring>
//#include <string>
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	freopen("jungleans", "w", stdout);
//	for (int i = 1; i < 81; i++) {
//		std::string num;
//		if (i < 10) num = "0" + std::to_string(i);
//		else num = std::to_string(i);
//		std::string filename = "jungleoutpost/" + num;
//		char chfilename[25];
//		strcpy(chfilename, filename.c_str());
//		//std::cout << chfilename << "\n";
//		freopen(chfilename, "r", stdin);
//		std::cin >> N;
//		for (int i = N - 1; i >= 0; i--) { std::cin >> pos[i].x >> pos[i].y; }
//		//if (N <= 4) {
//		//	std::cout << "1\n";
//		//	continue;
//		//}
//		std::cout << bi_serach(pos, N) - 1 << "\n";
//	}
//	return 0;
//}


//std::cout << s << " " << e << " " << N << "\n";
//std::cout << std::fixed;
//std::cout.precision(3);
//for (int i = 0; i < N; i++) { std::cin >> pos[i].x >> pos[i].y; }
//std::reverse(pos, pos + N);
//ld dist(const Pos& d1, const Pos& d2) {
//	return hypot((d2.x - d1.x), (d2.y - d1.y));
//}
//Line L(std::vector<Pos>& H, int p1, int p2, int N) {
//	p2 %= N;
//	return { H[p2].x - H[p1].x, H[p2].y - H[p1].y, 0 };
//}
//Line L(std::vector<Pos>& H, int p1) {
//	int p2 = (p1 + 1) % H.size();
//	p1 %= H.size();
//	return { H[p2].x - H[p1].x, H[p2].y - H[p1].y, 0 };
//}
//ld rotating_calipers(std::vector<Pos>& H) {
//	int f = 0, l = H.size();
//	ld MD = 0;
//	for (int i = 0; i < l; i++) {
//		while (L(H, i) / L(H, f) > -TOL) {
//			MD = std::max(MD, dist(H[i], H[f]));
//			f = (f + 1) % l;
//		}
//		MD = std::max(MD, dist(H[i], H[f]));
//	}
//	return MD;
//}