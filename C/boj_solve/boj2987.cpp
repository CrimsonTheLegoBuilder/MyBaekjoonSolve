//#define _CRT_SECURE_NO_WARNINGS
////#include <iostream>
//#include <stdio.h>
//#include <algorithm>
////#include <cmath>
//typedef long long ll;
//const int LEN = 100;
//int N, cnt;
//
//struct Pos { int x, y; } a, b, c, d;
//int cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//bool inner() {
//	return cross(a, b, d) >= 0 && cross(b, c, d) >= 0 && cross(c, a, d) >= 0;
//}
//void solve() {
//	//std::cin.tie(0)->sync_with_stdio(0);
//	//std::cout.tie(0);
//	//std::cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y;
//	scanf("%d %d\n%d %d\n%d %d", &a.x, &a.y, &b.x, &b.y, &c.x, &c.y);
//	ll area = cross(a, b, c);
//	if (area < 0) std::swap(b, c), area *= -1;
//	//std::cin >> N;
//	scanf("%d", &N);
//	while (N--) {
//		//std::cin >> d.x >> d.y;
//		scanf("%d %d", &d.x, &d.y);
//		cnt += inner();
//	}
//	//std::cout << (area >> 1) << "." << (area & 1) * 5 << "\n" << cnt << "\n";
//	printf("%d.%d\n%d\n", area >> 1, (area & 1) * 5, cnt);
//	return;
//}
//int main() { solve(); return 0; }//boj2987