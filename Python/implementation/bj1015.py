import sys

N = int(sys.stdin.readline().strip())
arr = [[0]*3 for _ in range(N)]
query = tuple(map(int, sys.stdin.readline().strip().split()))
for i in range(N):
    arr[i][0], arr[i][1] = i, query[i]
arr.sort(key=lambda x: x[1])
for i in range(N):
    arr[i][2] = i
arr.sort()
ans = [arr[i][2] for i in range(N)]
print(*ans)
