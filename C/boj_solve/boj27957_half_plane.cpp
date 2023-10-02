#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>

#define MAX 50005
#define INF (1e9)
#define DEBUG 0

#define all(v) v.begin(), v.end()
#define compress(v) sort(all(v)), v.erase(unique(all(v)), v.end())

using namespace std;
typedef long long ll;
typedef double ld;
typedef pair<ll, ll> pll;
typedef pair<ld, ld> pdd;
typedef complex<long double> cpx;

inline int readChar();
template<class T = int> inline T readInt();
template<class T> inline void writeInt(T x, char end = 0);
inline void writeChar(int x);
inline void writeWord(const char* s);
static const int buf_size = 1 << 18;
inline int getChar() {
#ifndef LOCAL
    static char buf[buf_size];
    static int len = 0, pos = 0;
    if (pos == len) pos = 0, len = fread(buf, 1, buf_size, stdin);
    if (pos == len) return -1;
    return buf[pos++];
#endif
}
inline int readChar() {
#ifndef LOCAL
    int c = getChar();
    while (c <= 32) c = getChar();
    return c;
#else
    char c; cin >> c; return c;
#endif
}
template <class T>
inline T readInt() {
#ifndef LOCAL
    int s = 1, c = readChar();
    T x = 0;
    if (c == '-') s = -1, c = getChar();
    while ('0' <= c && c <= '9') x = x * 10 + c - '0', c = getChar();
    return s == 1 ? x : -x;
#else
    T x; cin >> x; return x;
#endif
}
static int write_pos = 0;
static char write_buf[buf_size];
inline void writeChar(int x) {
    if (write_pos == buf_size) fwrite(write_buf, 1, buf_size, stdout), write_pos = 0;
    write_buf[write_pos++] = x;
}
template <class T>
inline void writeInt(T x, char end) {
    if (x < 0) writeChar('-'), x = -x;
    char s[24]; int n = 0;
    while (x || !n) s[n++] = '0' + x % 10, x /= 10;
    while (n--) writeChar(s[n]);
    if (end) writeChar(end);
}
inline void writeWord(const char* s) {
    while (*s) writeChar(*s++);
}
struct Flusher {
    ~Flusher() { if (write_pos) fwrite(write_buf, 1, write_pos, stdout), write_pos = 0; }
}flusher;

const ld PI = acos(-1);
struct Circle {
    ld x, y, r, i;
    bool operator < (const Circle& c) const {
        if (x - r != c.x - c.r) return x - r < c.x - c.r;
        if (x != c.x) return x < c.x;
        if (y != c.y) return y < c.y;
        return r > c.r;
    }
    bool operator == (const Circle& c) const {
        return x == c.x && y == c.y && r == c.r;
    }
};

vector<Circle> circle;
vector<ll> cvx[MAX << 2];

struct Line {
    ld x, y, theta;
};
vector<Line> th[MAX << 2];
ld Norm(ld theta) {
    if (abs(theta) <= 1e-10) return theta;
    if (theta < 0) theta += 2 * PI;
    if (theta > 2 * PI) theta -= 2 * PI;
    return theta;
}

inline ld alpha(ld v1, Line v2) {
    if (v2.theta >= INF) return v2.theta;
    ld ret = Norm(v1 - v2.theta);
    return ret;
}

inline ld dist(pll p1, pll p2) {
    return (ld)(p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
}

Line tangent(ld theta, Circle c) {
    ld t = Norm(theta + PI / 2);
    Line ret = { (ld)c.x + c.r * cos(t), (ld)c.y + c.r * sin(t), theta };
    return ret;
}

Line tangent(Circle c1, Circle c2) {
    if (c1 == c2) return { c1.x - c1.r, c1.y, INF };
    ld d = (c2.x - c1.x) * (c2.x - c1.x) + (c2.y - c1.y) * (c2.y - c1.y);

    if (d - (c1.r - c2.r) * (c1.r - c2.r) < 1e-10) return { c1.x - c1.r, c1.y, INF + 1 };

    ld d2 = sqrt(d - (c1.r - c2.r) * (c1.r - c2.r));
    ld theta = atan2(c2.y - c1.y, c2.x - c1.x);

    if (c1.r <= c2.r) theta += atan2(abs(c1.r - c2.r), d2);
    else theta -= atan2(abs(c1.r - c2.r), d2);
    return tangent(Norm(theta), c1);
}

inline ld ccw(pdd a, pdd b) {
    return a.first * b.second - a.second * b.first;
}

bool isLeft(Line l1, Line l2) {
    return ccw({ cos(l1.theta), sin(l1.theta) }, { l2.x - l1.x, l2.y - l1.y }) <= 0;
}

void add(ll n, ll x) {
    if (cvx[n].empty() || cvx[n].back() != x) cvx[n].push_back(x);
}

void Advance(ld& l, ll& p, ll& q, ll le, ll ri, ll n) {
    vector<ll>& left = cvx[le];
    vector<ll>& right = cvx[ri];
    ld a1 = alpha(l, tangent(circle[left[p]], circle[right[q]]));
    ld a2 = alpha(l, th[le][p]);
    ld a3 = alpha(l, th[ri][q]);
    ld a4 = alpha(l, tangent(circle[right[q]], circle[left[p]]));

#if DEBUG
    cout << "NEW : " << ' ' << left[p] << ' ' << right[q] << endl;
    double t1 = tangent(circle[left[p]], circle[right[q]]).theta;
    double t2 = left.size() == 1 ? 1e16 : tangent(circle[left[p]], circle[left[(p + 1) % left.size()]]).theta;
    double t3 = right.size() == 1 ? 1e16 : tangent(circle[right[q]], circle[right[(q + 1) % right.size()]]).theta;
    double t4 = tangent(circle[right[q]], circle[left[p]]).theta;
    cout << tan(t1) << ' ' << tan(t2) << ' ' << tan(t3) << ' ' << tan(t4) << endl;
    cout << t1 * 180 / PI << ' ' << t2 * 180 / PI << ' ' << t3 * 180 / PI << ' ' << t4 * 180 / PI << endl;
    cout << a1 * 180 / PI << ' ' << a2 * 180 / PI << ' ' << a3 * 180 / PI << ' ' << a4 * 180 / PI << endl;
#endif

    if (a1 < a2 && a1 < a3) { add(n, right[q]); }
    if (a4 < a2 && a4 < a3) { add(n, left[p]); }

    if (a2 < a3) {
        l = th[le][p].theta;
        p = (p + 1) % left.size();
    }
    else {
        l = th[ri][q].theta;
        q = (q + 1) % right.size();
    }
}

void Merge(ll s, ll e, ll n) {
    vector<ll>& ret = cvx[n];
    if (s == e) { ret.push_back(s); th[n].push_back({ circle[s].x - circle[s].r, circle[s].y, INF }); return; }

    ll m = s + e >> 1; Merge(s, m, n << 1); Merge(m + 1, e, n << 1 | 1);
#if DEBUG
    cout << endl << s << ' ' << e << endl;
#endif
    ll p = 0, q = 0;
    ld theta = 1.5707963265;
    for (ll i = 0; i < 2 * (e - s + 1) - 1; ++i) {
        if (theta >= INF) break;
        theta = Norm(theta);
        Line lp = tangent(theta, circle[cvx[n << 1][p]]);
        Line lq = tangent(theta, circle[cvx[n << 1 | 1][q]]);

#if DEBUG
        cout << theta / PI * 180 << ' ' << tan(theta) << endl;
        cout << circle[left[p]].x << ' ' << circle[left[p]].y << ' ' << circle[left[p]].r << ' ' << lp.x << ' ' << lp.y << ' ' << lp.theta / PI * 180 << endl;
        cout << circle[right[q]].x << ' ' << circle[right[q]].y << ' ' << circle[right[q]].r << ' ' << lq.x << ' ' << lq.y << ' ' << lq.theta / PI * 180 << endl;
#endif

        //        if(dist({circle[left[p]].x, circle[left[p]].y}, {circle[right[q]].x, circle[right[q]].y}) <= circle[right[q]].r - circle[left[p]].r) {
        //            add(ret, right[q]);
        //            p = (p + 1) % left.size();
        //            continue;
        //        }
        //        
        //        if(dist({circle[left[p]].x, circle[left[p]].y}, {circle[right[q]].x, circle[right[q]].y}) <= circle[left[p]].r - circle[right[q]].r) {
        //            add(ret, left[p]);
        //            q = (q + 1) % right.size();
        //            continue;
        //        }

        if (isLeft(lp, lq)) {
            add(n, cvx[n << 1][p]);
            if (ret.size() >= 3 && ret[0] == ret[ret.size() - 2] && ret[1] == ret.back()) { ret.pop_back(); break; }
            Advance(theta, p, q, n << 1, n << 1 | 1, n);
        }
        else {
            add(n, cvx[n << 1 | 1][q]);
            if (ret.size() >= 3 && ret[0] == ret[ret.size() - 2] && ret[1] == ret.back()) { ret.pop_back(); break; }
            Advance(theta, q, p, n << 1 | 1, n << 1, n);
        }
#if DEBUG
        cout << "CONVEX HULL : ";
        for (auto v : ret) cout << v << ' ';
        cout << endl;
#endif

        if (ret.size() >= 3 && ret[0] == ret[ret.size() - 2] && ret[1] == ret.back()) { ret.pop_back(); break; }
    }
    if (ret.size() >= 2 && ret[0] == ret.back()) ret.pop_back();
    th[n].resize(ret.size());
    for (ll i = 0; i < (ll)ret.size(); ++i) th[n][i] = tangent(circle[ret[i]], circle[ret[(i + 1) % ret.size()]]);

#if DEBUG
    cout << "CONVEX HULL : ";
    for (auto v : ret) cout << v << ' ';
    cout << endl;
#endif // DEBUG

    return;
}

ll N, T;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    N = readInt();
    circle.resize(N);
    for (ll i = 0; i < N; ++i) {
        ll x, y, z;
        x = readInt(); y = readInt(); z = readInt();
        circle[i].x = x; circle[i].y = y; circle[i].r = z;
        circle[i].i = i;
    }
    //for(ll i = 0; i < N; ++i) circle[i].r += 10;
    //compress(circle);

    Merge(0, circle.size() - 1, 1);
    vector<ll>& ans = cvx[1];

#if 0
    for (ll v : ans) cout << "Circles" << circle[v].x << ' ' << circle[v].y << ' ' << circle[v].r << '\n';
#endif // DEBUG

#if 0
    for (ll v : ans) cout << circle[v].i << ' ';
    cout << endl;
#endif // 1

    cout.precision(10); cout << fixed;
    if (ans.size() == 1) {
        cout << 2 * PI * circle[ans[0]].r;
        return 0;
    }

    ld sum = 0;
    for (ll i = 0; i < (ll)ans.size(); ++i) {
        ll r = ans[(i + 1) % ans.size()];
        ld theta = Norm(th[1][(i + ans.size() - 1) % ans.size()].theta - th[1][i].theta);
        sum += circle[ans[i]].r * theta;

        ld d = dist({ circle[ans[i]].x, circle[ans[i]].y }, { circle[r].x, circle[r].y });
        sum += sqrt(d - (circle[ans[i]].r - circle[r].r) * (circle[ans[i]].r - circle[r].r));
    }
    cout << sum;
    return 0;
}