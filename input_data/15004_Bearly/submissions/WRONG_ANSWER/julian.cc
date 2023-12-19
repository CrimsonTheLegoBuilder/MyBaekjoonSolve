// Copyright (2017) BAPC Jury. Author: Julian Neeleman

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

const double EPS = 0.000001, INF = 1 << 30;

struct Circle {
    double x, y, r;

    bool operator<(const Circle& rhs) const {
        return tie(x, y, r) < tie(rhs.x, rhs.y, rhs.r);
    }

    Circle operator+(const Circle& rhs) const {
        return { x + rhs.x, y + rhs.y, r + rhs.r };
    }

    Circle operator-(const Circle& rhs) const {
        return { x - rhs.x, y - rhs.y, r - rhs.r };
    }

    Circle operator*(const double& rhs) const {
        return { x * rhs, y * rhs, r * rhs };
    }

    Circle operator/(const double& rhs) const {
        return { x / rhs, y / rhs, r / rhs };
    }
};

struct Scope {
    Circle p;
    char e;

    bool operator<(const Scope& rhs) const {
        return tie(p, e) < tie(rhs.p, rhs.e);
    }
};

Circle c[20];
vector<Circle> v;
vector<vector<double> > g;

// Squared distance between centers of two circles.
double dst(const Circle& p, const Circle& q) {
    return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
}

// Circle-circle intersection.
void cc(const Circle& p, const Circle& q) {
    double d = dst(p, q);

    // Check circles are disjunct, one contains the other, or they coincide.
    if (d > p.r + q.r || d < abs(p.r - q.r) ||
        d < EPS && abs(p.r - q.r) < EPS) {
        return;
    }

    // Compute distance a from one circle's center to the
    // center of the intersection lens, and the distance from
    // this point to the intersection points.
    double a = (p.r * p.r - q.r * q.r + d * d) / (2.0 * d),
        h = sqrt(p.r * p.r - a * a);

    // Compute coordinates of center of the intersection lens.
    Circle s = p + (q - p) * a / d,
        t = { h * (q.y - p.y) / d, h * (q.x - p.x) / d, 0 };

    v.push_back(s + t);

    // The second point is distinct if h != 0.
    if (abs(h) > EPS) {
        v.push_back(s - t);
    }
}

// Circle-line intersection.
void cl(vector<Scope>& sc, const Circle& p, const Circle& q, const Circle& s) {
    double a = (q.x - p.x) * (q.x - p.x) + (q.y - p.y) * (q.y - p.y),
        b = 2 * ((q.x - p.x) * (p.x - s.x) +
            (q.y - p.y) * (p.y - s.y)),
        c = s.x * s.x + s.y * s.y + p.x * p.x + p.y * p.y -
            2 * (s.x * p.x + s.y * p.y) - s.r * s.r,
        D = b * b - 4 * a * c;

    // Check if intersections are real.
    if (D > EPS) {
        Circle t = p + (q - p) * ((-b + sqrt(D)) / (2 * a)),
            u = p + (q - p) * ((-b - sqrt(D)) / (2 * a));

        // Check the order and label the intersections
        // in the scope accordingly.
        sc.push_back({ u, u < t ? 'O' : 'S' });
        sc.push_back({ t, u < t ? 'S' : 'O' });
    }
}

// Shortest path.
double dijkstra(int s, int t) {
    int n = g.size();

    // Initialize data structures.
    vector<double> d(n, INF);
    priority_queue<pair<double, int> > q;
    vector<bool> seen(n);

    // Assign start values.
    d[s] = 0;
    q.push({ 0, s });

    while (!q.empty()) {
        int c = q.top().second;
        q.pop();

        if (seen[c]) {
            continue;
        }
        seen[c] = true;

        if (c == t) {
            return d[t];
        }

        for (int i = 0; i < n; i++) {
            if (seen[i] || g[c][i] == INF || d[i] < d[c] + g[c][i]) {
                continue;
            }

            d[i] = d[c] + g[c][i];
            q.push({ -d[i], i });
        }
    }

    return -1;
}

int main() {
    Circle b, m;
    cin >> b.x >> b.y >> m.x >> m.y;
    v.push_back(b);
    v.push_back(m);

    int N;
    cin >> N;

    for (int i = 0; i < N; i++) {
        cin >> c[i].x >> c[i].y >> c[i].r;
    }

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            cc(c[i], c[j]);
        }
    }

    for (const Circle& ci : v) {
        clog << "(" << ci.x << ", " << ci.y << ")" << endl;
    }

    // Construct the graph.
    g.assign(v.size(), vector<double>(v.size()));

    // For every pair of intersections, check if the
    // circles cover the entire line segment betweeen them.
    for (int i = 0; i < (int)v.size(); i++) {
        for (int j = i + 1; j < (int)v.size(); j++) {
            vector<Scope> sc;
            sc.push_back({ v[i], 'P' });
            sc.push_back({ v[j], 'P' });

            for (int k = 0; k < N; k++) {
                cl(sc, v[i], v[j], c[k]);
            }

            // Sort the scope.
            sort(sc.begin(), sc.end());

            int cn = 0;
            bool in = false, edge = true;
            for (const Scope& s : sc) {
                if (s.e == 'O') {
                    cn++;
                } else if (s.e == 'S') {
                    cn--;
                }

                // Check if we are inside the segment.
                in ^= (s.e == 'P');
                if (in && cn == 0) {
                    edge = false;
                    break;
                }
            }

            // Add edge if conditions pass.
            if (edge) {
                g[i][j] = dst(v[i], v[j]);
                g[j][i] = g[i][j];
            } else {
                g[i][j] = INF;
                g[j][i] = INF;
            }
        }
    }

    double ans = dijkstra(0, 1);
    cout << (ans == -1 ? "impossible" : to_string(ans)) << endl;

    return 0;
}
