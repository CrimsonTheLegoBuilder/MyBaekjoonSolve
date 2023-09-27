import sys

N = int(input())

set_1 = set()
cnt = 0

for i in range(N):
    a = sys.stdin.readline().strip()
    if a == 'ENTER':
        cnt += len(set_1)
        set_1.clear()
    elif i == N - 1:
        set_1.add(a)
        cnt += len(set_1)
    else:
        set_1.add(a)

print(cnt)
