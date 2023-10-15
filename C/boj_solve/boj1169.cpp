#include <iostream>
#include <algorithm>
const int LEN = 100;
int N, B, A[LEN];

struct Box {
	int b, l;
}S[LEN];
int sp;

struct Seg {
	int l, r, h;
}seg[LEN];
int S2[LEN], sp2;


int main() {
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> A[i];
		B = A[i];
		while (sp) {
			if (A[i] <= S[sp - 1].l) {
				B = std::max(B, S[sp - 1].b + 2 * A[i]);
				break;
			}
			else {
				B = std::max(B, S[sp - 1].b + 2 * S[sp-1].l);
				--sp;
			}
		}
		seg[i] = { B - A[i], B + A[i], A[i] };
		S[sp++] = { B, A[i] };
	}

	for (int i = 0; i < N; i++) {
		//std::cout << seg[i].l << " " << seg[i].r << " " << seg[i].h << "\n";
  		bool f = 1;
		while (sp2) {
			int j = S2[sp2 - 1];
			if (seg[j].h > seg[i].h) {  //pre > now
				if (seg[j].r >= seg[i].r) { f = 0;  break; }
				seg[i].l = seg[j].r;
				break;
			}
			else {  //pre <= now
				if (seg[j].l >= seg[i].l) {
					sp2--; continue;
				}
				break;
			}
		}
		if (f) S2[sp2++] = i;
	}
	for (int i = 0; i < sp2; i++) {
		std::cout << S2[i] + 1 << " ";
	}
	return 0;
}