#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;
const int LEN = 5e4;
int N, a{ 0 }, b{ 0 };

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator==(const Pos& p) const { return x == p.x && y == p.y; }
} S[LEN];
std::vector<Pos> A, B, AH,BH;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (!cross(d1, d3, d2) && (dot(d1, d3, d2) >= 0));
}
bool inner_bi_search(std::vector<Pos>& H, const Pos& x) {
	int sz = H.size();
	if (sz < 2 || cross(H[0], H[1], x) < 0 || cross(H[0], H[sz - 1], x) > 0) return 0;
	if (on_seg(H[0], H[1], x) || on_seg(H[0], H[sz - 1], x)) return 1;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		ll C = cross(H[0], H[m], x);
		if (C > 0) s = m;
		else e = m;
	}
	return cross(H[s], H[e], x) > 0 || on_seg(H[s], H[e], x);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
	}
	else {
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
	}
	return H;
}

int solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	A.resize(N);
	B.resize(N);
	for (int i = 0; i < N; i++) std::cin >> A[i].x >> A[i].y;
	for (int i = 0; i < N; i++) std::cin >> B[i].x >> B[i].y;
	AH = monotone_chain(A);
	BH = monotone_chain(B);
	for (int i = 0; i < N; i++) a += inner_bi_search(AH, B[i]);
	for (int i = 0; i < N; i++) b += inner_bi_search(BH, A[i]);
	std::cout << a << " " << b << "\n";
	return 0;
}
int main() { solve(); return 0; }//boj9875