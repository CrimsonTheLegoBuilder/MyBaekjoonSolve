#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
const int LEN = 101;
int N, B, A[LEN];

struct Box { int b, l; }S[LEN];
int sp;

struct Seg { int l, r, h; }seg[LEN];
int S2[LEN], sp2;


int main() {
	//freopen("boj1169.input.txt", "r", stdin);
	//while (1) {
	std::cin >> N;
	//if (!N) break;
	//	sp = 0; sp2 = 0;

	S2[sp2++] = 0;
	A[0] = 1'001;
	seg[0] = { -2'002, 0, 1'001 };
	for (int i = 1; i <= N; i++) {
		std::cin >> A[i];
		B = A[i];
		while (sp) {
			if (A[i] <= S[sp - 1].l) {
				B = std::max(B, S[sp - 1].b + 2 * A[i]);
				break;
			}
			else {
				B = std::max(B, S[sp - 1].b + 2 * S[sp - 1].l);
				--sp;
			}
		}
		seg[i] = { B - A[i], B + A[i], A[i] };
		S[sp++] = { B, A[i] };
	}
	//std::cout << seg[0].l << " " << seg[0].r << " " << seg[0].h << "\n";
	for (int i = 1; i <= N; i++) {
		//std::cout << seg[i].l << " " << seg[i].r << " " << seg[i].h << "\n";
		while (sp2) {
			int j = S2[sp2 - 1];
			if (seg[j].h > seg[i].h) {  //pre > now
				if (seg[j].r >= seg[i].r) break;
				seg[i].l = seg[j].r;
				S2[sp2++] = i;
				break;
			}
			else {  //pre <= now
				if (seg[j].l >= seg[i].l) {
					sp2--; continue;
				}
				S2[sp2++] = i;
				break;
			}
		}
	}
	for (int i = 1; i < sp2; i++) {
		std::cout << S2[i] << " ";
	}
	//std::cout << "\n";
	//}
	return 0;
}