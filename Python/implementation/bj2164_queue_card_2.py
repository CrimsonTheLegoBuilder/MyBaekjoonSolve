import collections as co

n = int(input())

q1 = co.deque(range(1, n + 1))

while len(q1) > 1:
    q1.popleft()
    q1.rotate(-1)

print(q1[0])
