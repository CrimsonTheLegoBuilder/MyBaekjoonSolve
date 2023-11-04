#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
typedef long long ll;
int H, W, r, h0, w0, h1, w1, N, X, cnt = 0;

struct Pos { int h, w; }P, B[12];

int dist(const Pos& b, const Pos& p) {
	return (p.h - b.h) * (p.h - b.h) + (p.w - b.w) * (p.w - b.w);
}

void makeBtn() {
	B[1] = { h0 + r, w0 + r };
	B[2] = { h0 + r, w0 + 3 * r + w1 };
	B[3] = { h0 + r, w0 + 5 * r + 2 * w1 };
	B[4] = { h0 + 3 * r + h1, w0 + r };
	B[5] = { h0 + 3 * r + h1, w0 + 3 * r + w1 };
	B[6] = { h0 + 3 * r + h1, w0 + 5 * r + 2 * w1 };
	B[7] = { h0 + 5 * r + 2 * h1, w0 + r };
	B[8] = { h0 + 5 * r + 2 * h1, w0 + 3 * r + w1 };
	B[9] = { h0 + 5 * r + 2 * h1, w0 + 5 * r + 2 * w1 };
	B[10] = { h0 + 7 * r + 3 * h1, w0 + r };
	B[0] = { h0 + 7 * r + 3 * h1, w0 + 3 * r + w1 };
	B[11] = { h0 + 7 * r + 3 * h1, w0 + 5 * r + 2 * w1 };
}

int point(const Pos& p) {
	for (int i = 0; i < 12; i++) {
		Pos b = B[i];
		if (dist(b, p) <= r * r) {
			std::cout << "O\n";
			return i + 48;
		}
	}
	std::cout << "X\n";
	return -1;
}

std::vector<int> console;
std::vector<int> total[1 << 11];


int main() {
	std::cin >> H >> W >> r >> h0 >> w0 >> h1 >> w1;
	makeBtn();
	std::cin >> N;
	while (N--) {
		std::cin >> P.h >> P.w;
		X = point(P);
		if (!~X) continue;
		if (48 <= X && X <= 57) console.push_back(X);
		if (X == 58) if (console.size()) console.pop_back();
		if (X == 59 && console.size()) {
			for (const int& i : console) {
				total[cnt].push_back(i);
			}
			console.clear();
			cnt++;
		}
	}
	std::cout << cnt << "\n";
	for (int i = 0; i < cnt; i++) {
		for (const char& j : total[i]) {
			std::cout << j;
		}
		std::cout << "\n";
	}

}

