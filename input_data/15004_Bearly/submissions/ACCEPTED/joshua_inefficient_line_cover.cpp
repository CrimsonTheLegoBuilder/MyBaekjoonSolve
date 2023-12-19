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
#include <set>

using namespace std;

using ld = long double;
/*
Instead of covering it, we have a 'cursor'
which we move forward, iterating over all circles each time.
Running tijme O(n^6). This happens to be actually faster
on many test cases. Also, we need surprisingly few places
where we need to check floats with an epsilon
*/


// This is a relative epsilon used in line_covered
// to increase the circles slightly (only a tiny bit is enough)
const ld EPSILONE = 1e-16;

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
bool operator<(pt lhs, pt rhs){ return make_pair(lhs.x, lhs.y) < make_pair(rhs.x, rhs.y); }

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
// If l1.b == l1.e, then both a = b = 0, and so it will return
// {1, -nan, -nan}
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
  ld cursor_t = 0;
  pt cursor_p = l1.b;

  // O(n^2) instead of intended O(n log n)
  while (cursor_t < 1) {
    bool found_circle = false;
    for (const auto c : circles) {
      // I use a slightly bigger circle, just to cut corners
      circle ceps = c;
      ceps.radius += EPSILONE * ceps.radius;
      
      if (length(cursor_p - ceps.centre) < ceps.radius) {
        // inside a circle => compute furthest intersection point
        // and if it's bigger, move cursor forward
        // We have to check for strictly larger mu, otherwise we might get
        // an infinite loop.
        const auto ret = intersect(l1, ceps);

        // these asserts are not really needed. These asserts fire
        // when we have duplicate points (i.e. l1.b == l1.e).
        // We already got rid of those.
        // In that case, the calculation is still correct,
        // as `mu > cursor_t` is `false` (because `mu` = `-nan`)
        assert(get<0>(ret) == 2);
        assert(isfinite(get<1>(ret)) && isfinite(get<2>(ret)));

        const auto mu = max(get<1>(ret), get<2>(ret));
        // epsilon here?
        if (mu > cursor_t) {
          found_circle = true;
          cursor_t = mu;
          cursor_p = l1.b + mu * (l1.e - l1.b);
          break;
        }
      }
    }

    if (!found_circle) break;
  }

  // epsilon here?
  return cursor_t >= 1;
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

  set<pt> newps;

  // compute all circle-circle intersections
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      const auto ret = intersect(cs[i], cs[j]);
      switch (get<0>(ret)) {
      case 2:
        newps.insert(get<2>(ret));
      // fallthrough :D
      case 1:
        newps.insert(get<1>(ret));
        break;
        // default: // no intersection (0), or containment (-1)
      }
    }
  }

  for(auto p : newps) ps.push_back(p);

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
