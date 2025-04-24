#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const {
		if (x == p.x) return (y < p.y);
		return x < p.x;
	}
} P[100'000];

ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2.x - d1.x) * (d4.x - d3.x) + (d2.y - d1.y) * (d4.y - d3.y); }
void monotone_chain(std::vector<Pos>& candi, std::vector<Pos>& hull) {
	std::sort(candi.begin(), candi.end());
	if (candi.size() <= 2) {
		for (const Pos& pos : candi) hull.push_back(pos);
		return;
	}
	for (int i = 0; i < candi.size(); ++i) {
		while (hull.size() > 1 && cross(hull[hull.size() - 2], hull[hull.size() - 1], hull[hull.size() - 1], candi[i]) <= 0) {
			hull.pop_back();
		}
		hull.push_back(candi[i]);
	}
	hull.pop_back();
	int s = hull.size() + 1;
	for (int i = candi.size() - 1; i >= 0; --i) {
		while (hull.size() > s && cross(hull[hull.size() - 2], hull[hull.size() - 1], hull[hull.size() - 1], candi[i]) <= 0) {
			hull.pop_back();
		}
		hull.push_back(candi[i]);
	}
	hull.pop_back();
}
bool is_cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	bool f11 = cross(d1, d2, d2, d3) * cross(d2, d1, d1, d4) > 0;
	bool f12 = cross(d3, d4, d4, d1) * cross(d4, d3, d3, d2) > 0;
	bool f1 = f11 && f12;
	bool f2 = (cross(d1, d3, d3, d2) == 0 && dot(d1, d3, d3, d2) >= 0) ||
		(cross(d1, d4, d4, d2) == 0 && dot(d1, d4, d4, d2) >= 0) ||
		(cross(d3, d1, d1, d4) == 0 && dot(d3, d1, d1, d4) >= 0) ||
		(cross(d3, d2, d2, d4) == 0 && dot(d3, d2, d2, d4) >= 0);
	return f1 || f2;
}

int Y, X, K, y, x;
std::vector<Pos> cu, cd, hu, hd;
Pos s = { 2, 2 }, e, su, su2, sd, sd2, eu, eu2, ed, ed2;
int main() {
	std::cin >> Y >> X >> K;
	e = { 2 * X, 2 * Y };
	for (int i = 0; i < K; ++i) {
		std::cin >> y >> x;
		P[i].y = 2 * y; P[i].x = 2 * x;
	}
	su2 = { 0, 4 }; sd2 = { 4, 0 }; eu2 = { 2 * X - 2, 2 * Y + 2 }; ed2 = { 2 * X + 2, 2 * Y - 2 };
	for (int i = 0; i < K; i++) {
		Pos d = { P[i].x, P[i].y };
		if (!cross(s, e, e, d)) {
			std::cout << 0 << "\n";
			return 0;
		}
		else if (cross(s, e, e, d) > 0 && cross(su2, eu2, eu2, d) <= 0) {
			Pos du = { P[i].x + 1, P[i].y - 1 };
			cu.push_back(du);
		}
		else if (cross(s, e, e, d) < 0 && cross(sd2, ed2, ed2, d) >= 0) {
			Pos dd = { P[i].x - 1, P[i].y + 1 };
			cd.push_back(dd);
		}
	}
	su = { 1, 3 }; sd = { 3, 1 }; eu = { 2 * X - 1, 2 * Y + 1 }; ed = { 2 * X + 1, 2 * Y - 1 };
	cu.push_back(su); cu.push_back(eu);
	cd.push_back(sd); cd.push_back(ed);
	monotone_chain(cu, hu);
	monotone_chain(cd, hd);
	for (int i = 0; i < hu.size(); i++) {
		for (int j = 0; j < hd.size(); j++) {
			if (is_cross(hu[i], hu[(i + 1) % hu.size()], hd[j], hd[(j + 1) % hd.size()])) {
				std::cout << 0 << "\n"; return 0;
			}
		}
	}
	std::cout << 1 << "\n";
	return 0;
}


/*
#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const {
		if (x == p.x) return (y < p.y);
		return x < p.x;
	}
} P[100'000];

ll cross(const Pos &d1, const Pos &d2, const Pos &d3, const Pos &d4) { return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x); }
ll dot(const Pos &d1, const Pos &d2, const Pos &d3, const Pos &d4) { return (d2.x - d1.x) * (d4.x - d3.x) + (d2.y - d1.y) * (d4.y - d3.y); }
void monotone_chain(std::vector<Pos> &candi, std::vector<Pos>& hull) {
	std::sort(candi.begin(), candi.end());
	if (candi.size() <= 2) {
		for (const Pos& pos : candi) hull.push_back(pos);
		return;
	}
	for (int i = 0; i < candi.size(); ++i) {
		while (hull.size() > 1 && cross(hull[hull.size() - 2], hull[hull.size() - 1], hull[hull.size() - 1], candi[i]) <= 0) {
			hull.pop_back();
		}
		hull.push_back(candi[i]);
	}
	hull.pop_back();
	int s = hull.size()+1;
	for (int i = candi.size()-1; i >= 0; --i) {
		while (hull.size() > s && cross(hull[hull.size() - 2], hull[hull.size() - 1], hull[hull.size() - 1], candi[i]) <= 0) {
			hull.pop_back();
		}
		hull.push_back(candi[i]);
	}
	hull.pop_back();
}
bool is_cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	bool f11 = cross(d1, d2, d2, d3) * cross(d2, d1, d1, d4) > 0;
	bool f12 = cross(d3, d4, d4, d1) * cross(d4, d3, d3, d2) > 0;
	bool f1 = f11 && f12;
	bool f2 = (cross(d1, d3, d3, d2) == 0 && dot(d1, d3, d3, d2) >= 0) ||
		(cross(d1, d4, d4, d2) == 0 && dot(d1, d4, d4, d2) >= 0) ||
		(cross(d3, d1, d1, d4) == 0 && dot(d3, d1, d1, d4) >= 0) ||
		(cross(d3, d2, d2, d4) == 0 && dot(d3, d2, d2, d4) >= 0);
	return f1 || f2;
}

int Y, X, K;
std::vector<Pos> cu, cd, hu, hd;
Pos su, sd, eu, ed;
int main() {
	std::cin >> Y >> X >> K;
	for (int i = 0; i < K; ++i) std::cin >> P[i].y >> P[i].x;
	su = { 1, 2 }; sd = { 2, 1 }; eu = { X, Y + 1 }; ed = { X + 1, Y };
	for (int i = 0; i < K; i++) {
		Pos du = { P[i].x + 1, P[i].y }, dd = { P[i].x, P[i].y + 1 };
		bool iu = is_cross(su, eu, du, dd), id = is_cross(sd, ed, du, dd);
		if (iu && id) { std::cout << 0 << "\n"; return 0; }
		if (iu) cu.push_back(du);
		if (id) cd.push_back(dd);
	}
	cu.push_back(su); cu.push_back(eu);
	cd.push_back(sd); cd.push_back(ed);
	monotone_chain(cu, hu);
	monotone_chain(cd, hd);
	for (int i = 0; i < hu.size(); i++) {
		for (int j = 0; j < hd.size(); j++) {
			if (is_cross(hu[i], hu[(i + 1) % hu.size()], hd[j], hd[(j + 1) % hd.size()])) {
				std::cout << 0 << "\n"; return 0;
			}
		}
	}
	std::cout << 1 << "\n";
	return 0;
}
*/