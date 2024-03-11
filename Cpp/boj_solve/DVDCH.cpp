#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

// https://github.com/ToxicPie/NaCl
template <typename T> struct P {
    T x, y;
    P(T x = 0, T y = 0) : x(x), y(y) {}
    bool operator<(const P& p) const { return tie(x, y) < tie(p.x, p.y); }
    bool operator==(const P& p) const { return tie(x, y) == tie(p.x, p.y); }
    P operator-() const { return { -x, -y }; }
    P operator+(P p) const { return { x + p.x, y + p.y }; }
    P operator-(P p) const { return { x - p.x, y - p.y }; }
    P operator*(T d) const { return { x * d, y * d }; }
    P operator/(T d) const { return { x / d, y / d }; }
    T dist2() const { return x * x + y * y; }
    double len() const { return sqrt(dist2()); }
    P unit() const { return *this / len(); }
    friend T dot(P a, P b) { return a.x * b.x + a.y * b.y; }
    friend T cross(P a, P b) { return a.x * b.y - a.y * b.x; }
    friend T cross(P a, P b, P o) { return cross(a - o, b - o); }
};
using pt = P<double>;

vector<pt> convex_hull(vector<pt> p) {
    sort(p.begin(), p.end());
    if (p[0] == p.back())
        return { p[0] };
    int n = p.size(), t = 0;
    vector<pt> h(n + 1);
    for (int _ = 2, s = 0; _--; s = --t, reverse(p.begin(), p.end()))
        for (pt i : p) {
            while (t > s + 1 && cross(i, h[t - 1], h[t - 2]) >= 0)
                t--;
            h[t++] = i;
        }
    return h.resize(t), h;
}

double find_dist(pt p1, pt p2, pt v1, pt v2, double len) {
    auto find_len = [&](double g_len) {
        return ((p1 + v1 * g_len) - (p2 + v2 * g_len)).len();
        };
    double l = 0, r = len;
    for (int i = 1; i <= 150; i++) {
        double m1 = l + (r - l) / 3, m2 = r - (r - l) / 3;
        if (find_len(m1) < find_len(m2)) {
            r = m2;
        }
        else {
            l = m1;
        }
    }
    return find_len((l + r) / 2);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    vector<pt> p(n);
    for (auto& [x, y] : p)
        cin >> x >> y;

    p = convex_hull(p);
    double total = 0;
    for (int i = 0; i < n; i++)
        total += (p[(i + 1) % n] - p[i]).len();

    auto p1 = p[0], p2 = p[0];
    int cur1 = 0, cur2 = 0;
    double acc = 0;
    while (true) {
        auto edge_len = (p[(cur2 + 1) % n] - p[cur2]).len();
        if (acc + edge_len <= total / 2) {
            acc += edge_len;
            cur2 = (cur2 + 1) % n;
            p2 = p[cur2];
        }
        else {
            auto rem_len = total / 2 - acc;
            p2 = p2 + (p[(cur2 + 1) % n] - p[cur2]) * (rem_len / edge_len);
            break;
        }
    }
    auto ans = (p1 - p2).len();
    for (int i = 1; i <= (n << 1); i++) {
        auto rem_len_1 = (p[(cur1 + 1) % n] - p1).len();
        auto rem_len_2 = (p[(cur2 + 1) % n] - p2).len();
        auto rem = min(rem_len_1, rem_len_2);
        ans = min(ans, find_dist(p1, p2, (p[(cur1 + 1) % n] - p1) / rem_len_1,
            (p[(cur2 + 1) % n] - p2) / rem_len_2, rem));
        if (abs(rem_len_1 - rem_len_2) <= 1e-7) {
            cur1 = (cur1 + 1) % n;
            p1 = p[cur1];
            cur2 = (cur2 + 1) % n;
            p2 = p[cur2];
        }
        else if (rem_len_1 <= rem_len_2) {
            cur1 = (cur1 + 1) % n;
            p1 = p[cur1];
            p2 = p2 + (p[(cur2 + 1) % n] - p2) * (rem / rem_len_2);
        }
        else {
            cur2 = (cur2 + 1) % n;
            p2 = p[cur2];
            p1 = p1 + (p[(cur1 + 1) % n] - p1) * (rem / rem_len_1);
        }
    }
    cout << ans << '\n';
}