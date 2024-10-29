/*

전체적인 로직은 제가 푼 코드 제출번호 70580386과 거의 비슷합니다. 교차 판정, 데이크스트라 등 로직이 맞는다면 AC가 나와야 합니다.

기하 파트에서 문제가 큽니다. 정수형만으로 해결 가능한 문제에 실수 자료형은 쓰지 않는 걸 추천드립니다. 기하 문제를 포함해서 프로그래밍에서 부동 소수점 오차는 어떻게 날 지 알 수 없어서 틀리기도 쉽고 디버깅도 까다롭습니다.

그리고 이 문제는 선분이 교차하는지만 알면 그래프 연결이 가능하기 떄문에 17386번 문제에서 AC를 받는 정수 자료형만 쓰는 로직으로 판단을 해야 정확한 판단이 가능합니다. 일차방정식을 사용한 선분 교차 알고리즘이 17386에서 AC를 받는 로직인지 확인이 필요합니다.

1
-5 0 5 0 4
-1 0 0 -5
0 -5 1 0
1 0 0 5
0 5 -1 0


*/
int main() { return 0; }
//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <random>
//#include <array>
//#include <tuple>
//#include <stack>
//typedef long long ll;
//typedef long double ld;
//typedef std::pair<int, int> pi;
//const ll INF = 1e17;
//const int LEN = 1e5 + 1;
//const ld TOL = 1e-7;
//int N, M, T, Q;
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
//struct Info { ll area, l, r; };
//
//using namespace std;
//typedef long long int ll;
//typedef pair<ll, ll> pll;
//
//pll operator-(pll x, pll y) { return { x.first - y.first, x.second - y.second }; }
//ll operator/(pll x, pll y) { return x.first * y.second - x.second * y.first; }
//
//
//bool ccw(pll x, pll y, pll z)
//{
//    pll v = y - x, u = z - y;
//    return v / u > 0;
//}
//
//ll n, i, j, k, maxi;
//pll p[100001], a;
//
//
//bool cmp(pll x, pll y)
//{
//    x = x - a, y = y - a;
//    if (x / y == 0)
//    {
//        if (x.first == y.first)
//            return x.second < y.second;
//        return x.first < y.first;
//    }
//    return x / y > 0;
//}
//
//ll dis(pll x, pll y)
//{
//    return ((x.first - y.first) * (x.first - y.first) + (x.second - y.second) * (x.second - y.second));
//}
//
//int main() {
//
//    cin >> n;
//    for (i = 1; i <= n; i++)
//        scanf("%lld %lld", &p[i].first, &p[i].second);
//
//    if (n == 1)
//        printf("0");
//    else
//    {
//
//        sort(p + 1, p + (n + 1));
//        a = p[1];
//
//        ll imsi = 1;
//        while (p[imsi] == p[1])
//        {
//            imsi++;
//            if (imsi > n)
//                break;
//        }
//        if (imsi > n)
//        {
//            printf("0");
//        }
//        else
//        {
//            sort(p + imsi, p + (n + 1), cmp);
//
//            stack<ll> s;
//
//            s.push(1), s.push(imsi);
//
//            ll first, second;
//            for (i = imsi + 1; i <= n; i++)
//            {
//                second = s.top();
//                s.pop();
//                first = s.top();
//
//                while (!ccw(p[first], p[second], p[i]))
//                {
//                    second = s.top();
//                    s.pop();
//                    if (s.empty())
//                        break;
//                    first = s.top();
//
//                }
//                s.push(second), s.push(i);
//
//            }
//
//            vector<ll> convex;
//
//            while (!s.empty())
//                convex.push_back(s.top()), s.pop();
//
//            ll aa = convex.size();
//
//            for (i = 0; i < aa; i++)
//                convex.push_back(convex[i]);
//
//            if (aa == 2)
//                maxi = dis(p[convex[0]], p[convex[1]]);
//            else
//            {
//                auto CCW = [&](int lp, int sp) -> ll {
//                    pll lpp = p[convex[lp + 1]] - p[convex[lp]];
//                    pll spp = p[convex[sp + 1]] - p[convex[sp]];
//                    return lpp / spp;
//                    };
//
//                for (int lp = 0, sp = 1; lp < aa; lp++)
//                {
//                    while (CCW(lp, sp) < 0) {
//                        sp = (sp + 1) % aa;
//                        if (maxi < dis(p[convex[lp]], p[convex[sp]]))
//                            maxi = dis(p[convex[lp]], p[convex[sp]]);
//                    }
//                    if (maxi < dis(p[convex[lp]], p[convex[sp]]))
//                        maxi = dis(p[convex[lp]], p[convex[sp]]);
//                }
//            }
//
//            cout << maxi;
//        }
//    }
//
//    return 0;
//}





////struct Pos3D {
////	ll x, y, z;
////	Pos3D(ll X = 0, ll Y = 0, ll Z = 0) : x(X), y(Y), z(Z) {}
////	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
////	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
////	bool operator < (const Pos3D& p) const { return x == p.x ? y == p.y ? z < p.z : y < p.y : x < p.x; }
////	ll operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
////	Pos3D operator / (const Pos3D& p) const {
////		Pos3D ret;
////		ret.x = y * p.z - z * p.y;
////		ret.y = z * p.x - x * p.z;
////		ret.z = x * p.y - y * p.x;
////		return ret;
////	}
////	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
////	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
////	Pos3D operator * (const ll& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
////	Pos3D operator / (const ll& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
////	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
////	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
////	Pos3D& operator *= (const ll& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
////	Pos3D& operator /= (const ll& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
////	ll Euc() const { return x * x + y * y + z * z; }
////	ld mag() const { return sqrtl(Euc()); }
////	friend std::istream& operator >> (std::istream& is, Pos3D& p) {
////		is >> p.x >> p.y >> p.z;
////		return is;
////	}
////	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) {
////		os << p.x << " " << p.y << " " << p.z << "\n";
////		return os;
////	}
////} candi[LEN], willy, MAXP{ INF, INF, INF };
////const Pos3D O3D = { 0, 0, 0 };
////using Face = std::array<int, 3>;
////std::vector<Face> Hull3D;
////struct Edge {
////	int face_num, edge_num;
////	Edge(int t = 0, int v = 0) : face_num(t), edge_num(v) {}
////};
////Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
////Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& d4) { return (d2 - d1) / (d4 - d3); }
////ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
////ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& d4) { return (d2 - d1) * (d4 - d3); }
////bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
////	ll ret = dot(d1, d3, d2);
////	return !cross(d1, d2, d3).mag() && ret >= 0;
////}
////bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
////	ll ret = dot(d1, d3, d2);
////	return !cross(d1, d2, d3).mag() && ret > 0;
////}
////int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
////	Pos3D CCW = cross(d1, d2, d3);
////	std::cout << CCW;
////	ll ret = CCW * norm;
////	std::cout << "ret: " << ret << "\n";
////	return !ret ? 0 : ret > 0 ? 1 : -1;
////}
////ld area(const std::vector<Pos3D>& H, const Pos3D& norm) {
////	ll ret = 0;
////	if (H.size() < 3) return ret;
////	Pos3D O = H[0];
////	int sz = H.size();
////	for (int i = 0; i < sz; i++) {
////		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
////		ret += cross(O, cur, nxt) * norm / norm.Euc();
////	}
////	return std::abs(ret * .5);
////}
////bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
////	return cross(a, b, c) * (p - a) > 0;
////}
//
//struct Pos3D {
//	ld x, y, z;
//	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
//	bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
//	bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
//	bool operator < (const Pos3D& p) const { return zero(x - p.x) ? zero(y - p.y) ? z < p.z : y < p.y : x < p.x; }
//	ld operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
//	Pos3D operator / (const Pos3D& p) const {
//		Pos3D ret;
//		ret.x = y * p.z - z * p.y;
//		ret.y = z * p.x - x * p.z;
//		ret.z = x * p.y - y * p.x;
//		return ret;
//	}
//	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
//	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
//	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
//	Pos3D operator / (const ld& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
//	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
//	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
//	Pos3D& operator *= (const ld& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
//	Pos3D& operator /= (const ld& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
//	ld Euc() const { return x * x + y * y + z * z; }
//	ld mag() const { return sqrtl(Euc()); }
//	ld lon() const { return atan2(y, x); }
//	ld lat() const { return atan2(z, sqrtl(x * x + y * y)); }
//	Pos3D unit() const { return *this / mag(); }
//	Pos3D norm(const Pos3D& p) const { return (*this / p).unit(); }
//	Pos3D rotate(const ld& th, const Pos3D& axis) const {
//		ld SIN = sin(th), COS = cos(th);
//		Pos3D u = axis.unit();
//		return u * (*this * u) * (1 - COS) + (*this * COS) - (*this / u) * SIN;
//	}
//	friend std::istream& operator >> (std::istream& is, Pos3D& p) {
//		is >> p.x >> p.y >> p.z;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) {
//		os << p.x << " " << p.y << " " << p.z << "\n";
//		return os;
//	}
//};
//const Pos3D O3D = { 0, 0, 0 };
//Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
//ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
//int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
//	Pos3D CCW = cross(d1, d2, d3);
//	ld ret = CCW * norm;
//	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
//}
//bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
//	return cross(a, b, c) * (p - a) > 0;
//}
//int main() {
//	std::cout << std::fixed;
//	std::cout.precision(15);
//	Pos3D a, b, c;
//	std::cin >> a >> b >> c;
//	std::cout << "above: " << above(a, b, c, O3D) << "\n";
//	Pos3D n = cross(a, b, c);
//	int x = ccw(a, b, c, n);
//	std::cout << x << "\n";
//	std::cout << (a - b);
//	std::cout << (b - c);
//	std::cout << "ccw a b: " << ccw(a, b, O3D, n) << "\n";
//	std::cout << "ccw b c: " << ccw(b, c, O3D, n) << "\n";
//	std::cout << "ccw c a: " << ccw(c, a, O3D, n) << "\n";
//	std::cout << x << "\n";
//	std::cout << n.Euc() << "\n";
//	std::cout << n;
//	std::cout << n.mag() << "\n";
//	std::cout << n * (O3D - a) << "\n";
//}
//
///*
//
//1 3
//-280000 -450000 0
//-437600 259800 148400
//275680 452688 1484
//
//9223372036854775807
//9011369165176521728
//312533749465952362168320
//
//*/