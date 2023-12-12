#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;

const int MAXC = 200;
const int BORDER = 10;

const double PI = 4*atan(1.0);
const double TOL = 1e-10;

class circle
{
public:
	double x, y, r;
} craters[MAXC];

class fenceSeg
{
public:
	int c1;					// first circle index
	double x1, y1;			// intersection point on first circle
	int c2;					// ditto for second circle
	double x2, y2;
	double ang;				// angle from center of circles to tangent points
};
fenceSeg fence[2*MAXC-2];
int nfence=0;

bool equal(double a, double b)
{
	return (fabs(a-b) < TOL);
}

double dist(double x1, double y1, double x2, double y2)
{
	double dx = x2-x1;
	double dy = y2-y1;
	return sqrt(dx*dx+dy*dy);
}

bool calcTangent(double x1, double y1, double r1, double x2, double y2, double r2, double startAng, double& xt1, double& yt1, double& xt2, double& yt2, double& ang)
{
	if (r2 < r1) {
		return calcTangent(x2, y2, r2, x1, y1, r1, startAng, xt2, yt2, xt1, yt1, ang);
	}
	double dx = x2-x1;
	double dy = y2-y1;
	double dist12 = sqrt(dx*dx+dy*dy);		// distance between centers
	double tdist = dist12*dist12 - (r2-r1)*(r2-r1);
	if (tdist <= 0.0)
		return false;
	tdist = sqrt(tdist);							// length of tangent line
//cout << dist12 << ' ' << tdist << endl;
	double a1 = atan2(dy, dx);					// angle of line between centers
	double a2 = atan2(fabs(r2-r1), tdist);		// angle between that line and
												//   tangent line
//	if (r2 > r1)
//		a2 = PI/2-a2;
//cout << "a1, a2 = " << a1 << ',' << a2 << endl;
	double a3 = a1 - a2 - PI/2;					// first angle of radius to
												//   tangent point in each circle
	if (a3 < -PI)
		a3 += 2*PI;
	double a4 = a1 + a2 + PI/2;					// second angle of radius to
												//   tangent point in each circle
	if (a4 > PI)
		a4 -= 2*PI;
	if (a4 < a3) {
		double tmp = a4;
		a4 = a3;
		a3 = tmp;
	}
//cout << "a3, a4 = " << a3 << ',' << a4 << endl;
	if (!equal(a3, startAng) && a3 > startAng) {
		ang = a3;
		xt1 = x1 + r1*cos(ang);
		yt1 = y1 + r1*sin(ang);
		xt2 = x2 + r2*cos(ang);
		yt2 = y2 + r2*sin(ang);
		return true;
	}
	else if (!equal(a4, startAng) && a4 > startAng) {
		ang = a4;
		xt1 = x1 + r1*cos(ang);
		yt1 = y1 + r1*sin(ang);
		xt2 = x2 + r2*cos(ang);
		yt2 = y2 + r2*sin(ang);
		return true;
	}
	else {
		return false;
	}
}

double getNextAngle(int ic, double startAng, int n)
{
//cout << "ic, startAngle = " << ic << ',' << startAng << endl;
	double xt1, yt1, xt2, yt2, ang;
	double nextAng = 3*PI;
	fence[nfence].ang = -2*PI;
	bool found = false;
	for(int i=0; i<n; i++) {
		if (i == ic)
			continue;
		if (calcTangent(craters[ic].x, craters[ic].y, craters[ic].r,
					craters[i].x, craters[i].y, craters[i].r, startAng,
					xt1, yt1, xt2, yt2, ang)) {
			if (ang < nextAng) {
//cout << "base circle " << ic << ": new angle = " << ang << " with cicle " << i << endl;
//printf("%.10f %.10f\n", nextAng, ang);
				fence[nfence].c1 = ic;
				fence[nfence].x1 = xt1;
				fence[nfence].y1 = yt1;
				fence[nfence].c2 = i;
				fence[nfence].x2 = xt2;
				fence[nfence].y2 = yt2;
				fence[nfence].ang = ang;
				nextAng = ang;
				found = true;
			}
			else if (equal(ang, nextAng) && dist(xt1, yt1, xt2, yt2) > dist(xt1, yt1, fence[nfence].x2, fence[nfence].y2)) {
//cout << "same tangent: " << xt1 << ',' << yt1 << ' ' << fence[nfence].x1 << ',' << fence[nfence].y2 << endl;
				fence[nfence].c1 = ic;
				fence[nfence].x1 = xt1;
				fence[nfence].y1 = yt1;
				fence[nfence].c2 = i;
				fence[nfence].x2 = xt2;
				fence[nfence].y2 = yt2;
				fence[nfence].ang = ang;
				nextAng = ang;
				found = true;
			}
		}
	}
	double retVal = fence[nfence].ang;
	if (found)
		nfence++;
	return retVal;
}

int inside(int ic, int n)
{
	double x = craters[ic].x;
	double y = craters[ic].y;
	double r = craters[ic].r;
	for(int i=0; i<n; i++) {
		if (i == ic)
			continue;
		if (r > craters[i].r)
			continue;
		double dx = craters[i].x - x;
		double dy = craters[i].y - y;
		if (sqrt(dx*dx+dy*dy)+r <= craters[i].r)
			return i;
	}
	return -1;
}

int removeConcentricCircles(int n)
{
	for(int i=0; i<n; i++) {
		int j = inside(i,n);
		if (j != -1) {
			craters[i] = craters[n-1];
			i--, n--;
		}
	}
	return n;
}

int main()
{
	int n;
	cin >> n;
	for(int i=0; i<n; i++) {
		cin >> craters[i].x >> craters[i].y >> craters[i].r;
		craters[i].r += BORDER;
	}
	n = removeConcentricCircles(n);
	if (n==1) {
		printf("%.8f\n", 2*PI*craters[0].r);
	}
	else {
		double minx = craters[0].x - craters[0].r;
		int ic = 0;
		for(int i=1; i<n; i++) {
			double tmp = craters[i].x - craters[i].r;
			if (tmp < minx || (tmp == minx && craters[i].y > craters[ic].y)) {
				ic = i;
				minx = tmp;
			}
		}
		double prevAng, newAng = -2*PI;		// anything < -PI will do
		do {
			prevAng = newAng;
			newAng = getNextAngle(ic, newAng, n);
			ic = fence[nfence-1].c2;
		} while (newAng > prevAng);
/**
for(int i=0; i<nfence; i++) {
cout << fence[i].c1 << ": " << fence[i].x1 << ',' << fence[i].y1 << ' ';
cout << fence[i].c2 << ": " << fence[i].x2 << ',' << fence[i].y2 << ' ';
cout << fence[i].ang << endl;
}
/**/
		double length = (fence[0].ang-fence[nfence-1].ang+2*PI)*craters[fence[0].c1].r;
		length += dist(fence[0].x1, fence[0].y1, fence[0].x2, fence[0].y2);
		for(int i=1; i<nfence; i++) {
			length += (fence[i].ang-fence[i-1].ang)*craters[fence[i].c1].r;
			length += dist(fence[i].x1, fence[i].y1, fence[i].x2, fence[i].y2);
		}
		printf("%.8f\n", length);
	}
}
