#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
int N, Q, q, cnt, LEN = 8000;
ll A, B, C;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
} P;
std::vector<Pos> T, LH, UH, BF;
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
std::vector<Pos> lower_hull(std::vector<Pos>& T) {  //lower monotone chain
	std::vector<Pos> H;
	std::sort(T.begin(), T.end());
	if (T.size() <= 2) {
		for (const Pos& pos : T) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < T.size(); i++) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], T[i]) <= 0) H.pop_back();
		H.push_back(T[i]);
	}
	return H;
}
std::vector<Pos> upper_hull(std::vector<Pos>& T) {  //upper monotone_chain
	std::vector<Pos> H;
	std::sort(T.begin(), T.end());
	if (T.size() <= 2) {
		for (int i = T.size() - 1; i >= 0; i--) H.push_back(T[i]);
		return H;
	}
	for (int i = T.size() - 1; i >= 0; i--) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], T[i]) <= 0) H.pop_back();
		H.push_back(T[i]);
	}
	return H;
}
void norm(ll& A, ll& B, ll& C) { if (B < 0) A = -A, B = -B, C = -C; }
int bi_search(const ll& A, const ll& B, const ll& C, const std::vector<Pos>& H, bool f = 0) {
	int sz = H.size();
	ll a = A, b = B, c = C; if (f) a = -a, b = -b, c = -c;
	ll S = a * H[0].x + b * H[0].y - c;
	ll E = a * H[sz - 1].x + b * H[sz - 1].y - c;
	if (!S || !E || (S > 0) != (E > 0)) return 0;  //cross
	if (S < 0) return -1;  //below line
	a = -a;
	int s = 0, e = sz - 2, m, ret = sz - 1;
	while (s <= e) {
		m = s + e >> 1;
		ll dx = H[m + 1].x - H[m].x;
		ll dy = H[m + 1].y - H[m].y;
		if (a * dx <= b * dy) {
			ret = std::min(ret, m);
			e = m - 1;
		}
		else s = m + 1;
	}
	a = -a;
	return a * H[ret].x + b * H[ret].y - c > 0;
}
void query(int q) {
	if (q == 1) {
		std::cin >> P.x >> P.y;
		//T.push_back(P);
		BF.push_back(P);
		cnt++;
		if (cnt == LEN) {
			cnt = 0; //N += LEN;
			//std::sort(T.begin(), T.end());
			for (const Pos& d : BF) {
				LH.push_back(d);
				UH.push_back(d);
			}
			LH = lower_hull(LH), UH = upper_hull(UH);
			BF.clear();
		}
		return;
	}
	if (q == 2) {
		bool F = 0;
		std::cin >> A >> B >> C;
		norm(A, B, C);
		int d = bi_search(A, B, C, LH);
		int u = -bi_search(A, B, C, UH, 1);
		if (d * u > 0) {
			for (int i = 0; i < BF.size(); i++) {
				Pos t = BF[i];
				ll cur = A * t.x + B * t.y - C;
				if (d * cur <= 0) { F = 1; break; }
			}
		}
		else F = 1;
		std::cout << (F ? "NO\n" : "YES\n");
	}
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("fencing_gold/14.in", "r", stdin);
	//freopen("fencing_gold/out.txt", "w", stdout);
	std::cin >> N >> Q;
	T.resize(N);
	for (int i = 0; i < N; i++) std::cin >> T[i].x >> T[i].y;
	std::sort(T.begin(), T.end());
	LH = lower_hull(T), UH = upper_hull(T);
	while (Q--) {
		std::cin >> q;
		query(q);
	}
	return;
}
int main() { solve(); return 0; }

/*

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
int N, Q, q, cnt, LEN = 1000;
ll A, B, C;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
} P;
std::vector<Pos> T, LH, UH;
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
void half_monotone_chain(std::vector<Pos>& T, std::vector<Pos>& H, bool r = 0) {
	if (!r) std::sort(T.begin(), T.end());
	if (r) std::reverse(T.begin(), T.end());
	H.clear();
	if (T.size() <= 2) {
		for (const Pos& pos : T) H.push_back(pos);
		return;
	}
	for (int i = 0; i < T.size(); i++) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], T[i]) <= 0) H.pop_back();

		H.push_back(T[i]);
	}
	return;
}
void get_hull() {
	half_monotone_chain(T, LH); half_monotone_chain(T, UH, 1);
}
void norm(ll& A, ll& B, ll& C) { if (B < 0) A = -A, B = -B, C = -C; }
int bi_search(const ll& A, const ll& B, const ll& C, const std::vector<Pos>& H, bool f = 0) {
	int sz = H.size();
	ll a = A, b = B, c = C; if (f) a = -a, b = -b, c = -c;
	ll S = a * H[0].x + b * H[0].y - c;
	ll E = a * H[sz - 1].x + b * H[sz - 1].y - c;
	if (!S || !E || (S > 0) != (E > 0)) return 0;  //cross
	if (S < 0) return -1;  //below line
	a = -a;
	int s = 0, e = sz - 2, m, ret = sz - 1;
	while (s <= e) {
		m = s + e >> 1;
		ll dx = H[m + 1].x - H[m].x;
		ll dy = H[m + 1].y - H[m].y;
		if (a * dx <= b * dy) {
			ret = std::min(ret, m);
			e = m - 1;
		}
		else s = m + 1;
	}
	a = -a;
	return a * H[ret].x + b * H[ret].y - c > 0;
}
void query(int q) {
	if (q == 1) {
		std::cin >> P.x >> P.y;
		T.push_back(P);
		cnt++;
		if (cnt == LEN) {
			cnt = 0; N += LEN;
			get_hull();
		}
		return;
	}
	if (q == 2) {
		bool F = 0;
		std::cin >> A >> B >> C;
		norm(A, B, C);
		int d = bi_search(A, B, C, LH);
		int u = -bi_search(A, B, C, UH, 1);
		if (d * u > 0) {
			for (int i = N; i < T.size(); i++) {
				Pos t = T[i];
				ll cur = A * t.x + B * t.y - C;
				if (d * cur <= 0) { F = 1; break; }
			}
		}
		else F = 1;
		std::cout << (F ? "NO\n" : "YES\n");
	}
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("fencing_gold/13.in", "r", stdin);
	//freopen("fencing_gold/out.txt", "w", stdout);
	std::cin >> N >> Q;
	T.resize(N);
	for (int i = 0; i < N; i++) std::cin >> T[i].x >> T[i].y;
	get_hull();
	while (Q--) {
		std::cin >> q;
		query(q);
	}
	return;
}
int main() { solve(); return 0; }

*/