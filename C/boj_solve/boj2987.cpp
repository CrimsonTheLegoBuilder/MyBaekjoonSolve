#include <stdio.h>
#include <algorithm>
typedef long long ll;
const int LEN = 100;
int N, cnt;

struct Pos { int x, y; } a, b, c, d;
int cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
bool inner() {
	return cross(a, b, d) >= 0 && cross(b, c, d) >= 0 && cross(c, a, d) >= 0;
}
void solve() {
	scanf("%d %d\n%d %d\n%d %d", &a.x, &a.y, &b.x, &b.y, &c.x, &c.y);
	ll area = cross(a, b, c);
	if (area < 0) std::swap(b, c), area *= -1;
	scanf("%d", &N);
	while (N--) {
		scanf("%d %d", &d.x, &d.y), cnt += inner();
	}
	printf("%d.%d\n%d\n", area >> 1, (area & 1) * 5, cnt);
}
int main() { solve(); }