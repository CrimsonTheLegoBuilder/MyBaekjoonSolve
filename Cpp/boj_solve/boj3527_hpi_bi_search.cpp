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
bool zero(const ld& x) { return std::abs(x) < TOL; }

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} pos[LEN]; const Pos O = { 0, 0 };
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
	ld mag() const { return hypot(vy, vx); }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
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
	ld operator * (const Line& l) const { return s * l.s; }
	ld operator / (const Line& l) const { return s / l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
};
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos> hull) {
	auto cw = [&](const Line& l1, const Line& l2, const Line& target) -> bool {
		if (l1.s / l2.s < TOL) return 0;
		Pos p = intersection(l1, l2);
		return target.above(p) > -TOL;
		};
	std::deque<Line> dq;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
		if (!dq.empty() && zero(dq.back() / l)) continue;
		while (dq.size() >= 2 && cw(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
		while (dq.size() >= 2 && cw(l, dq.front(), dq[1])) dq.pop_front();
		dq.push_back(l);
	}
	while (dq.size() >= 3 && cw(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
	while (dq.size() >= 3 && cw(dq.back(), dq.front(), dq[1])) dq.pop_front();
	for (int i = 0; i < dq.size(); i++) {
		Line cur = dq[i], nxt = dq[(i + 1) % (int)dq.size()];
		if (cur / nxt < TOL) {
			hull.clear();
			return 0;
		}
		hull.push_back(intersection(cur, nxt));
	}
	return 1;
}
int bi_search(Pos p[], int N) {
	int s = 1, e = N - 1, m;
	while (s < e) {
		m = s + e >> 1;
		std::vector<Line> HP;
		for (int i = 0; i < N; i++) HP.push_back(L(p[i], p[(i + m) % N]));
		if (half_plane_intersection(HP, {})) s = m + 1;
		else e = m;
	}
	return s - 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = N - 1; i >= 0; i--) std::cin >> pos[i];
	if (N <= 4) std::cout << "1\n";
	else std::cout << bi_search(pos, N) << "\n";
	return ;
}
int main() { solve(); return 0; }//boj3527 Jungle Outpost

//bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
//struct Pos { ld x, y; }pos[LEN];
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
//};
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//ld A(std::vector<Pos>& H) {
//	Pos O = { 0, 0 };
//	int l = H.size();
//	ld a = 0;
//	for (int i = 0; i < l; i++) {
//		a += cross(O, H[i], H[(i + 1) % l]);
//	}
//	return a / 2;
//}
//Pos IP(const Line& l1, const Line& l2) {
//	ld det = l1 / l2;	//ld det = l1.vy * l2.vx - l1.vx * l2.vy;
//	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
//}
//bool CW(const Line& l1, const Line& l2, const Line& l) {
//	if (l1 / l2 <= 0) return 0;
//	Pos p = IP(l1, l2);
//	return l.vy * p.x + l.vx * p.y >= l.c;
//}
//bool HPI(std::vector<Line>& HP, std::vector<Pos>& INX) {
//	std::deque<Line> D;
//	//std::sort(HP.begin(), HP.end());
//	for (const Line& l : HP) {
//		if (!D.empty() && z(D.back() / l)) continue;
//		while (D.size() >= 2 && CW(D[D.size() - 2], D.back(), l)) D.pop_back();
//		while (D.size() >= 2 && CW(l, D[0], D[1])) D.pop_front();
//		D.push_back(l);
//	}
//	while (D.size() > 2 && CW(D[D.size() - 2], D.back(), D[0])) D.pop_back();
//	while (D.size() > 2 && CW(D.back(), D[0], D[1])) D.pop_front();
//	//if (D.size() < 3) return 0;
//	std::vector<Pos> h;
//	for (int i = 0; i < D.size(); i++) {
//		Line cur = D[i], nxt = D[(i + 1) % D.size()];
//		if (cur / nxt <= TOL) return 0;
//		h.push_back(IP(cur, nxt));
//	}
//	INX = h;
//	return 1;
//}
//bool AREA(Pos p[], int m, int N) {  //area
//	std::vector<Line> HP;
//	for (int i = 0; i < N; i++) {
//		ld dy = p[(i + m) % N].y - p[i].y;
//		ld dx = p[i].x - p[(i + m) % N].x;  // -(p[i + 1].x - p[i].x)
//		ld c = dy * p[i].x + dx * p[i].y;// -hypot(dy, dx) * m;
//		HP.push_back({ dy, dx, c });
//	}
//	std::vector<Pos> INX;
//	return HPI(HP, INX);
//	//if (!HPI(HP, INX)) return 0;
//	//ld area = A(INX);
//	//return !z(area);
//}
//int bi_search(Pos p[], int N) {
//	int s = 1, e = N - 1, m;
//	while (s < e) {
//		m = s + e >> 1;
//		if (AREA(p, m, N)) s = m + 1;
//		else e = m;
//	}
//	return s - 1;
//}
//
//
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	for (int i = N - 1; i >= 0; i--) { std::cin >> pos[i].x >> pos[i].y; }
//	if (N <= 4) {
//		std::cout << "1\n";
//		return 0;
//	}
//	std::cout << bi_search(pos, N) << "\n";
//	return 0;
//}
//#include <string>
//int solve() {
//	std::cin >> N;
//	if (N < 5) return 1;
//	for (int i = N - 1; i >= 0; --i) std::cin >> pos[i].x >> pos[i].y;
//	return bi_search(pos, N);
//}
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	// freopen("jungleans", "w", stdout);
//	for (int i = 1; i < 81; i++) {
//		std::string num;
//		if (i < 10) num = "0" + std::to_string(i);
//		else num = std::to_string(i);
//		std::string filename = "jungleoutpost/" + num;
//		char chfilename[50];
//		strcpy(chfilename, filename.c_str());
//		//std::cout << chfilename << "\n";
//		freopen(chfilename, "r", stdin);
//		int ret = solve();
//		std::string answerfile = "jungleoutpost/" + num + ".a";
//		strcpy(chfilename, answerfile.c_str());
//		freopen(chfilename, "r", stdin);
//		int ans;
//		std::cin >> ans;
//		printf("#%d %d %d %d\n", i, ret, ans, ret == ans);
//	}
//	return 0;
//}

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



//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <deque>
//#include <cmath>
//typedef long long ll;
//typedef double ld;
//const int LEN = 50'000;
//const ld TOL = 1e-9;
//int N;
//
//bool z(ld x) { return std::abs(x) < TOL; }
//struct Pos { ld x, y; } pos[LEN];
//struct Vec {
//	ld vy, vx;
//	bool operator < (const Vec& v) const {
//		return z(vy - v.vy) ? vx < v.vx : vy < v.vy;
//	}
//	bool operator == (const Vec& v) const {
//		return (z(vy - v.vy) && z(vx - v.vx));
//	}
//	ld operator / (const Vec& v) const {
//		return vy * v.vx - vx * v.vy;
//	}
//};
//const Vec Zero = { 0, 0 };
//const Pos Pivot = { 0, 0 };
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
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//ld area(const std::vector<Pos>& H) {
//	int l = H.size();
//	ld a = 0;
//	for (int i = 0; i < l; i++) {
//		a += cross(Pivot, H[i], H[(i + 1) % l]);
//	}
//	return a / 2;
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
//		hull.push_back(intersection(cur, nxt));
//	}
//	return 1;
//}
//bool f(int m) {
//	std::vector<Line> HP;
//	ld dy, dx, c;
//	for (int i = 0, j = m; i < N; i++, j = (j + 1) % N) {
//		dy = pos[j].y - pos[i].y;
//		dx = pos[i].x - pos[j].x;
//		c = dy * pos[i].x + dx * pos[i].y;
//		HP.push_back({ {dy, dx}, c });
//	}
//	std::vector<Pos> hull;
//	//if (!half_plane_intersection(HP, hull)) return 0;
//	//ld a = area(hull);
//	//return !z(a);
//	return half_plane_intersection(HP, hull);
//}
//int bi_search() {
//	int s = 1, e = N - 1, m;
//	while (s < e) {
//		m = s + e >> 1;
//		if (f(m)) s = m + 1;
//		else e = m;
//	}
//	return s;
//}
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	for (int i = N - 1; i >= 0; i--) { std::cin >> pos[i].x >> pos[i].y; }
//	if (N <= 4) {
//		std::cout << "1\n";
//		return 0;
//	}
//	std::cout << bi_search() - 1 << "\n";
//	return 0;
//}