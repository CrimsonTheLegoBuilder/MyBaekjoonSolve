#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
//const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
int T, N, M, Q;
ll prl, nprl, total;
bool zero(const ld& x) { return std::abs(x) < TOL; }
ll nC2(const ll& x) { return ((x - 1) * x) >> 1; }

struct Pos {
	ll x, y, ci, hi, cnt;
	Pos(ll X = 0, ll Y = 0, ll CI = 0, ll HI = 0, ll CNT = 0) 
		: x(X), y(Y), ci(CI), hi(HI), cnt(CNT) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p);
	friend std::ostream& operator << (std::ostream& os, const Pos& p);
} NH[LEN], MH[LEN], seq[LEN]; const Pos O = { 0, 0 };
std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y << "\n"; return os; }
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	//C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	return;
}
int ccw(const std::vector<Pos>& H, const int& i, const int& f) {
	int sz = H.size();
	int idx1 = i % sz, idx2 = f % sz;
	return (H[(idx1 + 1) % sz] - H[idx1]) / (H[(idx2 + 1) % sz] - H[idx2]);
}
void rotating_calipers() {
	while (T--) {
		std::cin >> N;
		C.resize(N);
		for (int i = N - 1; i >= 0; i--) {
			std::cin >> C[i];
			C[i].ci = i;
		}
		H = graham_scan(C);
		int sz = H.size();

		if (sz <= 2) {
			std::cout << nC2(N) << "\n";
			continue;
		}

		for (int i = 0; i < sz; i++) {
			H[i].hi = i;
			ll on_seg = H[(i + 1) % sz].ci - H[i].ci;
			H[i].cnt = (on_seg + N) % N + 1;
		}
		//for (int i = 0; i < sz; i++) std::cout << H[i];
		prl = 0; nprl = 0; total = 0;
		for (int i = 0, f = 1; i < sz; i++) {
			while (ccw(H, i, f) > 0) f = (f + 1) % sz;
			//std::cout << i << " " << f << " ccw: " << ccw(H, i, f) << "\n";
			if (!ccw(H, i, f)) {
				prl += H[i].cnt * H[f].cnt - 1;
				if (H[i].hi == H[(f + 2) % sz].hi) prl--;
				if (H[(i + 2) % sz].hi == H[f].hi) prl--;
			}
			else {
				nprl += H[i].cnt - 1;
				if (H[i].hi == H[(f + 1) % sz].hi) nprl--;
			}
			//std::cout << H[i].cnt << '\n';
			//std::cout << H[i].cnt * H[f].cnt - 1 << "\n";
			//std::cout << nC2(H[i].cnt) << '\n';
			if (ccw(H, i + 1, (i - 1 + sz) % sz) >= 0) total += nC2(H[i].cnt);
		}
		//std::cout << total << " " << nprl << " " << prl << "\n";
		total += nprl + (prl >> 1);
		std::cout << total << "\n\n";
	}
	return;
}
int main() { init(); rotating_calipers(); return 0; }//boj6090 casting



//ll area(std::vector<Pos>& H) {
//	ll ret = 0;
//	int sz = H.size();
//	for (int i = 0; i < sz; i++) {
//		Pos cur = H[i], nxt = H[(i + 1) % sz];
//		ret += cross(O, cur, nxt);
//	}
//	return ret;
//}
//void norm(std::vector<Pos>& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
//std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	std::sort(C.begin(), C.end());
//	if (C.size() <= 2) { for (const Pos& pos : C) H.push_back(pos); }
//	else {
//		for (int i = 0; i < C.size(); i++) {
//			while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//				H.pop_back();
//			H.push_back(C[i]);
//		}
//		H.pop_back();
//		int s = H.size() + 1;
//		for (int i = C.size() - 1; i >= 0; i--) {
//			while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//				H.pop_back();
//			H.push_back(C[i]);
//		}
//		H.pop_back();
//	}
//	return H;
//}