#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 100;

struct Pos { ll x, y; } pos[LEN];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x);
}

