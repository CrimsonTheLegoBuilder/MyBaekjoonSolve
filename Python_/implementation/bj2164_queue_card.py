import collections as co

n = int(input())

q1 = co.deque(range(1, n + 1))
cnt = 0

while len(q1) > 1:
    if cnt == 0:
        q1.popleft()
        cnt += 1
    elif cnt == 1:
        q1.append(q1.popleft())
        cnt -= 1

print(q1[0])
