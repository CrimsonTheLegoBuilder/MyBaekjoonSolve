// 15004  Bearly Made It
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#define TEST int tt; cin >> tt; while (tt--)
#define all(v) (v).begin(), (v).end()
#define loop(e, v) for (auto& (e) : (v))

#define pii pair<int, int>
#define pll pair<ll, ll>
#define xx first
#define yy second

#define ll long long
#define ld long double

using namespace std;

const int MAX = 100000;
const int INF = 1e9;
const ll LINF = 1e18;
const ld EPS = 1e-16;

typedef pair<ld, ld> Point;

Point operator+(Point a, Point b) { return { a.xx + b.xx, a.yy + b.yy }; }
Point operator-(Point a, Point b) { return { a.xx - b.xx, a.yy - b.yy }; }
Point operator*(ld s, Point p) { return { s * p.xx, s * p.yy }; }
ld inner(Point a, Point b) { return a.xx * b.xx + a.yy * b.yy; }
ld euclid(const Point& a, const Point& b) { return sqrt((a.xx - b.xx) * (a.xx - b.xx) + (a.yy - b.yy) * (a.yy - b.yy)); }

struct Circle {
    Point cp;
    ld r;
};

struct Line {
    Point p1, p2;
};

ld vsize(Point p) { return sqrt(p.xx * p.xx + p.yy * p.yy); }
Point rot90(Point p) { return { -p.yy, p.xx }; }

tuple<int, Point, Point> intersect(Circle c1, Circle c2) {
    Point p1 = c1.cp, p2 = c2.cp;
    ld r1 = c1.r, r2 = c2.r;

    Point lineVec = p2 - p1;
    ld L = vsize(lineVec);

    if (L > r1 + r2) {                  // no-intersection
        return tuple<int, Point, Point>{0, {}, {}};
    }
    else if (L <= abs(r1 - r2)) {   // include
        return tuple<int, Point, Point>{-1, {}, {}};
    }

    ld x = (r1 * r1 - r2 * r2 + L * L) / (2 * L);
    Point mp = p1 + (x / L) * lineVec;
    ld y = sqrt(r1 * r1 - x * x);

    if (y <= 0) {   // meet 1
        return tuple<int, Point, Point>{1, mp, mp};
    }
    else {        // meet 2
        return tuple<int, Point, Point>{
            2,
                mp + (y / L) * rot90(lineVec),
                mp - (y / L) * rot90(lineVec)
        };
    }
}
tuple<int, ld, ld> intersect(Line ln, Circle cc) {
    // https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
    Point lineVec = ln.p2 - ln.p1;

    ld a = inner(lineVec, lineVec);
    ld b = 2 * inner(lineVec, ln.p1 - cc.cp);
    ld c = inner(cc.cp, cc.cp) + inner(ln.p1, ln.p1) - 2 * inner(cc.cp, ln.p1) - cc.r * cc.r;

    const auto D = b * b - 4 * a * c;

    if (D < 0) {        // no-intersection
        return tuple<int, ld, ld>{0, {}, {}};
    }
    else if (D == 0) {  // meet 1
        ld ret = -b / (2 * a);
        return tuple<int, ld, ld>{1, ret, ret};
    }
    else {              // meet 2
        ld ret1 = (-b + sqrt(D)) / (2 * a);
        ld ret2 = (-b - sqrt(D)) / (2 * a);
        if (ret1 > ret2) swap(ret1, ret2);

        return tuple<int, ld, ld>{2, ret1, ret2};
    }
}

bool incLine(Line ln, vector<Circle>& cir) {
    vector<pair<ld, int>> pene;

    pene.emplace_back(0, 0);    // Segment start-point
    pene.emplace_back(1, 0);    // Segment end-point

    loop(c, cir) {
        Circle tmp = c;
        tmp.r += (EPS * tmp.r);

        auto ret = intersect(ln, tmp);
        if (get<0>(ret) == 2) {
            pene.emplace_back(get<1>(ret), -1);
            pene.emplace_back(get<2>(ret), 1);
        }
    }
    sort(all(pene));    // Through the line

    int depth = 0;
    bool flag = false;
    loop(s, pene) {
        depth -= s.yy;

        if (s.yy == 0) {    // Line segment start or end
            flag = !flag;
        }

        if (flag && depth <= 0) {
            return false;
        }
    }
    return true;
}

vector<pair<ll, ld>> adj[MAX];
ld dist[MAX];
bool vst[MAX];

ld dijkstra(ll start, ll end) {
    priority_queue<pair<ld, ll>> pq;

    dist[start] = 0;
    pq.push({ 0, start });

    while (!pq.empty()) {
        ll me = pq.top().yy; pq.pop();

        if (vst[me]) continue;
        if (me == end) return dist[end];

        vst[me] = true;

        loop(nxt, adj[me]) {
            ll v = nxt.xx; ld w = nxt.yy;
            if (vst[v] || dist[v] < dist[me] + w) continue;

            dist[v] = dist[me] + w;
            pq.push({ -dist[v], v });
        }
    }

    return -1;
}

vector<Point> pnts;
vector<Circle> cirs;
ll N;

void solve() {
    pnts.resize(2);  // 0: start, 1: end
    cin >> pnts[0].xx >> pnts[0].yy >> pnts[1].xx >> pnts[1].yy;

    cin >> N;

    cirs.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> cirs[i].cp.xx >> cirs[i].cp.yy >> cirs[i].r;
    }


    // Make every edges
    for (ll i = 0; i < N; ++i) {
        for (ll j = i + 1; j < N; ++j) {
            auto ret = intersect(cirs[i], cirs[j]);

            if (get<0>(ret) == 2) {
                pnts.push_back(get<2>(ret));
                pnts.push_back(get<1>(ret));
            }
            else if (get<0>(ret) == 1) {
                pnts.push_back(get<1>(ret));
            }
        }
    }


    // Add possible edges
    int len = pnts.size();
    for (ll i = 0; i < len; ++i) {
        for (ll j = i + 1; j < len; ++j) {
            if (incLine({ pnts[i], pnts[j] }, cirs)) {
                ld dist = vsize(pnts[i] - pnts[j]);

                adj[i].emplace_back(j, dist);
                adj[j].emplace_back(i, dist);
            }
        }
    }

    // Find shortest path
    fill_n(dist, MAX, LDBL_MAX);
    ld rst = dijkstra(0, 1);

    if (rst >= 0) {
        cout << rst;
    }
    else {
        cout << "impossible";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    cout.precision(16);

    solve();

    return 0;
}
