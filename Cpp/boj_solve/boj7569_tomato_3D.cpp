#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <queue>
typedef long long ll;
const int LEN = 105;

int M, N, H;
int box[LEN][LEN][LEN];
struct Pos3D {
	int x, y, z;
	Pos3D(int X = 0, int Y = 0, int Z = 0) : x(X), y(Y), z(Z) {}
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D operator * (const int& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
};
typedef std::vector<Pos3D> Polygon;
Polygon DIR = { Pos3D(1, 0, 0), Pos3D(0, 1, 0), Pos3D(0, 0, 1) };
std::queue<Pos3D> Q;
void bfs() {
	auto valid = [&](const Pos3D& p) -> bool {
		return (0 <= p.x && p.x < M && 0 <= p.y && p.y < N && 0 <= p.z && p.z < H);
		};
	while (Q.size()) {
		Pos3D v = Q.front(); Q.pop();
		for (int i = 0; i < 2; i++) {
			for (Pos3D& d : DIR) {
				Pos3D nxt = v + d * (i == 0 ? 1 : -1);
				if (valid(nxt) && !box[nxt.z][nxt.y][nxt.x]) {
					box[nxt.z][nxt.y][nxt.x] = box[v.z][v.y][v.x] + 1;
					Q.push(nxt);
				}
			}
		}
	}
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> M >> N >> H;
	for (int z = 0; z < H; z++)
		for (int y = 0; y < N; y++)
			for (int x = 0; x < M; x++) {
				std::cin >> box[z][y][x];
				if (box[z][y][x] == 1) Q.push(Pos3D(x, y, z));
			}

	bfs();

	int latest = -1;
	for (int z = 0; z < H; z++)
		for (int y = 0; y < N; y++)
			for (int x = 0; x < M; x++) {
				if (!box[z][y][x]) { std::cout << "-1\n"; return; }
				if (latest < box[z][y][x]) latest = box[z][y][x];
			}
	
	std::cout << latest - 1 << "\n";
	return;
}
int main() { solve(); return 0; }