#include <iostream>
#include <algorithm>

struct Pos {
	int x, y, z;
	bool operator == (const Pos& p) const { return (x == p.x && y == p.y && z == p.z); }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos operator * (const int& s) const { return { x * s, y * s, z * s }; }
	Pos operator / (const int& s) const { return { x / s, y / s, z / s }; }
} pos[8] = {
	{0, 2, 0},  // A
	{0, 0, 0},  // B
	{2, 0, 0},  // C
	{2, 2, 0},  // D
	{0, 2, 2},  // A1
	{0, 0, 2},  // B1
	{2, 0, 2},  // C1
	{2, 2, 2},  // D1
};
int L1[2]{}, L2[2]{};
char s[5];


int main() {
	std::cin >> s;
	for (int i = 0, j = -1; s[i]; i++) {
		if (s[i] == '1') L1[j] += 4;
		else L1[++j] = s[i] - 'A';
	}	
	std::cin >> s;
	for (int i = 0, j = -1; s[i]; i++) {
		if (s[i] == '1') L2[j] += 4;
		else L2[++j] = s[i] - 'A';
	}
	Pos a1 = pos[L1[0]], a2 = pos[L1[1]];
	Pos b1 = pos[L2[0]], b2 = pos[L2[1]];
	Pos m1 = (a1 + a2) / 2;
	Pos m2 = (b1 + b2) / 2;
	bool f1 = (a1 == b1 || a1 == b2 || a2 == b1 || a2 == b2);
	bool f2 = m1 == m2;
	std::cout << ((f1 || f2) ? "Yes\n" : "No\n");
	return 0;
}
