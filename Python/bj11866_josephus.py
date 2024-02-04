import collections as co

n, k = map(int, input().split())
q1 = co.deque(range(1, n + 1))
ans = []

while len(q1) != 0:
    q1.rotate(1-k)
    ans.append(str(q1.popleft()))

print('<' + ', '.join(ans) + '>')
