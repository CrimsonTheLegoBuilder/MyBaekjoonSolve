#include <iostream>
#include <algorithm>

struct Pos {
	int x, y, z;
	bool operator == (const Pos& p) const {
		return (x == p.x && y == p.y && z == p.z);
	}
	Pos mid(const Pos& p) const {
		int i = (x + p.x) / 2;
		int j = (y + p.y) / 2;
		int k = (z + p.z) / 2;
		return { i, j, k };
	}
} pos[8] = {
	{0, 2, 0},  //A
	{0, 0, 0},  //B
	{2, 0, 0},  //C
	{2, 2, 0},  //D
	{0, 2, 2},  //A1
	{0, 0, 2},  //B1
	{2, 0, 2},  //C1
	{2, 2, 2},  //D1
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
	//std::cout << a1.x << a1.y << a1.z << " " << a2.x << a2.y << a2.z << " DEBUG\n";
	Pos b1 = pos[L2[0]], b2 = pos[L2[1]];
	//std::cout << b1.x << b1.y << b1.z << " " << b2.x << b2.y << b2.z << " DEBUG\n";
	bool f1 = (a1 == b1 || a1 == b2 || a2 == b1 || a2 == b2);
	bool f2 = (a1.mid(a2) == b1.mid(b2));
	//Pos m1 = a1.mid(a2), m2 = b1.mid(b2);
	//std::cout << m1.x << m1.y << m1.z << " " << m2.x << m2.y << m2.z << " DEBUG\n";
	if (f1 || f2) std::cout << "YES" << "\n";
	else std::cout << "NO" << "\n";
	return 0;
}