#include <iostream>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-12;
ld A = 0.0;
char grid[50][51];
int ly, lx;

struct Pos {
	ld x, y;
	bool operator < (const Pos& P) const {
		if (std::abs(x - P.x) < TOL) return y < P.y;
		return x < P.x;
	}
	bool operator == (const Pos& P) const {
		return (std::abs(x - P.x) < TOL && std::abs(y - P.y) < TOL);
	}
}L;
std::vector<Pos> H;

struct Slope {
	int dx;
	ld dy;
	bool operator < (const Slope& P) const {
		if (dx == P.dx) return dy < P.dy;
		return dx < P.dx;
	}
	bool operator == (const Slope& P) const {
		return (dx == P.dx && std::abs(dy - P.dy) < TOL);
	}
};
std::set<Slope> slopes;


ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}
void get_hull(int y, int x, const Slope& s, Pos& L, const int& N, const int& M) {
	if (y <= -1 || y >= N || x <= -1 || x >= M || grid[y][x] == '#') {
		H.push_back(L); return;  //when ray blocked or reaches the end of grid
	}
	int ycw, ynxt, xcw, xnxt;
	xcw = (L.x > 0 ? x + 1 : x - 1);       //for judge ccw or cw intersection point (x.coord')
	ycw = (L.x * L.y > 0 ? y + 1 : y - 1); //for judge ccw or cw intersection point (y.coord') - L.x * L.y > 0 -> 1st, 3rd plane
	xnxt = (L.x > 0 ? x + 1 : x);          //intersection's x coord'
	ynxt = (L.x * L.y > 0 ? y + 1 : y);    //intersection's y coord'
	Pos I = { 0, 0 };
	ld iy, ix;  //intersection's coord'
	iy = (ld)L.y + (xnxt - L.x) * s.dy;
	ix = (ld)L.x + (ynxt - L.y) / s.dx;
	if (y + TOL <= iy && iy <= y + 1 - TOL && (x - TOL > ix || x + 1 + TOL < ix)) {  //when y.coord' intersect with grid's side
		I.y = iy; I.x = xnxt;
		get_hull(xcw, y, s, I, N, M);
	}
	else if (x + TOL <= ix && ix <= x + 1 - TOL && (y - TOL > iy || y + 1 + TOL < iy)) {
		I.y = ynxt; I.x = ix;
		get_hull(x, ycw, s, I, N, M);
	}
	else {
		I.y = ynxt; I.x = xnxt;
		if (s.dy > 0 && (xcw <= -1 || xcw >= M || grid[y][xcw] == '#')) {
			H.push_back(I);
		}
	}
	return;
}
ld get_area(std::vector<Pos> H, Pos& L) {
	ld area = 0.0;
	for (int d1 = 0; d1 < H.size(); d1++) {
		int d2 = (d1 + 1) % H.size(), d3 = (d1 + 2) % H.size();
		area -= cross(H[d1], H[d2], H[d2], H[d3]);
	}
	return area / 2.0;
}



int main() {
	int N, M;
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) {
		std::cin >> grid[i];
		for (int j = 0; j < M; j++) {
			if (grid[i][j] == '*') {
				L.x = j + .5; L.y = i + .5;
				lx = j; ly = i;
			}
			if (grid[i][j] == '#') A += 1.0;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			int dx = (j > L.x ? 1 : -1);
			ld dy = (j - L.y) / (i - L.x);
			slopes.insert({ dx, dy });
		}
	}
	for (const Slope& s : slopes) {
		std::cout << s.dx << " " << s.dy << "\n";
	}
	for (const Slope& s : slopes) {
		get_hull(ly, lx, s, L, N, M);
	}
	return 0;
}