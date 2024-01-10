#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
const int LEN = 1e5;
int N, cnt = 0;

struct Pos {
	ll x, y;
	int i;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
}P;
std::vector<Pos> C, H;
int num[LEN];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	for (const Pos& i : H) { num[cnt++] = i.i; }
	return H;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y, C[i].i = i + 1;
	monotone_chain(C);
	std::sort(num, num + cnt);
	std::cout << C.size() << "\n";
	std::cout << cnt << "\n";
	for (int i = 0; i < cnt; i++) std::cout << num[i] << " ";
	return;
}
int main() { solve(); return 0; }