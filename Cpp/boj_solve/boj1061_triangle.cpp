#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
typedef long long ll;
const int LEN = 50;
int N, M, R, G, B, T;
bool FR, FG, FB, V[LEN][LEN];
std::string box[LEN];

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const {
		if (x == p.x) return (y < p.y);
		return x < p.x;
	}
};
std::vector<Pos> HR, HG, HB;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}



int main() {
	bool fR = 0, fG = 0, fB = 0;
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) {
		std::cin >> box[i];
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			R += (box[i][j] == 'R');
			G += (box[i][j] == 'G');
			B += (box[i][j] == 'B');
		}
	}
	T = R * G * B;

	int u, r, l, d;
	u = 0, d = N - 1, l = 0, r = M - 1;
	while (u <= d && l <= r) {
		for (int i = u; i <= d; i++) {
			if (!V[i][l] && !FR && box[i][l] == 'R') {
				HR.push_back({ l, i });
				V[i][l] = 1;
			}
			if (!V[i][r] && !FR && box[i][r] == 'R') {
				HR.push_back({ r, i });
				V[i][r] = 1;
			}
			if (!V[i][l] && !FG && box[i][l] == 'G') {
				HG.push_back({ l, i });
				V[i][l] = 1;
			}
			if (!V[i][r] && !FG && box[i][r] == 'G') {
				HG.push_back({ r, i });
				V[i][r] = 1;
			}
			if (!V[i][l] && !FB && box[i][l] == 'B') {
				HB.push_back({ l, i });
				V[i][l] = 1;
			}
			if (!V[i][r] && !FB && box[i][r] == 'B') {
				HB.push_back({ r, i });
				V[i][r] = 1;
			}
		}
		for (int j = l; j <= r; j++) {
			if (!V[u][j] && !FR && box[u][j] == 'R') {
				HR.push_back({ j, u });
				V[u][j] = 1;
			}
			if (!V[d][j] && !FR && box[d][j] == 'R') {
				HR.push_back({ j, d });
				V[d][j] = 1;
			}
			if (!V[u][j] && !FG && box[u][j] == 'G') {
				HG.push_back({ j, u });
				V[u][j] = 1;
			}
			if (!V[d][j] && !FG && box[d][j] == 'G') {
				HG.push_back({ j, d });
				V[d][j] = 1;
			}
			if (!V[u][j] && !FB && box[u][j] == 'B') {
				HB.push_back({ j, u });
				V[u][j] = 1;
			}
			if (!V[d][j] && !FB && box[d][j] == 'B') {
				HB.push_back({ j, d });
				V[d][j] = 1;
			}
		}
		u ++, d--, l++, r--;
		if (HR.size()) FR = 1;
		if (HG.size()) FG = 1;
		if (HB.size()) FB = 1;
	}
	if (!FR || !FG || !FB) {
		std::cout << "0 0 \n";
		return 0;
	}
	int cnt = 0, A = 0;
	for (const Pos& r : HR) {
		for (const Pos& g : HG) {
			for (const Pos& b : HB) {
				//std::cout << r.x << " " << r.y << " " << g.x << " " << g.y << " " << b.x << " " << b.y << "\n";
				//std::cout << r.x << " " << r.y << "\n";
				//std::cout << g.x << " " << g.y << "\n";
				//std::cout << b.x << " " << b.y << "\n";
				if (std::abs(cross(r, g, g, b)) == A) cnt++;
				if (std::abs(cross(r, g, g, b)) > A) {
					A = std::abs(cross(r, g, g, b));
					cnt = 1;
				}
			}
		}
	}
	std::cout << T << " " << cnt << "\n";
	std::cout << T - cnt << "\n";
	return 0;
}