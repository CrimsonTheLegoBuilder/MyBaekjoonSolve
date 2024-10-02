//not my code
//credit: ekzm0204
//from oj.uz
//https://oj.uz/submission/9669
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <math.h>
#define SZ(V) ((int)((V).size()))
using namespace std;

typedef long double DBL;
class station {
public:
	int M, L, U;
	int x, y;
	int r[22];
	int s[22], w[22];
} S[22];
int N;

DBL ABS(DBL x) {
	if (x < 0) return -x;
	return x;
}
const int TYPE_START = 1;
const int TYPE_INTERSECT = 2;
const int TYPE_END = 3;

const int UP = 0;
const int DOWN = 1;

const DBL PI = 3.1415926535897932384626433832795;
const DBL ep = 1.0e-8;
bool isEqual(DBL a, DBL b) {
	bool azero = ABS(a) < ep;
	bool bzero = ABS(b) < ep;

	if (azero && bzero)
		return true;

	if (azero != bzero)
		return false;

	DBL d = ABS(a - b);

	return (d / (ABS(a) + ABS(b))) < ep;
}
class EVENT {
public:
	int type;

	DBL X;

	int ai, aj, ad;
	int bi, bj, bd;

	const bool operator < (const EVENT& e) const {
		if (ABS(X - e.X) <= ep) {
			if (type == e.type) {
				return S[ai].r[aj] > S[e.ai].r[e.aj];
			}
			return type < e.type;
		}
		return X < e.X;
	}
};

int index[22][22][2];

class bogen {
public:
	int i, j, d;
	DBL X;

	const bool operator < (const bogen& B) const {
		return i < B.i;
	}
};

bogen list[1999];
int lh = 0;
vector<EVENT> Events;
EVENT E;

vector< pair<DBL, DBL> > getCircleIntersect(int ax, int ay, int ar, int bx, int by, int br) {
	vector< pair<DBL, DBL> > res;

	int dx = bx - ax;
	int dy = by - ay;
	int dd = dx * dx + dy * dy;

	if (dd == 0)
		return res;

	if (dd >= (ar + br) * (ar + br))
		return res;

	DBL d = sqrt((DBL)dd);
	if (ar + d <= br || br + d <= ar)
		return res;

	DBL a, b, c, R;
	a = bx - ax;
	b = by - ay;
	c = br;

	R = ar;

	DBL U = R * R + a * a + b * b - c * c;

	DBL A, B, C;
	A = 4 * a * a + 4 * b * b;
	B = -4 * a * U;
	C = U * U - 4 * b * b * R * R;

	DBL Det = B * B - 4 * A * C;
	if (isEqual(Det, 0)) Det = 0;
	if (Det <= -ep || isEqual(A, 0))
		return res;

	DBL pl = sqrt(Det);
	DBL x1 = (-B + pl) / (2 * A);
	DBL x2 = (-B - pl) / (2 * A);

	DBL y1 = 0;
	DBL y2 = 0;
	if (!isEqual(x1, x2)) { // b != 0
		y1 = (U - 2 * a * x1) / (2 * b);
		y2 = (U - 2 * a * x2) / (2 * b);
	}
	else {
		y1 = sqrt(R * R - x1 * x1);
		y2 = -y1;
	}

	res.push_back(make_pair(x1 + ax, y1 + ay));
	res.push_back(make_pair(x2 + ax, y2 + ay));

	return res;
}

DBL getY(int i, int j, int d, DBL x) {
	DBL cx = S[i].x, cy = S[i].y, r = S[i].r[j];
	if (cx - r < x && x < cx + r) {
		DBL t = sqrt(r * r - (x - cx) * (x - cx));
		DBL sign = (d == UP) ? 1 : -1;
		return cy + sign * t;
	}
	return cy;
}

DBL getY(bogen& b, DBL x) {
	return getY(b.i, b.j, b.d, x);
}


DBL getArea(bogen b, DBL sx, DBL ex) {
	DBL R = S[b.i].r[b.j];

	DBL sy = getY(b, sx);
	DBL ey = getY(b, ex);

	DBL cx = S[b.i].x;
	DBL cy = S[b.i].y;

	DBL st = atan2(sy - cy, sx - cx);
	DBL et = atan2(ey - cy, ex - cx);

	DBL sign = (b.d == UP) ? 1 : -1;
	DBL dt = sign * (st - et);

	for (int t = 0; t < 10; t++) {
		if (dt < 0) dt += 2 * PI;
		else if (dt >= 2 * PI) dt -= 2 * PI;
		else
			break;
	}

	DBL area_circle = (dt / 2 - cos(dt / 2) * sin(dt / 2)) * R * R;
	DBL area_rect = (ex - sx) * (sy + ey) / 2;

	return area_rect + sign * area_circle;
}

bool chk[20][20];
DBL prob[20], allP;
class PROB {
public:
	DBL p;
	int i;
	int s;

	const bool operator < (const PROB& X)const {
		return s < X.s;
	}
};

DBL sol = 0.0;
void PROC_LIST_INTERNAL(int p, DBL x) {
	if (p < 0 || p + 1 >= lh)
		return;
	if (isEqual(list[p].X, x))
		return;

	bogen upper = list[p + 1];
	bogen lower = list[p];

	DBL uArea = getArea(upper, list[p].X, x);
	DBL lArea = getArea(lower, list[p].X, x);

	DBL mx = (x + list[p].X) / 2;
	DBL my = (getY(upper, mx) + getY(lower, mx)) / 2;

	DBL area = uArea - lArea;

	if (isEqual(area, 0.0))
		return;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < S[i].M; j++) {
			DBL R = S[i].r[j];
			DBL dx = S[i].x - mx;
			DBL dy = S[i].y - my;

			chk[i][j] = (dx * dx + dy * dy <= R * R);

		}
	}

	allP = 1.0;
	vector<PROB> P;
	PROB o;
	for (int i = 0; i < N; i++) {
		prob[i] = 1.0;
		vector <pair<int, int> > Q;

		int all = S[i].U - S[i].L + 1;
		int nowFirst = S[i].L;
		int nowLast = S[i].U;
		for (int j = 0; j < S[i].M; j++) {
			if (chk[i][j]) {
				Q.push_back(make_pair(S[i].s[j], S[i].w[j]));
			}
		}
		sort(Q.begin(), Q.end());
		reverse(Q.begin(), Q.end());
		for (int j = 0; j < SZ(Q); j++) {
			int s = Q[j].first;
			int w = Q[j].second;
			if (w < nowFirst) w = nowFirst;
			int part = nowLast - w + 1;
			if (part > 0) {
				o.p = ((DBL)part / (DBL)all);
				o.i = i;
				nowLast = w - 1;

				o.s = s;

				P.push_back(o);
			}
		}
	}
	sort(P.begin(), P.end());
	reverse(P.begin(), P.end());
	DBL E = 0.0;

	for (int i = 0; i < SZ(P); i++) {
		o = P[i];
		DBL rp = allP * o.p / prob[o.i];

		E += rp * o.s;

		allP = allP / prob[o.i];
		prob[o.i] -= o.p;
		allP = allP * prob[o.i];
	}
	list[p].X = x;
	//	fprintf(stderr, "%Lf %Lf\n", area, E);
	sol += E * area;
}

void PROC_LIST(int p, DBL x) {
	PROC_LIST_INTERNAL(p, x);
	if (0 <= p && p < lh) {
		list[p].X = x;
	}
}
void EVENT_PROCESS() {
	sort(Events.begin(), Events.end());

	lh = 0;
	for (int i = 0; i < (int)Events.size(); i++) {
		E = Events[i];

		if (E.type == TYPE_START) {
			int p = 0;
			bogen b;
			for (p = 0; p < lh; p++) {
				b = list[p];
				DBL Y = getY(b, E.X);
				if (Y > S[E.ai].y || (isEqual(Y, S[E.ai].y) && list[p].d == UP))
					break;
			}
			PROC_LIST(p - 1, E.X);

			for (int j = lh + 1; j > p + 1; j--) {
				list[j] = list[j - 2];
			}

			b.i = E.ai; b.j = E.aj;
			b.d = DOWN;
			b.X = E.X;

			list[p] = b;
			b.d = UP;
			list[p + 1] = b;

			lh += 2;
		}

		if (E.type == TYPE_END) {
			int ui = index[E.ai][E.aj][UP];
			int di = index[E.ai][E.aj][DOWN];

			PROC_LIST(ui - 1, E.X);
			PROC_LIST(ui, E.X);
			PROC_LIST(di - 1, E.X);
			PROC_LIST(di, E.X);

			int tlh = lh;
			lh = 0;
			for (int j = 0; j < tlh; j++) {
				if (j == ui || j == di) continue;

				list[lh] = list[j];
				lh++;
			}
		}

		if (E.type == TYPE_INTERSECT) {
			int plus = -1;

			vector<int> I, tI;
			DBL nextX = E.X;
			for (int ti = i; ti < SZ(Events); ti++) {
				EVENT tE = Events[ti];

				if (!isEqual(E.X, tE.X)) {
					nextX = tE.X;
					break;
				}
				if (tE.type != TYPE_INTERSECT)
					continue;

				plus++;
			}
			for (int g = 0; g + 1 < lh; g++) {
				if (isEqual(getY(list[g], E.X), getY(list[g + 1], E.X))) {
					I.push_back(g);
					I.push_back(g + 1);
				}
			}
			if (SZ(I) == 0) {
				I = I;
			}
			sort(I.begin(), I.end());
			tI = I;
			I.clear();
			for (int x = 0; x < SZ(tI); x++) {
				if (x == 0 || tI[x - 1] != tI[x])
					I.push_back(tI[x]);
			}

			int ss = -1, ee = -1;
			vector<pair<int, int> > rev;

			for (int p = 0; p <= SZ(I); p++) {
				int pi = -1;
				if (p < SZ(I)) {
					pi = I[p];
					PROC_LIST(pi - 1, E.X);
					PROC_LIST(pi, E.X);
				}

				if (ss == -1) ss = ee = pi;
				else {
					if (pi != -1 && isEqual(getY(list[ss], E.X), getY(list[pi], E.X))) {
						ee = pi;
					}
					else {
						if (ss != -1) {
							rev.push_back(make_pair(ss, ee));
						}

						ss = ee = pi;
					}
				}
			}

			DBL midX = (E.X + nextX) / 2;
			for (int r = 0; r < SZ(rev); r++) {
				int s = rev[r].first;
				int e = rev[r].second;
				vector< pair<DBL, bogen> > S;
				for (int t = s; t <= e; t++) {
					list[t].X = E.X;
					S.push_back(make_pair(getY(list[t], midX), list[t]));
				}
				sort(S.begin(), S.end());
				for (int t = s; t <= e; t++) {
					list[t] = S[t - s].second;
				}

			}

			i += plus;
		}

		// indexing
		//printf("%.0lf$-----\n", E.X);
		for (int j = 0; j < lh; j++) {
			bogen b = list[j];
			index[b.i][b.j][b.d] = j;
			//printf("%.10lf|%.10lf\n", getY(list[j], E.X), getY(list[j], E.X+0.1));
		}
		//printf("-----\n");
	}
}
int main() {
	//freopen("input.txt", "r", stdin);
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		E.ai = i;
		scanf("%d %d %d %d %d", &S[i].x, &S[i].y, &S[i].M, &S[i].L, &S[i].U);
		for (int j = 0; j < S[i].M; j++) {
			E.aj = j;
			scanf("%d %d %d", &S[i].r[j], &S[i].s[j], &S[i].w[j]);

			E.type = TYPE_START;
			E.X = S[i].x - S[i].r[j];
			Events.push_back(E);

			E.type = TYPE_END;
			E.X = S[i].x + S[i].r[j];
			Events.push_back(E);

			for (int ii = 0; ii < i; ii++) {
				if (S[i].x == S[ii].x && S[i].y == S[ii].y)
					continue;

				for (int jj = 0; jj < S[ii].M; jj++) {
					vector<pair<DBL, DBL> > P = getCircleIntersect(S[i].x, S[i].y, S[i].r[j], S[ii].x, S[ii].y, S[ii].r[jj]);

					if (P.size() == 2) {
						for (int k = 0; k < 2; k++) {
							DBL x = P[k].first;
							DBL y = P[k].second;

							E.type = TYPE_INTERSECT;
							E.X = x;

							Events.push_back(E);
						}
					}
				}
			}
		}
	}

	EVENT_PROCESS();

	printf("%.16lf\n", (double)sol);
	return 0;
}