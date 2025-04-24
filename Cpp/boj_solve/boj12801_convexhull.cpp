#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;
const int LEN = 1e5;

int N, M;
struct Pos {
	int x, y;
	bool i;
	bool operator<(const Pos& p) const { return (x == p.x ? y < p.y : x < p.x); }
} LB, LI, RB, RI, B[LEN];
std::vector<Pos> I, BM, BR, BL, HR, HL;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return ((ll)d2.x - d1.x) * ((ll)d4.y - d3.y) - ((ll)d2.y - d1.y) * ((ll)d4.x - d3.x);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> B[i].x >> B[i].y;
		B[i].i = 0;
	}
	std::cin >> M;
	I.resize(M);
	for (int j = 0; j < M; j++) {
		std::cin >> I[j].x >> I[j].y;
		I[j].i = 1;
	}
	Pos IR = I[0], IL = I[0];
	for (int j = 0; j < M; j++) {
		if (cross(B[0], IL, IL, I[j]) > 0) IL = I[j];
		if (cross(B[0], IR, IR, I[j]) < 0) IR = I[j];
	}
	for (int i = 0; i < N; i++) {
		if (cross(B[0], IL, IL, B[i]) >= 0) BL.push_back(B[i]);
		if (cross(B[0], IR, IR, B[i]) <= 0) BR.push_back(B[i]);
		if (cross(B[0], IL, IL, B[i]) < 0 && cross(B[0], IR, IR, B[i]) > 0 && cross(IL, IR, IR, B[i]) >= 0) BM.push_back(B[i]);
	}

	bool FL = !BL.empty(), FR = !BR.empty();

	for (int j = 0; j < M; j++) {
		BL.push_back(I[j]);
		BR.push_back(I[j]);
	}
	if (FL) HL = monotone_chain(BL);
	if (FR) HR = monotone_chain(BR);
	int SL = HL.size();
	for (int i = 0; i < SL; i++) {
		if (HL[i].i && !HL[(i + 1) % SL].i) {
			LI = HL[i], LB = HL[(i + 1) % SL];
		}
	}
	int SR = HR.size();
	for (int i = 0; i < SR; i++) {
		if (!HR[i].i && HR[(i + 1) % SR].i) {
			RB = HR[i], RI = HR[(i + 1) % SR];
		}
	}
	int SM = BM.size(), O = 0;
	for (int i = 0; i < SM; i++) {
		bool fl = (!FL || cross(B[0], LI, LI, BM[i]) < 0 && cross(LB, LI, LI, BM[i]) < 0);
		bool fr = (!FR || cross(B[0], RI, RI, BM[i]) > 0 && cross(RB, RI, RI, BM[i]) > 0);
		O += fl && fr;
	}
	std::cout << N - O - 1 << "\n";
	return 0;
}
