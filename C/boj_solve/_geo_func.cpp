#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;
const ll MAX = 10'000'000'000'000'000;
struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const {
		if (x == p.x) return (y < p.y);
		return x < p.x;
	}
};
struct Star { ll x, y, vx, vy; };
int N, T, x, y, vx, vy;
std::vector<Star> stars;
Pos min_day;

ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2.x - d1.x) * (d4.x - d3.x) + (d2.y - d1.y) * (d4.y - d3.y); }
bool is_cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	bool f11 = cross(d1, d2, d2, d3) * cross(d2, d1, d1, d4) > 0;
	bool f12 = cross(d3, d4, d4, d1) * cross(d4, d3, d3, d2) > 0;
	bool f1 = f11 && f12;
	bool f2 = (!cross(d1, d3, d3, d2) && dot(d1, d3, d3, d2) >= 0) ||
		(!cross(d1, d4, d4, d2) && dot(d1, d4, d4, d2) >= 0) ||
		(!cross(d3, d1, d1, d4) && dot(d3, d1, d1, d4) >= 0) ||
		(!cross(d3, d2, d2, d4) && dot(d3, d2, d2, d4) >= 0);
	return f1 || f2;
}
ll cal_dist_sq(const Pos& d1, const Pos& d2) { return (d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y); }
std::vector<Pos> pos_at_N(std::vector<Star>& P, int D) {
	std::vector<Pos> S;
	for (int i = 0; i < N; i++) { S.push_back({ P[i].x + P[i].vx * D, P[i].y + P[i].vy * D }); }
	return S;
}
bool NCVXHIN(std::vector<Pos>& H, Pos& d) {
	int cnt = 0, L = H.size();
	for (int i = 0; i < L; i++) {
		Pos d1 = H[i], d2 = H[(i + 1) % L];
		if (d1.y > d2.y) std::swap(d1, d2);
		if (!cross(d1, d, d, d2) && dot(d1, d, d, d2) >= 0) return 1;
		if (d.x > std::max(d1.x, d2.x)) continue;
		if (d.y <= d1.y) continue;
		if (d.y > d2.y) continue;
		if (d.y == d1.y && d.y == d2.y) continue;
		if (cross(d1, d2, d2, d) < 0) continue;
		if (d1.y < d.y && d.y <= d2.y) cnt++;
		else if (cross(d1, d2, d2, d) > 0) cnt++;
	}
	return cnt % 2;
}
ll get_area(std::vector<Pos>& H) {
	ll A = 0, L = H.size();
	Pos pivot = { 0, 0 };
	for (int i = 0; i < L; i++) {
		A += cross(pivot, H[i], H[i], H[(i + 1) / L]);
	}
	return A;
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
ll rotating_calipers(std::vector<Pos>& H) {
	ll MD = 0;
	int i = 0, f2i = 1, l = H.size();
	for (i; i <= l; i++) {
		while ((f2i + 1) % l != i % l && cross(H[i % l], H[(i + 1) % l], H[f2i % l], H[(f2i + 1) % l]) > 0) {
			if (MD < cal_dist_sq(H[i % l], H[f2i % l])) MD = cal_dist_sq(H[i % l], H[f2i % l]);
			f2i++;
		}
		if (MD < cal_dist_sq(H[i % l], H[f2i % l])) MD = cal_dist_sq(H[i % l], H[f2i % l]);
	}
	return MD;
}
Pos ternary_search(std::vector<Star>& stars, int X) {
	if (X < 7) return { 0, X + 1 };
	int s = 0, e = X;
	int l, r;
	std::vector<Pos> SL, SR, HL, HR;
	ll L, R;
	while (e - s >= 3) {
		l = (s * 2 + e) / 3;
		r = (s + e * 2) / 3;
		SL = pos_at_N(stars, l);
		HL = monotone_chain(SL);
		L = rotating_calipers(HL);
		SR = pos_at_N(stars, r);
		HR = monotone_chain(SR);
		R = rotating_calipers(HR);
		if (L <= R) e = r;
		else s = l;
	}
	return { s, e + 1 };
}
Pos get_min(std::vector<Star>& stars, int X) {
	Pos days = ternary_search(stars, X);
	int mind = 0;
	ll MIN = MAX, MD;
	std::vector<Pos> S, H;
	for (int i = days.x; i < days.y; i++) {
		S = pos_at_N(stars, i);
		H = monotone_chain(S);
		MD = rotating_calipers(H);
		if (MIN > MD) {
			MIN = MD; mind = i;
		}
	}
	return { mind, MIN };
}
void input(int N) {
	for (int i = 0; i < N; i++) {
		std::cin >> x >> y >> vx >> vy;
		stars.push_back({ x, y, vx, vy });
	}
}



int main() {
	std::cin >> N >> T;
	input(N);
	min_day = get_min(stars, T);
	std::cout << min_day.x << "\n" << min_day.y << "\n";
	return 0;
}