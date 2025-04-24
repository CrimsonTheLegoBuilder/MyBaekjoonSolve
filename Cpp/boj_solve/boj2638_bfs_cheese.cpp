#include <iostream>
#include <queue>
#include <cstring>
typedef long long ll;
const int LEN = 100;
int grid[LEN][LEN];
int V[LEN][LEN];
int h = 0;
const int drc[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
struct Pos {
	int r, c;
};

bool BFS(int r, int c, int N, int M) {
	memset(V, 0, sizeof V);
	std::queue<Pos> Q;
	Q.push({ r,c });
	V[r][c] = 1;
	while (!Q.empty()) {
		Pos p = Q.front(); Q.pop();
		//std::cout << p.r << " " << p.c << "\n";
		for (int i = 0; i < 4; i++) {
			int nr = p.r + drc[i][0], nc = p.c + drc[i][1];
			if (0 <= nr && nr < N && 0 <= nc && nc < M) {
				if (!grid[nr][nc] && !V[nr][nc]) {
					Q.push({ nr,nc });
					V[nr][nc] = 1;
				}
				else if (grid[nr][nc] && V[nr][nc] < 2) {
					V[nr][nc]++;
				}
			}
		}
	}
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (V[i][j] == 2) {
				grid[i][j] = 0;
				cnt++;
			}
		}
	}
	if (cnt) {
		h++;
		return 0;
	}
	return 1;
}



int main() {
	int N, M;
	std::cin >> N >> M;
	for (int n = 0; n < N; n++) {
		for (int m = 0; m < M; m++) {
			std::cin >> grid[n][m];
		}
	}
	while (1) {
		bool F = BFS(0, 0, N, M);
		if (F) {
			std::cout << h << "\n";
			return 0;
		}
	}
}