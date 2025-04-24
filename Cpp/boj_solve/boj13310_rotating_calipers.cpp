#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
const ll MAX = 1e16;

struct Pos {
	ll x, y, vx, vy;
	Pos(ll X = 0, ll Y = 0, ll VX = 0, ll VY = 0) : x(X), y(Y), vx(VX), vy(VY) {}
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
	Pos move(const ll& d) const { return { x + d * vx, y + d * vy }; }
	ll Euc() const { return x * x + y * y; }
	//ld mag() const { return hypot(x, y); }
};
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
std::vector<Pos> move(const std::vector<Pos>& C, const int& d) {
	std::vector<Pos> MC;
	for (int i = 0; i < C.size(); i++) MC.push_back(C[i].move(d));
	return MC;
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) 
			H.pop_back();
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
Pos V(const std::vector<Pos>& H, int i) {
	int f = (i + 1) % H.size();
	i %= H.size();
	return { H[f].x - H[i].x, H[f].y - H[i].y };
}
ll ccw(const std::vector<Pos>& H, const int& i, const int& f) {
	int sz = H.size();
	return (H[(i + 1) % sz] - H[i]) / (H[(f + 1) % sz] - H[f]);
}
ll rotating_calipers(const std::vector<Pos>& H) {
	ll MD = 0;
	int f = 0, l = H.size();
	for (int i = 0; i < l; i++) {
		while (ccw(H, i, f) > 0) {
			MD = std::max(MD, (H[i] - H[f]).Euc());
			f = (f + 1) % l;
		}
		MD = std::max(MD, (H[i] - H[f]).Euc());
	}
	return MD;
}
Pos ternary_search(const std::vector<Pos>& C, const int& X) {
	int s = 0, e = X;
	int l, r;
	std::vector<Pos> SL, SR, HL, HR;
	ll DL, DR;
	while (e - s >= 3) {
		l = (s * 2 + e) / 3;
		r = (s + e * 2) / 3;
		SL = move(C, l); HL = monotone_chain(SL); DL = rotating_calipers(HL);
		SR = move(C, r); HR = monotone_chain(SR); DR = rotating_calipers(HR);
		if (DL > DR) s = l;
		else e = r;
	}
	int mind = 0;
	ll MIN = MAX, MD;
	std::vector<Pos> S, H;
	for (int i = s; i <= e; i++) {
		S = move(C, i); H = monotone_chain(S); MD = rotating_calipers(H);
		if (MIN > MD) {
			MIN = MD; mind = i;
		}
	}
	std::cout << mind << "\n" << MIN << "\n";
	return { mind, MIN };
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int N, T;
	//Pos min_day;
	std::vector<Pos> C;
	std::cin >> N >> T;
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y >> C[i].vx >> C[i].vy;
	//min_day = ternary_search(C, T);
	//std::cout << min_day.x << "\n" << min_day.y << "\n";
	ternary_search(C, T);
	return;
}
int main() { solve(); return 0; }//boj13310
