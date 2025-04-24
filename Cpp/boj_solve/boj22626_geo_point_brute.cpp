#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
//typedef double ld;
typedef long double ld;
const ld TOL = 1e-9;  //tolerance
const ld PI = acos(-1);  //PI = 3.14159265...
const int LEN = 100;
int N;

bool z(ld x) { return std::fabs(x) < TOL; }
struct Line {
	ld vy, vx, c;
	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; } //cross
};
struct Pos {
	ld x, y;
	Pos operator + (const Pos& p) const { return { x + p.x , y + p.y }; }
	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
	bool operator != (const Pos& p) const { return !z(x - p.x) || !z(y - p.y); }
}P[LEN][2];
Line L(const Pos& d1, const Pos& d2) {
	ld vy = d2.y - d1.y, vx = d1.x - d2.x;
	ld c = vy * d1.x + vx * d1.y;
	return { vy, vx, c };
}
ld a(const Line& l) { return atan2(l.vy, -l.vx); }
ld bisect(const Line& l1, const Line& l2) {
	ld a1 = a(l1), a2 = a(l2);
	return (a1 + a2) * .5;
}
Line L(const Pos& d, ld theta) {
	if (z(PI * .5 - std::abs(theta))) return { 1, 0, d.x };
	ld vy = tan(theta);  //std::cout << theta << " " << PI * .5 << " Line consist DEBUG\n";
	if (std::abs(theta) > (PI * .5)) vy *= -1;
	ld vx = (std::abs(theta) > (PI * .5)) ? 1 : -1;
	return { vy, vx, vy * d.x + vx * d.y };
}
Pos IP(const Line& l1, const Line& l2) {
	ld det = l1 / l2;  //ld det = l1.vy * l2.vx - l1.vx * l2.vy;  //ld det = cross(l1, l2);
	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
ld dist(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ccw = cross(d1, d2, d3), l = dist(d1, d2);
	return std::abs(ccw / l);
}
ld norm(ld a) {
	while (a < 0) a += PI;
	while (a >= PI) a -= PI;
	return a;
}
void brute() {
	for (int i = 0; i < N; i++) {
		std::cin >> P[i][0].x >> P[i][0].y >> P[i][1].x >> P[i][1].y;
	}
	Pos p1, p2, p3, p4;
	Line l1 = L(P[0][0], P[0][1]);
	Line l2 = L(P[1][0], P[1][1]);
	Line l3 = L(P[2][0], P[2][1]);
	if (z(l1 / l2) && z(l1 / l3)) { std::cout << "None\n"; return; }  //if 3 lines are parallel, answer doesn't exists
	p1 = IP(l1, l2);
	bool f = 1;
	for (int i = 2; i < N; i++) {
		if (p1 != IP(l1, L(P[i][0], P[i][1]))) f = 0;
	}
	if (f) { std::cout << p1.x << " " << p1.y << "\n"; return; }  //if all lines meets at only one point, answer == p1
	if (N == 3) { std::cout << "Many\n"; return; }  //if N == 3 and 3 lines are not parallel at the same time, answers are 4 or 2
	std::vector<Pos> tmp;
	if (!z(l1 / l2) && !z(l1 / l3) && !z(l2 / l3)) {  //if 3 lines are not parallel at the same time
		ld a1 = bisect(l1, l2), a2 = bisect(l1, l3);
		ld a3 = a1 + PI * .5, a4 = a2 + PI * .5;
		p1 = IP(L(IP(l1, l2), a1), L(IP(l1, l3), a2));
		p2 = IP(L(IP(l1, l2), a1), L(IP(l1, l3), a4));
		p3 = IP(L(IP(l1, l2), a3), L(IP(l1, l3), a2));
		p4 = IP(L(IP(l1, l2), a3), L(IP(l1, l3), a4));
		tmp = { p1, p2, p3, p4 };
	}
	else {  //if 2 lines are parallel && not one at the same time
		Line lp1, lp2, la;
		if (z(l1 / l2)) lp1 = l1, lp2 = l2, la = l3;
		else if (z(l2 / l3)) lp1 = l2, lp2 = l3, la = l1;
		else if (z(l1 / l3)) lp1 = l1, lp2 = l3, la = l2;
		ld a1 = norm(bisect(lp1, la)), a2 = norm(bisect(lp2, la));
		ld a3 = norm(a1 + PI * 0.5), a4 = norm(a2 + PI * 0.5);
		p1 = z(a1 - a2) ? IP(L(IP(lp1, la), a1), L(IP(lp2, la), a4)) : IP(L(IP(lp1, la), a1), L(IP(lp2, la), a2));
		p2 = z(a3 - a2) ? IP(L(IP(lp1, la), a3), L(IP(lp2, la), a4)) : IP(L(IP(lp1, la), a3), L(IP(lp2, la), a2));
		tmp = { p1, p2 };
	}
	int cnt = 0;
	Pos ans;
	for (const Pos& p : tmp) {
		ld D = dist(P[0][0], P[0][1], p);
		f = 1;
		for (int i = 3; i < N; i++) {
			if (!z(D - dist(P[i][0], P[i][1], p))) { f = 0; break; }
		}
		if (f) { cnt++; ans = p; }
	}
	if (cnt) {
		if (cnt > 1) { std::cout << "Many\n"; return; }
		else { std::cout << ans.x << " " << ans.y << "\n"; return; }
	}
	std::cout << "None\n";
	return;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N;
	if (N <= 2) { std::cout << "Many\n"; return 0; }  //if N == 1, all points are answer && N == 2, bisect line is answer
	brute();
	return 0;
}


//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <vector>
//typedef long long ll;
//typedef long double ld;
//const ld TOL = 1e-9;
//const ld PI = acos(-1);
//const int LEN = 100;
//int N;
//
//bool z(ld x) { return std::fabs(x) < TOL; }
//struct Line {
//	ld vy, vx, c;
//	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; } //cross
//};
//struct Pos {
//	ld x, y;
//	Pos operator + (const Pos& p) const { return { x + p.x , y + p.y }; }
//	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
//	bool operator != (const Pos& p) const { return !z(x - p.x) || !z(y - p.y); }
//}P[LEN][2], p1, p2, p3, p4;
//Line L(const Pos& d1, const Pos& d2) {
//	ld vy = d2.y - d1.y, vx = d1.x - d2.x;
//	ld c = vy * d1.x + vx * d1.y;
//	return { vy, vx, c };
//}
//ld a(const Line& l) { return atan2(l.vy, -l.vx); }
//ld bisect(const Line& l1, const Line& l2) {
//	ld a1 = a(l1), a2 = a(l2);
//	return (a1 + a2) * .5;
//}
//Line L(const Pos& d, ld theta) {
//	if (z(PI * .5 - std::abs(theta))) return { 1, 0, d.x };
//	ld vy = tan(theta);  //std::cout << theta << " " << PI * .5 << " Line consist DEBUG\n";
//	if (std::abs(theta) > (PI * .5)) vy *= -1;
//	ld vx = (std::abs(theta) > (PI * .5)) ? 1 : -1;
//	return { vy, vx, vy * d.x + vx * d.y };
//}
//Pos IP(const Line& l1, const Line& l2) {
//	ld det = l1 / l2;  //ld det = l1.vy * l2.vx - l1.vx * l2.vy;  //ld det = cross(l1, l2);
//	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
//}
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//ld dist(const Pos& d1, const Pos& d2) {
//	return hypot((d1.x - d2.x), (d1.y - d2.y));
//}
//ld dist(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ld ccw = cross(d1, d2, d3), l = dist(d1, d2);
//	return std::abs(ccw / l);
//}
//ld norm(ld a) {
//	while (a < 0) a += PI;
//	while (a >= PI) a -= PI;
//	return a;
//}
//
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(15);
//	std::cin >> N;
//	if (N <= 2) { std::cout << "Many\n"; return 0; }  //if N == 1, all points are answer && N == 2, bisects line is answer
//	for (int i = 0;i<N;i++){
//		std::cin >> P[i][0].x >> P[i][0].y >> P[i][1].x >> P[i][1].y;
//	}
//	Line l1 = L(P[0][0], P[0][1]);
//	Line l2 = L(P[1][0], P[1][1]);
//	Line l3 = L(P[2][0], P[2][1]);
//	if (z(l1 / l2) && z(l1 / l3)) { std::cout << "None\n"; return 0; }  //if 3 lines are parallel, answer doesn't exists
//	p1 = IP(l1, l2);
//	bool f = 1;
//	for (int i = 2; i < N; i++) {
//		if (p1 != IP(l1, L(P[i][0], P[i][1]))) f = 0;
//	}
//	if (f) { std::cout << p1.x << " " << p1.y << "\n"; return 0; }  //if all lines meets at only one point, answer == p1
//	if (N == 3) { std::cout << "Many\n"; return 0; }  //if N == 3 and 3 lines are not parallel at the same time, answers are 4 or 2
//	//std::cout << l1 / l2 << " " << l1 / l3 << " " << l2 / l3 << " cross value DEBUG\n";
//	if (!z(l1 / l2) && !z(l1 / l3) && !z(l2 / l3)) {  //if 3 lines are not parallel at the same time
//		ld a1 = bisect(l1, l2), a2 = bisect(l1, l3);
//		ld a3 = a1 + PI * .5, a4 = a2 + PI * .5;
//		p1 = IP(L(IP(l1, l2), a1), L(IP(l1, l3), a2));
//		p2 = IP(L(IP(l1, l2), a1), L(IP(l1, l3), a4));
//		p3 = IP(L(IP(l1, l2), a3), L(IP(l1, l3), a2));
//		p4 = IP(L(IP(l1, l2), a3), L(IP(l1, l3), a4));
//		//std::cout << p1.x << " " << p1.y << " p1 NP DEBUG\n";
//		//std::cout << p2.x << " " << p2.y << " p2 NP DEBUG\n";
//		//std::cout << p3.x << " " << p3.y << " p3 NP DEBUG\n";
//		//std::cout << p4.x << " " << p4.y << " p4 NP DEBUG\n";
//		std::vector<Pos> tmp = { p1, p2, p3, p4 };
//		int cnt = 0;
//		Pos ans;
//		for (const Pos& p : tmp) {
//			ld D = dist(P[0][0], P[0][1], p);
//			//std::cout << D << " D NP\n";
//			f = 1;
//			for (int i = 3; i < N; i++) {
//				//std::cout << D << " " << dist(P[i][0], P[i][1], p) << " " << i << " comp NP\n";
//				if (!z(D - dist(P[i][0], P[i][1], p))) { f = 0; break; }
//			}
//			if (f) { cnt++; ans = p; }
//		}
//		if (cnt) {
//			if (cnt > 1) { std::cout << "Many\n"; return 0; }
//			else {
//				std::cout << ans.x << " " << ans.y << "\n";
//				return 0;
//			}
//		}
//		std::cout << "None\n";
//		return 0;
//	}
//	Line lp1, lp2, la;  //if 2 lines are parallel && not one at the same time
//	if (z(l1 / l2)) lp1 = l1, lp2 = l2, la = l3;
//	else if (z(l2 / l3)) lp1 = l2, lp2 = l3, la = l1;
//	else if (z(l1 / l3)) lp1 = l1, lp2 = l3, la = l2;
//	ld a1 = norm(bisect(lp1, la)), a2 = norm(bisect(lp2, la));
//	ld a3 = norm(a1 + PI * 0.5), a4 = norm(a2 + PI * 0.5);
//	//std::cout << a1 << " " << a2 << " " << a3 << " " << a4 << " angle P DEBUG\n";
//	//Pos ax = IP(lp1, la); std::cout << ax.x << " " << ax.y << " ax P DEBUG\n";
//	//Pos ay = IP(lp2, la); std::cout << ay.x << " " << ay.y << " ay P DEBUG\n";
//	//Line lx = L(IP(lp2, la), a2); std::cout << lx.vy << " " << lx.vx << " " << lx.c << " lx P DEBUG\n";
//	//Line ly = L(IP(lp2, la), a4); std::cout << ly.vy << " " << ly.vx << " " << ly.c << " ly P DEBUG\n";
//	p1 = z(a1 - a2) ? IP(L(IP(lp1, la), a1), L(IP(lp2, la), a4)) : IP(L(IP(lp1, la), a1), L(IP(lp2, la), a2));
//	p2 = z(a3 - a2) ? IP(L(IP(lp1, la), a3), L(IP(lp2, la), a4)) : IP(L(IP(lp1, la), a3), L(IP(lp2, la), a2));
//	//std::cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << " p1 p2 P DEBUG\n";
//	std::vector<Pos> tmp = { p1, p2 };
//	int cnt = 0;
//	Pos ans;
//	for (const Pos& p : tmp) {
//		ld D = dist(P[0][0], P[0][1], p);
//		//std::cout << D << " D P\n";
//		f = 1;
//		for (int i = 3; i < N; i++) {
//			//std::cout << D << " " << dist(P[i][0], P[i][1], p) << " " << i << " comp P\n";
//			if (!z(D - dist(P[i][0], P[i][1], p))) { f = 0; break; }
//		}
//		if (f) { cnt++; ans = p; }
//	}
//	if (cnt) {
//		if (cnt > 1) { std::cout << "Many\n"; return 0; }
//		else {
//			std::cout << ans.x << " " << ans.y << "\n";
//			return 0;
//		}
//	}
//	std::cout << "None\n";
//	return 0;
//}