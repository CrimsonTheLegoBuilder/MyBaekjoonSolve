#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

using ld = long double;

struct pt {
  ld x, y;
};

struct circle {
  pt centre;
  ld radius;
};

struct line {
  pt b, e;
};

pt operator+(pt l, pt r) { return {l.x + r.x, l.y + r.y}; }
pt operator-(pt l, pt r) { return {l.x - r.x, l.y - r.y}; }
pt operator*(ld s, pt p) { return {s * p.x, s * p.y}; }
ld dot(pt l, pt r) { return l.x * r.x + l.y * r.y; }
ld length(pt p) { return hypot(p.x, p.y); }
pt rotccw(pt p) { return {-p.y, p.x}; }

istream &operator>>(istream &in, pt &p) { return in >> p.x >> p.y; }
istream &operator>>(istream &in, circle &c) { return in >> c.centre >> c.radius; }
ostream &operator<<(ostream &out, pt const &p) { return out << p.x  << ' ' << p.y; }
ostream &operator<<(ostream &out, circle const &c) { return out << c.centre << ' ' << c.radius; }
ostream &operator<<(ostream &out, line const &l) { return out << l.b << " -> " << l.e; }

// returns number of intersections and the points
tuple<int, pt, pt> intersect(const circle c1, const circle c2) {
  const auto p1 = c1.centre, p2 = c2.centre;
  const auto r1 = c1.radius, r2 = c2.radius;

  // compute distance between centres
  const auto diff = p2 - p1;
  const auto L = length(diff);

  // no overlap
  if (L > r1 + r2) return tuple<int, pt, pt>{0, {}, {}};

  // containment or equality
  if (L <= abs(r1 - r2)) return tuple<int, pt, pt>{-1, {}, {}};

  // distance to midpoint from centre 1
  const auto a = (r1 * r1 - r2 * r2 + L * L) / (2 * L);
  // mid point
  const auto m = p1 + (a / L) * diff;
  // height from mid point
  const auto h = sqrt(r1 * r1 - a * a);

  // only one intersection point if h is small, else two
  // may want to check with epsilon here
  if (h <= 0) {
    const auto ret = m;
    return tuple<int, pt, pt>{1, ret, ret};
  } else {
    const auto ret1 = m + (h / L) * rotccw(diff);
    const auto ret2 = m - (h / L) * rotccw(diff);
    return tuple<int, pt, pt>{2, ret1, ret2};
  }
}

// does not return the points, but only mu1 and mu2 such that
// pi = l1.b + mui * (l1.e - l1.b) are the intersections points
// returns them sorted
tuple<int, ld, ld> intersect(const line l1, const circle c2) {
  const auto diff = l1.e - l1.b;

  const auto a = dot(diff, diff);
  const auto b = 2 * dot(diff, l1.b - c2.centre);
  const auto c = dot(c2.centre, c2.centre) + dot(l1.b, l1.b)
                - 2 * dot(c2.centre, l1.b) - c2.radius * c2.radius;

  const auto det = b * b - 4 * a * c;

  // no solution, may want to check with an epsilon here.
  if (det < 0) return tuple<int, ld, ld>{0, {}, {}};

  // touches the circle, may want to check with epsilon here
  if (det == 0) {
    const auto mu = -b / (2 * a);
    return tuple<int, ld, ld>{1, mu, mu};
  }

  const auto mu1 = (-b + sqrt(det)) / (2 * a);
  const auto mu2 = (-b - sqrt(det)) / (2 * a);
  if (mu1 < mu2)
    return tuple<int, ld, ld>{2, mu1, mu2};
  else
    return tuple<int, ld, ld>{2, mu2, mu1};
}

bool line_covered(const line l1, const vector<circle> &circles) {
  vector<pair<ld, int>> scope;
  scope.emplace_back(0, 0);
  scope.emplace_back(1, 0);
  for (const auto c : circles) {
    const auto ret = intersect(l1, c);
    switch (get<0>(ret)) {
    case 2:
      scope.emplace_back(get<1>(ret), -1);
      scope.emplace_back(get<2>(ret), 1);
      break;
      // default: // touch or no intersection -> ignore
    }
  }

  sort(scope.begin(), scope.end());

  int depth = 0;
  bool on_line = false;
  for (const auto s : scope) {
    depth -= s.second;
    if (s.second == 0)
      on_line = !on_line;

    if (on_line && depth <= 0) {
      return false;
    }
  }
  return true;
}

using node = size_t;
ld dijkstra(const vector<vector<pair<node, ld>>> &graph, const node s,
            const node t) {
  const auto n = graph.size();

  // Initialize data structures.
  vector<ld> dist(n, numeric_limits<ld>::max());
  vector<bool> seen(n);

  priority_queue<pair<ld, node>> work;

  // Assign start values.
  dist[s] = 0;
  work.push({0, s});

  while (!work.empty()) {
    const node c = work.top().second;
    work.pop();

    if (seen[c]) continue;
    if (c == t) return dist[t];

    seen[c] = true;

    for (const auto &p : graph[c]) {
      const node i = p.first;
      const ld w = p.second;
      if (seen[i] || dist[i] < dist[c] + w) continue;

      dist[i] = dist[c] + w;
      work.push({-dist[i], i});
    }
  }

  return -1;
}

int main() {
  vector<pt> ps(2);
  cin >> ps[0] >> ps[1]; // barney and mom

  size_t n;
  cin >> n;

  vector<circle> cs(n);
  for (auto &c : cs) {
    cin >> c;
  }

  // compute all circle-circle intersections
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      const auto ret = intersect(cs[i], cs[j]);
      switch (get<0>(ret)) {
      case 2:
        ps.push_back(get<2>(ret));
      // fallthrough :D
      case 1:
        ps.push_back(get<1>(ret));
        break;
        // default: // no intersection (0), or containment (-1)
      }
    }
  }

  size_t m = ps.size();
  clog << "points: " << m << endl;

  vector<vector<pair<node, ld>>> graph(m);

  size_t e = 0;
  // compute all edges
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = i + 1; j < m; ++j) {
      if (line_covered({ps[i], ps[j]}, cs)) {
        e++;
        const auto dist = length(ps[i] - ps[j]);
        graph[i].emplace_back(j, dist);
        graph[j].emplace_back(i, dist);
      }
    }
  }

  clog << "edges: " << e << endl;

  // shortest path
  const auto l = dijkstra(graph, 0, 1);

  if (l >= 0) {
    cout << setprecision(numeric_limits<ld>::digits10 + 1) << l << endl;
  } else {
    cout << "impossible" << endl;
  }
}