#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-12;  //tolerance


char grid[50][51];
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
struct Slope {  //(vx, vy) : +-1, dy/dx
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


ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {  //get Torque / get area / CCW
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}
void get_hull(int x, int y, const Slope& s, Pos& L, const int& N, const int& M) {  //sweeping, get hull CCW
	Pos I = { 0, 0 };
	if (x <= -1 || x >= M || y <= -1 || y >= N || grid[y][x] == '#') {
		I.x = L.x; I.y = L.y;
		H.push_back(I); return;  //when ray blocked or reaches the end of grid, I is one of the hull.
	}
	int xnxt, ynxt;
	xnxt = (s.dx > 0 ? x + 1 : x - 1);        //next cell's idx (x.coord') - 1st, 4th quad -> +1 , 2nd, 3rd quad -> -1 for verify CCW or CW intersection'
	ynxt = (s.dx * s.dy > 0 ? y + 1 : y - 1); //next cell's idx (y.coord') - 1st, 2nd quad -> +1 , 3rd, 4th quad -> -1
	ld xinx, yinx;
	xinx = (s.dx > 0 ? x + 1 : x);            //intersection's x coord', move 1 step
	yinx = (s.dx * s.dy > 0 ? y + 1 : y);     //intersection's y coord', move 1 step
	ld ix, iy;
	ix = L.x + (yinx - L.y) / s.dy;  //intersection.p's x-coord', move nearest x-axis
	iy = L.y + (xinx - L.x) * s.dy;  //intersection.p's y-coord', move nearest y-axis
	if (y + TOL < iy && iy < y + 1 - TOL && (x - TOL > ix || ix > x + 1 + TOL)) {  //when intersection.p's y.coord' on grid's side and x's's out of range
		I.x = xinx; I.y = iy;
		get_hull(xnxt, y, s, I, N, M);  //into recursion and check the next cell if next == [#]. if [#] or out of grid, Pos I is one of the hull.
	}
	else if (x + TOL < ix && ix < x + 1 - TOL && (y - TOL > iy || iy > y + 1 + TOL)) {  //when intersection.p's x.coord' on grid's side and y's's out of range
		I.x = ix; I.y = yinx;
		get_hull(x, ynxt, s, I, N, M);  //into recursion and check the next cell if next == [#]. if [#] or out of grid, Pos I is one of the hull.
	}
	else {  //when ray intersect [#]'s corner
		I.y = yinx; I.x = xinx;
		if (s.dy > 0 && (xnxt <= -1 || xnxt >= M || grid[y][xnxt] == '#')) {  //1st, 3rd quadrant, CCW
			H.push_back(I);
		}
		if (s.dy < 0 && (ynxt <= -1 || ynxt >= N || grid[ynxt][x] == '#')) {  //2nd, 4th quadrant, CCW
			H.push_back(I);
		}
		get_hull(xnxt, ynxt, s, I, N, M);  //CCW - recursion - CW - return
		if (s.dy < 0 && (xnxt <= -1 || xnxt >= M || grid[y][xnxt] == '#')) {  //1st, 3rd quadrant, CW
			H.push_back(I);
		}
		if (s.dy > 0 && (ynxt <= -1 || ynxt >= N || grid[ynxt][x] == '#')) {  //2nd, 4th quadrant, CW
			H.push_back(I);
		}
	}
	return;
}
ld get_area(std::vector<Pos> H, Pos& L) {  //hull: sorted CCW -> get area to sum(Torque) / 2
	ld area = 0.0;                 //L = pivot
	for (int d1 = 0; d1 < H.size(); d1++) {
		int d2 = (d1 + 1) % H.size();
		area += cross(L, H[d1], H[d1], H[d2]);
		//std::cout << cross(L, H[d2], H[d2], H[d1]) << "\n";  //DEBUG
		//std::cout << H[d1].x << " " << H[d1].y << "\n";
	}
	return area / 2.0;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int N, M, ly = 0, lx = 0;
	ld A = 0.0;
	std::cin >> N >> M;
	A = (ld)N * M;
	for (int i = 0; i < N; i++) {
		std::cin >> grid[i];
		for (int j = 0; j < M; j++) {
			if (grid[i][j] == '*') {
				L.x = j + .5; L.y = i + .5;
				lx = j; ly = i;
			}
			if (grid[i][j] == '#') A -= 1.0;
		}
	}
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= M; j++) {
			int dx = (j > L.x ? 1 : -1);    //dx == -1 | Light | dx == +1
			ld dy = (i - L.y) / (j - L.x);  //dy/dx
			slopes.insert({ dx, dy });
		}
	}
	for (const Slope& s : slopes) {
		//std::cout << s.dx << " " << s.dy << "\n";  //DEBUG::ordered_set
		get_hull(lx, ly, s, L, N, M);
	}
	A -= get_area(H, L);
	std::cout << std::setprecision(12) << A << "\n";
	return 0;
}