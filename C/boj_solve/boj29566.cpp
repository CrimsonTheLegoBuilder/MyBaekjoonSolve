#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long double ld;
const ld TOL = 1e-9;

struct Pos {
	ld x, y;
	bool operator == (const Pos& p) const { return (std::abs(x - p.x) < TOL && std::abs(y - p.y) < TOL); }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (ld mag) const { return { x * mag, y * mag }; }
	ld dist() { return sqrtl(x * x + y * y); }
};
struct Circle {
	Pos c;
	ld R;
	bool operator == (const Circle& p) const { return c == p.c && std::abs(R - p.R) < TOL; }
};

std::vector<Pos> find_inx(const Circle& c1, const Circle& c2) {
	if (c1.c == c2.c && std::abs(c1.R - c2.R) > TOL) return {};
	if ((c1.c - c2.c).dist() > c1.R + c2.R) return {};
	if ((c1.c - c2.c).dist() + c1.R < c2.R || (c1.c - c2.c).dist() + c2.R < c1.R) return {};
	//if (std::abs((c1.c - c2.c).dist() - abs((c1.R + c2.R))) < TOL || std::abs((c1.c - c2.c).dist() - abs((c1.R - c2.R))) < TOL) {
	//	Pos vec = c2.c - c1.c;
	//	ld mag = c1.R / (c1.c - c2.c).dist();
	//	Pos d = c1.c + vec * mag;
	//	return { d };
	//}
	ld A = c1.R * c1.R, B = c2.R * c2.R;
	ld C = (c1.c - c2.c).dist() * (c1.c - c2.c).dist();
	ld cos_t = (A + C - B) / (2 * c1.R * (c1.c - c2.c).dist());
	ld theta = acos(cos_t);
	//std::cout << theta << "\n";
	Pos vec = c2.c - c1.c;
	ld vx1 = vec.x * cos(theta) - vec.y * sin(theta), vy1 = vec.x * sin(theta) + vec.y * cos(theta);
	ld vx2 = vec.x * cos(-theta) - vec.y * sin(-theta), vy2 = vec.x * sin(-theta) + vec.y * cos(-theta);
	Pos d1 = { c1.c.x + vx1 * c1.R / (c1.c - c2.c).dist(), c1.c.y + vy1 * c1.R / (c1.c - c2.c).dist() };
	Pos d2 = { c1.c.x + vx2 * c1.R / (c1.c - c2.c).dist(), c1.c.y + vy2 * c1.R / (c1.c - c2.c).dist() };
	return { d1, d2 };
}



int main() {
	std::cout << std::fixed;
	std::cout.precision(6);
	Circle a, b, c;
	std::cin >> a.c.x >> a.c.y >> b.c.x >> b.c.y >> c.c.x >> c.c.y >> a.R >> b.R >> c.R;
	std::vector<Pos> A = find_inx(a, b);
	std::vector<Pos> B = find_inx(b, c);
	std::vector<Pos> C = find_inx(c, a);
	for (const Pos& c1 : A) {
		//std::cout << c1.x << " " << c1.y << " c1 \n";
		for (const Pos& c2 : B) {
			//std::cout << c2.x << " " << c2.y << " c2 \n";
			for (const Pos& c3 : C) {
				//std::cout << c3.x << " " << c3.y << " c3 \n";
				if (c1 == c2 && c2 == c3) {
					std::cout << c1.x << " " << c1.y << "\n";
					return 0;
				}
			}
		}
	}
	std::cout << "Impossible\n";
	return 0;
}