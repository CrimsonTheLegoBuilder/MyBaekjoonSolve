#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <iomanip>
#include <limits>
#include <ctime>

using namespace std;

//Decleration of constants
double EPS = 1e-9,
		PI = acos( -1.0 );

//Decleration of datatypes
struct point
{
	double x, y;
	string name = "";
	point() { x = y = 0.0; }
	point(double _x, double _y) : x(_x), y(_y) {}
	bool operator< (point other) const {
		if ( fabs(x - other.x) > EPS ) {
			return x < other.x;
		} else {
			return y < other.y;
		}
	}

	bool operator= (point other) const {
		return (fabs(x - other.x) < EPS && (fabs(y - other.y) < EPS)); 
	}
};

//b = 1 : non vertical, b = 0 vertical line
struct line {
	double a, b, c;
	line() { a = b = c = 0.0; }
	line(double _a, double _b, double _c) : a(_a), b(_b), c(_c) {}
};

struct vec {
	double x, y;
	vec() { x = y = 0.0; }
	vec(double _x, double _y) : x(_x), y(_y) {}
};

struct circle {
	double x, y, r;
	circle() { x = y = r = 0.0; }
	circle(double _x, double _y, double _r) : x(_x), y(_y), r(_r) {}
};

vector<circle> cs;
vector<point> ps;
vector<vector<double> > gw;
vector<vector<int> > g;
vector<int> pred;
vector<double> gdist;

#define INF (1LL<<60)

void mydijkstra(int s){
	int n = g.size();
	gdist.assign(n, INF);
	pred.assign(n, -1);
	gdist[s] = 0.0;
	priority_queue<pair<double,int> > Q;
	Q.push({-gdist[s],s});
	vector<bool> seen (n);

	while(!Q.empty()){
		pair<bool,int> p = Q.top();
		int w = p.second;
		Q.pop();
		if(seen[w]) continue;
		seen[w] = true;
		for(auto to : g[w]){

			if(seen[to] || gdist[to] <= gdist[w] + gw[w][to]) continue;
			pred[to] = w;
			gdist[to] = gdist[w] + gw[w][to];
			Q.push({-gdist[to],to});
		}
	}
}


double dist(point p1, point p2) {
	//hypot(dx, dy) returns sqrt(dx * dx + dy * dy)
	return hypot(p1.x - p2.x, p1.y - p2.y);
}

vec toVec(point a, point b) {
	return vec(b.x - a.x, b.y - a.y);
}

vec scale(vec v, double s) {
	return vec(v.x * s, v.y * s);
}

point translate(point p, vec v) {
	return point(p.x + v.x, p.y + v.y);
}

double dot(vec a, vec b) { return (a.x * b.x + a.y * b.y); }

double norm_sq(vec v) { return v.x * v.x + v.y * v.y; }

//return the distance from p to the line defined by 
//two points a and b (a and b must be different)
//the closest point is stored in the 4th parameter
double distToLine(point p, point a, point b, point &c) {
	// formula: c = a + u * ab
	vec ap = toVec(a, p), ab = toVec(a, b);
	double u = dot(ap, ab) / norm_sq(ab);
	c = translate(a, scale(ab,u));
	return dist(c, p);
}

//returns the distance from p to the line segment ab defined by
//two points a and b (still OK if a == b)
//th closest point is stored in the 4th parameter
double distToLineSegment(point p, point a, point b, point &c) {
	vec ap = toVec(a, p), ab = toVec(a, b);
	double u = dot(ap, ab) / norm_sq(ab);
	if( u < 0.0 ) { 
		c = point(a.x, a.y);
		return dist(p, a);
	}
	if( u > 1.0 ) {
		c = point(b.x, b.y);
		return dist(p, b);
	}
	return distToLine(p,a,b,c);
}

//returns angle aob in rad
double angle(point a, point o, point b) {
	vec oa = toVec(o, a), ob = toVec(o, b);
	return acos(dot(oa,ob) / sqrt(norm_sq(oa) * norm_sq(ob)));
}

double cross(vec a, vec b) { return a.x * b.y - a.y * b.x; }

// note: to accept collinear points, we have to change the ' > 0 '
// returns true if the point r is on the left side of the line pq
bool ccw(point p, point q, point r) {
	return cross(toVec(p,q), toVec(p,r)) > 0;
}

// return true if the point r is on the same line as the line pq
bool collinear(point p, point q, point r) {
	return fabs(cross(toVec(p, q), toVec(p,r))) < EPS;
}

void pointsToLine(point p1, point p2, line &l){
	if (fabs(p1.x - p2.x) < EPS ) {
		l.a = 1.0; l.b = 0.0; l.c = -p1.x;
	} else {
		l.a = -(double)(p1.y - p2.y) / (p1.x - p2.x);
		l.b = 1.0; 	
		l.c = -(double)(l.a * p1.x) - p1.y;
	}
}

bool areParallel(line l1, line l2){ 
	return (fabs(l1.a - l2.a) < EPS) && (fabs(l1.b - l2.b) < EPS);
}

bool areSame(line l1, line l2) {
	return areParallel(l1,l2) && (fabs(l1.c - l2.c) < EPS);
}

// return true (+ intersection point) if two lines are intersect
bool areIntersect(line l1, line l2, point &p) {
	if(areParallel(l1,l2)) return false;
	//solve system of 2 linear algebraic equations with 2 unkowns
	p.x = (l2.b * l1.c - l1.b * l2.c) / (l2.a * l1.b - l1.a * l2.b);
	//special case: test for vertical line to avoid devision by zero
	if(fabs(l1.b) > EPS) p.y = -(l1.a * p.x + l1.c);
	else				 p.y = -(l2.a * p.x + l2.c);
	return true;
}

double DEG_to_RAD(double theta){
	return theta * PI / 180.0;
}

//rotate p by theta degree CCW w.r.t. origin (0, 0)
point rotate(point p, double theta) {
	double rad = DEG_to_RAD(theta); //multiply theta with PI / 180.0
	return point(p.x * cos(rad) - p.y * sin(rad),
				 p.x * sin(rad) + p.y * cos(rad));
}

// return whether two circles are the same
bool circleCircleCoincide( circle c0, circle c1) {
	point p0(c0.x, c0.y), p1(c1.x, c1.y);
	if( fabs(dist(p0,p1)) < EPS && c0.r == c1.r ) return true;
	return false;
}

// return whether two circles have only 1 intersection point
bool circleCircleTouch( circle c0, circle c1) {
	point p0(c0.x, c0.y), p1(c1.x, c1.y);
	if( fabs(dist(p0,p1) - c0.r - c1.r) < EPS) return true;
	return false;
}

bool circleCircleIntersection( circle c0, circle c1, point &p3, point &p4) {
	point p0(c0.x, c0.y), p1(c1.x, c1.y);
	double d = dist(p0,p1), r0 = c0.r, r1 = c1.r;
	if( (d > r0 + r1) or (d < fabs(r0 - r1)) ) return false;
	double a = (r0 * r0 - r1 * r1 + d * d) / (2 * d);
	double h = sqrt(r0 * r0 - a * a);
	point p2(p0.x + a * (p1.x - p0.x) / d,
			 p0.y + a * (p1.y - p0.y) / d);
	p3.x = p2.x + h * (p1.y - p0.y) / d;
	p3.y = p2.y - h * (p1.x - p0.x) / d;
	p4.x = p2.x - h * (p1.y - p0.y) / d;
	p4.y = p2.y + h * (p1.x - p0.x) / d;
	return true;
}

bool circleLineSegmentIntersection( point E, point L, circle c0, double &t1, double &t2) {
	point C(c0.x, c0.y);
	double r = c0.r;
	vec d = toVec(E, L), f = toVec(C, E);
	double 	a = dot(d, d), 
			b = 2 * dot(f, d), 
			c = dot(f,f) - r * r,
			discriminant = b * b - 4 * a * c;
	if( discriminant + EPS < 0 ) return false;
	//Ray didn't miss sphere, so there is a solution to the equation
	discriminant = sqrt( discriminant );

	//either solution may be on or off the ray so need to test both
	//t1 is always the smaller value, because BOTH discriminant and
	//a are nonnegative
 	t1 = (-b - discriminant) / (2 * a);
	t2 = (-b + discriminant) / (2 * a);

	// 3x HIT cases: 
	//		-o->					--|-->  |			|  --|->
	// Impale(t1 hit, t2 hit), Poke(t1 hit, t2 > 1), Exitwound(t1 < 0, t2 hit)

	// 3x MISS cases:
	//		->  o 						o ->				| -> |
	// Fallshort(t1 > 1, t2 > 1), Past (t1 < 0, t2 < 0), Completely inside (t1 < 0, t2 > 1)

	// if ( t1 >= 0 && t1 <= 1 || t2 >= 0 && t2 <= 1 ) {
	// 	//t1 is the intersection, and it's closer than t2
	// 	//since t1 uses -b - discriminant)
	// 	//Impale, Poke or Exitwound
	// 	// vec x1 = scale(d,t1)
	// 	// p = point(translate(E,x1));
	// 	return true;
	// }
	// return false;

	if(t1 < 0.0 && t2 < 0.0 || t1 > 1.0 && t2 > 1.0 ) return false;
	else return true;
}

void printpoint(point p) {
	cout << p.x << " " << p.y << endl;
}

void printGraph() {
	for(int i = 0; i < g.size(); i++){
		cout << i << ": ";
		for(int j = 0; j < g[i].size(); j++){
			cout << g[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {
	double xb, yb, xm, ym, n;
	cin >> xb >> yb >> xm >> ym >> n;
	point start(xb,yb), target(xm,ym);
	ps.push_back(start); //ps[0] is start
	ps.push_back(target); //ps[1] is end
	ps[0].name = "start";
	ps[1].name = "end";

	//Readin circles
	cs.assign(n, circle(0.0,0.0,0.0));
	for(int i = 0; i < n; i++){
		cin >> cs[i].x >> cs[i].y >> cs[i].r;
	}

	//Determine intersections between circles
	clock_t begin = clock();
	int64_t pcnt = 2;
	for(int i = 0; i < (n-1); i++){
		for(int j = i + 1; j < n; j++){
			if(circleCircleCoincide(cs[i],cs[j])) continue;
			point p, s;
			if(!circleCircleIntersection(cs[i],cs[j],p,s)) continue;
			p.name = to_string(i+1) + "_" + to_string(j+1) + "a";
			s.name = to_string(i+1) + "_" + to_string(j+1) + "b";
			if(circleCircleTouch(cs[i],cs[j])) { ps.push_back(p); pcnt++; }
			else {
				pcnt += 2;
				ps.push_back(p);
				ps.push_back(s);
			}
		}
	}
	clock_t end = clock();
	clog << "circle intersection: " << double(end - begin) / CLOCKS_PER_SEC << " sec" << endl;

	//Graph data
	gw.assign(ps.size(), vector<double> (ps.size(),0.0));
	g.assign(ps.size(), vector<int> ());
	int64_t ecnt = 0;

	//Build possible graph
	begin = clock();
	double sortingtime = 0.0;
	for(int i = 0; i < (ps.size()); i++) {
		for(int j = i + 1; j < ps.size(); j++){
			gw[i][j] = gw[j][i] = dist(ps[i],ps[j]);
			vector<pair<double,bool> > openclose;
			for(int k = 0; k < cs.size(); k++){
				double t1, t2;
				if(!circleLineSegmentIntersection(ps[i],ps[j],cs[k],t1,t2)) continue;
				t1 = max(0.0,t1);
				t2 = min(1.0,t2);
				openclose.push_back({t1,false});
				openclose.push_back({t2,true});
			}

			clock_t sortbegin = clock();
			sort(openclose.begin(), openclose.end());
			clock_t sortend = clock();
			sortingtime += (sortend - sortbegin);

			double lastOpen = 0.0, totalOpen = 0.0;
			int openCnt = 0;
			// cout << "openclose starts" << endl;
			for(int k = 0; k < openclose.size(); k++) {
				pair<double,bool> p = openclose[k];

				openCnt += (!p.second ? 1 : -1);
				if(openCnt == 1 && !p.second) {
					lastOpen = p.first;
				}
				if(openCnt == 0 && p.second) {
					totalOpen += p.first - lastOpen;
				}
			}

			//Line was totally inclosed in circles and can be considered
			if( fabs(totalOpen - 1.0) < EPS ) {
				ecnt++; //count total amount of edges
				g[i].push_back(j);
				g[j].push_back(i);
			}
		}
	}
	end = clock();
	clog << "linecover: " << double(end - begin) / CLOCKS_PER_SEC << " sec" << endl;
	clog << "includes sorttime: " << sortingtime / CLOCKS_PER_SEC << " sec" << endl;
	begin = clock();
	mydijkstra(0);
	end = clock();
	clog << "shortest path: " << double(end - begin) / CLOCKS_PER_SEC << " sec" << endl;
	clog << "points: " << pcnt << endl;
	clog << "edges: " << ecnt << endl;
	int cur = 1;
	while(cur != -1){
		clog << ps[cur].name << " " << ps[cur].x << " " << ps[cur].y << endl;
		cur = pred[cur];	
	}
	if(gdist[1] == INF) cout << "impossible" << endl;
	else cout << fixed << setprecision(numeric_limits<double>::digits10 + 1) << gdist[1] << endl;
	return 0;
}
