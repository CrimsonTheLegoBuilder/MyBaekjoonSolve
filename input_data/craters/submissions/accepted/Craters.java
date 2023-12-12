import java.util.*;

public class Craters
{
	public static final int MAXC = 200;
	public static final int BORDER = 10;

	public static final double PI = 4*Math.atan(1.0);
	public static final double TOL = 1e-10;

	public static Scanner in = new Scanner(System.in);

	public static circle [] craters = new circle[MAXC];
	public static fenceSeg [] fence = new fenceSeg[2*MAXC-2];
	public static int nfence=0;

	public static boolean equal(double a, double b)
	{
		return (Math.abs(a-b) < TOL);
	}

	public static double dist(double x1, double y1, double x2, double y2)
	{
		double dx = x2-x1;
		double dy = y2-y1;
		return Math.sqrt(dx*dx+dy*dy);
	}

	public static boolean calcTangent(double x1, double y1, double r1, double x2, double y2, double r2, double startAng, calcTanRet retval)
	{
		if (r2 < r1) {
			boolean ans = calcTangent(x2, y2, r2, x1, y1, r1, startAng, retval);
			double tmp = retval.xt1;
			retval.xt1 = retval.xt2;
			retval.xt2 = tmp;
			tmp = retval.yt1;
			retval.yt1 = retval.yt2;
			retval.yt2 = tmp;
			return ans;
		}
		double dx = x2-x1;
		double dy = y2-y1;
		double dist12 = Math.sqrt(dx*dx+dy*dy);		// distance between centers
		double tdist = dist12*dist12 - (r2-r1)*(r2-r1);
		if (tdist <= 0.0)
			return false;
		tdist = Math.sqrt(tdist);							// length of tangent line
//cout << dist12 << ' ' << tdist << endl;
		double a1 = Math.atan2(dy, dx);					// angle of line between centers
		double a2 = Math.atan2(Math.abs(r2-r1), tdist);		// angle between that line and
												//   tangent line
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
		if (!equal(a3, startAng) && a3 > startAng) {
			retval.ang = a3;
			retval.xt1 = x1 + r1*Math.cos(retval.ang);
			retval.yt1 = y1 + r1*Math.sin(retval.ang);
			retval.xt2 = x2 + r2*Math.cos(retval.ang);
			retval.yt2 = y2 + r2*Math.sin(retval.ang);
			return true;
		}
		else if (!equal(a4, startAng) && a4 > startAng) {
			retval.ang = a4;
			retval.xt1 = x1 + r1*Math.cos(retval.ang);
			retval.yt1 = y1 + r1*Math.sin(retval.ang);
			retval.xt2 = x2 + r2*Math.cos(retval.ang);
			retval.yt2 = y2 + r2*Math.sin(retval.ang);
			return true;
		}
		else {
			return false;
		}
	}

	public static double getNextAngle(int ic, double startAng, int n)
	{
		double xt1, yt1, xt2, yt2, ang;
		double nextAng = 3*PI;
		fence[nfence] = new fenceSeg();
		fence[nfence].ang = -2*PI;
		boolean found = false;
		calcTanRet retval = new calcTanRet();
		for(int i=0; i<n; i++) {
			if (i == ic)
				continue;
			if (calcTangent(craters[ic].x, craters[ic].y, craters[ic].r,
						craters[i].x, craters[i].y, craters[i].r, startAng,
						retval)) {
				xt1 = retval.xt1;
				xt2 = retval.xt2;
				yt1 = retval.yt1;
				yt2 = retval.yt2;
				ang = retval.ang;
				if (ang < nextAng) {
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

	public static int inside(int ic, int n)
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
			if (Math.sqrt(dx*dx+dy*dy)+r <= craters[i].r)
				return i;
		}
		return -1;
	}

	public static int removeConcentricCircles(int n)
	{
		for(int i=0; i<n; i++) {
			int j = inside(i,n);
			if (j != -1) {
				craters[i] = craters[n-1];
				i--;
				n--;
			}
		}
		return n;
	}

	public static void main(String [] args)
	{
		int n;
		n = in.nextInt();
		for(int i=0; i<n; i++) {
			craters[i] = new circle();
			craters[i].x = in.nextDouble();
			craters[i].y = in.nextDouble();
			craters[i].r = in.nextDouble();
			craters[i].r += BORDER;
		}
		n = removeConcentricCircles(n);
		if (n==1) {
			System.out.printf("%.6f\n", 2*PI*craters[0].r);
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
			double length = (fence[0].ang-fence[nfence-1].ang+2*PI)*craters[fence[0].c1].r;
			length += dist(fence[0].x1, fence[0].y1, fence[0].x2, fence[0].y2);
			for(int i=1; i<nfence; i++) {
				length += (fence[i].ang-fence[i-1].ang)*craters[fence[i].c1].r;
				length += dist(fence[i].x1, fence[i].y1, fence[i].x2, fence[i].y2);
			}
			System.out.printf("%.6f\n", length);
		}
	}
}

class circle
{
	public double x, y, r;
}

class fenceSeg
{
	public int c1;					// first circle index
	public double x1, y1;			// intersection point on first circle
	public int c2;					// ditto for second circle
	public double x2, y2;			
	public double ang;				// angle from center of circles to tangent points
}

class calcTanRet
{
	public double xt1, yt1, xt2, yt2, ang;
}
