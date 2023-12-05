#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 300'000;
int N;

struct Pos {
	ll x, y;
	bool operator < (const Pos& p) {
		bool f1 = x == 0 ? y > 0 : x > 0;
		bool f2 = p.x == 0 ? p.y > 0 : p.x > 0;
		if (f1 != f2) return f1;
		ll ccw = x * p.y - y * p.x;
		return ccw ? ccw > 0 : (x * x + y * y) < (p.x * p.x + p.y * p.y);
	}

} P[LEN], hyeok;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> P[i].x >> P[i].y;
	std::cin >> hyeok.x >> hyeok.y;
	for (int i = 0; i < N; i++) P[i].x -= hyeok.x, P[i].y -= hyeok.y;
	std::sort(P, P + N);
	for (int i = 0; i < N; i++) std::cout << P[i].x + hyeok.x << " " << P[i].y + hyeok.y << "\n";
	return 0;
}
