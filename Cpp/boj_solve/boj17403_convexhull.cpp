#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
const int LEN = 1e3;
int N, idx[LEN];

struct Pos {
	int x, y, i;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
} P;
std::vector<Pos> C;
int cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
int dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C, int id) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	if (H.size() > 2) for (const Pos& d : H) { idx[d.i] = id; }
	return H;
}
ll A(std::vector<Pos>& H) {
	Pos P = { 0, 0 };
	ll area = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		area += cross(P, cur, nxt);
	}
	return area;
}
void brute(std::vector<Pos>& C) {
	int cnt = 1;
	std::vector<Pos> H = monotone_chain(C, cnt), tmp;
	while (A(H)) {
		cnt++;
		tmp.clear();
		for (const Pos& c : C) {
			if (!idx[c.i]) tmp.push_back(c);
		}
		H = monotone_chain(tmp, cnt);
	}
	return;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int x, y;
	memset(idx, 0, sizeof idx);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> x >> y;
		C.push_back({ x, y, i });
	}
	brute(C);
	for (int i = 0; i < N; i++) std::cout << idx[i] << " ";
	return 0;
}