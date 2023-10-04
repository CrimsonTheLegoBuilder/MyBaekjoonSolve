#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
typedef long double ld;
ll cnt = 0;
bool V[8]{ 0 };
int S[8];

struct Pos {
	ld x, y;
}P[8];

ld cross(Pos& d1, Pos& d2, Pos& d3, Pos& d4) { return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x); }
ld delta[8][2] = { {0, 2}, {-sqrt(2), sqrt(2)}, {-2, 0}, {-sqrt(2), -sqrt(2)}, {0, -2}, {sqrt(2), -sqrt(2)}, {2, 0}, {sqrt(2), sqrt(2)} };

void dfs(int depth) {
	if (depth > 2) {
		if (cross(P[depth - 3], P[depth - 2], P[depth - 2], P[depth - 1]) < 0) {
			//std::cout << cross(P[depth - 3], P[depth - 2], P[depth - 2], P[depth - 1]);
			//std::cout << "x";
			return;
		}
	}
	if (depth == 8) {
		for (int i = 5; i <= 7; i++) {
			if (cross(P[i], P[(i + 1) % 8], P[(i + 1) % 8], P[(i + 2) % 8]) < 0) return;
		}
		cnt++;
		return;
	}
	for (int i = 1; i < 8; i++) {
		if (!V[i]) {
			V[i] = 1;
			P[depth].x = S[i] * delta[depth][0];
			P[depth].y = S[i] * delta[depth][1];
			dfs(depth + 1);
			V[i] = 0;
		}
	}
}

int main() {
	for (int i = 0; i < 8; i++) {
		std::cin >> S[i];
	}
	P[0].x = S[0] * delta[0][0];
	P[0].y = S[0] * delta[0][1];
	dfs(1);
	std::cout << cnt * 8;
}