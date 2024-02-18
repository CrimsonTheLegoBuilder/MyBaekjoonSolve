//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//const int LEN = 1000;
//int N, cnt = 0, Q;
//bool check[LEN];
//
//struct Pos {
//	ll x, y;
//	int i;
//	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//} P;
//std::vector<Pos> C, hulls[340];
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
//}
//bool between(const Pos& d1, const Pos& d2, const Pos& target) {
//	return (!ccw(d1, target, d2) && (dot(d1, target, d2) >= 0));
//}
//bool inner_check(const Pos& p, std::vector<Pos>& H) {
//	int sz = H.size();
//	if (sz <= 2 || ccw(H[0], H[1], p) < 0 || ccw(H[0], H[sz - 1], p) > 0) return 0;
//	if (between(H[0], H[1], p) || between(H[0], H[sz - 1], p)) return 1;
//	int s = 0, e = sz - 1, m;
//	while (s + 1 < e) {
//		m = s + e >> 1;
//		if (ccw(H[0], H[m], p) > 0) s = m;
//		else e = m;
//	}
//	return ccw(H[s], H[e], p) > 0 || between(H[s], H[e], p);
//}
//std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	std::sort(C.begin(), C.end());
//	if (C.size() <= 2) for (const Pos& pos : C) H.push_back(pos);
//	else {
//		for (int i = 0; i < C.size(); i++) {
//			while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//				H.pop_back();
//			H.push_back(C[i]);
//		}
//		H.pop_back();
//		int s = H.size() + 1;
//		for (int i = C.size() - 1; i >= 0; i--) {
//			while (H.size() > s && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//				H.pop_back();
//			H.push_back(C[i]);
//		}
//		H.pop_back();
//	}
//	for (const Pos& d : H) check[d.i] = 1;
//	return H;
//}
//void hull_brute(std::vector<Pos>& C) {
//	std::vector<Pos> H = monotone_chain(C), tmp;
//	while (H.size()) {
//		cnt++;
//		hulls[cnt] = H;
//		tmp.clear();
//		for (const Pos& c : C) if (!check[c.i]) tmp.push_back(c);
//		H = monotone_chain(tmp);
//	}
//	return;
//}
//int bi_search() {
//	std::cin >> P.x >> P.y;
//	int s = 1, e = cnt, m, ret = 0;
//	if (!inner_check(P, hulls[1])) return 0;
//	while (s <= e) {
//		m = s + e >> 1;
//		if (inner_check(P, hulls[m])) {
//			ret = std::max(m, ret);
//			s = m + 1;
//		}
//		else e = m - 1;
//	}
//	return ret;
//}
//void init() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	cnt = 0;
//	memset(check, 0, sizeof check);
//	C.resize(N);
//	for (int i = 0; i < N; i++)
//		std::cin >> C[i].x >> C[i].y, C[i].i = i;
//	return;
//}
//void solve() {
//	init();
//	hull_brute(C);
//	std::cin >> Q;
//	while (Q--) std::cout << bi_search() << "\n";
//	return;
//}
//int main() { solve(); return 0; }//boj27656

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#define sz(x) (int)(x).size()
#define ccw_check(x) x[sz(x) - 2], x[sz(x) - 1], C[i]
typedef long long ll;
const int LEN = 1000;
int N, cnt = 0, Q;
bool check[LEN];

struct Pos {
	ll x, y;
	int i;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
} P;
std::vector<Pos> C, hulls[340];
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
bool between(const Pos& d1, const Pos& d2, const Pos& target) {
	return (!ccw(d1, target, d2) && (dot(d1, target, d2) >= 0));
}
bool inner_check(const Pos& p, std::vector<Pos>& H) {
	int sz = H.size();
	if (sz <= 2 || ccw(H[0], H[1], p) < 0 || ccw(H[0], H[sz - 1], p) > 0) return 0;
	if (between(H[0], H[1], p) || between(H[0], H[sz - 1], p)) return 1;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (ccw(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	return ccw(H[s], H[e], p) > 0 || between(H[s], H[e], p);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) for (const Pos& pos : C) H.push_back(pos);
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(ccw_check(H)) <= 0) H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
		int s = H.size() + 1;
		for (int i = C.size() - 1; i >= 0; i--) {
			while (H.size() > s && ccw(ccw_check(H)) <= 0) H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	for (const Pos& d : H) check[d.i] = 1;
	return H;
}
void brute(std::vector<Pos>& C) {
	std::vector<Pos> H = monotone_chain(C), tmp;
	while (H.size()) {
		cnt++;
		hulls[cnt] = H;
		tmp.clear();
		for (const Pos& c : C) if (!check[c.i]) tmp.push_back(c);
		H = monotone_chain(tmp);
	}
	return;
}
int bi_search() {
	std::cin >> P.x >> P.y;
	int s = 1, e = cnt, m, ret = 0;
	if (!inner_check(P, hulls[1])) return 0;
	while (s <= e) {
		m = s + e >> 1;
		if (inner_check(P, hulls[m])) {
			ret = std::max(m, ret);
			s = m + 1;
		}
		else e = m - 1;
	}
	return ret;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	cnt = 0;
	memset(check, 0, sizeof check);
	C.resize(N);
	for (int i = 0; i < N; i++)
		std::cin >> C[i].x >> C[i].y, C[i].i = i;
	return;
}
void solve() {
	init();
	brute(C);
	std::cin >> Q;
	while (Q--) std::cout << bi_search() << "\n";
	return;
}
int main() { solve(); return 0; }//boj27656

//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//const int LEN = 1000;
//int N, cnt = 0, Q;
//bool check[LEN];
//
//struct Pos {
//	ll x, y;
//	int i;
//	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//} P;
//std::vector<Pos> C, hulls[340];
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
//}
//bool between(const Pos& d1, const Pos& d2, const Pos& target) {
//	return (!ccw(d1, target, d2) && (dot(d1, target, d2) >= 0));
//}
//bool inner_check(const Pos& p, std::vector<Pos>& H) {
//	int sz = H.size();
//	if (sz <= 2 || ccw(H[0], H[1], p) < 0 || ccw(H[0], H[sz - 1], p) > 0) return 0;
//	if (between(H[0], H[1], p) || between(H[0], H[sz - 1], p)) return 1;
//	int s = 0, e = sz - 1, m;
//	while (s + 1 < e) {
//		m = s + e >> 1;
//		if (ccw(H[0], H[m], p) > 0) s = m;
//		else e = m;
//	}
//	return ccw(H[s], H[e], p) > 0 || between(H[s], H[e], p);
//}
//std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	std::sort(C.begin(), C.end());
//	if (C.size() <= 2) {
//		for (const Pos& pos : C) H.push_back(pos);
//		for (const Pos& d : H) { check[d.i] = 1; }
//		return H;
//	}
//	for (int i = 0; i < C.size(); i++) {
//		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
//			H.pop_back();
//		}
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	int s = H.size() + 1;
//	for (int i = C.size() - 1; i >= 0; i--) {
//		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
//			H.pop_back();
//		}
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	for (const Pos& d : H) { check[d.i] = 1; }
//	return H;
//}
//ll A(std::vector<Pos>& H) {
//	Pos P = { 0, 0 };
//	ll area = 0;
//	int h = H.size();
//	for (int i = 0; i < h; i++) {
//		Pos cur = H[i], nxt = H[(i + 1) % h];
//		area += cross(P, cur, nxt);
//	}
//	return area;
//}
//void brute(std::vector<Pos>& C) {
//	std::vector<Pos> H = monotone_chain(C), tmp;
//	//while (A(H)) {
//	while (H.size()) {
//		cnt++;
//		hulls[cnt] = H;
//		tmp.clear();
//		for (const Pos& c : C) if (!check[c.i]) tmp.push_back(c);
//		H = monotone_chain(tmp);
//	}
//	//if (H.size()) cnt++;
//	return;
//}
//int bi_search() {
//	std::cin >> P.x >> P.y;
//	int s = 1, e = cnt, m, ret = 0;
//	if (!inner_check(P, hulls[1])) return 0;
//	while (s <= e) {
//		m = s + e >> 1;
//		if (inner_check(P, hulls[m])) {
//			ret = std::max(m, ret);
//			s = m + 1;
//		}
//		else e = m - 1;
//	}
//	return ret;
//}
//void init() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	cnt = 0;
//	memset(check, 0, sizeof check);
//	C.resize(N);
//	for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y, C[i].i = i;
//}
//void solve() {
//	init();
//	brute(C);
//	//std::cout << cnt << " DEBUG\n";
//	//for (int i = 1; i <= cnt; i++) {
//	//	for (const Pos& h : hulls[i]) std::cout << h.x << " " << h.y << "\n";
//	//	std::cout << "\n";
//	//}
//	std::cin >> Q;
//	while (Q--) std::cout << bi_search() << "\n";
//	return;
//}
//int main() { solve(); return 0; }//boj27656