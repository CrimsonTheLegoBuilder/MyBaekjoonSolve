def main():
  from fractions import Fraction as Q
  from collections import defaultdict as D
  import random

  class pt(tuple):
    def __new__(pt, l): return super().__new__(pt, l)
    def __xor__(a, b): return a[0] * b[1] - a[1] * b[0]
    def __and__(a, b): return a[0] * b[0] + a[1] * b[1]
    def __rmul__(b, a): return pt((a * b[0], a * b[1]))
    def __sub__(a, b): return pt((a[0] - b[0], a[1] - b[1]))
    def __add__(a, b): return pt((a[0] + b[0], a[1] + b[1]))

  def seg(a, b, c, d): return [pt((Q(a), Q(b))), pt((Q(c), Q(d)))]
  def sgn(a): return (a > 0) - (a < 0)
  def opp(a, b, c, d): return sgn(c - a ^ b - a) != sgn(d - a ^ b - a)
  def segseg(a, b, c, d): return opp(a, b, c, d) and opp(c, d, a, b)
  def linlin(a, b, c, d): return a - (a - c ^ d - c) / (b - a ^ d - c) * (b - a)

  n, *s = map(int, input().split())
  s, t = seg(*s)
  v = [[s, t], [s, t]] + [seg(*input().split()) for _ in range(n)]
  n += 2

  v[0][1] = v[1][0] = mid = pt((Q(random.randint(-10**6, 10**6)), Q(random.randint(-10**6, 10**6))))
  *v, = map(sorted, v)
  v = sorted(v, key=lambda s: (lambda d: d[1] * abs(d[1]) / (d & d))(s[1] - s[0]))

  to = D(lambda: [])
  to_ = D(lambda: [])
  idx = {}

  for i in range(n):
    cur = [*v[i]]
    for j in range(n):
      if not segseg(*v[i], *v[j]): continue
      cur.append(linlin(*v[i], *v[j]))
    d = v[i][1] - v[i][0]
    cur = sorted(set(cur), key=lambda p: p & d)
    for i in range(len(cur)):
      if i: to_[cur[i]].append(cur[i - 1])
      if i + 1 < len(cur): to[cur[i]].append(cur[i + 1])

  for k in to | to_:
    to[k] += to_[k]
    for i, u in enumerate(to[k]):
      idx[(k, u)] = i

  q, nq = [(s, to[s][0])], []
  vis = {q[0]}
  d = 0
  while True:
    for a, b in q:
      if b == t: return print(d)
      c = to[b][idx[(b, a)] - 1]
      if (b, c) not in vis:
        vis.add((b, c))
        q.append((b, c))
      if not (a - b ^ mid - s and a - b ^ mid - t) and (b, a) not in vis:
        vis.add((b, a))
        q.append((b, a))
    for a, b in q:
      if (b, a) not in vis:
        vis.add((b, a))
        nq.append((b, a))
    d, q, nq = d + 1, nq, []

main()
