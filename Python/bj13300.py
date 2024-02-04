import sys

people = [[0] * 6 for _ in range(2)]

N, K = map(int, sys.stdin.readline().strip().split())
ans = 0
for _ in range(N):
    s, g = map(int, sys.stdin.readline().strip().split())
    people[s][g-1] += 1

for i in range(2):
    for j in range(6):
        ans += people[i][j] // K + (people[i][j] % K != 0)

print(ans)
