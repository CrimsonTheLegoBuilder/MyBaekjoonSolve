#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
typedef long long ll;
const int LEN = 50;
int N, M;
bool FR, FG, FB;
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
	int u, r, l, d;
	u = 0, d = N - 1, l = M - 1, r = 0;
	while (u <= d && r <= l) {
		for (int i = u; i <= d; i++) {
			if (!FR && box[i][l] == 'R') {
				HR.push_back({ l, i });
				fR = 1;
			}
			if (!FR && box[i][r] == 'R') {
				HR.push_back({ r, i });
				fR = 1;
			}
			if (!FG && box[i][l] == 'G') {
				HG.push_back({ l, i });
				fG = 1;
			}
			if (!FG && box[i][r] == 'G') {
				HG.push_back({ r, i });
				fG = 1;
			}
			if (!FB && box[i][l] == 'B') {
				HB.push_back({ l, i });
				fB = 1;
			}
			if (!FB && box[i][r] == 'B') {
				HB.push_back({ r, i });
				fB = 1;
			}
		}
		for (int j = l; j <= r; j++) {
			if (!FR && box[u][j] == 'R') {
				HR.push_back({ j, u });
				fR = 1;
			}
			if (!FR && box[d][j] == 'R') {
				HR.push_back({ j, d });
				fR = 1;
			}
			if (!FG && box[u][j] == 'G') {
				HG.push_back({ j, u });
				fG = 1;
			}
			if (!FG && box[d][j] == 'G') {
				HG.push_back({ j, d });
				fG = 1;
			}
			if (!FB && box[u][j] == 'B') {
				HB.push_back({ j, u });
				fB = 1;
			}
			if (!FB && box[d][j] == 'B') {
				HB.push_back({ j, d });
				fB = 1;
			}
		}
		u ++, d--, l++, r--;
		if (fR) FR = 1;
		if (fG) FG = 1;
		if (fB) FB = 1;
	}
	if (!FR || !FG || !FB) {
		std::cout << "-1\n";
	}



}