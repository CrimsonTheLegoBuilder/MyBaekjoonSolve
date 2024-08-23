#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <unordered_set>

struct Pos {
	int x, y;
	Pos(int x = 0, int y = 0) : x(x), y(y) {}
	Pos operator + (const Pos& p) const { return Pos(x + p.x, y + p.y); }
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
};
struct PosHash {
	std::size_t operator()(const Pos& p) const {
		return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
	}
};
typedef std::vector<Pos> V;
std::unordered_set<Pos, PosHash> S;
int N, A, B;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cin >> N >> A >> B;
	V P(N);
	for (int i = 0; i < N; i++) {
		std::cin >> P[i].x >> P[i].y;
		S.insert(P[i]);
	}
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		bool f1, f2, f3;
		f1 = S.find(Pos(P[i].x + A, P[i].y)) != S.end();
		f2 = S.find(Pos(P[i].x, P[i].y + B)) != S.end();
		f3 = S.find(Pos(P[i].x + A, P[i].y + B)) != S.end();
		cnt += f1 * f2 * f3;
	}
	//std::cout << cnt << "\n";
}