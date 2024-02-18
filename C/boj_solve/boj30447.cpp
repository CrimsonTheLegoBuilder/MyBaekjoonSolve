#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-7;
const ld INF = 1e7;
int N;
ld w, h;

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
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
} P1, P2; const Pos O = { 0, 0 };
std::vector<Pos> box, CL, CR, HL, HR;
struct Vec {
	ld vy, vx;
	bool operator < (const Vec& v) const { return z(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (z(vy - v.vy) && z(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
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
Line rotate90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return { s, c };
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
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& p : C) H.push_back(p);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int sz = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > sz && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
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
ld area(std::vector<Pos>& H) {
	int sz = H.size();
	ld a{ 0 };
	for (int i = 0; i < sz; i++) a += cross(O, H[i], H[(i + 1) % sz]);
	return a * .5;
}
void mirror(std::vector<Pos>& C, const Pos& s1, const Pos& s2) {
	int sz = C.size();
	Line s = L(s1, s2);
	for (int i = 0; i < sz; i++) {
		Pos inx = intersection(s, rotate90(s, C[i]));
		Pos d = inx - C[i];
		C[i] += d * 2;
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> w >> h >> P1.x >> P1.y >> P2.x >> P2.y;

	ll A = w * h;
	box.resize(4);
	box[0] = Pos(0, 0);
	box[1] = Pos(w, 0);
	box[2] = Pos(w, h);
	box[3] = Pos(0, h);
	if (P2 < P1) std::swap(P1, P2);

	for (int i = 0; i < 4; i++) {
		if (ccw(P1, P2, box[i]) > 0) CL.push_back(box[i]);
		if (ccw(P1, P2, box[i]) < 0) CR.push_back(box[i]);
	}

	mirror(CR, P1, P2);
	CL.push_back(P1);
	CL.push_back(P2);
	CR.push_back(P1);
	CR.push_back(P2);
	HL = monotone_chain(CL);
	HR = monotone_chain(CR);

	std::vector<Line> HP;
	std::vector<Pos> HPI;
	int szl = HL.size(), szr = HR.size();
	ld overlap = 0;
	for (int i = 0; i < szl; i++) HP.push_back(L(HL[i], HL[(i + 1) % szl]));
	for (int i = 0; i < szr; i++) HP.push_back(L(HR[i], HR[(i + 1) % szr]));
	if (!half_plane_intersection(HP, HPI)) overlap = 0;
	else overlap = area(HPI);

	ll ans = A - overlap + TOL;
	std::cout << ans << "\n";
	return;
}
int main() { solve(); return 0; }//boj30447

	//std::cout << "HL\n";
	//for (int i = 0; i < szl; i++) std::cout << HL[i].x << " " << HL[i].y << "\n";
	//std::cout << "HR\n";
	//for (int i = 0; i < szr; i++) std::cout << HR[i].x << " " << HR[i].y << "\n";
	//std::cout << "A: " << A << " ans: " << ans << " P1: " << P1.x << " " << P1.y << " P2: " << P2.x << " " << P2.y << "\n";